#include <SFML/Graphics.hpp>
#include <iostream>
#include "utils.hpp"
#include "gui.hpp"

sf::Event g_curEvent;

void button(buttonContext cont){
	sf::Texture active = cont.defaultTexture;
	// hover
	if (g_curEvent.type == sf::Event::MouseMoved){
		if (inRange<float>(cont.pos.x, cont.pos.x + cont.size.x, g_curEvent.mouseMove.x) && inRange<float>(cont.pos.y, cont.pos.y + cont.size.y, g_curEvent.mouseMove.y)){
			active = cont.hoverTexture;
		}
	}
	// pressed
	if (g_curEvent.type == sf::Event::MouseButtonPressed){
		if (inRange<float>(cont.pos.x, cont.pos.x + cont.size.x, g_curEvent.mouseButton.x) && inRange<float>(cont.pos.y, cont.pos.y + cont.size.y, g_curEvent.mouseButton.y)){
			active = cont.clickedTexture;
		}
	}
	// released
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
}

int clicked(){
	std::cout << "clicked" << "\n";
	return 0;
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

int main(){
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
			if (event.type == sf::Event::MouseWheelMoved){
				// zoom *= 1 + event.mouseWheel.delta * 0.05;
				// std::cout << zoom << '\n';
				// // view.zoom(1 + event.mouseWheel.delta * 0.05);
				// window.setView(view);
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
		MainMenu(window);
		window.display();
    }
}