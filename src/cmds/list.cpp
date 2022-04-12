#include <cmds/create.hpp>
#include <header.hpp>
#include <cstring>

namespace cmds
{
    void list(std::string path)
    {
        std::string contents = filecontents(path);
        fileheader *file = reinterpret_cast<fileheader*>(contents.data());
        size_t offset = 0;

        while (true)
        {
            if (std::strcmp(file->signature, ILFS_SIGNATURE))
            {
                std::cout << "Error: File signature incorrect!" << std::endl;
                break;
            }

            std::cout << "Name: " << file->name << std::endl;
            if (file->type == ILFS_SYMLINK) std::cout << "Link: " << file->link << std::endl;
            std::cout << "Size: " << file->size << std::endl;

            std::cout << "Mode: ";
            switch (file->type)
            {
                case ILFS_DIRECTORY:
                    std::cout << "d";
                    break;
                case ILFS_SYMLINK:
                    std::cout << "l";
                    break;
                default:
                    std::cout << "-";
                    break;
            }

            fs::perms perms(fs::perms(file->mode));

            std::cout << ((perms & fs::perms::owner_read) != fs::perms::none ? "r" : "-")
                << ((perms & fs::perms::owner_write) != fs::perms::none ? "w" : "-")
                << ((perms & fs::perms::owner_exec) != fs::perms::none ? "x" : "-")
                << ((perms & fs::perms::group_read) != fs::perms::none ? "r" : "-")
                << ((perms & fs::perms::group_write) != fs::perms::none ? "w" : "-")
                << ((perms & fs::perms::group_exec) != fs::perms::none ? "x" : "-")
                << ((perms & fs::perms::others_read) != fs::perms::none ? "r" : "-")
                << ((perms & fs::perms::others_write) != fs::perms::none ? "w" : "-")
                << ((perms & fs::perms::others_exec) != fs::perms::none ? "x" : "-")
                << std::endl;

            offset += sizeof(fileheader) + file->size;
            if (offset >= fs::file_size(path)) break;
            else std::cout << std::endl;

            file = reinterpret_cast<fileheader*>(contents.data() + offset);
        }
    }
}