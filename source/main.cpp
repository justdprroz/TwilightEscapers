#include <SFML/Graphics.hpp>
#include <iostream>
#include "utils.hpp"
#include "gui.hpp"
#include "noise.hpp"

const siv::PerlinNoise perlin(time(NULL));
int const MAPsize = 64;
float scale = 1;
int MAP[MAPsize][MAPsize];
sf::Texture textures[16];
std::pair<int, int> playerPos = {100, 200};
sf::Texture playerTexture;

int clicked(){
	std::cout << "clicked" << "\n";
	state = 2;
	return 0;
}

void loadTextures(){
	std::cout << "Loading textures:\n";
	std::cout << "Loading tiles\n";
	std::cout << "Stone: " << textures[0].loadFromFile("textures/tiles/normal/stone.png") << '\n';
	std::cout << "Grass: " << textures[1].loadFromFile("textures/tiles/normal/grass.png") << '\n';
	std::cout << "loading entities textures\n";
	std::cout << "Guard: " << playerTexture.loadFromFile("textures/entities/guard.png") << '\n';
	std::cout << "Textures loaded\n";
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
	// backgroundTexture.loadFromFile("textures/background.png");
	// backgroundTexture.setSmooth(1);

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


void drawEntities(sf::RenderWindow &win){
		sf::Sprite cell;
		cell.setTexture(playerTexture);
		cell.setPosition(playerPos.first, playerPos.second);
		cell.setScale(sf::Vector2f(scale,scale));
		win.draw(cell);
}

void drawTiles(sf::RenderWindow &win) {
	for(int i = 0; i < MAPsize; i++){
		for(int i1 = 0; i1 < MAPsize; i1++){
			int curBlock = MAP[i][i1];
			sf::Sprite cell;
			cell.setTexture(textures[curBlock]);
			cell.setPosition(i * 16 * scale, i1 * 16 * scale);
			cell.setScale(sf::Vector2f(scale,scale));
			win.draw(cell);
		}
	}
}

void GameScreen(sf::RenderWindow &win){
	drawTiles(win);
	drawEntities(win);
}

void generateMap(){
	for(int i = 0; i < MAPsize; i++){
		for(int i1 = 0; i1 < MAPsize; i1++){
			int noise = (int)(perlin.accumulatedOctaveNoise2D_0_1(i * 4.0 / MAPsize, i1 * 4.0 / MAPsize, 8) * 32) % 2;
			MAP[i][i1] = noise;
		}
	}
}

int main(){
	loadTextures();
	generateMap();
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
				int scale = 1;
				width = event.size.width;
				height = event.size.height;
				view.setSize(sf::Vector2f(width * scale, height * scale));
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
				if (event.key.code == sf::Keyboard::W){
					
				}
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
			GameScreen(window);
		}
		window.display();
    }
}