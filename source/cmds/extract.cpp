// Copyright (C) 2022  ilobilo

#include <cmds/create.hpp>
#include <tools/misc.hpp>
#include <header.hpp>
#include <cstring>

namespace cmds
{
    void extract(std::stringstream &data, std::string dir)
    {
        data.seekp(0, std::ios::end);
        size_t size = data.tellp();
        data.seekp(0, std::ios::beg);

        std::string fullpath;
        size_t offset = 0;

        fileheader file;
        data.read(reinterpret_cast<char*>(&file), sizeof(fileheader));

        while (true)
        {
            if (std::strcmp(file.signature, ILAR_SIGNATURE))
            {
                std::cout << "Error: File signature incorrect!" << std::endl;
                break;
            }

            fullpath = dir + file.name;
            fs::remove_all(fullpath);

            switch (file.type)
            {
                case ILAR_REGULAR:
                {
                    strstream2ofstream(data, fullpath, file.size);
                    fs::permissions(fullpath, fs::perms(file.mode));
                    break;
                }
                case ILAR_DIRECTORY:
                    fs::create_directories(fullpath);
                    fs::permissions(fullpath, fs::perms(file.mode));
                    break;
                case ILAR_SYMLINK:
                    fs::create_symlink(file.link, fullpath);
                    break;
            }

            offset += sizeof(fileheader) + file.size;
            if (offset >= size) break;

            data.read(reinterpret_cast<char*>(&file), sizeof(fileheader));
        }
    }
}