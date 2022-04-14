// Copyright (C) 2022  ilobilo

#include <filesystem>
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;

void strstream2ofstream(std::stringstream &in, std::string out, size_t size)
{
    std::ofstream file(out, std::ios::binary);

    if (size == 0)
    {
        in.seekp(0, std::ios::end);
        size = in.tellp();
    }

    char *buffer = new char[size];

    in.read(buffer, size);
    file.write(buffer, size);

    delete[] buffer;
    file.close();
}

void file2strstream(std::string in, std::stringstream &out, size_t size)
{
    std::ifstream file(in, std::ios::binary);

    if (size == 0) size = fs::file_size(in);
    char *buffer = new char[size];

    file.read(buffer, size);
    out.write(buffer, size);

    delete[] buffer;
    file.close();
}