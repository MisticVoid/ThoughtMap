#include "core.h"

ButtonPanel::ButtonPanel(int sizeX, int sizeY, int posX, int posY) :background{ sf::Vector2f(sizeX,sizeY) }, posX{ posX }, posY{ posY }{
    background.setFillColor(sf::Color::Cyan);
}

void ButtonPanel::addButton(Button&& but, int index) {
    if (index < 0)
        buttons.push_back(std::move(but));
    else
        buttons[index] = std::move(but);
}

void ButtonPanel::draw(sf::RenderWindow& win) {
    win.draw(background);
    for (auto& it : buttons)
        it.draw(win);
}

bool ButtonPanel::checkPressed(int x, int y) {
    for (auto& it : buttons)
        if (it.pressed(x, y))
            return true;
    return false;
}

bool ButtonPanel::checkPressedR(int x, int y) {
    return false;
}