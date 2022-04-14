// Copyright (C) 2022  ilobilo

#pragma once

#include <boost/iostreams/filtering_stream.hpp>
#include <iostream>

namespace bio = boost::iostreams;

static constexpr uint8_t gziphdr[] = { 0x1F, 0x8B };
static constexpr uint8_t bzip2hdr[] = { 0x42, 0x5A, 0x68 };
static constexpr uint8_t lzmahdr[] = { 0xFD, 0x37, 0x7A, 0x58, 0x5A, 0x00 };

enum comp_type
{
    NONE,
    GZIP,
    BZIP2,
    LZMA
};

void compress(std::stringstream &data, comp_type comp = comp_type::NONE);
void decompress(std::stringstream &data);