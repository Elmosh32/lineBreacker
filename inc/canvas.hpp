#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <tuple>
#include "resourceManager.hpp"

class Canvas
{
public:
    // Canvas(int a_width, int a_height);
    Canvas(int a_width, int a_height);
    Canvas(int a_width, int a_height, sf::RenderWindow &win);
    // ~Canvas();
    void draw(sf::CircleShape a_circle, sf::RenderWindow &win);
    void draw(sf::RectangleShape a_rectangle, sf::RenderWindow &win);

    void createNewRenderWindow();
    std::string getName(sf::RenderWindow &a_win);
    std::tuple<std::string, std::string> getNameAndIP(bool a_withIP, sf::RenderWindow &a_win);
    std::string getIP(sf::RenderWindow &a_win);

    // sf::RenderWindow *getCanvas();
    sf::RenderWindow &getCanvas();
    int getWidth();
    int getHeight();
    void drawHearts(int a_numOfHearts, sf::RenderWindow &window);
    void drawSoundLogo(bool a_soundStatus, sf::RenderWindow &win);
    void drawScore(int a_score, sf::RenderWindow &win);
    void drawGameOver(sf::RenderWindow &win);
    void drawGameBackground(sf::RenderWindow &win);
    void drawLeaderboard(std::vector<std::tuple<std::string, std::string>> a_topTen, sf::RenderWindow &win);

    void drawOptionsWindow(sf::RenderWindow &win);
    sf::RectangleShape getStartGameBox();
    sf::RectangleShape getMultiplayerGameBox();
    sf::RectangleShape getLeaderboardBox();
    sf::RectangleShape getExitGameBox();
    void drawOptionWindowBackground(sf::RenderWindow &win);
    void drawGameText(sf::RenderWindow &win);
    void drawStartGameText(sf::RenderWindow &win);
    void drawMultiplayerText(sf::RenderWindow &win);
    void drawLeaderboardText(sf::RenderWindow &win);
    void drawExitGameText(sf::RenderWindow &win);
    void drawEnterName(sf::RenderWindow &win);
    void drawEnterIp(sf::RenderWindow &win);
    // void close();

    // void clearCanvas();
    std::vector<std::tuple<sf::RectangleShape, std::string>> getPlayerBoxes();


    sf::IntRect pushNewPlayer(std::string a_name, int a_numOfPlayer);
    void drawLastPlayer(sf::RenderWindow &win);
    void drawAllPlayers(sf::RenderWindow &win);
    void drawOnlinePlayersText(sf::RenderWindow &win);
    void drawPlayerConnect(std::string a_name, sf::RenderWindow &win);
    sf::RectangleShape getBox(int width, int height, float recWidth, float recHeight);
    std::vector<std::tuple<sf::IntRect, std::string>> drawGameOptions(std::string &a_msg, std::string &a_firstOption, std::string &a_secondOption, sf::RenderWindow &win);
    std::vector<sf::IntRect> drawGameRequest(std::string a_request, sf::RenderWindow &win);
    void drawPlayerDisconnect(std::string a_name, sf::RenderWindow &win);

    void drawGameRequestRejected(std::string &a_msg, sf::RenderWindow &win);
private:
    int m_canvasWidth;
    int m_canvasHeight;
    // sf::RenderWindow m_win;
    // std::unique_ptr<sf::RenderWindow> m_canvas;
    std::vector<std::tuple<sf::Text, sf::RectangleShape>> m_playerBoxes;
};
#endif // CANVAS_HPP
