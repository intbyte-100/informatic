#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <cstdlib>

int main() {

  sf::RenderWindow window(sf::VideoMode(500, 500),
                          "Приложение для вывода матрицы",
                          sf::Style::Titlebar | sf::Style::Close);

  const int cellSize = 50;
  const int gridSize = 10;

  sf::RectangleShape cells[gridSize][gridSize];

  for (int i = 0; i < gridSize; i++) {
    for (int j = 0; j < gridSize; j++) {
      cells[i][j].setSize(sf::Vector2f(cellSize - 2, cellSize - 2));
      cells[i][j].setPosition(i * cellSize, j * cellSize);
      cells[i][j].setFillColor(sf::Color::White);
      cells[i][j].setOutlineColor(sf::Color::Black);
      cells[i][j].setOutlineThickness(1);
    }
  }

  sf::Font font;

// чтобы шрифты на всех ос работали
#ifdef _WIN32
  font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
#else
  font.loadFromFile("/usr/share/fonts/Adwaita/AdwaitaSans-Regular.ttf");
#endif

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();

      if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          // Получение позиции клика
        }
      }
    }

    window.clear(sf::Color::White);

    //

    for (int i = 0; i < gridSize; i++) {
      for (int j = 0; j < gridSize; j++) {
        if ((i + j)%2 == 0) {
            cells[i][j].setFillColor(sf::Color::Green);
        }
        window.draw(cells[i][j]);
      }
    }

    // Отображение нового кадра
    window.display();
  }

  return 0;
}
