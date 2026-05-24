#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include <random>
#include <unordered_set>
#include <chrono>
#include <thread>
#include <sstream>
#include <iomanip>

using namespace std;
using namespace sf;

const int GRID_SIZE = 30;
const int CELL_SIZE = 25;
const int WINDOW_SIZE = GRID_SIZE * CELL_SIZE;
const int DELAY_MS = 20;
const int MAP_SIZE = 10;

// карта с картинки моего 8 варианта
const int PRESET_MAP[MAP_SIZE][MAP_SIZE] = {
    { 0, 1, 1, 0, 0, 1, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
    { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 0, 1, 1, 0, 1, 1, 0 },
    { 0, 1, 0, 1, 0, 0, 1, 0, 0, 0 },
    { 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 0, 0, 0, 0, 1, 0, 1, 0, 0 },
    { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

enum CellType {
    EMPTY,
    OBSTACLE,
    START,
    END,
    PATH,
    VISITED,
    OPEN_SET
};

struct Cell {
    int x, y;
    CellType type;
    double f, g, h;
    double weight;
    Cell* parent;
    vector<Cell*> neighbors;

    Cell(int x, int y) : x(x), y(y), type(EMPTY), f(0), g(0), h(0), weight(0.0), parent(nullptr) {}

    bool operator==(const Cell& other) const {
        return x == other.x && y == other.y;
    }

    void update_neighbors(vector<vector<Cell>>& grid) {
        neighbors.clear();
        int dx[] = {0, 0, -1, 1};
        int dy[] = {-1, 1, 0, 0};
        for (int i = 0; i < 4; i++) {
            int new_x = x + dx[i];
            int new_y = y + dy[i];
            if (isValid(new_x, new_y) && grid[new_x][new_y].type != OBSTACLE) {
                neighbors.push_back(&grid[new_x][new_y]);
            }
        }
    }

    static bool isValid(int x, int y) {
        return x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE;
    }
};

double heuristic(const Cell& a, const Cell& b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

bool isValid(int x, int y) {
    return x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE;
}

vector<vector<Cell>> make_grid() {
    vector<vector<Cell>> grid;
    for (int i = 0; i < GRID_SIZE; i++) {
        vector<Cell> row;
        for (int j = 0; j < GRID_SIZE; j++) {
            row.push_back(Cell(i, j));
        }
        grid.push_back(row);
    }
    return grid;
}

void load_preset_map(vector<vector<Cell>>& grid, Cell*& start, Cell*& end) {
    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j].type = EMPTY;
            grid[i][j].weight = 0.0;
            grid[i][j].f = grid[i][j].g = grid[i][j].h = 0;
            grid[i][j].parent = nullptr;
        }

    for (int row = 0; row < MAP_SIZE; row++)
        for (int col = 0; col < MAP_SIZE; col++)
            if (PRESET_MAP[row][col] == 1)
                grid[col][row].type = OBSTACLE;

    start = &grid[0][3];
    end   = &grid[9][9];
    start->type = START;
    start->weight = 0.0;
    end->type = END;
    end->weight = 0.0;
}

void draw_grid(RenderWindow& window, vector<vector<Cell>>& grid, bool show_weights) {
    static bool font_loaded = false;
    static Font loaded_font;

    if (!font_loaded) {
        if (loaded_font.loadFromFile("arial.ttf")) {
            font_loaded = true;
        } else {
            std::cout << "Failed to load arial.ttf" << std::endl;
        }
    }

    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            RectangleShape cell(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
            cell.setPosition(i * CELL_SIZE, j * CELL_SIZE);

            switch (grid[i][j].type) {
            case EMPTY:
                if (show_weights) {
                    int gray_value = 255 - (int)(grid[i][j].weight * 200);
                    cell.setFillColor(Color(gray_value, gray_value, gray_value));
                } else {
                    cell.setFillColor(Color::White);
                }
                break;
            case OBSTACLE:
                cell.setFillColor(Color::Black);
                break;
            case START:
                cell.setFillColor(Color(255, 165, 0));
                break;
            case END:
                cell.setFillColor(Color(64, 224, 208));
                break;
            case PATH:
                cell.setFillColor(Color(128, 0, 128));
                break;
            case VISITED:
                cell.setFillColor(Color::Red);
                break;
            case OPEN_SET:
                cell.setFillColor(Color::Green);
                break;
            }

            window.draw(cell);

            if (show_weights && grid[i][j].type != OBSTACLE &&
                grid[i][j].type != START && grid[i][j].type != END && font_loaded) {
                Text weight_text;
                weight_text.setFont(loaded_font);
                stringstream ss;
                ss << fixed << setprecision(1) << grid[i][j].weight;
                weight_text.setString(ss.str());
                weight_text.setCharacterSize(9);
                if (grid[i][j].type == VISITED || grid[i][j].type == PATH) {
                    weight_text.setFillColor(Color::White);
                } else if (grid[i][j].type == OPEN_SET) {
                    weight_text.setFillColor(Color::Black);
                } else {
                    weight_text.setFillColor(grid[i][j].weight > 0.6 ? Color::White : Color::Black);
                }
                FloatRect text_bounds = weight_text.getLocalBounds();
                weight_text.setPosition(
                    i * CELL_SIZE + (CELL_SIZE - text_bounds.width) / 2,
                    j * CELL_SIZE + (CELL_SIZE - text_bounds.height) / 2 - 5
                );
                window.draw(weight_text);
            }
        }
    }
}

struct CompareCells {
    bool operator()(const Cell* a, const Cell* b) {
        return a->f > b->f;
    }
};

vector<Cell*> reconstruct_path(Cell* current) {
    vector<Cell*> path;
    while (current != nullptr) {
        path.push_back(current);
        current = current->parent;
    }
    reverse(path.begin(), path.end());
    return path;
}

vector<Cell*> a_star_algorithm(RenderWindow& window, vector<vector<Cell>>& grid,
                                Cell* start, Cell* end, bool use_weights) {

    // open_set это очередь с приоритетом, туда кладём ячейки которые надо проверить
    // сортируется по f, то есть сначала берём ту что выглядит перспективнее
    priority_queue<Cell*, vector<Cell*>, CompareCells> open_set;

    // open_set_hash это хэш-сет, нужен чтобы быстро проверить есть ли ячейка в open_set
    // в очереди с приоритетом нельзя нормально искать поэтому дублируем
    unordered_set<Cell*> open_set_hash;

    // closed_set это ячейки которые мы уже полностью обработали, их трогать не надо
    unordered_set<Cell*> closed_set;

    // стартовая ячейка: g=0 потому что мы ещё никуда не прошли
    // h считаем как манхэттенское расстояние до конца
    // f = g + h, это и есть приоритет
    start->g = 0;
    start->h = heuristic(*start, *end);
    start->f = start->h;

    // кидаем старт в очередь, с него и начнём
    open_set.push(start);
    open_set_hash.insert(start);

    int step_counter = 0;

    // крутимся пока есть что проверять
    while (!open_set.empty()) {

        // обрабатываем события окна прямо внутри цикла алгоритма
        // иначе окно зависнет пока алгоритм работает
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return vector<Cell*>();
            }
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape) {
                    return vector<Cell*>();
                }
            }
        }

        // берём ячейку с наименьшим f, она самая перспективная
        Cell* current = open_set.top();
        open_set.pop();
        open_set_hash.erase(current);

        // если добрались до конца то, восстанавливаем путь
        if (current == end) {
            cout << "Путь найден за " << step_counter << " шагов!" << endl;
            return reconstruct_path(current);
        }

        // помечаем текущую ячейку как посещённую
        if (current->type != START) {
            current->type = VISITED;
        }

        // добавляем в closed_set чтобы больше не возвращаться сюда
        closed_set.insert(current);

        // обновляем список соседей текущей ячейки (4 стороны, без диагоналей)
        current->update_neighbors(grid);

        // перебираем всех соседей
        for (Cell* neighbor : current->neighbors) {

            // если сосед уже в closed_set, мы его уже обработали, пропускаем
            if (closed_set.find(neighbor) != closed_set.end()) {
                continue;
            }

            // считаем новую стоимость пути до этого соседа через текущую ячейку
            // если режим с весами то прибавляем вес клетки, иначе просто 1
            double tentative_g;
            if (use_weights) {
                tentative_g = current->g + 1.0 + neighbor->weight;
            } else {
                tentative_g = current->g + 1.0;
            }

            // если сосед ещё не в открытом списке, добавляем его туда
            if (open_set_hash.find(neighbor) == open_set_hash.end()) {
                neighbor->parent = current;
                neighbor->g = tentative_g;
                neighbor->h = heuristic(*neighbor, *end);
                neighbor->f = neighbor->g + neighbor->h;
                if (neighbor->type != END) {
                    neighbor->type = OPEN_SET;
                }
                open_set.push(neighbor);
                open_set_hash.insert(neighbor);
            }
            // если сосед уже в открытом списке но мы нашли путь к нему короче, обновляем
            else if (tentative_g < neighbor->g) {
                neighbor->parent = current;
                neighbor->g = tentative_g;
                neighbor->f = neighbor->g + neighbor->h;
            }
        }

        step_counter++;

        // перерисовываем экран после каждого шага чтобы была красивая визуализация
        window.clear();
        draw_grid(window, grid, use_weights);
        window.display();
        this_thread::sleep_for(chrono::milliseconds(DELAY_MS));
    }

    // если очередь опустела а конец не найден, значит пути нет
    return vector<Cell*>();
}

