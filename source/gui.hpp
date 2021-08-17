#pragma once
#include "utils.hpp"
#include <SFML/Graphics.hpp>

vec2f mousePos = vec2f(0, 0);
int mouseButton = -1;
bool released;
sf::Event g_curEvent;
bool newEvent;
int state = 0;

struct buttonContext {
	vec2f pos;
	vec2f size;
	sf::Texture defaultTexture;
	sf::Texture hoverTexture;
	sf::Texture clickedTexture;
	int (*function)();
	sf::RenderWindow* window;
};

void handleEvent(sf::Event &e){
	if (e.type == sf::Event::MouseMoved){
		mousePos = {(float)e.mouseMove.x, (float)e.mouseMove.y};
	}
	if (e.type == sf::Event::MouseButtonPressed){
		mouseButton = e.mouseButton.button;
	}
	if (e.type == sf::Event::MouseButtonReleased){
		mouseButton = -1;
		released = true;
	}
}

bool button(buttonContext cont){
	// Draw
	sf::Texture active = cont.defaultTexture;
	if (inRange<float>(cont.pos.x, cont.pos.x + cont.size.x, mousePos.x) && inRange<float>(cont.pos.y, cont.pos.y + cont.size.y, mousePos.y)){
		active = cont.hoverTexture;
	}
	if (inRange<float>(cont.pos.x, cont.pos.x + cont.size.x, mousePos.x) && inRange<float>(cont.pos.y, cont.pos.y + cont.size.y, mousePos.y) && mouseButton > -1){
		active = cont.clickedTexture;
	}
	if (g_curEvent.type == sf::Event::MouseButtonReleased){
		if (inRange<float>(cont.pos.x, cont.pos.x + cont.size.x, g_curEvent.mouseButton.x) && inRange<float>(cont.pos.y, cont.pos.y + cont.size.y, g_curEvent.mouseButton.y)){
			cont.function();
			active = cont.hoverTexture;
		}
	}
	sf::Sprite toDraw;
	toDraw.setTexture(active);
	toDraw.setPosition(cont.pos.x, cont.pos.y);
	sf::Vector2u texSize;
	texSize = active.getSize();
	toDraw.setScale(sf::Vector2f(cont.size.x / texSize.x, cont.size.y / texSize.y));
	cont.window->draw(toDraw);
	// Return if pressed
	return false;
}