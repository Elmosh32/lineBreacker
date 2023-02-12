#include "resourceManager.hpp"

// ResourceManager::ResourceManager()
// {

// }

// sf::Color *ResourceManager::getColor(sf::Color const &a_color)
// {

// }

ResourceManager::ResourceManager()
: m_colors(initColors())
{
    m_fonts = initFonts();
    m_sounds = initSounds();
    m_textures = initTextures();
};

std::vector<colorPtr> ResourceManager::initColors()
{
    return std::vector<colorPtr>{std::make_shared<sf::Color>(sf::Color::Transparent), std::make_shared<sf::Color>(sf::Color::Blue),
                                  std::make_shared<sf::Color>(sf::Color::Cyan), std::make_shared<sf::Color>(sf::Color::Green),
                                  std::make_shared<sf::Color>(sf::Color::Magenta), std::make_shared<sf::Color>(sf::Color::Yellow), std::make_shared<sf::Color>(sf::Color::Red)};
}

std::unordered_map<std::string, FontPtr> ResourceManager::initFonts()
{
    std::unordered_map<std::string, FontPtr> fonts;

    sf::Font led;
    sf::Font crass;
    sf::Font athletic;

    led.loadFromFile("../font/big/Big Daddy LED TFB.ttf");
    crass.loadFromFile("../font/crass/crass.ttf");
    athletic.loadFromFile("../font/athletic/ATHLS___.TTF");
    fonts = {{"Led", std::make_shared<sf::Font>(led)},
             {"Crass", std::make_shared<sf::Font>(crass)},
             {"Athletic", std::make_shared<sf::Font>(athletic)}};

    return fonts;
}


std::unordered_map<std::string, SoundPtr> ResourceManager::initSounds()
{
    std::unordered_map<std::string, SoundPtr> sounds;
    sf::SoundBuffer background;
    sf::SoundBuffer collision;
    sf::SoundBuffer fall;
    sf::SoundBuffer lose;
    sf::SoundBuffer win;

    background.loadFromFile("../sounds/Six.wav");
    collision.loadFromFile("../sounds/bang_1.wav");
    fall.loadFromFile("../sounds/mixkit-failure-arcade-alert-notification-240.wav");
    lose.loadFromFile("../sounds/mixkit-retro-arcade-lose-2027.wav");
    win.loadFromFile("../sounds/mixkit-winning-notification-2018.wav");

    sounds = {{"Background", std::make_shared<sf::SoundBuffer>(background)},
              {"Collision", std::make_shared<sf::SoundBuffer>(collision)},
              {"Fall", std::make_shared<sf::SoundBuffer>(fall)},
              {"Lose", std::make_shared<sf::SoundBuffer>(lose)},
              {"Win", std::make_shared<sf::SoundBuffer>(win)}};

    return sounds;
}


std::unordered_map<std::string, TexturePtr> ResourceManager::initTextures()
{
    std::unordered_map<std::string, TexturePtr> textures;
    sf::Texture BackgroundTexture;
    sf::Texture BackgroundMainTexture;
    sf::Texture heart;
    sf::Texture sound;
    sf::Texture soundOff;

    BackgroundTexture.loadFromFile("../images/game4.jpg");
    BackgroundMainTexture.loadFromFile("../images/brick.jpeg");
    heart.loadFromFile("../images/heart.png");
    sound.loadFromFile("../images/sound.png");
    soundOff.loadFromFile("../images/mute.png");
    textures = {{"Background", std::make_shared<sf::Texture>(BackgroundTexture)},
                {"MainBackground", std::make_shared<sf::Texture>(BackgroundMainTexture)},
                {"heart", std::make_shared<sf::Texture>(heart)},
                {"sound_on", std::make_shared<sf::Texture>(sound)},
                {"sound_off", std::make_shared<sf::Texture>(soundOff)}};
    return textures;
}

colorPtr ResourceManager::getPrevColor(sf::Color a_color)
{
    int index = 0;
    for (size_t i = 0; i < m_colors.size(); i++) {
        if (*(m_colors[i]) == a_color) {
            index = i - 1;
            break;
        }
    }
    
    return m_colors[index];
}

colorPtr ResourceManager::getColor(int a_index)
{
    return m_colors[a_index];
}

FontPtr ResourceManager::getFont(std::string const &a_name)
{
    return m_fonts.at(a_name);
}

SoundPtr ResourceManager::getSound(std::string const &a_name)
{
    return m_sounds.at(a_name);
}

TexturePtr ResourceManager::getTexture(std::string const &a_name)
{
    return m_textures.at(a_name);
}