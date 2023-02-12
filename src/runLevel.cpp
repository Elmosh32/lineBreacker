#include "runLevel.hpp"

RunLevel::RunLevel(int a_width, int a_height, sf::RenderWindow &win)
: m_canvas(a_width, a_height)
, m_paddle(Paddle(sf::Vector2f(m_canvas.getWidth() / 10, 20), sf::Color::Green, m_canvas.getWidth(), m_canvas.getHeight(), "bottom"))
, m_ball(sf::Vector2f(m_canvas.getWidth() / 2 - 6, m_paddle.getTop()- 16), 8, 8, sf::Color::Yellow, m_canvas.getWidth(), m_canvas.getHeight())
, m_win(win)
{
    readLevelParameters(m_level);
}

RunLevel &RunLevel::operator=(const RunLevel &a_otherLevel)
{
    m_paddle = Paddle(sf::Vector2f(m_canvas.getWidth() / 10, 20), sf::Color::Green, m_canvas.getWidth(), m_canvas.getHeight(), "bottom");
    m_ball = Ball(sf::Vector2f(m_canvas.getWidth() / 2 - 6, m_paddle.getTop() - 16), 8, 8, sf::Color::Yellow, m_canvas.getWidth(), m_canvas.getHeight());
    std::vector<Brick> bricks;
    std::copy(a_otherLevel.m_bricks.begin(), a_otherLevel.m_bricks.end(), std::back_inserter(bricks));
    m_bricks = bricks;
    m_lives = a_otherLevel.m_lives;
    m_totalScore = a_otherLevel.m_totalScore;
    m_level = a_otherLevel.m_level;
    return *this;
}

void RunLevel::nextLevel()
{
    m_paddle = Paddle(sf::Vector2f(m_canvas.getWidth() / 10, 20), sf::Color::Green, m_canvas.getWidth(), m_canvas.getHeight(), "bottom");
    m_ball = Ball(sf::Vector2f(m_canvas.getWidth() / 2 - 6, m_paddle.getTop() - 16), 8, 8, sf::Color::Yellow, m_canvas.getWidth(), m_canvas.getHeight());
    std::vector<Brick> bricks;
    readLevelParameters(m_level);
    playLevel();
}

bool RunLevel::readLevelParameters(int a_levelNumber)
{
    ResourceManager &rm = ResourceManager::getInstance();
    std::string filename = "level" + std::to_string(a_levelNumber) + "_bricks.txt";
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

    while(!file.eof()) {
        file >> width >> height >> color >> x >> y >> maxCollisions >> score;
        size.x = width;
        size.y = height;
        m_bricks.push_back(Brick(size, *rm.getColor(color), x, y, maxCollisions, score));
    }

    return true;
}

std::string RunLevel::playLevel()
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
        m_win.clear();
        drawAll(soundFlag);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) && soundFlag == false) {
            soundFlag = true;
            sound.play();
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::M) && soundFlag == true) {
            soundFlag = false;
            sound.stop();
        }

        m_win.display();

        if (flagGameStart) {
            m_ball.move(m_canvas);
            std::string result = gameCollision();
            if (result == "next_itr") {
                continue;
            } else if(result == "lose_life") {
                flagGameStart = false;
                continue;
            } else if(result == "next_level") {
                nextLevel();
                break;
            } else {
                return result;
            }
            
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            flagGameStart = true;
        } else {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                m_paddle.moveToLeft();
                m_ball.moveToMidOfPaddle(m_paddle.getRight() - (m_paddle.getRight() - m_paddle.getLeft()) / 2, m_paddle.getTop() - 16);
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                m_paddle.moveToRight();
                m_ball.moveToMidOfPaddle(m_paddle.getRight() - (m_paddle.getRight() - m_paddle.getLeft()) / 2, m_paddle.getTop() - 16);
            }
        }

        while (m_win.pollEvent(event))
        {
        }
    }
    return "";
}

