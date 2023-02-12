#ifndef BALL_HPP
#define BALL_HPP

#include <SFML/Graphics.hpp>
#include <unistd.h>

#include "canvas.hpp"

enum class VerticalCollision
{
    NoCollision,
    Top,
    Bottom
};
enum class HorizontalCollision
{
    NoCollision,
    Left,
    Right
};

class Ball
{
public:
    Ball(sf::Vector2f a_center, float a_radius, int a_speed, sf::Color a_color, size_t a_canvasWidth, size_t a_canvasHeight);

    void drawBall(sf::RenderWindow &a_win);
    sf::Vector2f move(Canvas &a_canvas);
    sf::Vector2f moveToMidOfPaddle(float a_x, float a_y);
    void moveBall(sf::Vector2f a_size);

    void checkCanvasCollision(Canvas &a_canvas);
    VerticalCollision checkCanvasTopCollision(Canvas &a_canvas);
    bool checkCanvasBottomCollision(Canvas &a_canvas);
    VerticalCollision checkTopCollision(Canvas &a_canvas);
    HorizontalCollision checkHorizontalCollision(Canvas &a_canvas);
    void handleHorizontalCollision(HorizontalCollision a_route, Canvas &a_canvas);
    void handleCanvasTopCollision();
    void handleCanvasBottomCollision(sf::Vector2f a_newPoint);

    void handlePaddleCollision(float a_topPaddle);

    void handleToplBrickCollision(float a_topBrick);
    void handlelBottomBrickCollision(float a_bottomBrick);
    void handlelRightBrickCollision(float a_rightBrick);
    void handlelLeftBrickCollision(float a_leftBrick);

    float getRadius();

    float getX();
    float getY();
    void setX(float a_x);
    void setY(float a_num);
    void changeXDirection();
    void changeYDirection();

private:
    sf::Vector2f m_center;
    float m_radius;
    int m_speed;

    sf::CircleShape m_ball;
    sf::Color m_color;

    int m_xPosition = -1;
    int m_yPosition = -1;

    size_t m_canvasWidth;
    size_t m_canvasHeight;
};

#endif 