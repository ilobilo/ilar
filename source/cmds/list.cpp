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
            if (std::strcmp(file->signature, ILAR_SIGNATURE))
            {
                std::cout << "Error: File signature incorrect!" << std::endl;
                break;
            }

            std::cout << "Name: " << file->name << std::endl;
            if (file->type == ILAR_SYMLINK) std::cout << "Link: " << file->link << std::endl;
            std::cout << "Size: " << file->size << std::endl;

            switch (file->type)
            {
                case ILAR_REGULAR:
                    std::cout << "Type: Regular" << std::endl << "Mode: -";
                    break;
                case ILAR_DIRECTORY:
                    std::cout << "Type: Directory" << std::endl << "Mode: d";
                    break;
                case ILAR_SYMLINK:
                    std::cout << "Type: Symlink" << std::endl << "Mode: l";
                    break;
                default:
                    std::cout << "Type: Unknown" << std::endl << "Mode: -";
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