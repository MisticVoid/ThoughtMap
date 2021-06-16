#include "core.h"

sf::Color alterColor(sf::Color c) {
    return sf::Color::White - c + sf::Color::Black;
}

Cell::Cell(Cell&& cel) : active{} {
    posX = cel.posX;
    posY = cel.posY;
    sizeX = cel.sizeX;
    sizeY = cel.sizeY;
    shp = std::move(cel.shp);
    text = cel.text;
    col = cel.col;
    resetTextPos();
}

Cell& Cell::operator=(Cell&& cel) {
    active = false;
    posX = cel.posX;
    posY = cel.posY;
    sizeX = cel.sizeX;
    sizeY = cel.sizeY;
    shp = std::move(cel.shp);
    text = cel.text;
    resetTextPos();
    col = cel.col;
    return *this;
}

Cell::Cell(int posX, int posY, int sizeX, int sizeY, ShapeB&& shape, sf::Text text,sf::Color col) :posX{ (double)posX }, posY{ (double)posY },
sizeX{ sizeX }, sizeY{ sizeY }, shp{ std::move(shape) }, text{ text }, col{col}, active{}{
    resetTextPos();
    shp.setPosition(posX, posY);
}

void Cell::activate() {
    if (!active) {
        active = true;
        color = true;
        text.setOutlineThickness(3);
        text.setOutlineColor(text.getFillColor());
        text.setFillColor(alterColor(text.getFillColor()));
    }
}

void Cell::deactive() {
    active = false;
    text.setOutlineThickness(0);
    if (color) {
        color = false;
        text.setFillColor(alterColor(text.getFillColor()));
    }
}

void Cell::draw(sf::RenderTarget& win) {
    shp.draw(win);
    win.draw(text);
}

bool Cell::pressed(int x, int y) {
    return posX <= x && x <= posX + sizeX && posY <= y && y <= posY + sizeY;
}

bool Cell::pressedR(int x, int y, Map& map) {
    if (pressed(x,y)) {
        map.correctLastActive(this);
        activate();
        return true;
    }
    return false;
}

void Cell::resetTextPos() {
    text.setPosition(sf::Vector2f(posX + sizeX / 2 - text.getGlobalBounds().width / 2, posY + sizeY / 2 - text.getGlobalBounds().height / 2 - text.getCharacterSize() / 2 + 10));
}

void Cell::addLeter(int letter) {
    sf::String ss = text.getString();
    ss += { char(letter) };
    text.setString(ss);
    if (sizeX - text.getGlobalBounds().width < 10) {
        ss[ss.getSize() - 1] = '\n';
        ss += { char(letter) };
    }
    text.setString(ss);
    resetTextPos();
}

void Cell::delLetter() {
    sf::String ss = text.getString();
    if (ss.getSize()) {
        ss.erase(ss.getSize() - 1);
        if (ss[ss.getSize() - 1] == '\n')
            ss.erase(ss.getSize() - 1);
        text.setString(ss);
        resetTextPos();
    }
}

void Cell::saveToFile(std::fstream& os) {
    os.write(reinterpret_cast<const char*>(&posX), sizeof(int));
    os.write(reinterpret_cast<const char*>(&posY), sizeof(int));
    os.write(reinterpret_cast<const char*>(&sizeX), sizeof(int));
    os.write(reinterpret_cast<const char*>(&sizeY), sizeof(int));
    int t = shp.getType();
    os.write(reinterpret_cast<const char*>(&t), sizeof(int));
    sf::Uint32 c = col.toInteger();
    os.write(reinterpret_cast<const char*>(&c), sizeof(sf::Uint32));
    c = text.getFillColor().toInteger();
    os.write(reinterpret_cast<const char*>(&c), sizeof(sf::Uint32));
    sf::String s = text.getString();
    int n = s.getSize();
    for (int i = 0; i < n; i++)
        os.write(reinterpret_cast<const char*>(&s[i]), sizeof(s[i]));
    sf::Uint32 zero = 0;
    os.write(reinterpret_cast<const char*>(&zero), sizeof(sf::Uint32));
}

Cell Cell::loadFromFile(std::fstream& is, sf::Font& font) {
    int ps[4];
    is.read(reinterpret_cast<char*>(ps), sizeof(int) * 4);
    int t;
    is.read(reinterpret_cast<char*>(&t), sizeof(int));
    sf::Uint32 cc;
    is.read(reinterpret_cast<char*>(&cc), sizeof(sf::Uint32));
    sf::Uint32 cctext;
    is.read(reinterpret_cast<char*>(&cctext), sizeof(sf::Uint32));
    sf::Uint32 c;
    sf::Text text("", font, 24);
    is.read(reinterpret_cast<char*>(&c), sizeof(sf::Uint32));
    while (c != 0) {
        text.setString(text.getString() + c);
        is.read(reinterpret_cast<char*>(&c), sizeof(sf::Uint32));
    }
    text.setFillColor(sf::Color(cctext));
    return Cell(ps[0], ps[1], ps[2], ps[3], ShapeB::generateShape(ShapeB::types(t), ps, sf::Color(cc)), text,sf::Color(cc));
}

void Cell::move(double x, double y) {
    posX += x;
    posY += y;
    shp.setPosition(posX,posY);
    resetTextPos();
}

void Cell::resize(int beginX, int beginY, int endX, int endY) {
    posX = beginX;
    posY = beginY;
    sizeX = endX - beginX;
    sizeY = endY - beginY;
    int T[4] = { posX,posY,sizeX,sizeY };
    shp.changeShape(shp.getType(),T);
    resetTextPos();
}