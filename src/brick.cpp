#include "brick.hpp"
#include <iostream>

Brick::Brick(sf::Vector2f& a_size, sf::Color a_color, int a_x, int a_y, int a_maxCollisions, int a_score)
: m_x(a_x)
, m_y(a_y)
, m_width(a_size.x)
, m_height(a_size.y)
, m_color(a_color)
, m_maxCollisions(a_maxCollisions)
, m_score(a_score)
, m_brick(a_size)
{
    m_brick.setFillColor(m_color);
    m_brick.setPosition(m_x, m_y);
}

void Brick::draw(sf::RenderWindow &a_win)
{
    a_win.draw(m_brick);
}

float Brick::getTop()
{
    return m_y;
}

float Brick::getBottom()
{
    return m_y + m_height;
}

float Brick::getRight()
{
    return m_x + m_width;
}

float Brick::getLeft()
{
    return m_x;
}

int Brick::getMaxCollisions()
{
    return m_maxCollisions;
}

int Brick::getScore()
{
    return m_score;
}

void Brick::addCollision()
{
    m_maxCollisions--;
}

sf::Color Brick::getColor()
{
    return m_color;
}

void Brick::setColor(sf::Color a_color)
{
    m_color = a_color;
    m_brick.setFillColor(m_color);
}

