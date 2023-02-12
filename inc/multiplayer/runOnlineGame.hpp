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
#include "playerManager.hpp"

#include "client.hpp"
#include "server.hpp"

class RunOnlineGame
{
public:
    RunOnlineGame();
    ~RunOnlineGame() = default;
    RunOnlineGame &operator=(const RunOnlineGame &a_otherLevel);

    void nextLevel();
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
    int getLevel();
    void drawAll(bool a_soundStatus);

private:
    PlayerManager PlayerOne;
    PlayerManager PlayerTwo;

};

#endif // RUN_LEVEL_HPP
