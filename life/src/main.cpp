#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <string>

const unsigned int WORLD_WIDTH = 200;
const unsigned int WORLD_HEIGHT = 200;
const unsigned int CELL_SIZE = 5;
const sf::Color LIVE_COLOR = sf::Color::White;
const sf::Color DEAD_COLOR = sf::Color::Black;
const sf::Color GRID_COLOR = sf::Color::Red;

struct Point {
    bool is_live = false;
};

void initWorld(std::vector<std::vector<Point>>& world) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    // for (unsigned int y = 0; y < WORLD_HEIGHT; ++y) {
    //     for (unsigned int x = 0; x < WORLD_WIDTH; ++x) {
    //         world[y][x].is_live = dis(gen) == 1;
    //     }
    // }
    // 
    world[WORLD_HEIGHT/2][WORLD_WIDTH/2].is_live = true;
    world[WORLD_HEIGHT/2][WORLD_WIDTH/2+1].is_live = true;
    world[WORLD_HEIGHT/2+1][WORLD_WIDTH/2].is_live = true;
    world[WORLD_HEIGHT/2+1][WORLD_WIDTH/2+1].is_live = true;
}

void updateWorld(std::vector<std::vector<Point>>& world, const std::vector<std::vector<Point>>& prevWorld) {
    auto countLiveNeighbors = [&](unsigned int x, unsigned int y) -> unsigned int {
        unsigned int count = 0;
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                if (dx == 0 && dy == 0) continue;
                int nx = x + dx;
                int ny = y + dy;
                if (nx >= 0 && ny >= 0 && nx < WORLD_WIDTH && ny < WORLD_HEIGHT && prevWorld[ny][nx].is_live) {
                    ++count;
                }
            }
        }
        return count;
    };

    for (unsigned int y = 0; y < WORLD_HEIGHT; ++y) {
        for (unsigned int x = 0; x < WORLD_WIDTH; ++x) {
            unsigned int liveNeighbors = countLiveNeighbors(x, y);
            
            world[y][x].is_live = liveNeighbors == 2 && !world[y][x].is_live;
            
        }
        
    }
}


void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

bool worldsAreEqual(const std::vector<std::vector<Point>>& w1, const std::vector<std::vector<Point>>& w2) {
    for (unsigned int y = 0; y < WORLD_HEIGHT; ++y) {
        for (unsigned int x = 0; x < WORLD_WIDTH; ++x) {
            if (w1[y][x].is_live != w2[y][x].is_live) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    // sf::RenderWindow window(sf::VideoMode(WORLD_WIDTH * CELL_SIZE, WORLD_HEIGHT * CELL_SIZE), "Game of Life", sf::Style::Titlebar | sf::Style::Close);

    // std::vector<std::vector<Point>> world(WORLD_HEIGHT, std::vector<Point>(WORLD_WIDTH));
    // std::vector<std::vector<Point>> prevWorld(WORLD_HEIGHT, std::vector<Point>(WORLD_WIDTH));

    // initWorld(world);

    // sf::Font font;
    // if (!font.loadFromFile("arial.ttf")) {
    //     return EXIT_FAILURE;
    // }

    // sf::Text statusText;
    // statusText.setFont(font);
    // statusText.setCharacterSize(24);
    // statusText.setFillColor(sf::Color::Red);

    // sf::Clock clock;
    // const sf::Time updateInterval = sf::seconds(0.05f);

    // bool optimalDetected = false;
    // bool showGrid = false;
    // bool isPaused = false;

    // while (window.isOpen()) {
    //     sf::Event event;
    //     while (window.pollEvent(event)) {
    //         if (event.type == sf::Event::Closed) {
    //             window.close();
    //         }
    //         else if (event.type == sf::Event::KeyPressed) {
    //             if (event.key.code == sf::Keyboard::G) {
    //                 showGrid = !showGrid;  
    //             }
    //             else if (event.key.code == sf::Keyboard::P) {
    //                 isPaused = !isPaused;  
    //                 if (isPaused) {
    //                     statusText.setString("Paused");
    //                 }
    //             }
    //             else if (event.key.code == sf::Keyboard::R) {
    //                 initWorld(world);  
    //                 isPaused = false;
    //                 optimalDetected = false;
    //                 statusText.setString("");
    //             }
    //         }
    //     }

    //     if (!isPaused && clock.getElapsedTime() >= updateInterval) {
    //         clock.restart();
    //         prevWorld = world;
    //         updateWorld(world, prevWorld);

    //         if (worldsAreEqual(world, prevWorld)) {
    //             statusText.setString("Optimal configuration detected");
    //             optimalDetected = true;
    //         }

    //         bool allDead = true;
    //         for (const auto& row : world) {
    //             for (const auto& cell : row) {
    //                 if (cell.is_live) {
    //                     allDead = false;
    //                     break;
    //                 }
    //             }
    //             if (!allDead) break;
    //         }

    //         if (allDead) {
    //             statusText.setString("All points died");
    //             optimalDetected = true;
    //         }
    //     }

    //     window.clear(sf::Color::Black);

    //     for (unsigned int y = 0; y < WORLD_HEIGHT; ++y) {
    //         for (unsigned int x = 0; x < WORLD_WIDTH; ++x) {
    //             sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    //             cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
    //             cell.setFillColor(world[y][x].is_live ? LIVE_COLOR : DEAD_COLOR);
    //             window.draw(cell);
    //         }
    //     }

    //     if (showGrid) {
    //         sf::VertexArray gridLines(sf::Lines);

    //         for (unsigned int i = 0; i <= WORLD_WIDTH; ++i) {
    //             gridLines.append(sf::Vertex(sf::Vector2f(i * CELL_SIZE, 0), GRID_COLOR));
    //             gridLines.append(sf::Vertex(sf::Vector2f(i * CELL_SIZE, WORLD_HEIGHT * CELL_SIZE), GRID_COLOR));
    //         }

    //         for (unsigned int i = 0; i <= WORLD_HEIGHT; ++i) {
    //             gridLines.append(sf::Vertex(sf::Vector2f(0, i * CELL_SIZE), GRID_COLOR));
    //             gridLines.append(sf::Vertex(sf::Vector2f(WORLD_WIDTH * CELL_SIZE, i * CELL_SIZE), GRID_COLOR));
    //         }

    //         window.draw(gridLines);
    //     }

    //     if (optimalDetected || isPaused) {
    //         window.draw(statusText);
    //     }

    //     window.display();
    // }

    return 0;
}