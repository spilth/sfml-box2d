#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <sstream>
#include <random>

static const float BOX2D_SCALE = 30.f;
static const float BOX_SIZE = 64.f;
static const float GRAVITY = 9.8f;

static const int SCREEN_WIDTH = 1920.f;
static const int SCREEN_HEIGHT = 1080.f;

void createGround(b2World &world, float x, float y);

void createBox(b2World &world, float x, float y, float radians);

int main() {
    float timescale;
    float verticalGravity = GRAVITY;

    std::stringstream info;

    std::random_device randomDevice;
    std::mt19937 twisterEngine(randomDevice());
    std::uniform_real_distribution<double> distribution(0.0, 2.0);

    sf::Font jetBrainsMono;
    jetBrainsMono.loadFromFile("fonts/JetBrainsMonoNL-Regular.ttf");

    sf::Text infoText("", jetBrainsMono, 18);
    infoText.setPosition(8, 8);

    sf::Texture boxTexture;
    boxTexture.loadFromFile("images/box.png");

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Box2D & SFML");
    window.setFramerateLimit(60);

    b2Vec2 gravity(0.f, verticalGravity);
    b2World world(gravity);

    createGround(world, SCREEN_WIDTH / 2.f, SCREEN_HEIGHT);

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                const sf::Vector2i &mousePosition = sf::Mouse::getPosition(window);
                createBox(world, mousePosition.x, mousePosition.y, distribution(twisterEngine));
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            timescale = 0.25f;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            timescale = 4.0f;
        } else {
            timescale = 1.0f;
        }

        sf::Time deltaTime = clock.restart() * timescale;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
            verticalGravity += 10.f * deltaTime.asSeconds();
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            verticalGravity -= 10.f * deltaTime.asSeconds();
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
            verticalGravity = GRAVITY;
        }

        info.str("");
        info << "Click the mouse to create a box." << "\n";
        info << "Press Up/Down keys to change Gravity. Return to reset it." << "\n";
        info << "Press Left/Right keys to change Timescale." << "\n";
        info << "Pres Esc to Quit." << "\n";
        info << "Gravity: " << verticalGravity << "\n";
        info << "Timescale: " << timescale << "\n";

        infoText.setString(info.str());

        world.SetGravity(b2Vec2(0.f, verticalGravity));
        world.Step(deltaTime.asSeconds(), 8, 3);

        window.clear(sf::Color::Black);

        for (b2Body *body = world.GetBodyList(); body != nullptr; body = body->GetNext()) {
            if (body->GetType() == b2_dynamicBody) {
                sf::Sprite boxSprite;
                boxSprite.setTexture(boxTexture);
                boxSprite.setOrigin(32.f, 32.f);
                boxSprite.setPosition(body->GetPosition().x * BOX2D_SCALE, body->GetPosition().y * BOX2D_SCALE);
                boxSprite.setRotation(body->GetAngle() * 180 / b2_pi);
                window.draw(boxSprite);
            }
        }

        window.draw(infoText);
        window.display();
    }

    return EXIT_SUCCESS;
}

void createGround(b2World &world, float x, float y) {
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(x / BOX2D_SCALE, y / BOX2D_SCALE);
    bodyDef.type = b2_staticBody;
    b2Body *body = world.CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox((SCREEN_WIDTH / 2.f) / BOX2D_SCALE, (2.f / 2) / BOX2D_SCALE);

    b2FixtureDef fixtureDef;
    fixtureDef.density = 0.f;
    fixtureDef.shape = &shape;

    body->CreateFixture(&fixtureDef);
}

void createBox(b2World &world, float x, float y, float radians) {
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(x / BOX2D_SCALE, y / BOX2D_SCALE);
    bodyDef.angle = radians * b2_pi;
    bodyDef.type = b2_dynamicBody;
    b2Body *body = world.CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox((BOX_SIZE / 2) / BOX2D_SCALE, (BOX_SIZE / 2) / BOX2D_SCALE);

    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.f;
    fixtureDef.friction = 0.7f;
    fixtureDef.shape = &shape;

    body->CreateFixture(&fixtureDef);
}