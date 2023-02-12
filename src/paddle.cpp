#include "paddle.hpp"
#include <iostream>

Paddle::Paddle(sf::Vector2f a_size, sf::Color a_color, size_t a_canvasWidth, size_t a_canvasHeight, std::string a_position)         
: m_canvasWidth(a_canvasWidth)
, m_canvasHeight(a_canvasHeight)                                                                                  
, m_width(a_size.x)
, m_height(a_size.y)
, m_x((m_canvasWidth - m_width) / 2)
, m_y( m_canvasHeight - (m_height + 30))
, m_color(a_color)
, m_paddle(sf::Vector2f(m_width, m_height))
{
    if (a_position == "top") {
        m_y = 20;
        m_paddle.setPosition(m_x, m_y);
    }else {
        m_paddle.setPosition(m_x, m_y);
    }

    m_paddle.setFillColor(m_color);
    m_paddle.setOutlineThickness(2);
    m_paddle.setOutlineColor(sf::Color::Red);
}

sf::Vector2f Paddle::moveToLeft()
{
    if (m_paddle.getPosition().x > 0) {
        m_x = m_paddle.getPosition().x - 20;
        m_paddle.setPosition(m_x, m_y);
        // m_paddle.move(-10, 0);
        // m_paddle.getPosition();
    }

    return m_paddle.getPosition();
}

sf::Vector2f Paddle::moveToRight()
{
    if (m_paddle.getPosition().x < m_canvasWidth - m_paddle.getSize().x) {
        m_x = m_paddle.getPosition().x + 20;
        m_paddle.setPosition(m_x, m_y);
            // m_paddle.move(10, 0);
    }

    return m_paddle.getPosition();
}

void Paddle::movePaddle(sf::Vector2f a_position)
{
    m_paddle.setPosition(a_position);
}

void Paddle::draw(sf::RenderWindow &a_win)
{
    a_win.draw(m_paddle);
}

float Paddle::getTop()
{
    return m_y;
}

float Paddle::getBottom()
{
    return m_y + m_height;
}

float Paddle::getRight()
{
    return m_x + m_width;
}

float Paddle::getLeft()
{
    return m_x;
}

