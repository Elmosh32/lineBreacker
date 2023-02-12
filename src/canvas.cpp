#include <SFML/Graphics.hpp>
#include <iostream>

#include "canvas.hpp"

#include <unistd.h>

// Canvas::Canvas(int a_width, int a_height)
//     : m_canvasWidth(a_width), m_canvasHeight(a_height)
//       // , m_win(sf::VideoMode(m_canvasWidth, m_canvasHeight), "Line Breaker")
//       ,
//       m_canvas(new sf::RenderWindow(sf::VideoMode(m_canvasWidth, m_canvasHeight), "Line Breaker"))
// {
// }

// Canvas::Canvas(int a_width, int a_height, sf::RenderWindow *win)
//     : m_canvasWidth(a_width), m_canvasHeight(a_height), m_canvas(win)
// {
// }

Canvas::Canvas(int a_width, int a_height)
    : m_canvasWidth(a_width)
    , m_canvasHeight(a_height)
    // , m_canvas(new sf::RenderWindow(sf::VideoMode(a_width, a_height), "Line Breaker"))
{
}

// Canvas::~Canvas()
// {
//     a_win.~RenderWindow();
// }

// sf::RenderWindow *Canvas::getCanvas()
// {
//     return m_canvas;
// }

// void Canvas::createNewRenderWindow()
// {

//     m_canvas = sf::RenderWindow window(sf::VideoMode(1200, 800), "Line Breaker");
// }
void Canvas::draw(sf::CircleShape a_ball, sf::RenderWindow &win)
{
    // sf::Texture *texture;
    // (*texture).loadFromFile("../ball.png");
    // // sf::Sprite sprite;
    // // sprite.setTexture(texture);
    // // sf::Vector2f targetSize(22, 22);
    // // sprite.setScale(
    // //     targetSize.x / sprite.getLocalBounds().width,
    // //     targetSize.y / sprite.getLocalBounds().height);
    // // sprite.setPosition(a_ball.getPosition().x+30,a_ball.getPosition().y);

    // // a_win.draw(sprite);
    // a_ball.setTexture(texture);
    win.draw(a_ball);
}

void Canvas::draw(sf::RectangleShape a_rectangle, sf::RenderWindow &win)
{
    win.draw(a_rectangle);
}

// sf::RenderWindow &Canvas::getCanvas()
// {
//     return *m_canvas.get();
// }

int Canvas::getWidth()
{
    return m_canvasWidth;
}

int Canvas::getHeight()
{
    return m_canvasHeight;
}

void Canvas::drawHearts(int a_numOfHearts, sf::RenderWindow &a_win)
{
    auto texture = ResourceManager::getInstance().getTexture("heart");
    int toLeft = 0;
    for (int i = 0; i < a_numOfHearts; i++)
    {
        sf::Sprite sprite;
        sprite.setTexture(*texture);
        sf::Vector2f targetSize(30, 30);
        sprite.setScale(
            targetSize.x / sprite.getLocalBounds().width,
            targetSize.y / sprite.getLocalBounds().height);
        toLeft += 30;

        sprite.setPosition(sf::Vector2f(m_canvasWidth - toLeft, 0));

        a_win.draw(sprite);
    }
}

void Canvas::drawSoundLogo(bool a_soundStatus, sf::RenderWindow &a_win)
{
    TexturePtr texture;
    if (a_soundStatus == true)
    {
        texture = ResourceManager::getInstance().getTexture("sound_on");
    }
    else
    {
        texture = ResourceManager::getInstance().getTexture("sound_off");
    }

    sf::Sprite sprite;
    sprite.setTexture(*texture);
    sf::Vector2f targetSize(20, 20);
    sprite.setScale(
        targetSize.x / sprite.getLocalBounds().width,
        targetSize.y / sprite.getLocalBounds().height);

    sprite.setPosition(sf::Vector2f(10, m_canvasHeight - 22));
    a_win.draw(sprite);
}

void Canvas::drawScore(int a_score, sf::RenderWindow &a_win)
{
    auto font = ResourceManager::getInstance().getFont("Led");
    sf::Text score("Score: " + std::to_string(a_score), *font, 20);
    score.setFillColor(sf::Color::Cyan);
    score.setStyle(sf::Text::Bold);
    score.setPosition(sf::Vector2f(m_canvasWidth / 2 - 70, 5));
    a_win.draw(score);
}

