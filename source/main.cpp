// Copyright 2021 JustDprroz

#include <iostream>
#include <SFML/Graphics.hpp>
#include <World.hpp>
#include <utility>

int main() {
    // sf::RenderWindow window(sf::VideoMode(1000, 800), "EscapeFromTwilight", sf::Style::Default);
    // while (window.isOpen()) {
    //     sf::Event event;
    //     while (window.pollEvent(event)) {
    //         if (event.type == sf::Event::Closed) {
    //             window.close();
    //         }
    //     }
    // }

    // Chunk c;
    // c.SaveChunk("chunks");
    World w;
    std::string command;
    while (true) {
        std::cout << "type your command, type help for command list" << '\n';
        std::cin >> command;
        if (command == "exit"){
            std::cout << "goodbye" << '\n';
            break;
        }
        else if (command == "load") {
            std::cout << "type directory path for load" << '\n';
            std::cin >> command;
            w.LoadChunks(command);
        }
        else if (command == "save") {
            std::cout << "type directory path for save" << '\n';
            std::cin >> command;
            w.SaveChunks(command);
        }
        else if (command == "help") {
            std::cout << "help - show this info" << '\n';
            std::cout << "exit - exit app now" << '\n';
            std::cout << "load - load map from file" << '\n';
            std::cout << "save - save map to file" << '\n';
            std::cout << "set - set new chunk" << '\n';
        }
        else if (command == "set"){
            std::cout << "type pos of new chunk" << '\n';
            int x = 0, y = 0;
            std::cin >> x >> y;
            w.SetChunk({x, y}, Chunk());
        }
        else {
            std::cout << "Unknown command, please try again" << '\n';
        }
    }
    return 0;
}
