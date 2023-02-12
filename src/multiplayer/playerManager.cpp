#include "playerManager.hpp"

PlayerManager::PlayerManager(int a_width, int a_height, sf::RenderWindow &a_win, SafeQueue<sf::Vector2f> &a_ballPositionQueueFirst, SafeQueue<sf::Vector2f> &a_paddlePositionQueueFirst, SafeQueue<sf::Vector2f> &a_ballPositionQueueSecond, SafeQueue<sf::Vector2f> &a_paddlePositionQueueSecond)
: m_canvas(a_width, a_height)
, m_win(a_win)
, m_firstPlayerPaddle(sf::Vector2f(m_canvas.getWidth() / 10, 20), sf::Color::Green, m_canvas.getWidth(), m_canvas.getHeight(), "bottom")
, m_secondPlayerPaddle(sf::Vector2f(m_canvas.getWidth() / 10, 20), sf::Color::Red, m_canvas.getWidth(), m_canvas.getHeight(), "top")
, m_firstPlayerBall(sf::Vector2f(m_canvas.getWidth() / 2 - 6, m_firstPlayerPaddle.getTop() - 16), 8, 8, sf::Color::Yellow, m_canvas.getWidth(), m_canvas.getHeight())
, m_secondPlayerBall(sf::Vector2f(m_canvas.getWidth() / 2 - 6, m_secondPlayerPaddle.getBottom() - 5), 8, 8, sf::Color::Blue, m_canvas.getWidth(), m_canvas.getHeight())
, m_ballPositionQueueFirst(a_ballPositionQueueFirst)
, m_paddlePositionQueueFirst(a_paddlePositionQueueFirst)
, m_ballPositionQueueSecond(a_ballPositionQueueSecond)
, m_paddlePositionQueueSecond(a_paddlePositionQueueSecond)
{
    readLevelParameters(m_level);
}

PlayerManager &PlayerManager::operator=(const PlayerManager &a_otherLevel)
{
    m_firstPlayerPaddle = Paddle(sf::Vector2f(m_canvas.getWidth() / 10, 20), sf::Color::Green, m_canvas.getWidth(), m_canvas.getHeight(), "bottom");
    m_secondPlayerPaddle = Paddle(sf::Vector2f(m_canvas.getWidth() / 10, 20), sf::Color::Red, m_canvas.getWidth(), m_canvas.getHeight(), "top");
    m_firstPlayerBall = Ball(sf::Vector2f(m_canvas.getWidth() / 2 - 6, m_firstPlayerPaddle.getTop() - 16), 8, 8, sf::Color::Yellow, m_canvas.getWidth(), m_canvas.getHeight());
    m_secondPlayerBall = Ball(sf::Vector2f(m_canvas.getWidth() / 2 - 6, m_secondPlayerPaddle.getTop() + 16), 8, 8, sf::Color::Blue, m_canvas.getWidth(), m_canvas.getHeight());
    std::vector<Brick> bricks;
    std::copy(a_otherLevel.m_bricks.begin(), a_otherLevel.m_bricks.end(), std::back_inserter(bricks));
    m_bricks = bricks;
    m_lives = a_otherLevel.m_lives;
    m_totalScore = a_otherLevel.m_totalScore;
    m_level = a_otherLevel.m_level;
    return *this;
}

void PlayerManager::nextLevel(int a_level)
{
    m_firstPlayerPaddle = Paddle(sf::Vector2f(m_canvas.getWidth() / 10, 20), sf::Color::Green, m_canvas.getWidth(), m_canvas.getHeight(), "bottom");
    m_secondPlayerPaddle = Paddle(sf::Vector2f(m_canvas.getWidth() / 10, 20), sf::Color::Red, m_canvas.getWidth(), m_canvas.getHeight(), "top");
    m_firstPlayerBall = Ball(sf::Vector2f(m_canvas.getWidth() / 2 - 6, m_firstPlayerPaddle.getTop() - 16), 8, 8, sf::Color::Yellow, m_canvas.getWidth(), m_canvas.getHeight());
    m_secondPlayerBall = Ball(sf::Vector2f(m_canvas.getWidth() / 2 - 6, m_secondPlayerPaddle.getTop() + 16), 8, 8, sf::Color::Blue, m_canvas.getWidth(), m_canvas.getHeight());
    std::vector<Brick> bricks;
    readLevelParameters(a_level);
}

