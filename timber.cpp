#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

int main() {
    // Create a video mode object
    VideoMode vm(1920, 1080);

    // Create and open a window for the game
    RenderWindow window(vm, "Timber!!!", Style::Fullscreen);

    // Create a texture to hold a graphic
    Texture textureBackground;

    // Load graphic into texture
    textureBackground.loadFromFile("graphics/background.png");

    // Create a sprite
    Sprite spriteBackground;

    // Attach the texture to the sprite
    spriteBackground.setTexture(textureBackground);

    // Set the spriteBackground to cover the screen
    spriteBackground.setPosition(0,0);

    while (window.isOpen())
    {
        // Handle player input
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        // Update scene

        // Draw scene
        window.clear();

        // Draw game scene
        window.draw(spriteBackground);

        // Show everything we drew
        window.display();
    }

    return 0;
}