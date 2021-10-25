#pragma once

#include <string>
#include <utils/Utils.hpp>

#define protected public

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