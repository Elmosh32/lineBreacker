#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "canvas.hpp"
#include "ball.hpp"
#include "paddle.hpp"
#include "playerManager.hpp"
#include "server.hpp"
#include "client.hpp"

class GameManager
{
public:
    GameManager(int a_width, int a_height, sf::RenderWindow &win);
    void chosePlayer();
    void createClient();
    void startGame(std::string a_playerName);
    void getGameRequest();
    void getNewPlayers();
    void drawplayer(std::string a_playerName);
    void chosePlayerToPlay(std::string a_playerName);
    std::string getServerIp();
    std::string getServerPort();
    std::string choseOption(std::string a_playerName);
    std::vector<std::tuple<sf::IntRect, std::string>> getPlayersRects();
    void sendGameRequest(std::string a_firstPlayer, std::string a_secondPlayer);
    void getGameRequest(std::string a_msg);
    std::tuple<std::string, std::string> getNames(std::string a_msg);
    void update();
    void draw();
    void processInput();
    void sendUpdates();
    void cleanup();
    void updateScore(int a_score);
    void Lose();
    void NextLevel();
    void runGame();
    void runNextLevel();
    void EndGame();
    void Connect(); 
    void ReceiveMessages();
    void AcceptClients();
    void signalHandler(int signum);
    

private:
    PlayerManager m_firstPlayer;
    PlayerManager m_secondPlayer;
    Canvas m_canvas;
    Paddle m_paddle;
    Ball m_ball;
    
    std::string playerType;
    int m_lives = 3;
    int m_totalScore = 0;
};
