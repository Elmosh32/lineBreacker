#ifndef BRICK_HPP
#define BRICK_HPP

#include <SFML/Graphics.hpp>

#include "canvas.hpp"

class Brick
{
public:
    Brick(sf::Vector2f &a_size, sf::Color a_color, int a_x, int a_y, int a_maxCollisions, int a_score);
    void draw(sf::RenderWindow &a_win);

    void addCollision();
    int getMaxCollisions();

    float getTop();
    float getBottom();
    float getRight();
    float getLeft();
    int getScore();
    
    sf::Color getColor();
    void setColor(sf::Color a_color);


private:
    int m_x;
    int m_y;

    size_t m_width;
    size_t m_height;

    sf::Color m_color;

    int m_maxCollisions;
    int m_score;
    
    sf::RectangleShape m_brick;
};

#endif // BRICK_HPP
