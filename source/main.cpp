#include <SFML/Graphics.hpp>
#include <iostream>
#include "utils.hpp"
#include "gui.hpp"

int clicked(){
	std::cout << "clicked" << "\n";
	state = 1;
	return 0;
}

sf::Texture textures[16];

void loadTextures(){
	textures[0] = sf::Texture();
}

void MainMenu(sf::RenderWindow &win) {
	buttonContext bc;
	bc.pos = vec2f(0,0);
	bc.size = vec2f(192,108);
	bc.window = &win;
	bc.function = &clicked;
	sf::Texture toload;
	toload.loadFromFile("textures/playButton.png");
	bc.defaultTexture = sf::Texture(toload);
	toload.loadFromFile("textures/playButtonHover.png");
	bc.hoverTexture = sf::Texture(toload);
	toload.loadFromFile("textures/playButtonPressed.png");
	bc.clickedTexture = sf::Texture(toload);
	button(bc);

	sf::Texture backgroundTexture;
	backgroundTexture.loadFromFile("textures/background.png");
	backgroundTexture.setSmooth(1);

	sf::Sprite background;
	background.setTexture(backgroundTexture);
	background.setPosition(0,0);
	background.setScale(sf::Vector2f(10,10));
	// win.draw(background);
}

void DebugScreen(sf::RenderWindow &win){
	sf::RectangleShape cell;
	cell.setSize(sf::Vector2f(640, 480));
	cell.setFillColor(sf::Color::Green);
	cell.setPosition(0, 0);
	win.draw(cell);
}

int MAP[16][16];

void GameScreen(sf::RenderWindow &win){
	for(int i = 0; i < 16; i++){
		for(int i1 = 0; i1 < 16; i1++){
			int curBlock = MAP[i][i1];
			if (curBlock == 1){

			}
		}
	}
}

int main(){
	for(int i = 0; i < 16; i++){
		for(int i1 = 0; i1 < 16; i1++){
			MAP[i][i1] = 1;
		}
	}
	int width = 1000, height = 1000;
	sf::RenderWindow window(sf::VideoMode(width, height), "EscapeFromTwilight", sf::Style::Default);
	window.setPosition({0,0});
	sf::View view;
	window.setView(view);
	float zoom = 2.f;
	while (window.isOpen())
    {
		sf::Event event;
		while (window.pollEvent(event))
		{	
			handleEvent(event);
			g_curEvent = event;
			if (event.type == sf::Event::Closed){
				window.close();
				return 0;
			}
			if (event.type == sf::Event::Resized) {
				width = event.size.width;
				height = event.size.height;
				view.setSize(sf::Vector2f(width, height));
				view.setCenter(sf::Vector2f(width / 2, height / 2));
				window.setView(view);
			}
			if (event.type == sf::Event::KeyReleased){
				if (event.key.code == sf::Keyboard::Escape){
					window.close();
					return 0;
				}
			}
			if (event.type == sf::Event::KeyPressed){

			}

			if (event.type == sf::Event::MouseMoved){

			}
		}
		window.clear();
		if (state == 0){
			MainMenu(window);
		}
		if (state == 1){
			DebugScreen(window);
		}
		if (state == 2){
			MainMenu(window);
		}
		window.display();
    }
}