void generate_random_obstacles(vector<vector<Cell>>& grid, float obstacle_density = 0.25f) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dis(0.0f, 1.0f);
    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++)
            if (grid[i][j].type == EMPTY && dis(gen) < obstacle_density)
                grid[i][j].type = OBSTACLE;
}

void generate_random_weights(vector<vector<Cell>>& grid) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> weight_dis(0.0f, 1.0f);
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j].type == EMPTY) {
                float rand_val = weight_dis(gen);
                if (rand_val < 0.5)
                    grid[i][j].weight = weight_dis(gen) * 0.2;
                else if (rand_val < 0.75)
                    grid[i][j].weight = 0.2 + weight_dis(gen) * 0.3;
                else if (rand_val < 0.9)
                    grid[i][j].weight = 0.5 + weight_dis(gen) * 0.3;
                else
                    grid[i][j].weight = 0.8 + weight_dis(gen) * 0.2;
            }
        }
    }
}

int main() {
    RenderWindow window(VideoMode(WINDOW_SIZE, WINDOW_SIZE), "A* Algorithm Visualization");
    window.setFramerateLimit(60);

    vector<vector<Cell>> grid = make_grid();

    Cell* start = &grid[0][0];
    Cell* end = &grid[GRID_SIZE - 1][GRID_SIZE - 1];
    start->type = START;
    start->weight = 0.0;
    end->type = END;
    end->weight = 0.0;

    generate_random_obstacles(grid, 0.25f);

    bool algorithm_run = false;
    bool use_weights = false;
    bool preset_map_mode = false;
    vector<Cell*> path;

    cout << "=== A* Algorithm Visualization ===" << endl;
    cout << "Управление:" << endl;
    cout << "SPACE - запустить алгоритм A*" << endl;
    cout << "W - переключить режим с весами/без весов" << endl;
    cout << "R - сгенерировать новое поле" << endl;
    cout << "M - загрузить карту из изображения (10x10, без весов)" << endl;
    cout << "ESC - прервать выполнение алгоритма" << endl;
    cout << endl;
    cout << "Текущий режим: БЕЗ весов" << endl;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            if (event.type == Event::KeyPressed) {

                if (event.key.code == Keyboard::Space && !algorithm_run) {
                    bool run_weights = preset_map_mode ? false : use_weights;
                    cout << "Запуск алгоритма A* в режиме "
                         << (run_weights ? "С весами" : "БЕЗ весов");
                    if (preset_map_mode) cout << " [карта из изображения]";
                    cout << "..." << endl;

                    for (int i = 0; i < GRID_SIZE; i++) {
                        for (int j = 0; j < GRID_SIZE; j++) {
                            if (grid[i][j].type == VISITED ||
                                grid[i][j].type == OPEN_SET ||
                                grid[i][j].type == PATH) {
                                grid[i][j].type = EMPTY;
                            }
                            grid[i][j].f = 0;
                            grid[i][j].g = 0;
                            grid[i][j].h = 0;
                            grid[i][j].parent = nullptr;
                        }
                    }

                    if (preset_map_mode) {
                        for (int row = 0; row < MAP_SIZE; row++)
                            for (int col = 0; col < MAP_SIZE; col++)
                                if (PRESET_MAP[row][col] == 1)
                                    grid[col][row].type = OBSTACLE;
                        start->type = START;
                        end->type = END;
                    }

                    path = a_star_algorithm(window, grid, start, end, run_weights);
                    algorithm_run = true;

                    if (!path.empty()) {
                        double total_weight = 0;
                        for (Cell* cell : path) {
                            if (cell->type != START && cell->type != END) {
                                cell->type = PATH;
                                total_weight += cell->weight;
                            }
                        }
                        cout << "Путь найден! Длина: " << path.size() << " ячеек" << endl;
                        if (run_weights)
                            cout << "Суммарный вес пути: " << fixed << setprecision(2) << total_weight << endl;
                    } else {
                        cout << "Путь не найден!" << endl;
                    }
                }

                if (event.key.code == Keyboard::W && !algorithm_run && !preset_map_mode) {
                    use_weights = !use_weights;
                    if (use_weights) {
                        generate_random_weights(grid);
                        cout << "Режим переключен: С весами" << endl;
                    } else {
                        for (int i = 0; i < GRID_SIZE; i++)
                            for (int j = 0; j < GRID_SIZE; j++)
                                grid[i][j].weight = 0.0;
                        cout << "Режим переключен: БЕЗ весов" << endl;
                    }
                    for (int i = 0; i < GRID_SIZE; i++)
                        for (int j = 0; j < GRID_SIZE; j++)
                            if (grid[i][j].type == VISITED ||
                                grid[i][j].type == OPEN_SET ||
                                grid[i][j].type == PATH)
                                grid[i][j].type = EMPTY;
                    path.clear();
                    algorithm_run = false;
                }

                if (event.key.code == Keyboard::R) {
                    cout << "Генерация нового поля..." << endl;
                    preset_map_mode = false;

                    grid = make_grid();
                    start = &grid[0][0];
                    end = &grid[GRID_SIZE - 1][GRID_SIZE - 1];
                    start->type = START;
                    start->weight = 0.0;
                    end->type = END;
                    end->weight = 0.0;

                    generate_random_obstacles(grid, 0.25f);
                    if (use_weights)
                        generate_random_weights(grid);

                    algorithm_run = false;
                    path.clear();
                    cout << "Новое поле создано. Режим: "
                         << (use_weights ? "С весами" : "БЕЗ весов") << endl;
                }

                if (event.key.code == Keyboard::M) {
                    cout << "Загрузка карты из изображения (10x10, без весов)..." << endl;
                    preset_map_mode = true;

                    grid = make_grid();
                    load_preset_map(grid, start, end);

                    algorithm_run = false;
                    path.clear();
                    cout << "Карта загружена. Старт: (0,3), Конец: (9,9)" << endl;
                    cout << "Нажмите SPACE для запуска A* по этой карте (без весов)." << endl;
                }
            }
        }

        window.clear();
        draw_grid(window, grid, preset_map_mode ? false : use_weights);
        window.display();
    }

    return 0;
}