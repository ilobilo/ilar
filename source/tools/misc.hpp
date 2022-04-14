// Copyright (C) 2022  ilobilo

#pragma once

#include <filesystem>
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;

void strstream2ofstream(std::stringstream &in, std::string out, size_t size = 0);
void file2strstream(std::string in, std::stringstream &out, size_t size = 0);