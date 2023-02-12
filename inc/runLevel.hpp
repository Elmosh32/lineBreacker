#ifndef RUN_LEVEL_HPP
#define RUN_LEVEL_HPP

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

#include "client.hpp"
#include "server.hpp"

class RunLevel
{
public:
    RunLevel(int a_width, int a_height, sf::RenderWindow &a_win);

    ~RunLevel() = default;
    RunLevel &operator=(const RunLevel &a_otherLevel);

    void nextLevel();
    std::string playLevel();
    std::vector<std::tuple<std::string, std::string>> updateLeaderboard(int a_score);

private:
    bool readLevelParameters(int a_levelNumber);
    std::string gameCollision();
    bool checkPaddleCollision();

    CollisionDirection checkBricksCollision(Brick &a_brick);
    CollisionDirection DirectionOfCollision();
    CollisionDirection brickHorizontalCollision();
    void handleBrickCollision(CollisionDirection a_direction, Brick &a_brick);
    void drawAll(bool a_soundStatus);

private:
    Canvas m_canvas;
    Paddle m_paddle;
    Ball m_ball;
    sf::RenderWindow &m_win;
    std::vector<Brick> m_bricks;

    int m_lives = 3;
    int m_level = 1;
    int m_totalScore = 0;
};

#endif // RUN_LEVEL_HPP
