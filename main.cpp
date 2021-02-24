#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

static const float SCALE = 30.f;
static const float BOX_SIZE = 64.f;
static const float GRAVITY = 9.8f;

static const float SCREEN_WIDTH = 1920.f;
static const float SCREEN_HEIGHT = 1080.f;

void createGround(b2World &world, float x, float y);

void createBox(b2World &world, int x, int y);

int main() {
    sf::Texture boxTexture;
    boxTexture.loadFromFile("box.png");

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Box2D & SFML");

    b2Vec2 gravity(0.f, GRAVITY);
    b2World world(gravity);

    createGround(world, SCREEN_WIDTH / 2, SCREEN_HEIGHT);

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                const sf::Vector2i &mousePosition = sf::Mouse::getPosition(window);
                createBox(world, mousePosition.x, mousePosition.y);
            }
        }

        sf::Time deltaTime = clock.restart();

        world.Step(deltaTime.asSeconds(), 8, 3);

        window.clear(sf::Color::Black);

        for (b2Body *bodyIterator = world.GetBodyList(); bodyIterator != 0; bodyIterator = bodyIterator->GetNext()) {
            if (bodyIterator->GetType() == b2_dynamicBody) {
                sf::Sprite boxSprite;
                boxSprite.setTexture(boxTexture);
                boxSprite.setOrigin(32.f, 32.f);
                boxSprite.setPosition(bodyIterator->GetPosition().x * SCALE, bodyIterator->GetPosition().y * SCALE);
                boxSprite.setRotation(bodyIterator->GetAngle() * 180 / b2_pi);
                window.draw(boxSprite);
            }
        }

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
    shape.SetAsBox((SCREEN_WIDTH / 2) / SCALE, (2.f / 2) / SCALE);

    b2FixtureDef fixtureDef;
    fixtureDef.density = 0.f;
    fixtureDef.shape = &shape;

    body->CreateFixture(&fixtureDef);
}

void createBox(b2World &world, int x, int y) {
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(x / SCALE, y / SCALE);
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