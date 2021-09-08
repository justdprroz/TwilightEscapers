#include <SFML/Graphics.hpp>
#include <iostream>
#include "utils.hpp"
#include "gui.hpp"
#include "noise.hpp"

const siv::PerlinNoise perlin(time(NULL));
int const MAPsize = 64;
float scale = 2;
int MAP[MAPsize][MAPsize];
sf::Texture textures[16];
std::pair<float, float> playerPos = {4, 5};
float plOnScX = 4 * 16 * 2, plOnScY = 5 * 16 * 2;
float dvx = 0, dvy = 0;
sf::Texture playerTextureFront;
sf::Texture playerTextureBack;
sf::Texture playerTexture;
int tilesize = 16;
sf::Clock clockS;
float velX;
float velY;
bool Wpress, Apress, Spress, Dpress;
int speed = 5;

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
	std::cout << "Guard front: " << playerTextureFront.loadFromFile("textures/entities/guard.png", sf::IntRect(0, 0, 32, 32)) << '\n';
	std::cout << "Guard back: " << playerTextureBack.loadFromFile("textures/entities/guard.png", sf::IntRect(32, 0, 32, 32)) << '\n';
	playerTexture = playerTextureFront;
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
	float locscaleX = (float)tilesize / (float)playerTexture.getSize().x;
	float locscaleY = (float)tilesize / (float)playerTexture.getSize().y;
	sf::Sprite cell;
	cell.setTexture(playerTexture);
	cell.setPosition(playerPos.first * tilesize * scale, playerPos.second * tilesize * scale);
	cell.setScale(sf::Vector2f(locscaleX * scale,locscaleY * scale));
	win.draw(cell);
}

void drawTiles(sf::RenderWindow &win) {
	for(int i = 0; i < MAPsize; i++){
		for(int i1 = 0; i1 < MAPsize; i1++){
			int curBlock = MAP[i][i1];
			sf::Sprite cell;
			cell.setTexture(textures[curBlock]);
			cell.setPosition(i * tilesize * scale, i1 * tilesize * scale);
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
			int noise = (int)(perlin.accumulatedOctaveNoise2D_0_1(i * 4.0 / MAPsize, i1 * 4.0 / MAPsize, 8) * 8) % 2;
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
		double lastframetime = clockS.getElapsedTime().asSeconds();
		clockS.restart();
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
				view.setCenter(sf::Vector2f(width / 2 + dvx, height / 2 + dvy));
				window.setView(view);
			}
			if (event.type == sf::Event::KeyPressed){
				if (event.key.code == sf::Keyboard::W){
					Wpress = true;
				}
				if (event.key.code == sf::Keyboard::S){
					Spress = true;
				}
				if (event.key.code == sf::Keyboard::D){
					Dpress = true;
				}
				if (event.key.code == sf::Keyboard::A){
					Apress = true;
				}
			}
			if (event.type == sf::Event::KeyReleased){
				if (event.key.code == sf::Keyboard::Escape){
					window.close();
					return 0;
				}
				if (event.key.code == sf::Keyboard::W){
					Wpress = false;
				}
				if (event.key.code == sf::Keyboard::S){
					Spress = false;
				}
				if (event.key.code == sf::Keyboard::D){
					Dpress = false;
				}
				if (event.key.code == sf::Keyboard::A){
					Apress = false;
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
			velX = 0;
			velY = 0;
			if (Wpress){
				playerTexture = playerTextureBack;
				velY -= speed;
			}
			if (Spress){
				playerTexture = playerTextureFront;
				velY += speed;
			}
			if (Dpress){
				velX += speed;
			}
			if (Apress){
				velX -= speed;
			}
			float dY = velY * lastframetime, dX = velX * lastframetime;
			playerPos.first += dX;
			playerPos.second += dY;
			std::cout << view.getSize().x << ' ' << view.getSize().y << '\n';
			std::cout << window.getSize().x << ' ' << window.getSize().y << '\n';
			// std::cout << plOnScX << ' ' << plOnScY << '\n';
			if (plOnScX < 200 && dX < 0 || plOnScX > window.getSize().x - 200 && dX > 0){
				dvx += dX * tilesize * 2;
				view.move(dX * tilesize * 2, 0);
			} else{
				plOnScX += dX * tilesize * 2;
			}
			if (plOnScY < 200 && dY < 0 || plOnScY > window.getSize().y - 200 && dY > 0){
				view.move(0, dY * tilesize * 2);
				dvy += dY * tilesize * 2;
			} else{
				plOnScY += dY * tilesize * 2;
			}
			window.setView(view);
			GameScreen(window);
		}
		window.display();
    }
}