void Canvas::drawGameOver(sf::RenderWindow &a_win)
{
    a_win.clear(sf::Color::Transparent);
    auto font = ResourceManager::getInstance().getFont("Led");

    sf::Text gameOver("Game Over", *font, 100);
    gameOver.setFillColor(sf::Color::Red);
    gameOver.setStyle(sf::Text::Bold);
    gameOver.setPosition(sf::Vector2f(300, 400));
    a_win.draw(gameOver);
    a_win.display();

    sf::sleep(sf::seconds(2));
}

void Canvas::drawOptionsWindow(sf::RenderWindow &a_win)
{

    drawOptionWindowBackground(a_win);
    drawGameText(a_win);

    a_win.draw(getStartGameBox());
    drawStartGameText(a_win);

    a_win.draw(getMultiplayerGameBox());
    drawMultiplayerText(a_win);

    a_win.draw(getLeaderboardBox());
    drawLeaderboardText(a_win);

    a_win.draw(getExitGameBox());
    drawExitGameText(a_win);
}

sf::RectangleShape Canvas::getStartGameBox()
{
    sf::RectangleShape startGameBox;

    startGameBox.setPosition(m_canvasWidth / 2 - 50, m_canvasHeight / 2 - 100);
    startGameBox.setSize(sf::Vector2f(140, 40));
    startGameBox.setFillColor(sf::Color(128, 128, 128, 128));
    startGameBox.setOutlineColor(sf::Color(255, 255, 255));
    startGameBox.setOutlineThickness(2.0);
    return startGameBox;
}

sf::RectangleShape Canvas::getMultiplayerGameBox()
{
    sf::RectangleShape multiplayerBox;

    multiplayerBox.setPosition(m_canvasWidth / 2 - 50, m_canvasHeight / 2);
    multiplayerBox.setSize(sf::Vector2f(140, 40));
    multiplayerBox.setFillColor(sf::Color(128, 128, 128, 128));
    multiplayerBox.setOutlineColor(sf::Color(255, 255, 255));
    multiplayerBox.setOutlineThickness(2.0);
    return multiplayerBox;
}

sf::RectangleShape Canvas::getLeaderboardBox()
{
    sf::RectangleShape LeaderboardBox;

    LeaderboardBox.setPosition(sf::Vector2f(m_canvasWidth / 2 - 50, m_canvasHeight / 2 + 100));
    LeaderboardBox.setSize(sf::Vector2f(140, 40));
    LeaderboardBox.setFillColor(sf::Color(128, 128, 128, 128));
    LeaderboardBox.setOutlineColor(sf::Color(255, 255, 255));
    LeaderboardBox.setOutlineThickness(2.0);
    return LeaderboardBox;
}

sf::RectangleShape Canvas::getExitGameBox()
{
    sf::RectangleShape exitGameBox;

    exitGameBox.setPosition(sf::Vector2f(m_canvasWidth / 2 - 50, m_canvasHeight / 2 + 200));
    exitGameBox.setSize(sf::Vector2f(140, 40));
    exitGameBox.setFillColor(sf::Color(128, 128, 128, 128));
    exitGameBox.setOutlineColor(sf::Color(255, 255, 255));
    exitGameBox.setOutlineThickness(2.0);
    return exitGameBox;
}
void Canvas::drawGameBackground(sf::RenderWindow &a_win)
{
    auto texture = ResourceManager::getInstance().getTexture("Background");
    sf::Sprite background;
    sf::Vector2u TextureSize;
    sf::Vector2u WindowSize;
    TextureSize = ((*texture).getSize());
    WindowSize = a_win.getSize();

    float ScaleX = (float)WindowSize.x / TextureSize.x;
    float ScaleY = (float)WindowSize.y / TextureSize.y;

    background.setTexture(*texture);
    background.setScale(ScaleX, ScaleY);
    a_win.draw(background);
}

void Canvas::drawOptionWindowBackground(sf::RenderWindow &a_win)
{
    auto texture = ResourceManager::getInstance().getTexture("MainBackground");
    sf::Sprite background;
    sf::Vector2u TextureSize;
    sf::Vector2u WindowSize;

    TextureSize = ((*texture).getSize());
    WindowSize = a_win.getSize();

    float ScaleX = (float)WindowSize.x / TextureSize.x;
    float ScaleY = (float)WindowSize.y / TextureSize.y;

    background.setTexture(*texture);
    background.setScale(ScaleX, ScaleY);
    a_win.draw(background);
}

