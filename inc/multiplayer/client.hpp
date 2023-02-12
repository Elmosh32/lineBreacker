#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <X11/Xlib.h>

#undef None
#define None wNone

#undef Status
#define Status wStatus

#undef BadRequest
#define BadRequest wBadRequest

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <cstring>
#include <sstream>
#include <thread>
#include <atomic>
#include <tuple>
#include <mutex>
#include <functional>

#include <future>

#include "canvas.hpp"
#include "playerManager.hpp"

#include "threadPoolExecuter.hpp"

    class Client
{
public:
    // Client(int a_width, int a_height);
    Client(int a_width, int a_height, sf::RenderWindow &win);
    void createClient();
    std::string getGameRequest();

private:
    std::vector<std::string> split(const std::string& s, char delimiter);
    std::string sendGameRequest();
    std::vector<std::tuple<sf::IntRect, std::string>> getPlayersRects();
    void handleOutcomingGameRequest(std::string &a_secondPlayer);
    bool handleIncomingGameRequest(std::string &a_msg);
    std::string choseOption(std::string &a_msg, std::string &a_firstOption, std::string &a_secondOption);
    std::tuple<std::string, std::string> getNames(std::string &a_msg);
    std::string waitGameRequestResponse();
    bool findSubString(std::string a_string, std::string a_subString);
    void connectFirstPlayer(std::string a_newPort);
    void connectSecondPlayer(std::string a_newPort);
    void runGame();
private:
    Canvas m_canvas;
    sf::RenderWindow &m_win;

    sf::TcpSocket m_socket;
    std::vector<std::tuple<sf::IntRect, std::string>> m_onlinePlayers;
    std::mutex m_onlinePlayersMutex;
    std::string m_playerName;
    // size_t m_numOfPlayers = 0;
    SafeQueue<sf::Vector2f> m_ballPositionQueueFirst;
    SafeQueue<sf::Vector2f> m_paddlePositionQueueFirst;

    SafeQueue<sf::Vector2f> m_ballPositionQueueSecond;
    SafeQueue<sf::Vector2f> m_paddlePositionQueueSecond;
    std::atomic_int m_numOfPlayers = 0;
    std::atomic_bool m_playerChosen = false;
    std::atomic_bool m_stop = false;
    std::atomic_bool m_response = false;

    size_t m_port = 5000;
};

#endif // CLIENT_HPP
