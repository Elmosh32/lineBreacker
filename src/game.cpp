
#include "game.hpp"
#include <iostream>

Game::Game(int a_width, int a_height, sf::RenderWindow &a_win)
// : m_level(a_width,a_height, a_win)
// , m_client(a_width,a_height, a_win)
: m_canvas(a_width,a_height)
, m_win(a_win)
{

}

void Game::gameStartWindow()
{
    if (choseOption() == "startGame"){
        startGame();
    } else if (choseOption() == "leaderboard") {
        updateLeaderboard(0);
        gameStartWindow();
    } else if (choseOption() == "multiplayer") {
        startOnlineGame();
    }
}

void Game::startGame()
{
    RunLevel level = RunLevel(1200, 800, m_win);
    std::string score = level.playLevel();
    m_canvas.drawLeaderboard(updateLeaderboard(std::stoi(score)), m_win);
    gameStartWindow();
}

void Game::startOnlineGame()
{
    Client client(1200, 800, m_win);
    client.createClient();
}

std::string Game::choseOption()
{
    sf::Event event;

    sf::IntRect startBox(m_canvas.getStartGameBox().getPosition().x, m_canvas.getStartGameBox().getPosition().y, m_canvas.getStartGameBox().getGlobalBounds().width, m_canvas.getStartGameBox().getGlobalBounds().height);
    sf::IntRect multiplayerBox(m_canvas.getMultiplayerGameBox().getPosition().x, m_canvas.getMultiplayerGameBox().getPosition().y, m_canvas.getMultiplayerGameBox().getGlobalBounds().width, m_canvas.getMultiplayerGameBox().getGlobalBounds().height);
    sf::IntRect leaderboardBox(m_canvas.getLeaderboardBox().getPosition().x, m_canvas.getLeaderboardBox().getPosition().y, m_canvas.getLeaderboardBox().getGlobalBounds().width, m_canvas.getLeaderboardBox().getGlobalBounds().height);
    sf::IntRect exitBox(m_canvas.getExitGameBox().getPosition().x, m_canvas.getExitGameBox().getPosition().y, m_canvas.getExitGameBox().getGlobalBounds().width, m_canvas.getExitGameBox().getGlobalBounds().height);

    while (m_win.isOpen() && !sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        m_win.clear();
        m_canvas.drawOptionsWindow(m_win);
        m_win.display();

        sf::Cursor cursor;

        if (startBox.contains(sf::Mouse::getPosition(m_win)))
        {
            if (cursor.loadFromSystem(sf::Cursor::Hand))
            {
                m_win.setMouseCursor(cursor);
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if (cursor.loadFromSystem(sf::Cursor::Arrow))
                {
                    m_win.setMouseCursor(cursor);
                }

                return "startGame";
            }
        }
        else if (multiplayerBox.contains(sf::Mouse::getPosition(m_win)))
        {
            if (cursor.loadFromSystem(sf::Cursor::Hand))
            {
                m_win.setMouseCursor(cursor);
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if (cursor.loadFromSystem(sf::Cursor::Arrow))
                {
                    m_win.setMouseCursor(cursor);
                }

                return "multiplayer";
            }
        }
        else if (leaderboardBox.contains(sf::Mouse::getPosition(m_win)))
        {
            if (cursor.loadFromSystem(sf::Cursor::Hand))
            {
                m_win.setMouseCursor(cursor);
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if (cursor.loadFromSystem(sf::Cursor::Arrow))
                {
                    m_win.setMouseCursor(cursor);
                }

                return "leaderboard";
            }
        }
        else if (exitBox.contains(sf::Mouse::getPosition(m_win)))
        {
            if (cursor.loadFromSystem(sf::Cursor::Hand))
            {
                m_win.setMouseCursor(cursor);
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                m_win.close();
                return "exitGame";
            }
        }
        else
        {
            if (cursor.loadFromSystem(sf::Cursor::Arrow))
            {
                m_win.setMouseCursor(cursor);
            }
        }

        while (m_win.pollEvent(event))
        {
        }
    }
    return "";
}

std::vector<std::tuple<std::string, std::string>> Game::updateLeaderboard(int a_score)
{
    std::string filename = "leaderboard.txt";
    std::string path = "../leaderboard/";
    std::fstream file;
    file.open(path + filename, std::ios::in);
    std::vector<std::tuple<std::string, std::string>> nameAndScore;

    std::vector<std::string> topTen;
    std::string score;
    std::string name;
    bool flag = false;
    int counter = 0;

    while (!file.eof())
    {
        std::string name;
        std::getline(file, name);

        std::string score;
        std::getline(file, score);

        if (counter == 10)
        {
            break;
        }

        if (flag)
        {
            nameAndScore.push_back(std::make_tuple(name, score));
            counter++;
            continue;
        }

        if (stoi(score) > a_score)
        {
            nameAndScore.push_back(std::make_tuple(name, score));
            counter++;
            continue;
        }
        else
        {
            flag = true;
            if (counter <= 8)
            {
                std::string username = std::get<0>(m_canvas.getNameAndIP(false, m_win));
                nameAndScore.push_back(std::make_tuple(username, std::to_string(a_score)));
                nameAndScore.push_back(std::make_tuple(name, score));
                counter += 2;
                continue;
            }
            m_canvas.drawEnterName(m_win);
            std::string username = std::get<0>(m_canvas.getNameAndIP(false, m_win));
            nameAndScore.push_back(std::make_tuple(username, std::to_string(a_score)));
            counter++;
        }
    }

    file.close();

    if (flag)
    {
        std::ofstream outputFile(path + filename);

        for (auto t = nameAndScore.begin(); t != nameAndScore.end(); ++t)
        {
            outputFile << std::get<0>(*t) << '\n'
                       << std::get<1>(*t) << '\n';
        }
    }

    if (a_score == 0)
    {
        m_canvas.drawLeaderboard(nameAndScore, m_win);
    }

    return nameAndScore;
}
