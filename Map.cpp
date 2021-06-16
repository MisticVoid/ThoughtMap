#include "core.h"

Map::Map(int sizeX, int sizeY, int posX, int posY, sf::Font& font, bool clear) : board{}, v{ sf::Vector2f(sizeX / 2,sizeY / 2),sf::Vector2f(sizeX,sizeY) },
s{}, posX{ posX }, posY{ posY }, cells{}, lines{}, lastActive{}, lastActiveLine{}, lastActiveImg{}, input{}, eraseV{}, col{}, color{}, t{}, moveType{}, resizeType{}{
    board.create(sizeX, sizeY);
    s.setPosition(sf::Vector2f(posX, posY));
    board.setView(v);
    s.setTexture(board.getTexture());
    if (!clear) {
        int T[4] = { sizeX / 2 - 200, sizeY / 2 - 100, 400, 200 };
        addCell(Cell(sizeX / 2 - 200, sizeY / 2 - 100, 400, 200, ShapeB::generateShape(ShapeB::types::OCTAGON, T, sf::Color::Blue), sf::Text("TITLE", font, 24),sf::Color::Blue));
    }
}

void Map::draw(sf::RenderWindow& win) {
    board.clear(sf::Color::White);
    for (auto& it : cells)
        it.draw(board);
    for (auto& it : lines)
        it.draw(board);
    for (auto& it : imgs)
        it.draw(board);
    board.display();
    win.draw(s);
}

void Map::correctLastActive(Cell* cel) {
    if (lastActive != cel) {
        if (lastActive)
            lastActive->deactive();
        lastActive = cel;
        if (lastActive == nullptr)
            input = EMPTY;
        else
            input = CELL;
        deactiveOther();
    }
}

void Map::correctLastActiveImg(Img* img) {
    if (lastActiveImg != img) {
        lastActiveImg = img;
        if (lastActiveImg == nullptr)
            input = EMPTY;
        else
            input = IMG;
        deactiveOther();
    }
}

void Map::correctLastActiveLine(Line* line) {
    if (lastActiveLine != line) {
        lastActiveLine = line;
        if (lastActiveLine == nullptr)
            input = EMPTY;
        else
            input = LINE;
        deactiveOther();
    }
}

void Map::addCell(Cell&& cel) {
    cells.push_back(std::move(cel));
}

void Map::addLine(Line&& line) {
    lines.push_back(std::move(line));
}

void Map::addImg(Img&& img) {
    imgs.push_back(std::move(img));
}

bool Map::checkPressed(int x, int y) {
    sf::Vector2f pos = getCord(x, y);
    clearAllLastActive();
    for (int i = 0; i < lines.size(); i++) {
        if (lines[i].pressed(pos.x, pos.y,zom)) { 
            if (eraseV) {
                lines.erase(lines.begin() + i);
                clearErase();
            }
            else if (color) {
                lines[i].changeColor(col);
            }
            else if (moveType != types::EMPTY) {
                moveType = types::LINE;
                correctLastActiveLine(&lines[i]);
            }
            else if (resizeType != types::EMPTY) {
                resizeType = types::LINE;
                correctLastActiveLine(&lines[i]);
            }
            return true;
        }
    }
    for (int i = 0; i < cells.size(); i++) {
        if (cells[i].pressed(pos.x, pos.y)) {
            if (eraseV) {
                cells.erase(cells.begin() + i);
                clearErase();
            }
            else if (color) {
                cells[i].changeColor(col);
            }
            else if (shape) {
                cells[i].changeShape(t);
            }
            else if (moveType != types::EMPTY) {
                moveType = types::CELL;
                correctLastActive(&cells[i]);
            }
            else if (resizeType != types::EMPTY) {
                resizeType = types::CELL;
                correctLastActive(&cells[i]);
            }
            return true;
        }
    }
    for (int i = 0; i < imgs.size(); i++) {
        if (imgs[i].pressed(pos.x, pos.y)) {
            if (eraseV) {
                imgs.erase(imgs.begin() + i);
                clearErase();
            }
            else if (shape) {
                imgs[i].changeShape(t);
            }
            else if (moveType != types::EMPTY) {
                moveType = types::IMG;
                correctLastActiveImg(&imgs[i]);
            }
            else if (resizeType != types::EMPTY) {
                resizeType = types::IMG;
                correctLastActiveImg(&imgs[i]);
            }
            return true;
        }
    }
    clearErase();
    moveType = types::EMPTY;
    resizeType = types::EMPTY;
    return false;
}

