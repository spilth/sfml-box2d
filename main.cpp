#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <sstream>
#include <random>

static const float SCALE = 30.f;
static const float BOX_SIZE = 64.f;
static const float GRAVITY = 9.8f;

static const int SCREEN_WIDTH = 1920.f;
static const int SCREEN_HEIGHT = 1080.f;

void createGround(b2World &world, float x, float y);

void createBox(b2World &world, float x, float y, float radians);

int main() {
    float timescale;
    float verticalGravity = GRAVITY;

    std::stringstream output;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.0, 2.0);

    sf::Font font;
    font.loadFromFile("fonts/JetBrainsMonoNL-Regular.ttf");

    sf::Text text;
    text.setFont(font);
    text.setPosition(8, 8);
    text.setCharacterSize(18);

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
                createBox(world, mousePosition.x, mousePosition.y, dist(mt));
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

        output.str("");
        output << "Click the mouse to create a box." << "\n";
        output << "Press Up/Down keys to change Gravity. Return to reset it." << "\n";
        output << "Press Left/Right keys to change Timescale." << "\n";
        output << "Pres Esc to Quit." << "\n";
        output << "Gravity: " << verticalGravity << "\n";
        output << "Timescale: " << timescale << "\n";

        text.setString(output.str());

        world.SetGravity(b2Vec2(0.f, verticalGravity));
        world.Step(deltaTime.asSeconds(), 8, 3);

        window.clear(sf::Color::Black);

        for (b2Body *body = world.GetBodyList(); body != nullptr; body = body->GetNext()) {
            if (body->GetType() == b2_dynamicBody) {
                sf::Sprite boxSprite;
                boxSprite.setTexture(boxTexture);
                boxSprite.setOrigin(32.f, 32.f);
                boxSprite.setPosition(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
                boxSprite.setRotation(body->GetAngle() * 180 / b2_pi);
                window.draw(boxSprite);
            }
        }

        window.draw(text);
        window.display();
    }

    return EXIT_SUCCESS;
}

void createGround(b2World &world, float x, float y) {
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(x / SCALE, y / SCALE);
    bodyDef.type = b2_staticBody;
    b2Body *body = world.CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox((SCREEN_WIDTH / 2.f) / SCALE, (2.f / 2) / SCALE);

    b2FixtureDef fixtureDef;
    fixtureDef.density = 0.f;
    fixtureDef.shape = &shape;

    body->CreateFixture(&fixtureDef);
}

void createBox(b2World &world, float x, float y, float radians) {
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(x / SCALE, y / SCALE);
    bodyDef.angle = radians * b2_pi;
    bodyDef.type = b2_dynamicBody;
    b2Body *body = world.CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox((BOX_SIZE / 2) / SCALE, (BOX_SIZE / 2) / SCALE);

    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.f;
    fixtureDef.friction = 0.7f;
    fixtureDef.shape = &shape;

    body->CreateFixture(&fixtureDef);
}