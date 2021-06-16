#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<iostream>
#include<vector>
#include<cmath>
#include<wchar.h>
#include<fstream>
#include "winapi.h"
#include "winapi.h"

#define M_PI 3.14159265358979323846

class ShapeB {
private:
	
	sf::Vector2f pos;
	sf::Shape* shape;
	sf::Texture* tex;
public:
	enum types {
		RECTANGLE = 0,
		OCTAGON = 1,
		ELIPSE = 2
	};
private:
	types t;

public:
	ShapeB() : pos{}, shape{ nullptr }, tex{ nullptr }, t{} {}
	ShapeB(int posX, int posY, sf::Shape* shape,types t, sf::Texture* tex=nullptr);
	ShapeB(ShapeB&& sha);
	ShapeB(ShapeB& sha);
	~ShapeB();
	void draw(sf::RenderTarget& win);
	ShapeB& operator=(ShapeB&& sha);
	void setPosition(int x, int y);
	void setTexture(sf::Texture* tex);
	types getType() { return t; }
	bool isTexture() { return bool(tex); }
	sf::Shape* getShape() { return shape; }
	void changeColor(sf::Color col) { shape->setFillColor(col); }
	void changeShape(types t, int n[]);
	static ShapeB generateShape(types t,int n[], sf::Color col);
	void move(int x, int y);

private:
	static ShapeB generateRectangle(int n[4], sf::Color col);
	static ShapeB generateOctagon(int n[4], sf::Color col);
	static ShapeB generateELIPSE(int n[4], sf::Color col);
	static sf::Shape* octagonCreator(int n[4]);
	static sf::Shape* elpiseCreator(int n[4]);
	static sf::Vector2f getPoint(int i,int n[4]);
};

class Button {
	int posX, posY;
	int sizeX, sizeY;
	ShapeB shp;
	sf::Text text;
	int offsetX, offsetY;
	void (*fun)(void*);
	void* data;
public:
	Button() :posX{}, posY{}, sizeX{}, sizeY{}, shp{}, text{}, offsetX{}, offsetY{}, fun{}, data{} {}
	Button(Button&& but);
	Button(int posX, int posY, int sizeX, int sizeY, sf::Shape* shape,ShapeB::types t, sf::Text text, void (*fun)(void*),void* data, int offsetX=0,int offsetY=0);
	Button(int posX, int posY, int sizeX, int sizeY, ShapeB&& shape, sf::Text text, void (*fun)(void*), void* data, int offsetX=0,int offsetY=0);
	bool pressed(int x, int y);
	Button& operator=(Button&& but);
	void draw(sf::RenderTarget& win);
};

class MainControl;

class Menu {
	int sizeX, sizeY;
	sf::Texture tex;
	sf::RectangleShape background;
	ShapeB title;
	Button butNew;
	Button butLoad;
	Button butExit;
public:
	Menu(int x, int y,sf::Font& font, MainControl* contr);
	void draw(sf::RenderWindow &win);
	void checkPressed(int x, int y);
};

class ButtonPanel {
	std::vector<Button> buttons;
	sf::RectangleShape background;
	int posX, posY;
public:
	ButtonPanel(int sizeX, int sizeY, int posX, int posY);
	void addButton(Button&& but,int index=-1);
	void draw(sf::RenderWindow& win);
	bool checkPressed(int x, int y);
	bool checkPressedR(int x, int y);
};

class Map;

sf::Color alterColor(sf::Color);

class Cell {
	double posX, posY;
	int sizeX, sizeY;
	ShapeB shp;
	sf::Text text;
	sf::Color col;
	bool active;
	bool color;
public:
	Cell() :posX{}, posY{}, sizeX{}, sizeY{}, shp{}, text{}, col{}, active{}, color{} {}
	Cell(Cell&& cel);
	Cell(int posX, int posY, int sizeX, int sizeY, ShapeB&& shape, sf::Text text,sf::Color col);
	Cell& operator=(Cell&& cel);
	bool pressedR(int x, int y,Map& map);
	void draw(sf::RenderTarget& win);
	void activate();
	void deactive();
	void addLeter(int letter);
	void delLetter();
	void resetTextPos();
	void saveToFile(std::fstream& os);
	bool pressed(int x, int y);
	void changeColor(sf::Color col) { this->col = col; shp.changeColor(col); }
	void changeShape(ShapeB::types t) { int T[4] = { posX,posY,sizeX,sizeY }; shp.changeShape(t, T); }
	void changeTextColor(sf::Color col) { text.setFillColor(col); }
	static Cell loadFromFile(std::fstream& is,sf::Font& font);
	void move(double x, double y);
	void resize(int beginX, int beginY, int endX, int endY);
};

