#pragma once

#include <vector>


const int WIDTH = 800;
const int HEIGHT = 600;
const int SIZE = 20;
const int NUM_X = WIDTH / SIZE;
const int NUM_Y = HEIGHT / SIZE;
const float MOVE_INTERVAL = 0.1f; 

enum Direction { UP, DOWN, LEFT, RIGHT };


struct SnakeSegment {
    static int new_id;
    int x, y, id;
    SnakeSegment(int x, int y) : x(x), y(y) {id = new_id++;}
};

class Snake {
public:
    Snake();
    void move();
    void grow();
    void changeDirection(Direction newDirection);
    std::vector<SnakeSegment> getSegments() const;
    
    

private:
    std::vector<SnakeSegment> segments;
    Direction direction;
};
