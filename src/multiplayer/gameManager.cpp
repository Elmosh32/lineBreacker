#include "client.hpp"
#include "gameManager.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <csignal>
#include <thread>
#include <chrono>

GameManager::GameManager(int a_width, int a_height, sf::RenderWindow &win)
    : m_canvas(a_width, a_height, win)
{
}

GameManager::void chosePlayer()
{
    std::cout << "Choose player type: 1. Server 2. Client" << std::endl;
    std::cin >> playerType;
}

GameManager::void createClient()
{
    m_client = std::make_unique<Client>();
}

GameManager::void startGame(GameManager::std::string a_playerName)
{
    m_canvas.setPlayerName(a_playerName);
    m_canvas.setGameStarted(true);
}

GameManager::void getGameRequest()
{
    std::string msg = m_client->receive();
    std::tuple<std::string, std::string> names = getNames(msg);
    m_canvas.setPlayerChosenToPlayName(std::get<0>(names));
    m_canvas.setPlayerChosenName(std::get<1>(names));
    m_canvas.setGameRequest(true);
}

GameManager::void getNewPlayers()
{
    std::string msg = m_client->receive();
    std::vector<std::tuple<sf::IntRect, std::string>> playersRects = getPlayersRects(msg);
    m_canvas.setPlayersRects(playersRects);
    m_canvas.setNewPlayers(true);
}

GameManager::void drawplayer(GameManager::std::string a_playerName)
{
    m_canvas.setPlayerChosenName(a_playerName);
    m_canvas.setPlayerChosen(true);
}

GameManager::void chosePlayerToPlay(GameManager::std::string a_playerName)
{
    m_canvas.setPlayerChosenToPlayName(a_playerName);
    m_canvas.setPlayerChosenToPlay(true);
}

GameManager::std::string getServerIp()
{
    std::string ip;
    std::cout << "Enter server ip: ";
    std::cin >> ip;
    return ip;
}

GameManager::std::string getServerPort()
{
    std::string port;
    std::cout << "Enter server port: ";
    std::cin >> port;
    return port;
}

GameManager::std::string choseOption(GameManager::std::string a_playerName)
{
    std::string option;
    std::cout << "Choose option: 1. Play with " << a_playerName << " 2. Play with new player" << std::endl;
    std::cin >> option;
    return option;
}

GameManager::std::vector<std::tuple<sf::IntRect, GameManager::std::string>> getPlayersRects()
{
    std::string msg = m_client->receive();
    std::vector<std::tuple<sf::IntRect, std::string>> playersRects;
    std::string delimiter = ";";
    size_t pos = 0;
    std::string token;
    while ((pos = msg.find(delimiter)) != std::string::npos)
    {
        token = msg.substr(0, pos);
        std::string delimiter2 = ",";
        size_t pos2 = 0;
        std::string token2;
        std::string name;
        sf::IntRect rect;
        while ((pos2 = token.find(delimiter2)) != std::string::npos)
        {
            token2 = token.substr(0, pos2);
            if (token2.find("name") != std::string::npos)
            {
                name = token2.substr(token2.find(":") + 1);
            }
            else if (token2.find("rect") != std::string::npos)
            {
                std::string delimiter3 = ":";
                size_t pos3 = 0;
                std::string token3;
                std::vector<int> rectValues;
                while ((pos3 = token2.find(delimiter3)) != std::string::npos)
                {
                    token3 = token2.substr(0, pos3);
                    if (token3.find("x") != std::string::npos)
                    {
                        rectValues.push_back(std::stoi(token3.substr(token3.find("x") + 1)));
                    }
                    else if (token3.find("y") != std::string::npos)
                    {
                        rectValues.push_back(std::stoi(token3.substr(token3.find("y") + 1)));
                    }
                    else if (token3.find("w") != std::string::npos)
                    {
                        rectValues.push_back(std::stoi(token3.substr(token3.find("w") + 1)));
                    }
                    else if (token3.find("h") != std::string::npos)
                    {
                        rectValues.push_back(std::stoi(token3.substr(token3.find("h") + 1)));
                    }
                    token2.erase(0, pos3 + delimiter3.length());
                }
                rect = sf::IntRect(rectValues[0], rectValues[1], rectValues[2], rectValues[3]);
            }
            token.erase(0, pos2 + delimiter + 1); 
        }
        playersRects.push_back(std::make_tuple(rect, name));
    }
    return playersRects;
}