bool PlayerManager::readLevelParameters(int a_levelNumber)
{
    ResourceManager &rm = ResourceManager::getInstance();
    std::string filename = "online_level" + std::to_string(a_levelNumber) + "_bricks.txt";
    std::string path = "../levels/";
    std::fstream file;
    file.open(path + filename, std::ios::in);

    if (file.fail()) {
        return false;
    }

    sf::Vector2f size;
    float width;
    float height;
    int color;
    float x;
    float y;
    int maxCollisions;
    int score;
    std::string newLine;

    while (!file.eof()) {
        file >> width >> height >> color >> x >> y >> maxCollisions >> score;
        size.x = width;
        size.y = height;
        m_bricks.push_back(Brick(size, *rm.getColor(color), x, y, maxCollisions, score));
    }

    return true;
}

std::string PlayerManager::playLevel()
{
    auto backgroundSound = ResourceManager::getInstance().getSound("Background");
    m_win.setFramerateLimit(60);
    sf::Event event;

    sf::Sound sound;
    sound.setBuffer(*backgroundSound);
    sound.setLoop(true);
    sound.play();
    bool soundFlag = true;
    bool flagGameStart = false;
    m_canvas.drawSoundLogo(soundFlag, m_win);

    while (m_win.isOpen() && !sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        while (m_win.pollEvent(event))
        {
            m_win.clear();
            drawAll(soundFlag);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) && soundFlag == false)
            {
                soundFlag = true;
                sound.play();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::M) && soundFlag == true)
            {
                soundFlag = false;
                sound.stop();
            }
            m_win.display();

            if (flagGameStart)
            {
                sf::Vector2f ballPosition = m_firstPlayerBall.move(m_canvas);
                m_ballPositionQueueFirst.pushBack(std::move(ballPosition));

                std::string result = gameCollision();
                if (result == "next_itr")
                {
                    continue;
                }
                else if (result == "lose_life")
                {
                    flagGameStart = false;
                    continue;
                }
                else
                {
                    return result;
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                flagGameStart = true;
            }
            else
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                {
                    sf::Vector2f paddlePosition = m_firstPlayerPaddle.moveToLeft();
                    m_paddlePositionQueueFirst.pushBack(paddlePosition);

                    sf::Vector2f ballPosition = m_firstPlayerBall.moveToMidOfPaddle(m_firstPlayerPaddle.getRight() - (m_firstPlayerPaddle.getRight() - m_firstPlayerPaddle.getLeft()) / 2, m_firstPlayerPaddle.getTop() - 16);
                    m_ballPositionQueueFirst.pushBack(ballPosition);
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                {
                    sf::Vector2f paddlePosition = m_firstPlayerPaddle.moveToRight();
                    m_paddlePositionQueueFirst.pushBack(paddlePosition);

                    sf::Vector2f ballPosition = m_firstPlayerBall.moveToMidOfPaddle(m_firstPlayerPaddle.getRight() - (m_firstPlayerPaddle.getRight() - m_firstPlayerPaddle.getLeft()) / 2, m_firstPlayerPaddle.getTop() - 16);
                    m_ballPositionQueueFirst.pushBack(ballPosition);
                }
            }
        }
    }
    return "";
}