PWSTR copyPWSTR(PWSTR str);

class Img {
	double posX, posY;
	int sizeX, sizeY;
	ShapeB shp;
	PWSTR name;
public:
	Img() :posX{}, posY{}, sizeX{}, sizeY{}, shp{}, name{} {}
	Img(Img&& img) :posX{ img.posX }, posY{ img.posY }, sizeX{ img.sizeX }, sizeY{ img.sizeY }, shp{ std::move(img.shp) }, name{ std::move(img.name) }{ img.name = nullptr; }
	Img(int posX, int posY, int sizeX, int sizeY, ShapeB&& shape, PWSTR name) :posX{ (double)posX }, posY{ (double)posY },
		sizeX{ sizeX }, sizeY{ sizeY }, shp{ std::move(shape) }, name{copyPWSTR(name)}{}
	Img& operator=(Img&& img);
	bool pressed(int x, int y);
	bool pressedR(int x, int y);
	void draw(sf::RenderTarget& win);
	void saveToFile(std::fstream& os);
	void changeShape(ShapeB::types t) { int T[4] = { posX,posY,sizeX,sizeY }; shp.changeShape(t,T); }
	static Img loadFromFile(std::fstream& is);
	void move(double x, double y);
	void resize(int beginX, int beginY, int endX, int endY);
};

class Line {
	double beginX, beginY, endX, endY;
	sf::Color col;
public:
	Line() : beginX{}, beginY{}, endX{}, endY{}, col{} {}
	Line(Line&& line) : beginX{ line.beginX }, beginY{ line.beginY }, endX{ line.endX }, endY{ line.endY }, col{line.col}{}
	Line(int beginX, int beginY, int endX, int endY, sf::Color c = sf::Color::Black) : beginX{ (double)beginX }, beginY{ (double)beginY }, endX{ (double)endX }, endY{ (double)endY }, col{c}{}
	Line& operator=(Line&& line);
	bool pressed(int x, int y, double scale);
	void draw(sf::RenderTarget& win);
	void setBegin(int x,int y) { beginX = x; beginY = y; }
	void setEnd(int x, int y) { endX = x; endY = y; }
	void saveToFile(std::fstream& os);
	void changeColor(sf::Color col) { this->col = col; }
	static Line loadFromFile(std::fstream& is);
	void move(double x, double y);
	void resize(int beginX, int beginY, int endX, int endY);
};

class Map {
	sf::RenderTexture board;
	sf::View v;
	sf::Sprite s;
	int posX;
	int posY;
	double zom = 1;
	std::vector<Cell> cells;
	std::vector<Line> lines;
	std::vector<Img> imgs;
	Cell* lastActive;
	Line* lastActiveLine;
	Img* lastActiveImg;
	int input;
	bool eraseV;
	sf::Color col;
	bool color;
	ShapeB::types t;
	bool shape;
	int moveType;
	int resizeType;
public:
	enum types {
		ANYTHING = -1,
		EMPTY = 0,
		CELL = 1,
		LINE = 2,
		IMG = 3,
	};
	Map(int sizeX, int sizeY, int posX, int posY,sf::Font &font,bool clear);
	void draw(sf::RenderWindow& win);
	void zoom(float a) { v.zoom(exp(-a / 10)); zom *= exp(-a / 10); board.setView(v); }
	void down(sf::Time t) { v.move(0, t.asMicroseconds() / 1000000.0 * zom); board.setView(v); }
	void up(sf::Time t) { v.move(0, -t.asMicroseconds() / 1000000.0 * zom); board.setView(v); }
	void right(sf::Time t) { v.move(t.asMicroseconds() / 1000000.0 * zom,0); board.setView(v); }
	void left(sf::Time t) { v.move(-t.asMicroseconds() / 1000000.0 * zom,0); board.setView(v); }
	void correctLastActive(Cell* cel);
	void correctLastActiveLine(Line* line);
	void correctLastActiveImg(Img* line);
	void addCell(Cell&& cel);
	void addLine(Line&& line);
	void addImg(Img&& img);
	void inputK(int key, bool shift);
	bool checkPressed(int x, int y);
	bool checkPressedR(int x, int y);
	bool isInput() { return input != EMPTY; }
	void clearAllLastActive();
	void deactiveOther();
	sf::Vector2f getCord(int x,int y){ return board.mapPixelToCoords(sf::Vector2i(x - posX, y - posY), v); }
	void saveToFile(std::fstream& os);
	void loadFromFile(std::fstream& is,sf::Font& font);
	void erase() { eraseV = true; }
	void clearErase() { eraseV = false; }
	bool isErase() { return eraseV; }
	void setColor(sf::Color col) { this->col = col; }
	void colorFalse() { color = false; }
	void colorTrue() { color = true;}
	void setShape(ShapeB::types t) { this->t = t; }
	void shapeFalse() { shape = false; }
	void shapeTrue() { shape = true; }
	int getMove() { return moveType; }
	void clearMove() { moveType = types::EMPTY; }
	void moveActive() { moveType = types::ANYTHING; }
	void move(double x, double y);
	int getResize() { return resizeType; }
	void clearResize() { resizeType = types::EMPTY; }
	void resize(double beginX, double beginY, double x, double y);
	void resizeActive() { resizeType = types::ANYTHING; }
};

