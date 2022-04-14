// Copyright (C) 2022  ilobilo

#pragma once

#include <filesystem>
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;

namespace cmds
{
    std::string filecontents(std::string path);

    void createfile(std::string parent, fs::path path, std::stringstream &image);
    void createsymlink(std::string parent, fs::path path, std::stringstream &image);
    void createdir(std::string parent, fs::path path, std::stringstream &image);
    void create(std::string parent, fs::path path, std::stringstream &image);
}