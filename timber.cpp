#include <sstream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

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

    // prepare player
    Texture texturePlayer;
    texturePlayer.loadFromFile("graphics/player.png");
    Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(580, 720);
    side playerSide = side::LEFT;

    // prepare gravestone
    Texture textureRIP;
    textureRIP.loadFromFile("graphics/rip.png");
    Sprite spriteRIP;
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(600, 860);

    // prepare axe
    Texture textureAxe;
    textureAxe.loadFromFile("graphics/axe.png");
    Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(700, 830);
    // line up axe with tree
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    // prepare log
    Texture textureLog;
    textureLog.loadFromFile("graphics/log.png");
    Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810, 720);
    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;

    // control player input
    bool acceptInput = false;

    // Prepare sounds
    // Player chops sound
    SoundBuffer chopBuffer; // SoundBuffer object loads sound .wav file into RAM
    // this class holds the actual audio data from the sound
    chopBuffer.loadFromFile("sound/chop.wav");
    Sound chop;
    chop.setBuffer(chopBuffer); // associate sound object with SoundBuffer object
    // so we can call the play function of the sound object 

    // sound for player death
    SoundBuffer deathBuffer;
    deathBuffer.loadFromFile("sound/death.wav");
    Sound death;
    death.setBuffer(deathBuffer);

    // sound for out of time
    SoundBuffer ootBuffer;
    ootBuffer.loadFromFile("sound/out_of_time.wav");
    Sound outOfTime;
    outOfTime.setBuffer(ootBuffer);

    while (window.isOpen())
    {
        // Handle player input
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::KeyReleased && !paused)
            {
                // listen for key presses again
                acceptInput = true;
                // hide axe
                spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        // Start game
        if (Keyboard::isKeyPressed(Keyboard::Return))
        {
            paused = false;
            score = 0;
            timeRemaining = 600;

            // make branches disappear, starting in 2nd position
            for (int i = 1; i < NUM_BRANCHES; i++)
            {
                branchPositions[i] = side::NONE;
            }

            // hide gravestone
            spriteRIP.setPosition(675, 2000);

            // position player
            spritePlayer.setPosition(580, 720);

            acceptInput = true;
        }

        if (acceptInput)
        {
            // handle right cursor key
            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                playerSide = side::RIGHT;
                score++;
                timeRemaining += (2 / score) + .15;
                spriteAxe.setPosition(
                    AXE_POSITION_RIGHT,
                    spriteAxe.getPosition().y
                );
                spritePlayer.setPosition(1200, 720);
                updateBranches(score);
                spriteLog.setPosition(810, 720);
                logSpeedX = -5000;
                logActive = true;
                acceptInput = false;
                chop.play();
            }

            // handle left cursor key
            if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                playerSide = side::LEFT;
                score++;
                timeRemaining += (2 / score) + .15;
                spriteAxe.setPosition(
                    AXE_POSITION_LEFT,
                    spriteAxe.getPosition().y
                );
                spritePlayer.setPosition(580, 720);
                updateBranches(score);
                spriteLog.setPosition(810, 720);
                logSpeedX = 5000;
                logActive = true;
                acceptInput = false;
                chop.play();
            }
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
                outOfTime.play();
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

            // handle log
            if (logActive)
            {
                spriteLog.setPosition(
                    spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
                    spriteLog.getPosition().y + (logSpeedY * dt.asSeconds())
                );
                // if log reaches right edge
                if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000)
                {
                    logActive = false;
                    spriteLog.setPosition(810, 720);
                }
            }

            // is the player dead?
            if (branchPositions[5] == playerSide)
            {
                paused = true;
                acceptInput = false;
                spriteRIP.setPosition(525, 760); // draw gravestone
                spritePlayer.setPosition(2000, 660); // hide player
                messageText.setString("SQUISHED!!!");
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(
                    textRect.left + textRect.width / 2.0f,
                    textRect.top + textRect.height / 2.0f
                );
                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
                death.play();
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

        // Draw player
        window.draw(spritePlayer);

        // Draw axe
        window.draw(spriteAxe);

        // Draw log
        window.draw(spriteLog);

        // Draw gravestone
        window.draw(spriteRIP);

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