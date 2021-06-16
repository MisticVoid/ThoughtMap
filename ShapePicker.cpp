#include "core.h"

void choseShape(void* data) {
	static_cast<ShapeView*>(data)->view->setShape(static_cast<ShapeView*>(data)->t);
}

ShapePicker::ShapePicker(int posX, int posY, int sizeX, int sizeY, View* view) : posX{ posX }, posY{ posY }, sizeX{ sizeX }, sizeY{ sizeY }, visible{} {
	double width = sizeX;
	double high = sizeY / 3;
	sv[0] = { ShapeB::types::RECTANGLE,view };
	sv[1] = { ShapeB::types::OCTAGON,view };
	sv[2] = { ShapeB::types::ELIPSE,view };
	const char* names[] = { "resourse/box.png","resourse/octagon.png","resourse/elipse.png" };
	for (int i = 0; i < 3; i++) {
		int T[4] = { posX, posY + i * high, width, high };
		ShapeB shp = ShapeB::generateShape(ShapeB::types::RECTANGLE, T, sf::Color::White);
		sf::Texture* tex = new sf::Texture();
		tex->loadFromFile(names[i]);
		shp.setTexture(tex);
		buttons[i] = Button(posX, posY + i * high, width, high,std::move(shp), sf::Text(), choseShape, sv + i);
	}
}

void ShapePicker::draw(sf::RenderWindow& win) {
	if (visible) {
		for (int i = 0; i < 3; i++) {
			buttons[i].draw(win);
		}
	}
}

bool ShapePicker::checkPressed(int x, int y) {
	if (visible) {
		for (int i = 0; i < 3; i++) {
			if (buttons[i].pressed(x, y))
				return true;
		}
	}
	return false;
}