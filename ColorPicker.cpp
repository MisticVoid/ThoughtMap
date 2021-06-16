#include "core.h"

void choseColor(void* data) {
	static_cast<ColorView*>(data)->view->setColor(static_cast<ColorView*>(data)->col);
}
																																	
ColorPicker::ColorPicker(int posX, int posY, int sizeX, int sizeY, View* view) : posX{ posX }, posY{ posY }, sizeX{ sizeX }, sizeY{ sizeY }, visible{} {
	int i = 0;
	double width = sizeX / 3;
	double high = sizeY / 9;
	for (int j = 0; j < 3; j++) {
		for (int k = 0; k < 3; k++) {
			for (int q = 0; q < 3; q++) {
				cv[i] = { sf::Color(255 * j / 2,255 * k / 2,255 * q / 2),view };
				int T[4] = { posX + i % 3 * width, posY + i / 3 * high, width, high };
				buttons[i] = Button(posX + i % 3 * width, posY + i / 3 * high, width, high, ShapeB::generateShape(ShapeB::types::RECTANGLE, T, cv[i].col), sf::Text(), choseColor, cv + i);
				i++;
			}
		}
	}
}

void ColorPicker::draw(sf::RenderWindow& win) {
	if (visible) {
		for (int i = 0; i < 27; i++) {
			buttons[i].draw(win);
		}
	}
}

bool ColorPicker::checkPressed(int x, int y) {
	if (visible) {
		for (int i = 0; i < 27; i++) {
			if (buttons[i].pressed(x, y))
				return true;
		}
	}
	return false;
}