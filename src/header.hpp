#pragma once

#include <cstddef>
#include <cstdint>

static constexpr size_t PATH_LENGTH = 128;
static constexpr char ILFS_SIGNATURE[] = "ILFS";

enum filetypes
{
    ILFS_REGULAR,
    ILFS_DIRECTORY,
    ILFS_SYMLINK
};

struct fileheader
{
    char signature[5];
    char name[PATH_LENGTH];
    char link[PATH_LENGTH];
    uint64_t size;
    uint8_t type;
    uint32_t mode;
};