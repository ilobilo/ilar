// Copyright (C) 2022  ilobilo

#pragma once

#include <iostream>

static constexpr uint8_t gziphdr[] = { 0x1F, 0x8B };
static constexpr uint8_t bzip2hdr[] = { 0x42, 0x5A, 0x68 };
static constexpr uint8_t lzmahdr[] = { 0xFD, 0x37, 0x7A, 0x58, 0x5A, 0x00 };

enum class comp_type
{
    none = 0,
    gzip = 1,
    bzip2 = 2,
    lzma = 3
};

void compress(std::stringstream &data, comp_type comp = comp_type::none);
void decompress(std::stringstream &data);