std::string RunLevel::gameCollision()
{
    Collision collision;
    sf::Event event;
    ResourceManager &rm = ResourceManager::getInstance();
    sf::Sound hit;
    hit.setBuffer(*rm.getSound("Collision"));

    for (size_t i = 0; i < m_bricks.size(); i++) {

        CollisionDirection collisionRes = collision.checkBricksCollision(m_ball, m_bricks[i]);

        if (collision.checkPaddleCollision(m_ball, m_paddle)) {

            m_ball.handlePaddleCollision(m_paddle.getTop());
        }

        if (collisionRes != CollisionDirection::NoCollision) {

            collision.handleBrickCollision(m_ball, collisionRes, m_bricks[i]);

            m_bricks[i].setColor(*rm.getPrevColor(m_bricks[i].getColor()));
            m_bricks[i].draw(m_win);
            m_ball.changeYDirection();
            m_ball.move(m_canvas);
            m_bricks[i].addCollision();
            // sf::sleep(sf::milliseconds(80));
            // usleep(3000);

            // hit.play();

            if (m_bricks[i].getMaxCollisions() == 0) {
                m_totalScore += m_bricks[i].getScore();
                m_bricks.erase(m_bricks.begin() + i);
                if (m_bricks.size() == 0) {

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

    m_ball.checkCanvasCollision(m_canvas);

    if (m_ball.checkCanvasBottomCollision(m_canvas)){
        m_lives--;
        if (m_lives == 0) {
            // m_canvas.drawGameOver();
            // lose();
            return std::to_string(m_totalScore); 
        }
        m_ball.moveToMidOfPaddle(m_paddle.getRight() - (m_paddle.getRight() - m_paddle.getLeft()) / 2, m_paddle.getTop() - 16);
        return "lose_life";
    }

    while (m_win.pollEvent(event))
    {
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        m_paddle.moveToLeft();
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        m_paddle.moveToRight();
    }
    
    return "next_itr";
}

// void RunLevel::getGems()
// {
//     ResourceManager &rm = ResourceManager::getInstance();

//     sf::Sprite tempSprite1(*rm.getTexture("firstGem"));
//     sf::Sprite tempSprite2(*rm.getTexture("secondGem"));
//     sf::Sprite tempSprite3(*rm.getTexture("thirdGem"));
//     sf::Sprite tempSprite4(*rm.getTexture("lifeGem"));
//     sf::RenderTexture ren;


//     tempSprite1.setPosition(100,0);
//     tempSprite2.setPosition(400, 50);
//     tempSprite3.setPosition(700, 100);
//     tempSprite4.setPosition(200, 300);
//     sf::Vector2f targetSize(30, 30);
//     tempSprite1.setScale(
//         targetSize.x / tempSprite1.getLocalBounds().width,
//         targetSize.y / tempSprite1.getLocalBounds().height);
//     tempSprite2.setScale(
//         targetSize.x / tempSprite2.getLocalBounds().width,
//         targetSize.y / tempSprite2.getLocalBounds().height);
//     tempSprite3.setScale(
//         targetSize.x / tempSprite3.getLocalBounds().width,
//         targetSize.y / tempSprite3.getLocalBounds().height);
//     tempSprite4.setScale(
//         targetSize.x / tempSprite4.getLocalBounds().width,
//         targetSize.y / tempSprite4.getLocalBounds().height);
//     srand(time(NULL));

//     for (int i = tempSprite1.getPosition().y; i < m_canvas.getHeight(); i += 5)
//     {
//         m_win.clear();
//         m_canvas.drawGameBackground();
//         tempSprite1.setPosition(tempSprite1.getPosition().x, tempSprite1.getPosition().y + 5);
//         tempSprite2.setPosition(tempSprite2.getPosition().x, tempSprite2.getPosition().y + 5);
//         tempSprite3.setPosition(tempSprite3.getPosition().x, tempSprite3.getPosition().y + 5);
//         tempSprite4.setPosition(tempSprite4.getPosition().x, tempSprite4.getPosition().y + 5);
//         m_win.draw(tempSprite1);
//         m_win.draw(tempSprite2);
//         m_win.draw(tempSprite3);
//         m_win.draw(tempSprite4);
//         m_win.display();
//     }

//     // for (unsigned int i = 0; i < *rm.getTexture("firstGem").getSize().x; i++)
//     // {

//     // }
//     // usleep(10000000);
// }




void RunLevel::drawAll(bool a_soundStatus)
{
    m_canvas.drawGameBackground(m_win);
    m_canvas.drawHearts(m_lives, m_win);
    m_canvas.drawScore(m_totalScore, m_win);
    m_paddle.draw(m_win);
    m_ball.drawBall(m_win);
    m_canvas.drawSoundLogo(a_soundStatus, m_win);

    for (size_t i = 0; i < m_bricks.size(); i++) {
        m_bricks[i].draw(m_win);
    }
}

