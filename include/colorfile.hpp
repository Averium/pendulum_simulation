#pragma once

#include "file.hpp"
#include "raylib.hpp"


class ColorFile : public File<Color> {

public:
    ColorFile();
    ColorFile(const std::string path);

protected:

    void read_data(void);
    void write_data(void);

};