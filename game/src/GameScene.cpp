#include "GameScene.h"
#include "GameView.h"
#include <algorithm>
#include <cstdlib>
#include <float.h>
#include <math.h>


using sf::Keyboard;
using sf::Vector2f;
using sf::Vector2i;

static const float PLAYER_SPEED = 200;

static Vector2f Normalize(const Vector2f &value)
{
    const float length = std::hypotf(value.x, value.y);
    if (length < FLT_EPSILON)
    {
        return Vector2f(0, 0);
    }

    return value / length;
}

static Vector2f Round(const Vector2f &value)
{
    return Vector2f(roundf(value.x), roundf(value.y));
}

static float y_impulse = 0;
static float y_velocity = 0;
static bool can_jump_again = true;

static Vector2f GetPlayerDirection()
{
    Vector2f direction;


    if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up))
    {
        if (can_jump_again) {
            y_impulse = -1;
            can_jump_again = false;
        }
    }
    if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left))
    {
        direction.x = -1;
    }
    else if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right))
    {
        direction.x = +1;
    }


    return Normalize(direction);
}



GameScene *NewGameScene()
{
    GameScene *pLogic = new GameScene;
    TmxLevel &level = pLogic->level;

    level.LoadFromFile("res/platformer.tmx");
    pLogic->player = level.GetFirstObject("player");
    pLogic->coins = level.GetAllObjects("coin");
    pLogic->enemies = level.GetAllObjects("enemy");
    pLogic->blocks = level.GetAllObjects("block");

    pLogic->startPosition = pLogic->player.sprite.getPosition();

    return pLogic;
}

void UpdateGameScene(void *pData, GameView &view, float deltaSec)
{

    GameScene *pLogic = reinterpret_cast<GameScene *>(pData);

    (void)deltaSec;

    TmxObject &player = pLogic->player;
    auto direction = GetPlayerDirection();



    if (y_impulse < 0) {
        y_velocity = y_impulse * std::abs(y_impulse);
    } else {
        y_velocity = y_impulse * y_impulse;
    }

    y_impulse += (deltaSec * (std::abs(y_impulse) < 0.4 ? 10 : 1)) * 2;

    y_impulse = (std::min(1.0f, y_impulse));


    direction.y = y_velocity * 2;

    const Vector2f movement = Round(direction * PLAYER_SPEED * deltaSec);

    bool hasXCollision = false;
    bool hasYCollision = false;



    for (auto i = pLogic->blocks.begin(); i != pLogic->blocks.end(); i++) {
        auto rect = player.sprite.getGlobalBounds();

        rect.top += movement.y;

        hasYCollision = hasYCollision || rect.intersects(i->rect);

        if (hasYCollision) {
            rect.top -= movement.y;
            y_impulse = 0;
        }

        rect.left += movement.x;

        hasXCollision = hasXCollision || rect.intersects(i->rect);

    }

    can_jump_again = hasYCollision && movement.y > 0;

    player.MoveBy({hasXCollision ? 0 : movement.x, hasYCollision ? 0 : movement.y});


    for (auto i = pLogic->coins.begin(); i != pLogic->coins.end();) {
        if (pLogic->player.sprite.getGlobalBounds().intersects(i->sprite.getGlobalBounds())) {
            i = pLogic->coins.erase(i);
        }else {
            ++i;
        }
    }


    for (auto i = pLogic->enemies.begin(); i != pLogic->enemies.end(); i++) {
        if (pLogic->player.sprite.getGlobalBounds().intersects(i->sprite.getGlobalBounds())) {
            pLogic->player.sprite.setPosition(pLogic->startPosition);
        }
    }

    const Vector2i windowSize = view.windowSize;
    SetCameraCenter(view, player.sprite.getPosition() + Vector2f(windowSize.x / 4, windowSize.y / 4));
}

void DrawGameScene(void *pData, GameView &view)
{
    // Извлекаем указатель на GameLogic, ранее переданный в игровой цикл.
    GameScene *pLogic = reinterpret_cast<GameScene *>(pData);
    sf::RenderTarget &target = view.window;

    pLogic->level.Draw(target);
    for (const TmxObject &coin : pLogic->coins)
    {
        target.draw(coin.sprite);
    }
    for (const TmxObject &enemy : pLogic->enemies)
    {
        target.draw(enemy.sprite);
    }
    target.draw(pLogic->player.sprite);
}

void DestroyGameScene(GameScene *&pScene)
{
    delete pScene;
    pScene = nullptr;
}
