#include "core.h"

void save(void* view) {
    static_cast<View*>(view)->saveToFile();
}

void addC(void* view) {
    static_cast<View*>(view)->beginAddingEl(Map::types::CELL);
}

void addL(void* view) {
    static_cast<View*>(view)->beginAddingEl(Map::types::LINE);
}

void addI(void* view) {
    static_cast<View*>(view)->beginAddingEl(Map::types::IMG);
}

void erase(void* view) {
    static_cast<View*>(view)->activeErase();
}

void colorP(void* view) {
    static_cast<View*>(view)->activeColor();
}

void shapeP(void* view) {
    static_cast<View*>(view)->activeShape();
}

void moveB(void* map) {
    static_cast<Map*>(map)->moveActive();
}

void resizeB(void* map) {
    static_cast<Map*>(map)->resizeActive();
}

void menu(void* contr) {
    static_cast<MainControl*>(contr)->toMenu();
}

void close(void* contr) {
    static_cast<MainControl*>(contr)->close();
}

void View::buttonAdder(int posX, void (*fun)(void*), void* dataT, const char name[],sf::Font& font) {
    sf::RectangleShape* rec = new sf::RectangleShape(sf::Vector2f(80, 80));
    sf::Texture* tex = new sf::Texture();
    tex->loadFromFile(name);
    rec->setTexture(tex);
    panel.addButton(Button(posX, 0, 100, 100, ShapeB(0, 0, rec, ShapeB::types::RECTANGLE, tex), sf::Text("", font), fun, dataT, 10, 10));
}

View::View(int sizeX, int sizeY, sf::Font& font, MainControl* contr, bool clear) :map{ sizeX,sizeY - 100,0,100,font,clear }, panel{ sizeX, 100, 0, 0 }, font{ &font },
addEl{}, holdactive{}, tmp{}, tmp2{}, tmp3{}, e{}, colPicker{ sizeX - 300,100,300,900,this }, shpPicker{sizeX-400,100,400,600,this} {
    void (*funT[11])(void*) = {save, addC, addL, addI, erase, colorP, shapeP,moveB,resizeB,menu, close };
    const char names[11][200] = { "resourse/save.png","resourse/addsq.png","resourse/addln.png","resourse/addimg.png","resourse/erase.png","resourse/paint.png","resourse/shape.png","resourse/move.png","resourse/resize.png","resourse/menu.png","resourse/x.png" };
    for (int i = 0; i < 7; i++) {
        buttonAdder(i * 100, funT[i], this, names[i], font);
    }
    for (int i = 7; i < 9; i++) {
        buttonAdder(i * 100, funT[i], &map, names[i], font);
    }
    for (int i = 10; i >= 9; i--) {
        buttonAdder(sizeX - (11-i) * 100, funT[i], contr, names[i], font);
    }
}

void View::draw(sf::RenderWindow& win) {
    map.draw(win);
    panel.draw(win);
    colPicker.draw(win);
    shpPicker.draw(win);
}

void View::checkPressed(int x, int y) {
    if (addEl != Map::types::EMPTY) {
        holdactive = true;
        sf::Vector2f pos = map.getCord(x, y);
        beginX = pos.x;
        beginY = pos.y;
        if (addEl == Map::types::CELL) {
            sf::RectangleShape* sha = new sf::RectangleShape(sf::Vector2f(0, 0));
            sha->setFillColor(sf::Color::Blue);
            tmp = ShapeB(pos.x, pos.y, sha, ShapeB::types::RECTANGLE);
        }
        if (addEl == Map::types::LINE) {
            tmp2.setBegin(pos.x, pos.y);
            tmp2.setEnd(pos.x, pos.y);
        }
    }
    else if (map.getMove() != Map::types::EMPTY) {
        if (map.checkPressed(x, y)) {
            holdactive = true;
            sf::Vector2f pos = map.getCord(x, y);
            beginX = pos.x;
            beginY = pos.y;
        }
    }
    else if (map.getResize() != Map::types::EMPTY) {
        if (map.getResize() != Map::types::ANYTHING) {
            holdactive = true;
            sf::Vector2f pos = map.getCord(x, y);
            beginX = pos.x;
            beginY = pos.y;
        }
        else
            map.checkPressed(x, y);
    }
    else {
        if (!panel.checkPressed(x, y)) {
            if (!colPicker.checkPressed(x, y)) {
                if(!shpPicker.checkPressed(x,y))
                    map.checkPressed(x, y);
                else {
                    map.clearAllLastActive();
                }
            }
            else {
                map.clearAllLastActive();
            }
        }
        else {
            map.clearAllLastActive();
        }

        if (e != 1) {
            map.clearErase();
        }
        if (e != 2) {
            colPicker.makeInvisible();
        }
        if (e != 3) {
            map.colorFalse();
        }
        if (e != 4) {
            shpPicker.makeInvisible();
        }
        if (e != 5) {
            map.shapeFalse();
        }
        e = 0;
    }
}

