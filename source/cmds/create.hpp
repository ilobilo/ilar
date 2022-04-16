// Copyright (C) 2022  ilobilo

#pragma once

#include <filesystem>
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;

namespace cmds
{
    std::string filecontents(std::string path);

    void createfile(std::string parent, fs::path path, std::stringstream &archive);
    void createsymlink(std::string parent, fs::path path, std::stringstream &archive);
    void createdir(std::string parent, fs::path path, std::stringstream &archive);
    void create(std::string parent, fs::path path, std::stringstream &archive);
}