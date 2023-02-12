#include "ball.hpp"

#include <iostream>

Ball::Ball(sf::Vector2f a_center, float a_radius, int a_speed, sf::Color a_color, size_t a_canvasWidth, size_t a_canvasHeight)
: m_center(a_center)
, m_radius(a_radius)
, m_speed(a_speed)
, m_ball(m_radius)
, m_color(a_color)
, m_canvasWidth(a_canvasWidth)
, m_canvasHeight(a_canvasHeight)
{
    m_ball.setPosition(m_center.x, m_center.y);
    m_ball.setOutlineColor(sf::Color::Magenta);
    m_ball.setFillColor(m_color);
    m_ball.setOutlineColor(sf::Color::Cyan);
    m_ball.setOutlineThickness(1);
    // sf::Texture *texture;
    // (*texture).loadFromFile("../ball.png");
    // // sf::Sprite sprite;
    // // sprite.setTexture(texture);
    // // sf::Vector2f targetSize(22, 22);
    // // sprite.setScale(
    // //     targetSize.x / sprite.getLocalBounds().width,
    // //     targetSize.y / sprite.getLocalBounds().height);
    // // sprite.setPosition(a_ball.getPosition().x+30,a_ball.getPosition().y);

    // // m_canvas.draw(sprite);
    // a_ball.setTexture(texture);
}

sf::Vector2f Ball::move(Canvas &a_canvas)
{
    m_center.x += m_speed * m_xPosition;
    m_center.y += m_speed * m_yPosition;

    m_ball.setPosition(m_center.x, m_center.y);
    return m_ball.getPosition();
}

sf::Vector2f Ball::moveToMidOfPaddle(float a_x, float a_y)
{
    m_center.x = a_x;
    m_center.y = a_y;

    m_ball.setPosition(m_center.x, m_center.y);
    return m_ball.getPosition();
}

void Ball::moveBall(sf::Vector2f a_position)
{
    m_ball.setPosition(a_position.x,  a_position.y - 20);
}

void Ball::checkCanvasCollision(Canvas &a_canvas)
{
    HorizontalCollision horizontalRes = checkHorizontalCollision(a_canvas);
    if (horizontalRes != HorizontalCollision::NoCollision)
    {
        handleHorizontalCollision(horizontalRes, a_canvas);
        m_xPosition *= -1;
    }

    if (checkCanvasTopCollision(a_canvas) != VerticalCollision::NoCollision)
    {
        handleCanvasTopCollision();
        m_yPosition *= -1;
    }
}


HorizontalCollision Ball::checkHorizontalCollision(Canvas &a_canvas)
{
    if (m_center.x >= a_canvas.getWidth() - m_radius) {
        return HorizontalCollision::Right;
    }

    if (m_center.x < 0) {
        return HorizontalCollision::Left;
    }

    return HorizontalCollision::NoCollision;
}

VerticalCollision Ball::checkCanvasTopCollision(Canvas &a_canvas)
{
    if (m_center.y < 0) {
        return VerticalCollision::Top;
    }

    return VerticalCollision::NoCollision;
}

bool Ball::checkCanvasBottomCollision(Canvas &a_canvas)
{
    if (m_center.y >= a_canvas.getHeight() - m_radius)
    {
        return true;
    }

    return false;
}

void Ball::handleHorizontalCollision(HorizontalCollision a_route, Canvas &a_canvas)
{
    if (a_route == HorizontalCollision::Right) {
        while (m_center.x > a_canvas.getWidth() - m_radius) {
            m_center.x--;
        }

        return;
    }

    while (m_center.x < 0) {
        m_center.x++;
    }

}

void Ball::handleCanvasTopCollision()
{
    while (m_center.y < 0) {
        m_center.y++;
    }
}


void Ball::handlePaddleCollision(float a_top)
{
    while (m_center.y > a_top - 2 * m_ball.getRadius())
    {
        m_center.y--;
    }

    m_yPosition *= -1;
}


void Ball::handleToplBrickCollision(float a_topBrick)
{
    while (m_center.y < a_topBrick) {
        m_center.y--;
    }
}

void Ball::handlelBottomBrickCollision(float a_bottomBrick)
{
    while (m_center.y < a_bottomBrick) {
        m_center.y++;
    }
}

void Ball::handlelRightBrickCollision(float a_rightBrick)
{
    while (m_center.x < a_rightBrick) {
        m_center.x--;
    }
}

void Ball::handlelLeftBrickCollision(float a_leftBrick)
{
    while (m_center.x > a_leftBrick) {
        m_center.x++;
    }
}

float Ball::getX()
{
    return m_center.x;
}

float Ball::getY()
{
    return m_center.y;
}

void Ball::setX(float a_x)
{
    m_center.x = a_x;
}

void Ball::setY(float a_y)
{
    m_center.y = a_y;
}

void Ball::drawBall(sf::RenderWindow &a_win)
{
    a_win.draw(m_ball);
}

float Ball::getRadius()
{
    return m_radius;
}

void Ball::changeXDirection()
{
    m_xPosition *= -1;
}

void Ball::changeYDirection()
{
    m_yPosition *= -1;
}
