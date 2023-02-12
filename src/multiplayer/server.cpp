#include "server.hpp"

void Server::AcceptClients()
{
    if (m_listener.listen(m_port) != sf::Socket::Done) {
        std::cout << "Error: could not listen to port m_port" << std::endl;
        return;
    }

    while (true) {
        std::unique_ptr<sf::TcpSocket> client(new sf::TcpSocket);
        if (m_listener.accept(*client) != sf::Socket::Done) {
            std::cout << "Error: could not accept client" << std::endl;
            return;
        }
      

        std::thread receiveMessages(&Server::ReceiveMessages, this, std::move(client));
        receiveMessages.detach();
    }
}

void Server::ReceiveMessages(std::unique_ptr<sf::TcpSocket> a_client)
{
    std::cout << "New client connected: " << a_client->getRemoteAddress() << std::endl;
    m_clients.push_back(std::move(a_client));

    while (true) {
        char data[1024];
        std::size_t received;
        sf::Socket::Status status = m_clients.back()->receive(data, sizeof(data), received);
        if (status == sf::Socket::Done) {
            std::string receivedString(data, received);
            std::cout << "in ReceiveMessages: " << receivedString << "\n";
            if (receivedString.find(" sent request to ") != std::string::npos) {
                sendGameRequest(receivedString);
            } else if (receivedString.find(" accepted ") != std::string::npos) {
                acceptInvite(receivedString);
            } else if (receivedString.find(" rejected ") != std::string::npos) {
                rejectInvite(receivedString);
            } else {
                std::cout << "Name received: " << receivedString << std::endl;
                m_activePlayers.insert(std::make_pair(m_numOfPlayers, receivedString));
                m_numOfPlayers++;

                for (size_t i = 0; i < m_clients.size(); i++) {
                    for (const auto &[key, value] : m_activePlayers) {
                        if (i != key) {
                            std::string name = value;
                            name += '\0';
                            if (m_clients[i]->send(name.c_str(), name.size()) != sf::Socket::Done) {
                                continue;
                            }
                        }
                    }
                }
            }
        } else {
            continue;
        }
    }
}

void Server::sendGameRequest(std::string a_msg)
{
    std::string player1, player2;
    std::tie(player1, player2) = getNames(a_msg);
    std::cout << "in sendGameRequest " << a_msg <<  "    in sendGameRequest "<< "\n";

    std::cout << "player1: " << player1 << std::endl;
    std::cout << "player2: " << player2 << std::endl;

    for (size_t i = 0; i < m_clients.size(); i++) {
        for (const auto &[key, value] : m_activePlayers) {
            if (value.compare(player2) == 0 && i == key) {
                std::string request;
                std::string clientNewIp = m_poolIP.GetIpFromPool();
                std::string newPort = "5100";
                if (a_msg.find("playAgainst") != std::string::npos) {
                    request = "|" + player1 + "| want to play against " + player2 + " port:" + newPort;
                } else {
                    request = "|" + player1 + "| want to play with " + player2 + " port:" + newPort;
                }
                std::cout << "in sendGameRequest:" << request;
                request += '\0';
                if (m_clients[i]->send(request.c_str(), request.size()) != sf::Socket::Done) {
                    continue;
                }
            }
        }
    }
}


void Server::rejectInvite(std::string a_msg)
{
    std::string firstPlayer, secondPlayer;
    std::vector<std::string> names = getSubstringsBetweenBars(a_msg);
    std::cout << "in rejectInvite " << a_msg << "\n";
    firstPlayer = names[0];
    secondPlayer = names[1];
    std::cout << "player1: " << firstPlayer << std::endl;
    std::cout << "player2: " << secondPlayer << std::endl;

    for (size_t i = 0; i < m_clients.size(); i++) {
        for (const auto &[key, value] : m_activePlayers) {
            if (value.compare(secondPlayer) == 0 && i == key) {
                std::string reject = "Your game request has been rejected by " + secondPlayer;
                reject += '\0';
                std::cout << "inrejectInvite: " << reject << "\n";
                if (m_clients[i]->send(reject.c_str(), reject.size()) != sf::Socket::Done) {
                    continue;
                }
            }
        }
    }
}

//getNames function is responsible for getting the names of the players from the message
std::tuple<std::string, std::string> Server::getNames(std::string a_msg)
{
    std::string player1;
    std::string player2;

    std::string with = " sent request to Play with ";
    std::string against = " sent request to Play against ";

    size_t withIndex = a_msg.find(with);
    size_t againstIndex = a_msg.find(against);

    if (withIndex != std::string::npos) {
        player1 = a_msg.substr(0, withIndex);
        player2 = a_msg.substr(withIndex + with.length());
    } else {
        player1 = a_msg.substr(0, againstIndex);
        player2 = a_msg.substr(againstIndex + against.length());
    }

    return std::make_tuple(player1, player2);
}

std::vector<std::string> Server::getSubstringsBetweenBars(const std::string &s)
{
    std::vector<std::string> substrings;

    size_t start = s.find('|');
    size_t end = s.find('|', start + 1);
    while (start != std::string::npos && end != std::string::npos)
    {
        substrings.push_back(s.substr(start + 1, end - start - 1));
        start = s.find('|', end + 1);
        end = s.find('|', start + 1);
    }

    return substrings;
}

void Server::acceptInvite(std::string a_msg)
{
    std::vector<std::string> names = getSubstringsBetweenBars(a_msg);
    std::cout << "in acceptInvite " << a_msg << "\n";
    std::string firstPlayer = names[2];
    std::string request = names[1];
    std::string secondPlayer = names[0];
    std::cout << "player1: " << firstPlayer << std::endl;
    std::cout << "player2: " << secondPlayer << std::endl;
    std::string newPort = "5100";

    for (size_t i = 0; i < m_clients.size(); i++)
    {
        for (const auto &[key, value] : m_activePlayers)
        {
            if (value.compare(firstPlayer) == 0 && i == key)
            {
                std::string acceptRequest = firstPlayer + " accept " + request + "you port:" + newPort;
                acceptRequest += '\0';
                std::cout << "acceptInvite: " << acceptRequest << "\n";
                if (m_clients[i]->send(acceptRequest.c_str(), acceptRequest.size()) != sf::Socket::Done)
                {
                    continue;
                }
            }
        }
    }
}

Server::~Server()
{
    m_listener.close();
}

int main()
{
    Server serv;
    serv.AcceptClients();
}
