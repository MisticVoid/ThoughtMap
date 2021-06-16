#include "core.h"

void MainControl::activate(){
    sf::Event event;
    sf::Clock clk;
    clk.restart();
    sf::Time t;
    while (window.isOpen()){
        if (delView)
            deleteView();
        t = clk.getElapsedTime();
        window.clear();
        if (view) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            mapView->checkCycle(mousePos.x, mousePos.y);
        }
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f worldPos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (view) {
                        mapView->checkPressed(worldPos.x, worldPos.y);
                    }
                    else {
                        menu->checkPressed(worldPos.x, worldPos.y);
                    }
                }
                if (event.mouseButton.button == sf::Mouse::Right) {
                    if (view) {
                        mapView->checkPressedR(worldPos.x, worldPos.y);
                    }
                }
            }
            else if (view) {
                sf::Vector2f worldPos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                if (event.type == sf::Event::MouseButtonReleased) {
                    if (event.mouseButton.button == sf::Mouse::Left)
                        mapView->onRealiseL(worldPos.x, worldPos.y);
                }
                else if (event.type == sf::Event::MouseWheelScrolled)
                        mapView->zoom(event.mouseWheelScroll.delta);
                else if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Down)
                        mapView->down(t);
                    else if (event.key.code == sf::Keyboard::Up)
                        mapView->up(t);
                    else if (event.key.code == sf::Keyboard::Right)
                        mapView->right(t);
                    else if (event.key.code == sf::Keyboard::Left)
                        mapView->left(t);
                    else if (event.key.code >= sf::Keyboard::A && event.key.code <= sf::Keyboard::Z || event.key.code==sf::Keyboard::BackSpace || event.key.code == sf::Keyboard::Space)
                        if (mapView->isInput())
                            mapView->inputK(event.key.code, sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift));
                        
                }
            }
        }
        if (view) {
            mapView->draw(window);
        }
        else{
            menu->draw(window);
        }
        window.display();
    }
}

void MainControl::deleteView() {
    delete mapView;
    mapView = nullptr;
    delView = false;
}

void MainControl::close() {
    window.close();
}

void MainControl::creatNewView(bool clear) {
    view = true;
    delete menu;
    menu = nullptr;
    mapView = new View(1920,1080,fl.font,this, clear);
}

void MainControl::loadMap() {
    mapView->loadFromFile(fl.font);
}

void MainControl::toMenu() {
    view = false;
    menu = new Menu(1920, 1080, fl.font, this);
    delView = true;
}

MainControl::~MainControl() {
    if (menu)
        delete menu;
    if (mapView)
        delete mapView;
}

