#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "runLevel.hpp"
#include "canvas.hpp"
// #include "server.hpp"

class Game
{
public:
    Game(int a_width, int a_height, sf::RenderWindow &a_win);
    void gameStartWindow();


private:
    void startGame();
    void startOnlineGame();
    std::string choseOption();
    std::vector<std::tuple<std::string, std::string>> updateLeaderboard(int a_score);

private:
    Canvas m_canvas;
    sf::RenderWindow &m_win;
};

#endif // GAME_HPP
