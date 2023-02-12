#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "resourceManager.hpp"
#include "ball.hpp"
#include "paddle.hpp"
#include "brick.hpp"

enum class CollisionDirection
{
    NoCollision,
    Top,
    Bottom,
    Left,
    Right,
    Collision
};

class Collision
{
public:
    bool checkPaddleCollision(Ball a_ball, Paddle a_paddle);

    CollisionDirection checkBricksCollision(Ball a_ball, Brick &a_brick);

    CollisionDirection DirectionOfCollision();
    CollisionDirection brickHorizontalCollision();
    void handleBrickCollision(Ball a_ball, CollisionDirection a_direction, Brick &a_brick);

};

#endif