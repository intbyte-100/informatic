#include "Snake.h"


int SnakeSegment::new_id = 0;
Snake::Snake() {
    direction = RIGHT;
    segments.push_back(SnakeSegment(NUM_X / 2, NUM_Y / 2));
}

void Snake::move() {
    
    for (int i = segments.size() - 1; i > 0; --i) {
        segments[i].x = segments[i - 1].x;
        segments[i].y = segments[i - 1].y;
    }

    
    if (direction == UP) segments[0].y -= 1;
    if (direction == DOWN) segments[0].y += 1;
    if (direction == LEFT) segments[0].x -= 1;
    if (direction == RIGHT) segments[0].x += 1;

    
    if (segments[0].x < 0) segments[0].x = NUM_X - 1;
    if (segments[0].x >= NUM_X) segments[0].x = 0;
    if (segments[0].y < 0) segments[0].y = NUM_Y - 1;
    if (segments[0].y >= NUM_Y) segments[0].y = 0;
}

void Snake::grow() {
    segments.push_back(SnakeSegment(segments.back().x - 1, segments.back().y - 1));
}

void Snake::changeDirection(Direction newDirection) {
    if ((direction == UP && newDirection != DOWN) ||
        (direction == DOWN && newDirection != UP) ||
        (direction == LEFT && newDirection != RIGHT) ||
        (direction == RIGHT && newDirection != LEFT)) {
        direction = newDirection;
    }
}

std::vector<SnakeSegment> Snake::getSegments() const {
    return segments;
}