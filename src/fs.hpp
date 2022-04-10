#pragma once

#include <cstddef>
#include <cstdint>

static constexpr size_t NAME_LENGTH = 64;

struct fileheader
{
    char name[NAME_LENGTH];
    uint64_t length;
};