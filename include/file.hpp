#pragma once

#include <Map>
#include <string>
#include <fstream>

#include "debug.hpp"


template <class DATATYPE>
class File {

protected:

    virtual void read_data(void) = 0;
    virtual void write_data(void) {};

    const std::string path;
    std::fstream file;

    const bool read_only;
    std::map<const std::string, DATATYPE> data;


public:

    File(void) : read_only(false) {}
    File(const std::string path, bool read_only = false) : path(path), read_only(read_only) {}

    inline DATATYPE operator() (const std::string key) { return data[key]; }
    inline void operator() (const std::string key, DATATYPE value) { data[key] = value; }

    void load(void) {
        file.open(path, std::ios::in);

        if (!file.is_open()) { message("Failed to open file:", path); }

        read_data();
        file.close();
    }

    void save(void) {
        if (read_only) { message("The following file is read-only:", path); }
        
        file.open(path, std::ios::out);

        if (!file.is_open()) { message("Failed to open file:", path); }

        write_data();
        file.close();
    }

    const std::map<const std::string, DATATYPE>* all(void) const {
        return &data;
    }

};