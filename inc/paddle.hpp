#ifndef PADDLE_HPP
#define PADDLE_HPP

#include <SFML/Graphics.hpp>

#include "canvas.hpp"

class Paddle
{
public:
    Paddle(sf::Vector2f a_size, sf::Color a_color, size_t a_canvasWidth, size_t a_canvasHeight, std::string a_position);

    void draw(sf::RenderWindow &a_win);
    void erase(Canvas &a_canvas);

    sf::Vector2f moveToLeft();
    sf::Vector2f moveToRight();
    void movePaddle(sf::Vector2f a_size);
    sf::Vector2f &getPaddlePosition();

    float getTop();
    float getBottom();
    float getRight();
    float getLeft();

private:

    size_t m_canvasWidth;
    size_t m_canvasHeight;

    size_t m_width;
    size_t m_height;
    
    int m_x;
    int m_y;

    sf::Color m_color;
    sf::RectangleShape m_paddle;
};


#endif // PADDLE_HPP
