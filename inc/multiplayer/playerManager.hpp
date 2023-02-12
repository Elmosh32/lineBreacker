#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <string>
#include <sstream>
#include <tuple>

#include "resourceManager.hpp"
#include "canvas.hpp"
#include "paddle.hpp"
#include "brick.hpp"
#include "ball.hpp"
#include "collision.hpp"

// #include "client.hpp"
// #include "server.hpp"
#include "safeQueue.hpp"

class PlayerManager
{
public:
    PlayerManager(int a_width, int a_height, sf::RenderWindow &a_win, SafeQueue<sf::Vector2f> &a_ballPositionQueueFirst, SafeQueue<sf::Vector2f> &a_paddlePositionQueueFirst, SafeQueue<sf::Vector2f> &a_ballPositionQueueSecond, SafeQueue<sf::Vector2f> &a_paddlePositionQueueSecond);
    ~PlayerManager() = default;
    PlayerManager &operator=(const PlayerManager &a_otherLevel);

    void nextLevel(int a_level);
    std::string playLevel();
    std::vector<std::tuple<std::string, std::string>> updateLeaderboard(int a_score);

private:
    bool readLevelParameters(int a_levelNumber);
    void lose();
    std::string gameCollision();
    bool checkPaddleCollision();
    CollisionDirection checkBricksCollision(Brick &a_brick);
    CollisionDirection DirectionOfCollision();
    CollisionDirection brickHorizontalCollision();
    void handleBrickCollision(CollisionDirection a_direction, Brick &a_brick);
    void drawAll(bool a_soundStatus);
    void moveSecondPlayerElements();

private:
    Canvas m_canvas;
    sf::RenderWindow &m_win;

    Paddle m_firstPlayerPaddle;
    Paddle m_secondPlayerPaddle;

    Ball m_firstPlayerBall;
    Ball m_secondPlayerBall;

    SafeQueue<sf::Vector2f> &m_ballPositionQueueFirst;
    SafeQueue<sf::Vector2f> &m_paddlePositionQueueFirst;

    SafeQueue<sf::Vector2f> &m_ballPositionQueueSecond;
    SafeQueue<sf::Vector2f> &m_paddlePositionQueueSecond;

    std::vector<Brick> m_bricks;

    std::string playerType = "bot";

    int m_level = 1;
    int m_lives = 3;
    int m_totalScore = 0;
};
