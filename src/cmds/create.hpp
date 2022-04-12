#pragma once

#include <filesystem>
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;

namespace cmds
{
    std::string filecontents(fs::path path);
    std::string filecontents(std::string path);

    void createfile(std::string parent, fs::path path, std::ofstream &image);
    void createsymlink(std::string parent, fs::path path, std::ofstream &image);
    void createdir(std::string parent, fs::path path, std::ofstream &image);
    void create(std::string parent, fs::path path, std::ofstream &image);
}