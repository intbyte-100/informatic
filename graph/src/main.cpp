#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <functional>


//   // ...
// ...
//

void drawGraph(sf::RenderWindow &window, std::function<float(float)> func,
               float xMin, float xMax, float scaleX, float scaleY,
               sf::Color color) {
  sf::VertexArray graph(sf::LinesStrip);

  for (float x = xMin; x <= xMax; x += 0.1f) {
    float y = func(x); // Вычисление значения функции

    // Преобразование координат в экранные
    float screenX = 400 + x * scaleX;
    float screenY = 300 - y * scaleY;

    // Добавление точки в массив вершин
    graph.append(sf::Vertex(sf::Vector2f(screenX, screenY), color));
  }

  window.draw(graph);
}

int main() {

  sf::RenderWindow window(sf::VideoMode(800, 600),
                          "Приложение для вывода графиков",
                          sf::Style::Titlebar | sf::Style::Close);

  sf::CircleShape userPoint(5); // Радиус 5 пикселей
  userPoint.setFillColor(sf::Color::Red);
  bool userPointExists =
      false; // Переменная для проверки существования пользовательской точки



  sf::Font font;
  
  // чтобы шрифты на всех ос работали
  #ifdef _WIN32
    font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
  #else
    font.loadFromFile("/usr/share/fonts/Adwaita/AdwaitaSans-Regular.ttf");
  #endif
  

  sf::Text coordinatesText("", font, 20);
  coordinatesText.setPosition(10, 10);
  coordinatesText.setFillColor(sf::Color::White);

  // Оси X и Y
  sf::VertexArray xAxis(sf::Lines, 2);
  xAxis[0].position = sf::Vector2f(50, 300);  // Начало оси X
  xAxis[0].color = sf::Color::White;          // Цвет оси
  xAxis[1].position = sf::Vector2f(750, 300); // Конец оси X
  xAxis[1].color = sf::Color::White;

  sf::VertexArray yAxis(sf::Lines, 2);
  yAxis[0].position = sf::Vector2f(400, 50);  // Начало оси Y
  yAxis[0].color = sf::Color::White;          // Цвет оси
  yAxis[1].position = sf::Vector2f(400, 550); // Конец оси Y
  yAxis[1].color = sf::Color::White;

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();

      if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          // Получение позиции клика
          sf::Vector2i mousePos = sf::Mouse::getPosition(window);

          // Преобразование экранных координат в "математические"
          float mathX = (mousePos.x - 400) / 30.0f;  // Масштаб 30 по X
          float mathY = -(mousePos.y - 300) / 30.0f; // Масштаб 100 по Y

          // Установка новой пользовательской точки
          userPoint.setPosition(mousePos.x - userPoint.getRadius(),
                                mousePos.y - userPoint.getRadius());
          userPointExists = true; // Помечаем, что точка существует

          std::string zone;

          bool isAboveABS = mathY > std::abs(mathX);
          bool isAboveLine = mathY > 3;

          if (mathY == std::abs(mathX) || mathY == 3) {
            zone = "The point is on the function plot";
          } else if (isAboveLine && isAboveABS) {
            zone = "2";
          } else if (!isAboveLine && isAboveABS) {
            zone = "1";
          } else if (isAboveLine && !isAboveABS && mathX < 0) {
            zone = "3";
          } else if (!isAboveABS && !isAboveLine) {
            zone = "5";
          } else {
            zone = "4";
          }

          // Обновление текста с координатами точки
          coordinatesText.setString("Coordinates: (" + std::to_string(mathX) +
                                    ", " + std::to_string(mathY) +
                                    ")\nZone: " + zone);
        }
      }
    }

    window.clear(sf::Color::Black);

    //

    // Отрисовка осей
    window.draw(xAxis);
    window.draw(yAxis);

    drawGraph(
        window, [](float x) { return 2.5; }, -10, 10, 30, 30, sf::Color::Blue);

    drawGraph(
        window, [](float x) { return std::abs(x); }, -10, 10, 30, 30,
        sf::Color::Red);

    // Отрисовка пользовательской точки, если она существует
    if (userPointExists) {
      window.draw(userPoint);
      window.draw(coordinatesText);
    }

    // Отображение нового кадра
    window.display();
  }

  return 0;
}
