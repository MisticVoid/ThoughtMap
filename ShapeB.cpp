#include "core.h"

ShapeB::ShapeB(int posX, int posY, sf::Shape* shape, types t, sf::Texture* tex) :pos{ posX,posY }, shape{ shape }, t{ t }, tex{ tex }{
    shape->setPosition(pos);
}

ShapeB::ShapeB(ShapeB&& sha) : pos{ sha.pos }, shape{ sha.shape }, t{ sha.t }, tex{ sha.tex } {
    sha.shape = nullptr;
    sha.tex = nullptr;
}

ShapeB::ShapeB(ShapeB& sha) : pos{ sha.pos }, shape{ sha.shape }, t{ sha.t }, tex{ sha.tex } {
    sha.shape = nullptr;
    sha.tex = nullptr;
}

ShapeB::~ShapeB() {
    if (shape)
        delete shape;
    if (tex)
        delete tex;
}

void ShapeB::setPosition(int x, int y) {
    pos = sf::Vector2f(x, y);
    shape->setPosition(pos);
}

void ShapeB::setTexture(sf::Texture* tex) {
    if (this->tex) 
        delete this->tex;
    this->tex = tex;
    shape->setTexture(this->tex,true); 
}

void ShapeB::draw(sf::RenderTarget& win) {
    win.draw(*shape);
}

ShapeB& ShapeB::operator=(ShapeB&& sha) {
    pos = sha.pos;
    shape = sha.shape;
    sha.shape = nullptr;
    tex = sha.tex;
    sha.tex = nullptr;
    t = sha.t;
    return *this;
}

ShapeB ShapeB::generateShape(types t, int n[], sf::Color col) {
    switch (t) {
    case types::RECTANGLE:
        return generateRectangle(n, col);
        break;
    case types::OCTAGON:
        return generateOctagon(n, col);
        break;
    case types::ELIPSE:
        return generateELIPSE(n, col);
        break;
    }
    throw std::exception("unknow type");
}

ShapeB ShapeB::generateRectangle(int n[4], sf::Color col) {
    sf::RectangleShape* shape = new sf::RectangleShape(sf::Vector2f(n[2], n[3]));
    shape->setFillColor(col);
    return ShapeB(n[0], n[1], shape, types::RECTANGLE);
}

sf::Shape* ShapeB::octagonCreator(int n[4]) {
    sf::ConvexShape* shape = new sf::ConvexShape(8);
    int shift = std::min(n[2], n[3]) / 4;
    int shiftOn[8][2] = { {1,0},{-1,0},{0,1},{0,-1},{-1,0},{1,0},{0,-1},{0,1} };
    int valueOn[8][2] = { {0,0},{1,0},{1,0},{1,1},{1,1},{0,1},{0,1},{0,0} };
    for (int i = 0; i < 8; i++)
        shape->setPoint(i, sf::Vector2f(n[2] * valueOn[i][0] + shift * shiftOn[i][0], n[3] * valueOn[i][1] + shift * shiftOn[i][1]));
    return shape;
}

ShapeB ShapeB::generateOctagon(int n[4], sf::Color col) {
    sf::Shape* shape = octagonCreator(n);
    shape->setFillColor(col);
    return ShapeB(n[0], n[1], shape, types::OCTAGON);
}

ShapeB ShapeB::generateELIPSE(int n[4], sf::Color col) {
    sf::Shape* shape = elpiseCreator(n);
    shape->setFillColor(col);
    return ShapeB(n[0], n[1], shape, types::ELIPSE);
}

sf::Shape* ShapeB::elpiseCreator(int n[4]) {
    sf::ConvexShape* shape = new sf::ConvexShape(40);
    for (int i = 0; i < 40; i++)
        shape->setPoint(i,getPoint(i, n));
    return shape;
}

sf::Vector2f ShapeB::getPoint(int i,int n[4]) {
    static const float pi = 3.141592654f;

    float angle = i * 2 * M_PI / 40 - pi / 2;
    float x = std::cos(angle) * n[2]/2;
    float y = std::sin(angle) * n[3]/2;

    return sf::Vector2f(n[2]/2 + x, n[3]/2 + y);
}

void ShapeB::changeShape(types t, int n[]) {
    sf::Color col=shape->getFillColor();
    delete shape;
    switch (t) {
    case types::RECTANGLE:
        shape = new sf::RectangleShape(sf::Vector2f(n[2], n[3]));
        break;
    case types::OCTAGON:
        shape = octagonCreator(n);
        break;
    case types::ELIPSE:
        shape = elpiseCreator(n);
        break;
    }
    changeColor(col);
    if (tex)
        shape->setTexture(tex);
    setPosition(n[0], n[1]);
    this->t = t;
}

void ShapeB::move(int x, int y) {
    pos += sf::Vector2f(x, y);
    shape->setPosition(pos);
}