void Canvas::drawGameText(sf::RenderWindow &a_win)
{
    auto font = ResourceManager::getInstance().getFont("Led");
    sf::Text game("Line Breaker", *font, 100);
    game.setFillColor(sf::Color::Cyan);
    game.setStyle(sf::Text::Bold);
    game.setPosition(sf::Vector2f(m_canvasWidth / 6, 80));
    a_win.draw(game);
}

void Canvas::drawStartGameText(sf::RenderWindow &a_win)
{
    auto font = ResourceManager::getInstance().getFont("Athletic");
    sf::Text startGame("Start Game", *font, 30);
    startGame.setPosition(m_canvasWidth / 2 - 30, m_canvasHeight / 2 - 100);
    a_win.draw(startGame);
}

void Canvas::drawMultiplayerText(sf::RenderWindow &a_win)
{
    auto font = ResourceManager::getInstance().getFont("Athletic");
    sf::Text multiplayer("Multiplayer", *font, 30);
    multiplayer.setPosition(m_canvasWidth / 2 - 36, m_canvasHeight / 2);
    a_win.draw(multiplayer);
}

void Canvas::drawLeaderboardText(sf::RenderWindow &a_win)
{
    auto font = ResourceManager::getInstance().getFont("Athletic");
    sf::Text Leaderboard("Leaderboard", *font, 30);
    Leaderboard.setPosition(sf::Vector2f(m_canvasWidth / 2 - 36, m_canvasHeight / 2 + 100));
    a_win.draw(Leaderboard);
}

void Canvas::drawExitGameText(sf::RenderWindow &a_win)
{
    auto font = ResourceManager::getInstance().getFont("Athletic");
    sf::Text exitGame("Exit Game", *font, 30);
    exitGame.setPosition(sf::Vector2f(m_canvasWidth / 2 - 25, m_canvasHeight / 2 + 200));
    a_win.draw(exitGame);
}

void Canvas::drawLeaderboard(std::vector<std::tuple<std::string, std::string>> a_topTen, sf::RenderWindow &a_win)
{
    auto led = ResourceManager::getInstance().getFont("Led");
    auto crass = ResourceManager::getInstance().getFont("Crass");
    a_win.clear(sf::Color::Transparent);
    sf::Text scoreText("", *led, 20);
    scoreText.setFillColor(sf::Color::Green);
    int place = 1;
    int posY = 160;

    drawGameBackground(a_win);

    sf::Text leaderboard("<Leaderboard>", *crass, 36);
    leaderboard.setFillColor(sf::Color::Green);
    leaderboard.setStyle(sf::Text::Bold);
    leaderboard.setOutlineColor(sf::Color::Blue);
    leaderboard.setPosition(sf::Vector2f(480, 10));

    a_win.draw(leaderboard);
    for (auto t = a_topTen.begin(); t != a_topTen.end(); ++t)
    {
        scoreText.setString(std::to_string(place) + ".\n" + "Name: " + std::get<0>(*t) + "\nScore: " + std::get<1>(*t) + "\n");
        scoreText.setString(std::to_string(place) + ".\n" + "Name: " + std::get<0>(*t) + "\nScore: " + std::get<1>(*t) + "\n");
        if (place == 5)
        {
            scoreText.setPosition(sf::Vector2f(350, posY));
            posY = 60;
        }
        else if (place > 5)
        {
            scoreText.setPosition(sf::Vector2f(700, posY));
        }
        else
        {
            scoreText.setPosition(sf::Vector2f(350, posY));
        }

        a_win.draw(scoreText);
        posY += 100;
        place++;
    }

    a_win.display();
    sf::sleep(sf::seconds(3));
}

void Canvas::drawEnterName(sf::RenderWindow &a_win)
{
    auto crass = ResourceManager::getInstance().getFont("Crass");

    a_win.clear();
    drawGameBackground(a_win);

    sf::Text game("Please enter your name:\n", *crass, 30);
    game.setFillColor(sf::Color::Cyan);
    game.setStyle(sf::Text::Bold);
    game.setPosition(sf::Vector2f(200, 200));
    a_win.draw(game);
}

void Canvas::drawEnterIp(sf::RenderWindow &a_win)
{
    auto crass = ResourceManager::getInstance().getFont("Crass");

    sf::Text game("Please enter the IP address of the server:\n", *crass, 30);
    game.setFillColor(sf::Color::Cyan);
    game.setStyle(sf::Text::Bold);
    game.setPosition(sf::Vector2f(200, 300));
    a_win.draw(game);
}

