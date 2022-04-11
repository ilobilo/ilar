#pragma once

#include <filesystem>
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;

std::string filecontents(fs::path path);

void createreg(std::string parent, fs::path path, std::ofstream &image);
void createdir(std::string parent, fs::path path, std::ofstream &image);
void createfile(std::string parent, fs::path path, std::ofstream &image);