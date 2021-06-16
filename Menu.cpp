#include "core.h"

void closeWin(void* contr) {
    static_cast<MainControl*>(contr)->close();
}

void newView(void* contr) {
    static_cast<MainControl*>(contr)->creatNewView(false);
}

void loadView(void* contr) {
    static_cast<MainControl*>(contr)->creatNewView(true);
    static_cast<MainControl*>(contr)->loadMap();
}

Menu::Menu(int x, int y, sf::Font& font, MainControl* contr) : sizeX{ x }, sizeY{ y }, background{ sf::RectangleShape(sf::Vector2f(x, y)) }, butNew{}{
    if (!tex.loadFromFile("resourse/backMenu.jpg"))
        std::cout << "error" << std::endl;
    background.setTexture(&tex);
    sf::ConvexShape* box = new sf::ConvexShape(6);
    sf::Vector2f points[6] = { {50,0},{0,50},{50,100},{550,100},{600,50},{550,0} };
    for (int i = 0; i < 6; i++)
        box->setPoint(i, points[i]);
    box->setFillColor(sf::Color::Blue);
    int posX = sizeX / 2 - 600 / 2;
    butNew = Button(posX, 400, 600, 100, box, ShapeB::types::RECTANGLE, sf::Text("NEW MAP", font), newView, contr); //TODO add fun
    sf::ConvexShape* box2 = new sf::ConvexShape(*box);
    sf::ConvexShape* box3 = new sf::ConvexShape(*box);
    butLoad = Button(posX, 600, 600, 100, box2, ShapeB::types::RECTANGLE, sf::Text("LOAD MAP", font), loadView, contr); //TODO add fun
    butExit = Button(posX, 800, 600, 100, box3, ShapeB::types::RECTANGLE, sf::Text("EXIT", font), closeWin, contr);
    sf::Texture* texTitle = new sf::Texture();
    texTitle->loadFromFile("resourse/title.png");
    int T[4] = { x / 2 - 800 / 2,200 / 2,800,137 };
    title = ShapeB::generateShape(ShapeB::types::RECTANGLE, T, sf::Color::White);
    title.setTexture(texTitle);
}

void Menu::draw(sf::RenderWindow& win) {
    win.draw(background);
    title.draw(win);
    butNew.draw(win);
    butLoad.draw(win);
    butExit.draw(win);
}

void Menu::checkPressed(int x, int y) {
    if (!butNew.pressed(x, y))
        if (!butLoad.pressed(x, y))
            butExit.pressed(x, y);
}