std::tuple<std::string, std::string> Canvas::getNameAndIP(bool a_withIP, sf::RenderWindow &a_win)
{
    auto crass = ResourceManager::getInstance().getFont("Crass");

    sf::Event event;
    drawEnterName(a_win);
    a_win.display();

    sf::String playerInput;
    sf::Text playerText("", *crass, 30);
    sf::Font font;
    playerText.setPosition(210, 250);
    playerText.setFillColor(sf::Color::Cyan);
    playerText.setStyle(sf::Text::Bold);

    a_win.setKeyRepeatEnabled(false);

    int counter = 0;
    while ( a_win.isOpen())
    {
        while ( a_win.pollEvent(event))
        {
            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode < 128)
                {
                    playerInput += event.text.unicode;
                    playerText.setString(playerInput);
                    a_win.draw(playerText);
                    a_win.display();
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                if (!a_withIP)
                {
                    return std::make_tuple(playerInput, "");
                }

                if (counter == 1)
                {
                    std::string ip = getIP(a_win);
                    std::string in = playerInput;
                    return std::make_tuple(in, ip);
                }
                counter++;
            }
        }
    }

    return std::make_tuple(playerInput, "");
}

std::string Canvas::getName(sf::RenderWindow &a_win)
{
    auto crass = ResourceManager::getInstance().getFont("Crass");

    sf::Event event;
    drawEnterName(a_win);
    a_win.display();

    std::string name;
    sf::String playerInput;
    sf::Text playerText("", *crass, 30);
    sf::Font font;
    playerText.setPosition(210, 250);
    playerText.setFillColor(sf::Color::Cyan);
    playerText.setStyle(sf::Text::Bold);

    a_win.setKeyRepeatEnabled(false);

    while ( a_win.isOpen())
    {
        while ( a_win.pollEvent(event))
        {
            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode < 128)
                {
                    playerInput += event.text.unicode;
                    playerText.setString(playerInput);
                    a_win.draw(playerText);
                    a_win.display();
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                name = playerInput;
                // std::cout << "here: " << name << "\n";
                return name;
            }
        }
    }

    return name;
}

std::string Canvas::getIP(sf::RenderWindow &a_win)
{
    auto crass = ResourceManager::getInstance().getFont("Crass");

    sf::Event event;
    drawEnterIp(a_win);
    a_win.display();

    sf::String playerInput;
    sf::Text playerText("", *crass, 30);
    sf::Font font;
    playerText.setPosition(210, 350);
    playerText.setFillColor(sf::Color::Cyan);
    playerText.setStyle(sf::Text::Bold);

    a_win.setKeyRepeatEnabled(false);

    while ( a_win.isOpen())
    {
        while ( a_win.pollEvent(event))
        {
            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode < 128)
                {
                    playerInput += event.text.unicode;
                    playerText.setString(playerInput);
                    a_win.draw(playerText);
                    a_win.display();
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                return playerInput;
            }
        }
    }

    return playerInput;
}

void Canvas::drawOnlinePlayersText(sf::RenderWindow &a_win)
{
    auto font = ResourceManager::getInstance().getFont("Athletic");
    size_t width = 200;
    size_t height = 200;

    sf::Text playerName("Online players:", *font, 30);
    playerName.setPosition(sf::Vector2f(width, height));
    a_win.draw(playerName);
}

sf::IntRect Canvas::pushNewPlayer(std::string a_name, int a_numOfPlayer)
{
    // drawPlayerConnect(a_name);
    auto font = ResourceManager::getInstance().getFont("Athletic");
    size_t width = 200;
    size_t height = 300 + (a_numOfPlayer * 100);

    sf::Text playerName(a_name, *font, 30);
    playerName.setPosition(sf::Vector2f(width, height));
    sf::RectangleShape box = getBox(width, height, playerName.getGlobalBounds().width + 30, playerName.getGlobalBounds().height + 30);
    m_playerBoxes.push_back(std::make_tuple(playerName, box));
    return sf::IntRect(box.getPosition().x, box.getPosition().y, box.getGlobalBounds().width, box.getGlobalBounds().height);
}

void Canvas::drawAllPlayers(sf::RenderWindow &a_win)
{
    for (const auto &[playerName, playerBox] : m_playerBoxes)
    {
        a_win.draw(playerName);
        a_win.draw(playerBox);
    }

    a_win.display();
}

void Canvas::drawLastPlayer(sf::RenderWindow &a_win)
{
    a_win.draw(std::get<0>(m_playerBoxes.back()));
    a_win.draw(std::get<1>(m_playerBoxes.back()));
    a_win.display();
}

