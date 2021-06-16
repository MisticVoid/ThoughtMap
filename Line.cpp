#include "core.h"

double distanceFromLine(int p1x, int p1y, int p2x, int p2y, int cx, int cy) {
    return abs((p2x-p1x)*(p1y-cy)-(p1x-cx)*(p2y-p1y))/std::sqrt((p2x - p1x)*(p2x - p1x)+ (p2y - p1y)* (p2y - p1y));
}

Line& Line::operator=(Line&& line) {
    beginX = line.beginX;
    beginY = line.beginY;
    endX = line.endX;
    endY = line.endY;
    col = line.col;
    return *this;
}

bool Line::pressed(int x, int y,double scale) {
    return std::min(beginX, endX)-10 * scale <= x && x <= std::max(beginX, endX) + 10 * scale && std::min(beginY, endY) - 10 * scale <= y && y <= std::max(beginY, endY) + 10 * scale && distanceFromLine(beginX,beginY,endX,endY,x,y)<20 * scale;
}

void Line::draw(sf::RenderTarget& win) {
    sf::Vertex line[2] = { sf::Vertex(sf::Vector2f(beginX,beginY),col),sf::Vertex(sf::Vector2f(endX,endY),col) };
    win.draw(line, 2, sf::Lines);
}

void Line::saveToFile(std::fstream& os) {
    os.write(reinterpret_cast<const char*>(&beginX), sizeof(int));
    os.write(reinterpret_cast<const char*>(&beginY), sizeof(int));
    os.write(reinterpret_cast<const char*>(&endX), sizeof(int));
    os.write(reinterpret_cast<const char*>(&endY), sizeof(int));
    sf::Uint32 c = col.toInteger();
    os.write(reinterpret_cast<const char*>(&c), sizeof(sf::Uint32));
}

Line Line::loadFromFile(std::fstream& is) {
    int n[4];
    is.read(reinterpret_cast<char*>(n), sizeof(int) * 4);
    sf::Uint32 c;
    is.read(reinterpret_cast<char*>(&c), sizeof(sf::Uint32));
    return Line(n[0], n[1], n[2], n[3], sf::Color(c));
}

void Line::move(double x, double y) {
    beginX += x;
    endX += x;
    beginY += y;
    endY += y;
}

void Line::resize(int beginX, int beginY, int endX, int endY) {
    this->beginX = beginX;
    this->beginY = beginY;
    this->endX = endX;
    this->endY = endY;
}