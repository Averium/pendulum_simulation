#include "colorfile.hpp"


ColorFile::ColorFile() : File() {}


ColorFile::ColorFile(const std::string path) : File(path, true) {
    load();
}


void ColorFile::read_data(void) {
    std::string name;
    unsigned int r, g, b, a;

    while (file >> name >> r >> g >> b >> a) {
        data[name] = {
            (unsigned char)(r),
            (unsigned char)(g),
            (unsigned char)(b),
            (unsigned char)(a)
        };
    }
}


void ColorFile::write_data(void) {
    for (const auto& pair : data) {
        Color color = pair.second;
        file << pair.first << " " << color.r << " " << color.g << " " << color.b << "\n";
    }
}