GameManager::void sendGameRequest(GameManager::std::string a_firstPlayer, GameManager::std::string a_secondPlayer)
{
    std::string msg = "gameRequest:" + a_firstPlayer + "," + a_secondPlayer;
    m_client->send(msg);
}

GameManager::void getGameRequest(GameManager::std::string a_msg)
{
    std::tuple<std::string, std::string> names = getNames(a_msg);
    m_canvas.setPlayerChosenToPlayName(std::get<0>(names));
    m_canvas.setPlayerChosenName(std::get<1>(names));
    m_canvas.setGameRequest(true);
}

GameManager::std::tuple<GameManager::std::string, GameManager::std::string> getNames(GameManager::std::string a_msg)
{
    std::string delimiter = ":";
    size_t pos = 0;
    std::string token;
    std::string firstPlayer;
    std::string secondPlayer;
    while ((pos = a_msg.find(delimiter)) != std::string::npos)
    {
        token = a_msg.substr(0, pos);
        if (token.find("firstPlayer") != std::string::npos)
        {
            firstPlayer = token.substr(token.find(":") + 1);
        }
        else if (token.find("secondPlayer") != std::string::npos)
        {
            secondPlayer = token.substr(token.find(":") + 1);
        }
        a_msg.erase(0, pos + delimiter.length());
    }
    return std::make_tuple(firstPlayer, secondPlayer);
}

GameManager::void update()
{
    m_canvas.update();
}

GameManager::void draw()
{
    m_canvas.draw();
}

GameManager::void processInput()
{
    m_canvas.processInput();
}

GameManager::void sendUpdates()
{
    m_client->send(m_canvas.getRect());
}


GameManager::void cleanup()
{
    m_client->close();
}

GameManager::void updateScore(int a_score)
{
    m_canvas.setScore(a_score);
}

GameManager::void Lose()
{
    m_canvas.setLose(true);
}

GameManager::void NextLevel()
{
    m_canvas.setNextLevel(true);
}

GameManager::void runGame()
{
    m_canvas.setGame(true);
}

GameManager::void runNextLevel()
{
    m_canvas.setNextLevel(true);
}

GameManager::void EndGame()
{
    m_canvas.setEndGame(true);
}

GameManager::void Connect()
{
    m_canvas.setConnect(true);
}

GameManager::void ReceiveMessages()
{
    while (m_client->isConnected())
    {
        std::string msg = m_client->receive();
        if (msg.find("gameRequest") != std::string::npos)
        {
            getGameRequest(msg);
        }
        else if (msg.find("update") != std::string::npos)
        {
            m_canvas.setPlayersRects(getPlayersRects());
        }
        else if (msg.find("score") != std::string::npos)
        {
            updateScore(std::stoi(msg.substr(msg.find(":") + 1)));
        }
        else if (msg.find("lose") != std::string::npos)
        {
            Lose();
        }
        else if (msg.find("nextLevel") != std::string::npos)
        {
            NextLevel();
        }
        else if (msg.find("runGame") != std::string::npos)
        {
            runGame();
        }
        else if (msg.find("runNextLevel") != std::string::npos)
        {
            runNextLevel();
        }
        else if (msg.find("endGame") != std::string::npos)
        {
            EndGame();
        }
        else if (msg.find("connect") != std::string::npos)
        {
            Connect();
        }
    }
}

GameManager::void AcceptClients()
{
    while (m_server->isConnected())
    {
        m_server->accept();
    }
}

GameManager::void signalHandler(int signum)
{
    std::cout << "Interrupt signal (" << signum << ") received.\n";
    m_client->close();
    m_server->close();
    exit(signum);
}

GameManager::void init()
{
    m_canvas.init();
    m_client = new Client();
    m_server = new Server();
    m_client->connect();
    m_server->start();
    m_client->send("connect");
    m_canvas.setConnect(true);
}

GameManager::void run()
{
    init();
    std::thread receiveMessages(&GameManager::ReceiveMessages, this);
    std::thread acceptClients(&GameManager::AcceptClients, this);
    while (m_canvas.isOpen())
    {
        processInput();
        update();
        draw();
        sendUpdates();
    }
    cleanup();
    receiveMessages.join();
    acceptClients.join();
}

