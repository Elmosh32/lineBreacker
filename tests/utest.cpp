#include <SFML/Graphics.hpp>

#include "game.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Line Breaker", sf::Style::Close);
    Game game = Game(1200, 800, window);
    game.gameStartWindow();
}