bool Map::checkPressedR(int x, int y) {
    sf::Vector2f pos = getCord(x, y);
    for (auto& it : cells) {
        if (color) {
            if (it.pressed(pos.x, pos.y)) {
                it.changeTextColor(col);
                return true;
            }
        }
        else if (it.pressedR(pos.x, pos.y, *this))
            return true;
    }
    for (auto& it : imgs)
        if (it.pressedR(pos.x, pos.y))
            return true;
    clearAllLastActive();
    return false;
}

void Map::inputK(int key, bool shift) {
    if (input == CELL)
        if (key == sf::Keyboard::BackSpace)
            lastActive->delLetter();
        else if (key == sf::Keyboard::Space)
            lastActive->addLeter(' ');
        else
            lastActive->addLeter(key + 'a' - 32 * shift);
}

void Map::deactiveOther() {
    if (input != CELL) {
        if (lastActive)
            lastActive->deactive();
    }
}

void Map::clearAllLastActive() {
    correctLastActive(nullptr);
    correctLastActiveLine(nullptr);
    correctLastActiveImg(nullptr);
}

void Map::saveToFile(std::fstream& os) {
    char beginMarker[] = "TMAP";
    os.write(beginMarker, sizeof(char) * 4);
    int n = cells.size();
    os.write(reinterpret_cast<const char*>(&n), sizeof(int));
    for (auto& it : cells)
        it.saveToFile(os);
    n = lines.size();
    os.write(reinterpret_cast<const char*>(&n), sizeof(int));
    for (auto& it : lines)
        it.saveToFile(os);
    n = imgs.size();
    os.write(reinterpret_cast<const char*>(&n), sizeof(int));
    for (auto& it : imgs)
        it.saveToFile(os);
}

void Map::loadFromFile(std::fstream& is, sf::Font& font) {
    char beginMarker[4];
    is.read(beginMarker, 4);
    if (!strcmp(beginMarker, "TMAP")) {
        std::cout << "wrong file type" << std::endl;
        return;
    }
    int n;
    is.read(reinterpret_cast<char*>(&n), sizeof(int));
    while (n--) {
        cells.push_back(Cell::loadFromFile(is, font));
    }
    is.read(reinterpret_cast<char*>(&n), sizeof(int));
    while (n--) {
        lines.push_back(Line::loadFromFile(is));
    }
    is.read(reinterpret_cast<char*>(&n), sizeof(int));
    while (n--) {
        imgs.push_back(Img::loadFromFile(is));
    }
}

void Map::move(double x, double y) {
    switch (moveType) {
    case types::CELL:
        lastActive->move(x, y);
        break;
    case types::LINE:
        lastActiveLine->move(x, y);
        break;
    case types::IMG:
        lastActiveImg->move(x, y);
        break;
    }
}

void Map::resize(double beginX,double beginY,double x, double y) {
    switch (getResize()) {
    case types::CELL:
        lastActive->resize(std::min(beginX, (double)x), std::min(beginY, (double)y), std::max(beginX, (double)x), std::max(beginY, (double)y));
        break;
    case types::LINE:
        lastActiveLine->resize(beginX, beginY, x, y);
        break;
    case types::IMG:
        lastActiveImg->resize(std::min(beginX, (double)x), std::min(beginY, (double)y), std::max(beginX, (double)x), std::max(beginY, (double)y));
        break;
    }
}