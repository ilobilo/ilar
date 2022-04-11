#pragma once

#include <cstddef>
#include <cstdint>

static constexpr size_t NAME_LENGTH = 64;
static constexpr char ILFS_MAGIC[] = "ILFS";

enum filetypes
{
    ILFS_REGULAR,
    ILFS_DIRECTORY,
    ILFS_SYMLINK
};

struct fileheader
{
    char magic[5];
    char name[NAME_LENGTH];
    uint64_t length;
    uint8_t type;
};