#include "collision.hpp"


bool Collision::checkPaddleCollision(Ball a_ball, Paddle a_paddle)
{
    if (a_ball.getX() < a_paddle.getRight() && a_ball.getX() > a_paddle.getLeft() - 2 * a_ball.getRadius() && a_ball.getY() > a_paddle.getTop() - 2 * a_ball.getRadius()){
        return true;
    }

    return false;
}

CollisionDirection Collision::checkBricksCollision(Ball a_ball, Brick &a_brick)
{
    float aboveTheRectangle = a_ball.getY() - a_brick.getTop();
    float underTheRectangle = a_brick.getBottom() - a_ball.getY();
    float inRightOfRectangle = a_brick.getRight() - a_ball.getX();
    float inLeftOfRectangle = a_ball.getX() - a_brick.getLeft();

    if (inLeftOfRectangle > 0 && inRightOfRectangle > 0 && aboveTheRectangle > 0 && underTheRectangle > 0){
        float min = std::min(aboveTheRectangle, std::min(underTheRectangle, std::min(inRightOfRectangle, inLeftOfRectangle)));

        if (min == aboveTheRectangle){
            return CollisionDirection::Top;
        } else if (min == underTheRectangle) {
            return CollisionDirection::Bottom;
        } else if (min == inRightOfRectangle){
            return CollisionDirection::Right;
        } else {
            return CollisionDirection::Left;
        }
    }

    return CollisionDirection::NoCollision;
}

void Collision::handleBrickCollision(Ball a_ball, CollisionDirection a_direction, Brick &a_brick)
{

    if (a_direction == CollisionDirection::Top) {
        a_ball.handleToplBrickCollision(a_brick.getTop());
        return;
    }

    if (a_direction == CollisionDirection::Bottom) {
        a_ball.handlelBottomBrickCollision(a_brick.getBottom());
        return;
    }

    if (a_direction == CollisionDirection::Right) {
        a_ball.handlelBottomBrickCollision(a_brick.getRight());
        return;
    }

    if (a_direction == CollisionDirection::Left) {
        a_ball.handlelBottomBrickCollision(a_brick.getLeft());
    }
}

