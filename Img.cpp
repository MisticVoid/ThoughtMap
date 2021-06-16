#include "core.h"

PWSTR copyPWSTR(PWSTR str) {
    size_t s = wcslen(str) + 1;
    PWSTR ssc = new WCHAR[s];
    for (int i = 0; i < s - 1; i++)
        ssc[i] = str[i];
    ssc[s - 1] = 0;
    return ssc;
}

Img& Img::operator=(Img&& img) {
    posX = img.posX;
    posY = img.posY;
    sizeX = img.sizeX;
    sizeY = img.sizeY;
    shp = std::move(img.shp);
    name = std::move(img.name);
    return *this;
}

bool Img::pressed(int x, int y) {
    return posX <= x && x <= posX + sizeX && posY <= y && y <= posY + sizeY;
}

bool Img::pressedR(int x, int y) {
    if (pressed(x, y)) {
        PWSTR nameN = fileOpen();
        if (nameN) {
            sf::Texture* tex = new sf::Texture();
            tex->loadFromFile(sf::String(nameN));
            shp.setTexture(tex);
            name = nameN;
        }
        return true;
    }
    return false;
}

void Img::draw(sf::RenderTarget& win) {
    shp.draw(win);
}

void Img::saveToFile(std::fstream& os) {
    os.write(reinterpret_cast<const char*>(&posX), sizeof(int));
    os.write(reinterpret_cast<const char*>(&posY), sizeof(int));
    os.write(reinterpret_cast<const char*>(&sizeX), sizeof(int));
    os.write(reinterpret_cast<const char*>(&sizeY), sizeof(int));
    int t = shp.getType();
    os.write(reinterpret_cast<const char*>(&t), sizeof(int));
    int n = wcslen(name);
    for (int i = 0; i < n; i++)
        os.write(reinterpret_cast<const char*>(&name[i]), sizeof(WCHAR));
    WCHAR zero = 0;
    os.write(reinterpret_cast<const char*>(&zero), sizeof(WCHAR));
}

Img Img::loadFromFile(std::fstream& is) {
    int n[4];
    is.read(reinterpret_cast<char*>(n), sizeof(int) * 4);
    int t;
    is.read(reinterpret_cast<char*>(&t), sizeof(int));
    WCHAR name[4096];
    int i = 0;
    do {
        is.read(reinterpret_cast<char*>(&name[i]), sizeof(WCHAR));
    } while (name[i++] != 0);
    ShapeB shp = ShapeB::generateShape(ShapeB::types(t), n, sf::Color::White);
    sf::Texture* tex = new sf::Texture();
    PWSTR str = copyPWSTR(name);
    if (!tex->loadFromFile(sf::String(str))) {
        tex->loadFromFile("resourse/error.jpg");
        delete str;
        str = new WCHAR[20];
        WCHAR T[20] = L"resourse/error.jpg";
        T[19] = 0;
        for (int i = 0; i < 20; i++)
            str[i] = T[i];
    }
    shp.setTexture(tex);
    return Img(n[0], n[1], n[2], n[3], std::move(shp), str);
}

void Img::move(double x, double y) {
    posX += x;
    posY += y;
    shp.setPosition(posX, posY);
}

void Img::resize(int beginX, int beginY, int endX, int endY) {
    posX = beginX;
    posY = beginY;
    sizeX = endX - beginX;
    sizeY = endY - beginY;
    int T[4] = { posX,posY,sizeX,sizeY };
    shp.changeShape(shp.getType(), T);
}