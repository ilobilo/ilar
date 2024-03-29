// Copyright (C) 2022  ilobilo

#include <cmds/create.hpp>
#include <header.hpp>
#include <cstring>

namespace cmds
{
    std::string filecontents(std::string path)
    {
        std::ostringstream buf;
        std::ifstream input(fs::path(path), std::ios::binary);
        buf << input.rdbuf();
        input.close();
        return buf.str();
    }

    void createfile(std::string parent, fs::path path, std::stringstream &archive)
    {
        size_t size = fs::file_size(path);
        std::string name = parent + "/" + path.filename().string();
        std::string contents = filecontents(path);

        size_t name_length = PATH_LENGTH > name.length() ? name.length() : PATH_LENGTH;

        fileheader file;
        std::strncpy(file.signature, ILAR_SIGNATURE, 5);
        std::strncpy(file.name, name.c_str(), name_length);
        file.name[name_length] = 0;

        file.size = size;
        file.type = ILAR_REGULAR;
        file.mode = static_cast<uint32_t>(fs::status(path).permissions());

        archive.write(reinterpret_cast<char*>(&file), sizeof(fileheader));
        archive.write(contents.c_str(), size);
    }

    void createsymlink(std::string parent, fs::path path, std::stringstream &archive)
    {
        std::string name = parent + "/" + path.filename().string();

        fileheader file;
        std::strncpy(file.signature, ILAR_SIGNATURE, 5);
        std::strncpy(file.name, name.c_str(), PATH_LENGTH);
        std::strncpy(file.link, fs::read_symlink(path).c_str(), PATH_LENGTH);

        file.size = 0;
        file.type = ILAR_SYMLINK;
        file.mode = 0777;

        archive.write(reinterpret_cast<char*>(&file), sizeof(fileheader));
    }

    void createdir(std::string parent, fs::path path, std::stringstream &archive)
    {
        std::string name = parent + "/" + path.filename().string();

        fileheader file;
        std::strncpy(file.signature, ILAR_SIGNATURE, 5);
        std::strncpy(file.name, name.c_str(), PATH_LENGTH);

        file.size = 0;
        file.type = ILAR_DIRECTORY;
        file.mode = static_cast<uint32_t>(fs::status(path).permissions());

        archive.write(reinterpret_cast<char*>(&file), sizeof(fileheader));

        for (const auto &entry : fs::directory_iterator(path))
            create(parent + "/" + path.filename().string(), entry, archive);
    }

    void create(std::string parent, fs::path path, std::stringstream &archive)
    {
        switch (fs::symlink_status(path).type())
        {
            case fs::file_type::regular:
                createfile(parent, path, archive);
                break;
            case fs::file_type::directory:
                createdir(parent, path, archive);
                break;
            case fs::file_type::symlink:
                createsymlink(parent, path, archive);
                break;
            default:
                std::cout << path.c_str() << ": Unknown file type!" << std::endl;
                break;
        }
    }
}