class View;

struct ColorView{
	sf::Color col;
	View* view;
};

class ColorPicker {
	Button buttons[27];
	ColorView cv[27];
	bool visible;
	int posX, posY, sizeX, sizeY;
public:
	ColorPicker(int posX,int posY,int sizeX,int sizeY,View* view);
	void draw(sf::RenderWindow& win);
	void makeVisible() { visible = true; }
	void makeInvisible() { visible = false; }
	bool checkPressed(int x, int y);
};

struct ShapeView {
	ShapeB::types t;
	View* view;
};

class ShapePicker {
	Button buttons[3];
	ShapeView sv[3];
	bool visible;
	int posX, posY, sizeX, sizeY;
public:
	ShapePicker(int posX, int posY, int sizeX, int sizeY, View* view);
	void draw(sf::RenderWindow& win);
	void makeVisible() { visible = true; }
	void makeInvisible() { visible = false; }
	bool checkPressed(int x, int y);
};

class View {
	Map map;
	ButtonPanel panel;
	sf::Font* font;
	int addEl;
	bool holdactive;
	double beginX, beginY;
	ShapeB tmp;
	Line tmp2;
	Img tmp3;
	int e;
	ColorPicker colPicker;
	ShapePicker shpPicker;
public:
	View(int sizeX, int sizeY, sf::Font& font, MainControl* mapG, bool clear);
	void draw(sf::RenderWindow& win);
	void checkPressed(int x, int y);
	void checkPressedR(int x, int y);
	void zoom(float a) { map.zoom(a); }
	void down(sf::Time t) { map.down(t); }
	void up(sf::Time t) { map.up(t); }
	void right(sf::Time t) { map.right(t); }
	void left(sf::Time t) { map.left(t); }
	void inputK(int key, bool shift) { map.inputK(key,shift); }
	bool isInput() { return map.isInput(); }
	void beginAddingEl(Map::types t) { addEl = t; }
	void onRealiseL(int x, int y);
	void saveToFile();
	void loadFromFile(sf::Font& font);
	void activeErase() { map.erase(); e = 1; }
	void setColor(sf::Color col) { map.setColor(col); map.colorTrue(); e = 3; }
	void activeColor() { colPicker.makeVisible(); e = 2; }
	void setShape(ShapeB::types t) { map.setShape(t); map.shapeTrue(); e = 5; }
	void activeShape() { shpPicker.makeVisible(); e = 4; }
	void checkCycle(double x, double y);
private:
	void buttonAdder(int posX,void (*fun)(void*), void* dataT, const char name[],sf::Font& font);
};

class FontLoad {
public:
	sf::Font font;
	FontLoad() :font{} { font.loadFromFile("resourse/arial.ttf"); }
};

class MainControl {
	bool view;
	bool delView;
	sf::RenderWindow window;
	FontLoad fl;
	Menu *menu;
	View *mapView;
	
public:
	MainControl() : view{ false }, delView{}, window{ sf::VideoMode(1920, 1080), "MAP" }, fl{}, menu{ new Menu(1920,1080,fl.font,this) }, mapView{ nullptr } {}
	void activate();
	void close();
	void creatNewView(bool clear);
	void loadMap();
	void toMenu();
	~MainControl();
private:
	void deleteView();
};













