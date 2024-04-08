#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

using namespace sf;

void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

// where is the player?
enum class side { LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];

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

    // Make a tree sprite
    Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");
    Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(810, 0);

    // Prepare the bee
    Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");
    Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(400, 200);
    bool beeActive = false;
    float beeSpeed = 0.0f;

    // Make cloud sprites
    Texture textureCloud;
    textureCloud.loadFromFile("graphics/cloud.png");
    Sprite spriteCloud1;
    Sprite spriteCloud2;
    Sprite spriteCloud3;
    spriteCloud1.setTexture(textureCloud);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud3.setTexture(textureCloud);
    spriteCloud1.setPosition(0,0);
    spriteCloud2.setPosition(0,150);
    spriteCloud3.setPosition(0,300);
    bool cloud1Active = false;
    bool cloud2Active = false;
    bool cloud3Active = false;
    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;

    Clock clock;
    //Time bar
    RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(Color::Red);
    timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

    Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    bool paused = true;

    int score = 0;
    Text messageText;
    Text scoreText;
    Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");
    messageText.setFont(font);
    scoreText.setFont(font);
    messageText.setString("Press Enter to start!");
    scoreText.setString("Score = 0");
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);
    messageText.setFillColor(Color::White);
    scoreText.setFillColor(Color::White);

    FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin(
        textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f
    );
    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
    scoreText.setPosition(20, 20);

    // prepare branches
    Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");
    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000, -2000);
        branches[i].setOrigin(220, 20);
    }

    // delete later
    updateBranches(1);
    updateBranches(2);
    updateBranches(3);
    updateBranches(4);
    updateBranches(5);

    while (window.isOpen())
    {
        // Handle player input
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        // Start game
        if (Keyboard::isKeyPressed(Keyboard::Return))
        {
            paused = false;
            score = 0;
            timeRemaining = 6;
        }



        // Update scene
        if (!paused)
        {
            Time dt = clock.restart();
            timeRemaining -= dt.asSeconds();
            timeBar.setSize(
                Vector2f(
                    timeBarWidthPerSecond * timeRemaining,
                    timeBarHeight
                    )
                );
            
            if (timeRemaining >= 0.0f)
            {
                paused = true;
                messageText.setString("Out of time!!!");
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(
                    textRect.left + textRect.width / 2.0f,
                    textRect.top + textRect.height / 2.0f
                );
                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
            }

            // Bee setup
            if (!beeActive)
            {
                srand((int)time(0));
                beeSpeed = (rand() % 200) + 200;

                srand((int)time(0) * 10);
                float height = (rand() % 500) + 500;
                spriteBee.setPosition(2000, height);
                beeActive = true;
            }
            else
            {
                // Move bee
                spriteBee.setPosition(
                    spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()),
                    spriteBee.getPosition().y
                );
                // if bee reaches the left edge of screen
                if (spriteBee.getPosition().x < -100)
                {
                    beeActive = false; // new bee for next frame
                }
            }

            // Manage clouds
            if (!cloud1Active)
            {
                srand((int)time(0) * 10);
                cloud1Speed = (rand() % 200);

                srand((int)time(0) * 10);
                float height = (rand() % 150);
                spriteCloud1.setPosition(-200, height);
                cloud1Active = true;
            }
            else
            {
                spriteCloud1.setPosition(
                    spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()),
                    spriteCloud1.getPosition().y
                );
            }
            // if cloud reaches right edge of screen
            if (spriteCloud1.getPosition().x > 1920)
            {
                cloud1Active = false; // new cloud1 next frame
            }

            if (!cloud2Active)
            {
                srand((int)time(0) * 10);
                cloud2Speed = (rand() % 200);

                srand((int)time(0) * 10);
                float height = (rand() % 150);
                spriteCloud2.setPosition(-200, height);
                cloud2Active = true;
            }
            else
            {
                spriteCloud2.setPosition(
                    spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()),
                    spriteCloud2.getPosition().y
                );
            }
            // if cloud reaches right edge of screen
            if (spriteCloud2.getPosition().x > 1920)
            {
                cloud2Active = false; // new cloud2 next frame
            }

            if (!cloud3Active)
            {
                srand((int)time(0) * 10);
                cloud3Speed = (rand() % 200);

                srand((int)time(0) * 10);
                float height = (rand() % 150);
                spriteCloud3.setPosition(-200, height);
                cloud3Active = true;
            }
            else
            {
                spriteCloud3.setPosition(
                    spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()),
                    spriteCloud3.getPosition().y
                );
            }
            // if cloud reaches right edge of screen
            if (spriteCloud3.getPosition().x > 1920)
            {
                cloud3Active = false; // new cloud1 next frame
            }

            // update score
            std::stringstream ss;
            ss << "Score = " << score;
            scoreText.setString(ss.str());

            // update branches
            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                float height = i * 150;
                if (branchPositions[i] == side::LEFT)
                {
                    branches[i].setPosition(610, height); // move sprite to left side
                    branches[i].setRotation(180); // flip sprite
                }
                else if (branchPositions[i] == side::RIGHT)
                {
                    branches[i].setPosition(1330, height); // move sprite to right side
                    branches[i].setRotation(0); // set sprite rotation to normal
                }
                else
                {
                    branches[i].setPosition(3000, height); // hide branch
                }
            }
        }

        // Draw scene
        window.clear();

        // Draw game scene
        window.draw(spriteBackground);

        // Draw clouds
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);

        // Draw branches
        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            window.draw(branches[i]);
        }

        // Draw tree
        window.draw(spriteTree);

        // Draw bee
        window.draw(spriteBee);

        // Draw score
        window.draw(scoreText);

        // Draw timebar
        window.draw(timeBar);

        if (paused)
        {
            window.draw(messageText);
        }

        // Show everything we drew
        window.display();
    }

    return 0;
}

void updateBranches(int seed)
{
    // move all branches down one place
    for (int i = NUM_BRANCHES - 1; i > 0; i--)
    {
        branchPositions[i] = branchPositions[i - 1];
    }
    
    // spawn new branch at position 0
    srand((int)time(0) + seed);
    int r = (rand() % 5);
    switch (r) {
        case 0:
            branchPositions[0] = side::LEFT;
            break;
        case 1:
            branchPositions[0] = side::RIGHT;
            break;
        default:
            branchPositions[0] = side::NONE;
            break;
    }
}