#pragma once

#include <utils/Utils.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <string>

class Base {
public:
    Base(){
        m_className = "Base";
    };
    virtual std::string getClassName(){
        return m_className;
    }
protected:
    std::string m_className;
};