// std::vector<std::tuple<sf::RectangleShape, std::string>> Canvas::getPlayerBoxes()
// {
//     return m_playerBoxes;
// }

void Canvas::drawPlayerDisconnect(std::string a_name, sf::RenderWindow &a_win)
{
    a_win.clear(sf::Color::Transparent);
    a_win.clear();
    drawGameBackground(a_win);
    auto font = ResourceManager::getInstance().getFont("Athletic");
    std::string msg = "Player " + a_name + " has left the server";
    sf::Text playerName(msg, *font, 30);
    playerName.setPosition(sf::Vector2f(200, 200));
    a_win.draw(playerName);
    a_win.display();
}

sf::RectangleShape Canvas::getBox(int width, int height, float recWidth, float recHeight)
{
    sf::RectangleShape box;

    box.setPosition(width - 20, height);
    box.setSize(sf::Vector2f(recWidth, recHeight));
    box.setFillColor(sf::Color(128, 128, 128, 128));
    box.setOutlineColor(sf::Color(255, 255, 255));
    box.setOutlineThickness(2.0);
    return box;
}

void Canvas::drawPlayerConnect(std::string a_name, sf::RenderWindow &a_win)
{
    a_win.clear(sf::Color::Transparent);
    a_win.clear();
    drawGameBackground(a_win);
    auto font = ResourceManager::getInstance().getFont("Athletic");
    std::string msg = "Welcome to the server " + a_name;
    sf::Text playerName(msg, *font, 30);
    playerName.setPosition(sf::Vector2f(100, 100));
    a_win.draw(playerName);
    a_win.display();
}

std::vector<std::tuple<sf::IntRect, std::string>> Canvas::drawGameOptions(std::string &a_msg, std::string &a_firstOption, std::string &a_secondOption, sf::RenderWindow &a_win)
{
    a_win.clear(sf::Color::Transparent);
    drawGameBackground(a_win);
    std::vector<std::tuple<sf::IntRect, std::string>> gameTypeBoxes;
    //    std::string chose = "What you wanna do?";

    auto font = ResourceManager::getInstance().getFont("Athletic");

    sf::Text message(a_msg, *font, 60);
    message.setFillColor(sf::Color::Green);
    message.setStyle(sf::Text::Bold);
    message.setOutlineColor(sf::Color::Blue);
    message.setPosition(sf::Vector2f(400, 200));

    sf::Text firstOption(a_firstOption, *font, 30);
    firstOption.setPosition(sf::Vector2f(200, 400));
    sf::RectangleShape firstOptionBox = getBox(200, 400, firstOption.getGlobalBounds().width + 30, firstOption.getGlobalBounds().height + 30);
    sf::IntRect firstOptionRect = sf::IntRect(firstOptionBox.getPosition().x, firstOptionBox.getPosition().y, firstOptionBox.getGlobalBounds().width, firstOptionBox.getGlobalBounds().height);
    gameTypeBoxes.push_back(std::make_tuple(firstOptionRect, a_firstOption));

    // std::string with = "Play with " + a_name;
    sf::Text secondOption(a_secondOption, *font, 30);
    secondOption.setPosition(sf::Vector2f(800, 400));
    sf::RectangleShape secondOptionBox = getBox(800, 400, secondOption.getGlobalBounds().width + 30, secondOption.getGlobalBounds().height + 30);
    sf::IntRect secondOptionRect = sf::IntRect(secondOptionBox.getPosition().x, secondOptionBox.getPosition().y, secondOptionBox.getGlobalBounds().width, secondOptionBox.getGlobalBounds().height);
    gameTypeBoxes.push_back(std::make_tuple(secondOptionRect, a_secondOption));

    a_win.draw(message);
    a_win.draw(firstOption);
    a_win.draw(firstOptionBox);
    a_win.draw(secondOption);
    a_win.draw(secondOptionBox);

    a_win.display();

    return gameTypeBoxes;
}

void Canvas::drawGameRequestRejected(std::string &a_msg, sf::RenderWindow &a_win)
{
    a_win.clear(sf::Color::Transparent);
    drawGameBackground(a_win);

    auto font = ResourceManager::getInstance().getFont("Athletic");
    sf::Text rejectMsg(a_msg, *font, 40);
    rejectMsg.setFillColor(sf::Color::Red);
    rejectMsg.setOutlineColor(sf::Color::Blue);
    rejectMsg.setPosition(sf::Vector2f(300, 200));

    a_win.draw(rejectMsg);

    a_win.display();
    sf::sleep(sf::seconds(2));
}