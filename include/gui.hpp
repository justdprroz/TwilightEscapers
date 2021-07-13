#pragma once
#include "utils.hpp"
#include <SFML/Graphics.hpp>

vec2f mousePos = vec2f(0, 0);
int mouseButton = -1;

struct buttonContext {
	vec2f pos;
	vec2f size;
	sf::Texture defaultTexture;
	sf::Texture hoverTexture;
	sf::Texture clickedTexture;
	int (*function)();
	sf::RenderWindow* window;
};