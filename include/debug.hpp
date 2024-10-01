#pragma once

#include <iostream>


template <typename TYPE>
void debug(TYPE data) {
    std::cout << "DEBUG: " << data << std::endl;
}

template <typename TYPE>
void debug(const char* message, TYPE data) {
    std::cout << "DEBUG: " << message << " " << data << std::endl;
}

template <typename TYPE>
void message(TYPE data) {
    std::cout << "MESSAGE: " << data << std::endl;
}

template <typename TYPE>
void message(const char* message, TYPE data) {
    std::cout << "MESSAGE: " << message << " " << data << std::endl;
}