#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <memory>
#include <typeinfo>
#include <algorithm>
#include <string>

using colorPtr = std::shared_ptr<sf::Color>;
using FontPtr = std::shared_ptr<sf::Font>;
using SoundPtr = std::shared_ptr<sf::SoundBuffer>;
using TexturePtr = std::shared_ptr<sf::Texture>;

class ResourceManager {
public:
    static ResourceManager &getInstance() {
        static ResourceManager resource;
        return resource;
    }

public:
    FontPtr getFont(std::string const &name);
    SoundPtr getSound(std::string const &a_name);
    TexturePtr getTexture(std::string const &a_name);

    colorPtr getPrevColor(sf::Color a_color);
    colorPtr getColor(int a_index);

    std::unordered_map<std::string, TexturePtr> initTextures();
    std::unordered_map<std::string, SoundPtr> initSounds();
    std::unordered_map<std::string, FontPtr> initFonts();
    std::vector<colorPtr> initColors();

private:
    ResourceManager();
    ResourceManager(ResourceManager const &) = delete;
    ResourceManager &operator=(ResourceManager const &) = delete;

    ~ResourceManager() = default;

private:
    std::vector<colorPtr> m_colors;
    std::unordered_map<std::string, FontPtr> m_fonts;
    std::unordered_map<std::string, SoundPtr> m_sounds;
    std::unordered_map<std::string, TexturePtr> m_textures;
};

#endif // RESOURCE_MANAGER_HPP
