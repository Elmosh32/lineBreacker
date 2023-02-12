#include "point.hpp"

Point::Point(float a_x, float a_y)
: m_x(a_x)
, m_y(a_y)
{};

float Point::getX()
{
    return m_x;
}

float Point::getY()
{
    return m_y;
}

void Point::setX(float a_x)
{
    m_x = a_x;
}

void Point::setY(float a_y)
{
    m_y = a_y;
}