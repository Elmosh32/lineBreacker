#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include <SFML/Graphics.hpp>
#include "point.hpp"
#include "canvas.hpp"

class Rectangle
{
public:
    Rectangle(Point a_leftTop, Point a_rightBottom, size_t a_width, size_t a_height, sf::Color a_color);

    void draw(Canvas &a_canvas);
    void erase(Canvas &a_canvas);

private:
    Point m_leftTop;
    Point m_rightBottom;

    size_t m_width;
    size_t m_height;
    
    sf::Color m_color;
};

#endif // RECTANGLE_HPP