std::string PlayerManager::gameCollision()
{
    Collision collision;
    ResourceManager &rm = ResourceManager::getInstance();
    sf::Sound hit;
    hit.setBuffer(*rm.getSound("Collision"));

    for (size_t i = 0; i < m_bricks.size(); i++)
    {

        CollisionDirection collisionRes = collision.checkBricksCollision(m_firstPlayerBall, m_bricks[i]);

        if (collision.checkPaddleCollision(m_firstPlayerBall, m_firstPlayerPaddle))
        {

            m_firstPlayerBall.handlePaddleCollision(m_firstPlayerPaddle.getTop());
        }

        if (collisionRes != CollisionDirection::NoCollision)
        {

            collision.handleBrickCollision(m_firstPlayerBall, collisionRes, m_bricks[i]);

            m_bricks[i].setColor(*rm.getPrevColor(m_bricks[i].getColor()));
            m_bricks[i].draw(m_win);
            m_firstPlayerBall.changeYDirection();
            m_firstPlayerBall.move(m_canvas);
            m_bricks[i].addCollision();
            // sf::sleep(sf::milliseconds(80));
            // usleep(3000);

            // hit.play();

            if (m_bricks[i].getMaxCollisions() == 0)
            {
                m_totalScore += m_bricks[i].getScore();
                m_bricks.erase(m_bricks.begin() + i);
                if (m_bricks.size() == 0)
                {

                    m_level++;
                    // sf::Sound win;
                    // win.setBuffer(*rm.getSound("Win"));
                    // win.play();
                    // getGems();

                    return "next_level";
                }
            }
        }
    }

    m_firstPlayerBall.checkCanvasCollision(m_canvas);

    if (m_firstPlayerBall.checkCanvasBottomCollision(m_canvas))
    {
        m_lives--;
        if (m_lives == 0)
        {
            // m_canvas.drawGameOver();
            m_canvas.drawLeaderboard(updateLeaderboard(m_totalScore), m_win);
            lose();
            return "game_over";
        }
        sf::Vector2f ballPosition = m_firstPlayerBall.moveToMidOfPaddle(m_firstPlayerPaddle.getRight() - (m_firstPlayerPaddle.getRight() - m_firstPlayerPaddle.getLeft()) / 2, m_firstPlayerPaddle.getTop() - 16);
        m_ballPositionQueueFirst.pushBack(ballPosition);
        return "lose_life";
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        sf::Vector2f paddlePosition = m_firstPlayerPaddle.moveToLeft();
        m_paddlePositionQueueFirst.pushBack(paddlePosition);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        sf::Vector2f paddlePosition = m_firstPlayerPaddle.moveToRight();
        m_paddlePositionQueueFirst.pushBack(paddlePosition);
    }
    moveSecondPlayerElements();

    return "next_itr";
}

void PlayerManager::moveSecondPlayerElements()
{

    while (!m_paddlePositionQueueSecond.isEmpty() || !m_ballPositionQueueSecond.isEmpty())
    {
        sf::Vector2f ballPosition;
        sf::Vector2f paddlePosition;
        if (!m_paddlePositionQueueSecond.isEmpty())
        {
            m_paddlePositionQueueSecond.pop(paddlePosition);
            m_secondPlayerPaddle.movePaddle(paddlePosition);
        }

        if (!m_ballPositionQueueSecond.isEmpty())
        {
            m_ballPositionQueueSecond.pop(ballPosition);
            m_secondPlayerBall.moveBall(ballPosition);
        }
    }
}

void PlayerManager::lose()
{
    m_level = 1;
    m_lives = 3;
    m_totalScore = 0;
    m_firstPlayerPaddle = Paddle(sf::Vector2f(m_canvas.getWidth() / 10, 20), sf::Color::Green, m_canvas.getWidth(), m_canvas.getHeight(), "bottom");
    m_secondPlayerPaddle = Paddle(sf::Vector2f(m_canvas.getWidth() / 10, 20), sf::Color::Red, m_canvas.getWidth(), m_canvas.getHeight(), "top");
    m_firstPlayerBall = Ball(sf::Vector2f(m_canvas.getWidth() / 2 - 6, m_firstPlayerPaddle.getTop() - 16), 8, 8, sf::Color::Yellow, m_canvas.getWidth(), m_canvas.getHeight());
    m_secondPlayerBall = Ball(sf::Vector2f(m_canvas.getWidth() / 2 - 6, m_secondPlayerPaddle.getTop() + 16), 8, 8, sf::Color::Blue, m_canvas.getWidth(), m_canvas.getHeight());
    m_bricks.clear();
    readLevelParameters(m_level);
}

std::vector<std::tuple<std::string, std::string>> PlayerManager::updateLeaderboard(int a_score)
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

void PlayerManager::drawAll(bool a_soundStatus)
{
    m_canvas.drawGameBackground(m_win);
    m_canvas.drawHearts(m_lives, m_win);
    m_canvas.drawScore(m_totalScore, m_win);
    m_firstPlayerPaddle.draw(m_win);
    m_secondPlayerPaddle.draw(m_win);
    m_firstPlayerBall.drawBall(m_win);
    m_secondPlayerBall.drawBall(m_win);
    m_canvas.drawSoundLogo(a_soundStatus, m_win);

    for (size_t i = 0; i < m_bricks.size(); i++)
    {
        m_bricks[i].draw(m_win);
    }
}
