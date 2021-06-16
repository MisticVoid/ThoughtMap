#include "core.h"

Button::Button(Button&& but) {
    posX = but.posX;
    posY = but.posY;
    sizeX = but.sizeX;
    sizeY = but.sizeY;
    shp = std::move(but.shp);
    text = but.text;
    fun = but.fun;
    data = but.data;
    offsetX = but.offsetX;
    offsetX = but.offsetY;
    text.setPosition(sf::Vector2f(posX + sizeX / 2 - text.getGlobalBounds().width / 2, posY + sizeY / 2 - text.getGlobalBounds().height));
}

Button::Button(int posX, int posY, int sizeX, int sizeY, sf::Shape* shape, ShapeB::types t, sf::Text text, void (*fun)(void*), void* data, int offsetX, int offsetY) :posX{ posX }, posY{ posY },
sizeX{ sizeX }, sizeY{ sizeY }, shp{ posX,posY,shape,t }, text{ text }, offsetX{ offsetX }, offsetY{ offsetY }, fun{ fun }, data{ data } {
    text.setPosition(sf::Vector2f(posX + sizeX / 2 - text.getGlobalBounds().width / 2, posY + sizeY / 2 - text.getGlobalBounds().height));
}

Button::Button(int posX, int posY, int sizeX, int sizeY, ShapeB&& shape, sf::Text text, void (*fun)(void*), void* data, int offsetX, int offsetY) : posX{ posX }, posY{ posY },
sizeX{ sizeX }, sizeY{ sizeY }, shp{ std::move(shape) }, text{ text }, offsetX{ offsetX }, offsetY{ offsetY }, fun{ fun }, data{ data }{
    text.setPosition(sf::Vector2f(posX + sizeX / 2 - text.getGlobalBounds().width / 2, posY + sizeY / 2 - text.getGlobalBounds().height));
    shp.setPosition(posX + offsetX, posY + offsetY);
}

Button& Button::operator=(Button&& but) {
    posX = but.posX;
    posY = but.posY;
    sizeX = but.sizeX;
    sizeY = but.sizeY;
    shp = std::move(but.shp);
    text = but.text;
    fun = but.fun;
    data = but.data;
    text.setPosition(sf::Vector2f(posX + sizeX / 2 - text.getGlobalBounds().width / 2, posY + sizeY / 2 - text.getGlobalBounds().height));
    return *this;
}

void Button::draw(sf::RenderTarget& win) {
    shp.draw(win);
    win.draw(text);
}

bool Button::pressed(int x, int y) {
    if (posX <= x && x <= posX + sizeX && posY <= y && y <= posY + sizeY) {
        fun(data);
        return true;
    }
    return false;
}