void View::onRealiseL(int x, int y) {
    if (addEl == Map::types::CELL && holdactive) {
        sf::Vector2f pos = map.getCord(x, y);
        ((sf::RectangleShape*)(tmp.getShape()))->setSize(sf::Vector2f(abs(pos.x - beginX), abs(pos.y - beginY)));
        map.addCell(Cell(std::min(pos.x, float(beginX)), std::min(pos.y, float(beginY)), abs(pos.x - beginX), abs(pos.y - beginY), std::move(tmp), sf::Text("T", *font, 25),sf::Color::Blue));
        addEl = Map::types::EMPTY;
        holdactive = false;
    }
    else if (addEl == Map::types::LINE && holdactive) {
        sf::Vector2f pos = map.getCord(x, y);
        tmp2.setBegin(beginX, beginY);
        tmp2.setEnd(pos.x, pos.y);
        map.addLine(std::move(tmp2));
        addEl = Map::types::EMPTY;
        holdactive = false;
    }
    else if (addEl == Map::types::IMG && holdactive) {
        sf::Vector2f pos = map.getCord(x, y);
        sf::RectangleShape* rec = new sf::RectangleShape(sf::Vector2f(abs(pos.x - beginX), abs(pos.y - beginY)));
        sf::Texture* tex = new sf::Texture();
        PWSTR name = fileOpen();
        if (name) {
            tex->loadFromFile(sf::String(name));
            rec->setTexture(tex);
            tmp3 = Img(std::min(pos.x, float(beginX)), std::min(pos.y, float(beginY)), abs(pos.x - beginX), abs(pos.y - beginY), ShapeB(std::min(pos.x, float(beginX)), std::min(pos.y, float(beginY)), rec, ShapeB::types::RECTANGLE, tex), name);
            map.addImg(std::move(tmp3));
        }
        addEl = Map::types::EMPTY;
        holdactive = false;
    }
    else if (map.getResize() != Map::types::ANYTHING && map.getResize() != Map::types::EMPTY && holdactive) {
        sf::Vector2f pos = map.getCord(x, y);
        map.resize(beginX, beginY, pos.x, pos.y);
    }
    if (map.getMove()!=Map::types::ANYTHING && map.getMove() != Map::types::EMPTY) {
        map.clearMove();
        holdactive = false;
    }
    if (map.getResize() != Map::types::ANYTHING && holdactive) {
        map.clearResize();
        holdactive = false;
    }
}

void View::checkPressedR(int x, int y) {
    map.clearMove();
    map.clearResize();
    holdactive = false;
    if (!panel.checkPressedR(x, y))
        map.checkPressedR(x, y);
    else {
        map.clearAllLastActive();
    }
    map.clearErase();
    colPicker.makeInvisible();
    map.colorFalse();
    shpPicker.makeInvisible();
    map.shapeFalse();
}

void View::saveToFile() {
    PWSTR fp = fileSave();
    if (fp) {
        std::fstream os(fp, std::fstream::out | std::fstream::binary | std::fstream::trunc);
        if (!os)
            std::cout << "error save failed" << std::endl;
        else
            std::cout << "saving" << std::endl;
        map.saveToFile(os);
        os.close();
    }
    else
        std::cout << "error save failed incorect path" << std::endl;
}

void View::loadFromFile(sf::Font& font) {
    PWSTR fp = fileOpen();
    if (fp) {
        std::fstream is(fp, std::fstream::in | std::fstream::binary);
        if (!is)
            std::cout << "error load failed" << std::endl;
        else
            std::cout << "load" << std::endl;
        map.loadFromFile(is, font);
        is.close();
    }
    else
        std::cout << "error load failed incorect path" << std::endl;
}

void View::checkCycle(double x, double y) {
    if (map.getMove() != Map::types::ANYTHING && map.getMove() != Map::types::EMPTY) {
        sf::Vector2f pos = map.getCord(x, y);
        map.move(pos.x - beginX, pos.y - beginY);
        beginX = pos.x;
        beginY = pos.y;
    }
}