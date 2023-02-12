#include <iostream>

#include "client.hpp"

Client::Client(int a_width, int a_height, sf::RenderWindow &a_win)
    : m_canvas(a_width, a_height)
    , m_win(a_win)
{
}

    void Client::createClient()
    {
        XInitThreads();
        sf::IpAddress serverIp = sf::IpAddress::getLocalAddress();

        if (m_socket.connect(serverIp, m_port) == sf::Socket::Done) {
            m_playerName = m_canvas.getName(m_win);

            if (m_socket.send(m_playerName.c_str(), m_playerName.size()) != sf::Socket::Done) {
                std::cout << "Failed to send message to client" << std::endl;
            }

            while(!m_stop) {
                std::thread receiveMessagesThread([this](){ 
                                                    std::string gameRequest = getGameRequest();                    
                                                     handleIncomingGameRequest(gameRequest);
                                                     m_stop = true;
                                                    });

                std::thread chosePlayerThread([this]() {
                                                std::string chosenPlayer = sendGameRequest();
                                                if (!chosenPlayer.empty())
                                                {
                                                    handleOutcomingGameRequest(chosenPlayer);
                                                    m_stop = true;
                                                    std::string response = waitGameRequestResponse();
                                                    if (response.find("rejected") != std::string::npos)
                                                    {
                                                        m_canvas.drawGameRequestRejected(response, m_win);
                                                        m_stop = false;
                                                    }
                                                    else if (response.find("accept") != std::string::npos)
                                                    {
                                                        connectSecondPlayer(response);
                                                        m_stop = true;
                                                    }
                                                }                        
                                            });
                                            
                receiveMessagesThread.join();
                chosePlayerThread.join();
            }    
        }
    }

    std::string Client::getGameRequest()
    {
        while(!m_stop) {
            char data[1024];
            std::size_t received;
            sf::Socket::Status status = m_socket.receive(data, sizeof(data), received);

            if (status == sf::Socket::Done) {
                std::string receivedString(data, received);
                std::vector<std::string> messages = split(receivedString, '\0');
                std::unique_lock<std::mutex> lock(m_onlinePlayersMutex);

                for (const auto &m : messages) {
                    if (std::find_if(m_onlinePlayers.begin(), m_onlinePlayers.end(),
                                    [&](const std::tuple<sf::IntRect, std::string> &t)
                                    { return std::get<1>(t) == m; }) == m_onlinePlayers.end()) {
                        if (m.find("want to play") != std::string::npos) {
                            m_stop = true;
                            return m;
                        } else if(m.find("rejected") != std::string::npos){
                            continue;
                        } else {
                            m_onlinePlayers.push_back(std::make_tuple(m_canvas.pushNewPlayer(m, m_numOfPlayers), m));
                            m_numOfPlayers++;
                        }
                    }
                }
            }
        }
        return "";
    }

    std::string Client::sendGameRequest()
    {
        sf::Event event;
        m_canvas.drawPlayerConnect(m_playerName, m_win);
        m_canvas.drawOnlinePlayersText(m_win);
        m_canvas.drawAllPlayers(m_win);
        int playersCounter = m_numOfPlayers;
        while (!m_stop) {
            if (playersCounter != m_numOfPlayers) {
                m_canvas.drawAllPlayers(m_win);
                playersCounter = m_numOfPlayers;
            }
            
            while (m_win.pollEvent(event)) {
                sf::Cursor cursor;
                std::unique_lock<std::mutex> lock(m_onlinePlayersMutex);

                if (event.type == sf::Event::MouseMoved || event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(m_win);
                    for (const auto &[rec, name] : m_onlinePlayers) {
                    
                        if (rec.contains(mousePosition)) {
                            if (cursor.loadFromSystem(sf::Cursor::Hand)) {
                                m_win.setMouseCursor(cursor);
                            }
                        
                            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                                if (cursor.loadFromSystem(sf::Cursor::Arrow)) {
                                    m_win.setMouseCursor(cursor);
                                }
                                m_stop = true;
                                return name;
                            }
                        } else {
                            if (cursor.loadFromSystem(sf::Cursor::Arrow)) {
                                m_win.setMouseCursor(cursor);
                            }
                        }
                    }
                }
            }
        
        }

        return "";
    }

    bool Client::handleIncomingGameRequest(std::string &a_msg)
    {
        std::string request;
        std::size_t start_pos = a_msg.find('|');
        std::size_t end_pos = a_msg.find('|', start_pos + 1);
        std::string secondPlayer = a_msg.substr(start_pos + 1, end_pos - start_pos - 1);

        size_t portPos = a_msg.find("port:");
        std::string incomingRequest = a_msg.substr(0, portPos);
        // std::string newPort = a_msg.substr(portPos+5);
        std::string newPort = "";

        if (findSubString(incomingRequest, "play with"))
        {
            request = "to play with ";
        }
        else
        {
            request = "to play against ";
        }

        std::string accept = "Accept";
        std::string reject = "Reject";
        std::string response;

        while (incomingRequest.find('|') != std::string::npos)
        {
            incomingRequest.erase(std::remove(incomingRequest.begin(), incomingRequest.end(), '|'), incomingRequest.end());
        }
       
        std::string userChoice = choseOption(incomingRequest, accept, reject);
        if (userChoice == accept) {
            response = "|" + m_playerName + "|" + " accepted " + "|" + request + "|" + "|" + secondPlayer + "|";
        } else {
            response = "|" + m_playerName + "| rejected |" + secondPlayer + "| game request";
            m_stop = false;
        }

        if (m_socket.send(response.c_str(), response.size()) == sf::Socket::Done) {
            std::cout << " <=> msg sent " << std::endl;
        } else {
            std::cout << " <=> Failed to send message to client" << std::endl;
        }

        if (userChoice == accept)
        {
            // m_socket.disconnect();
            connectFirstPlayer(newPort);
            return true;
        }
        
        return false;
    }

    void Client::handleOutcomingGameRequest(std::string &a_secondPlayer)
    {
        std::string chose = "What you wanna do?";
        std::string against = "Play against " + a_secondPlayer;
        std::string with = "Play with " + a_secondPlayer;
        std::string userChoice = choseOption(chose, against, with);
        std::string request = m_playerName + " sent request to " + userChoice;

        if (m_socket.send(request.c_str(), request.size()) == sf::Socket::Done) {
            std::cout << " <=> msg sent " << std::endl;
        } else {
            std::cout << " <=> Failed to send message to client" << std::endl;
        }
    }

    std::string Client::waitGameRequestResponse()
    {
        while (!m_response) {
            char data[1024];
            std::size_t received;
            sf::Socket::Status status = m_socket.receive(data, sizeof(data), received);

            if (status == sf::Socket::Done) {
                std::string receivedString(data, received);
                std::vector<std::string> messages = split(receivedString, '\0');
                std::unique_lock<std::mutex> lock(m_onlinePlayersMutex);

                for (const auto &m : messages) {
                    if (m.find("rejected") != std::string::npos) {
                        m_response = true;
                        sleep(1);
                        return m;
                    } else  if (m.find("accept") != std::string::npos) {
                        m_response = true;
                        // m_socket.disconnect();
                        return m;
                    }
                    
                }
            }else{
                std::cout << "failed" << std::endl;
            }
        }
        return "";
    }

    std::string Client::choseOption(std::string &a_msg, std::string &a_firstOption, std::string &a_secondOption)
    {

        std::vector<std::tuple<sf::IntRect, std::string>> gameTypeBoxes = m_canvas.drawGameOptions(a_msg, a_firstOption, a_secondOption, m_win);
        sf::IntRect fistOption = std::get<0>(gameTypeBoxes[0]);
        sf::IntRect secondOption = std::get<0>(gameTypeBoxes[1]);

        sf::Event event;
    
        while (m_win.isOpen() && !sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            sf::Cursor cursor;

            if (fistOption.contains(sf::Mouse::getPosition(m_win))) {
                if (cursor.loadFromSystem(sf::Cursor::Hand)) {
                    m_win.setMouseCursor(cursor);
                }

                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    if (cursor.loadFromSystem(sf::Cursor::Arrow)) {
                        m_win.setMouseCursor(cursor);
                    }

                    return std::get<1>(gameTypeBoxes[0]);
                }
            } else if (secondOption.contains(sf::Mouse::getPosition(m_win))) {
                if (cursor.loadFromSystem(sf::Cursor::Hand)) {
                    m_win.setMouseCursor(cursor);
                }

                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    if (cursor.loadFromSystem(sf::Cursor::Arrow)) {
                        m_win.setMouseCursor(cursor);
                    }

                    return std::get<1>(gameTypeBoxes[1]);
                }
            } else {
                if (cursor.loadFromSystem(sf::Cursor::Arrow)) {
                    m_win.setMouseCursor(cursor);
                }
            }

            while (m_win.pollEvent(event))
            {
            }
        }
        return "";
    }

    std::tuple<std::string, std::string> Client::getNames(std::string &a_msg)
    {
        size_t pos1 = a_msg.find(":");
        size_t pos2 = a_msg.find(":", pos1 + 1);

        std::string player1 = a_msg.substr(pos1 + 1, pos2 - pos1 - 1);
        std::string player2 = a_msg.substr(pos2 + 1);

        return std::make_tuple(player1, player2);
    }

    std::vector<std::string> Client::split(const std::string &s, char delimiter)
    {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter))
        {
            tokens.push_back(token);
        }

        return tokens;
    }

    bool Client::findSubString(std::string a_string, std::string a_subString)
    {
        std::size_t pos = a_string.find(a_subString);
        if (pos != std::string::npos) {
            return true;
        } 

        return false;
    }

    void Client::connectFirstPlayer(std::string a_newPort)
    {
        // m_socket.disconnect();
        // m_win.clear();

        sf::TcpListener listeningSocket;
        if (listeningSocket.listen(5100) != sf::Socket::Done) {
            std::cout << "Error: could not listen to port m_port" << std::endl;
            return;
        }

        sf::TcpSocket socket;
        if (listeningSocket.accept(socket) != sf::Socket::Done) {
            std::cout << "Error: could not accept client" << std::endl;
            return;
        }

        std::thread Treceive([&]()
                             {
        while (true) {
            char data[1024];
            std::size_t received;
            sf::Socket::Status status = socket.receive(data, sizeof(data), received);
            
            if (status == sf::Socket::Done) {
                std::string receivedString(data, received);
                std::stringstream stream(receivedString);
                std::string keyword = receivedString.substr(0, receivedString.find(":"));
                std::string numbers = receivedString.substr(receivedString.find(":") + 1);

                std::stringstream n(numbers);
                float x, y;
                n >> x >> y;
                sf::Vector2f pos(x,y);
                if(keyword == "ball") {
                    m_ballPositionQueueSecond.pushBack(pos);
                } else if(keyword == "paddle") {
                    m_paddlePositionQueueSecond.pushBack(pos);
                }
            } else {
                std::cout << "Failed to receive message in connectFirstPlayer" << std::endl;
            }

        } });

        std::thread Tsend([&]()
                      {
                          while (true) {
        if (!m_ballPositionQueueFirst.isEmpty())
        {
            sf::Vector2f pos;
            m_ballPositionQueueFirst.pop(pos);
            std::string position = "ball:" + std::to_string(pos.x) + "," + std::to_string(pos.y);
            if (socket.send(position.c_str(), position.size()) != sf::Socket::Done)
            {
                std::cout << "Failed to send message to client" << std::endl;
            }
        }

        if (!m_paddlePositionQueueFirst.isEmpty())
        {
            sf::Vector2f pos;
            m_paddlePositionQueueFirst.pop(pos);
            std::string position = "paddle:" + std::to_string(pos.x) + "," + std::to_string(pos.y);
            if (socket.send(position.c_str(), position.size()) != sf::Socket::Done)
            {
                std::cout << "Failed to send message to client" << std::endl;
            }
        }

                
    } });
        PlayerManager p1(m_canvas.getWidth(), m_canvas.getHeight(), m_win, m_ballPositionQueueFirst, m_paddlePositionQueueFirst, m_ballPositionQueueSecond, m_paddlePositionQueueSecond);
        p1.playLevel();
        Tsend.join();
        Treceive.join();
    }

    void Client::connectSecondPlayer(std::string a_newMsg)
    {
        // m_socket.disconnect();
        // m_win.clear();

        // size_t portPos = a_newMsg.find("port:");
        // std::string incomingRequest = a_newMsg.substr(0, portPos);
        // std::string newPort = a_newMsg.substr(portPos + 5);
        // int port = std::stoi(newPort);
        sf::TcpSocket socket;
        socket.connect(sf::IpAddress::getLocalAddress(), 5100);

        std::thread Tsend([&]()
                          {
        while (true) {
            if (!m_ballPositionQueueSecond.isEmpty()) {
                sf::Vector2f pos;
                m_ballPositionQueueSecond.pop(pos);
                std::string position = "ball:"+std::to_string(pos.x) + "," + std::to_string(pos.y);
                if (socket.send(position.c_str(), position.size()) != sf::Socket::Done) {
                    std::cout << "Failed to send message to client" << std::endl;
                }
            }

            if (!m_paddlePositionQueueSecond.isEmpty()) {
                sf::Vector2f pos;
                m_paddlePositionQueueSecond.pop(pos);
                std::string position = "paddle:" + std::to_string(pos.x) + "," + std::to_string(pos.y);
                if (socket.send(position.c_str(), position.size()) != sf::Socket::Done) {
                    std::cout << "Failed to send message to client" << std::endl;
                }
            }

          
        } });
        std::thread Treceive([&]()
                             {
        while (true) {
        char data[1024];
        std::size_t received;
        sf::Socket::Status status = socket.receive(data, sizeof(data), received);
        if (status == sf::Socket::Done)
        {
            std::string receivedString(data, received);
            std::stringstream stream(receivedString);
            std::string keyword = receivedString.substr(0, receivedString.find(":"));
            std::string numbers = receivedString.substr(receivedString.find(":") + 1);

            std::stringstream n(numbers);
            float x, y;
            n >> x >> y;
            sf::Vector2f pos(x, y);
            if (keyword == "ball")
            {
                m_ballPositionQueueFirst.pushBack(pos);
            }
            else if (keyword == "paddle")
            {
                m_paddlePositionQueueFirst.pushBack(pos);
            }
        }
        else
        {
            std::cout << "Failed to receive message in connectSecondPlayer" << std::endl;
        }
                  
        } });
        PlayerManager p2(m_canvas.getWidth(), m_canvas.getHeight(), m_win, m_ballPositionQueueSecond, m_paddlePositionQueueSecond, m_ballPositionQueueFirst, m_paddlePositionQueueFirst);
        p2.playLevel();
        Tsend.join();
        Treceive.join();
    }

