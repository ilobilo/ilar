#include <cmds/create.hpp>
#include <cstring>
#include <fs.hpp>

void listfiles(std::string path)
{
    std::string contents = filecontents(path);
    size_t filelength = fs::file_size(path);
    fileheader *file = reinterpret_cast<fileheader*>(contents.data());
    size_t offset = 0;

    while (offset < filelength && !std::strcmp(file->magic, ILFS_MAGIC))
    {
        std::cout << "Name: " << file->name << ", Size: " << file->size << std::endl;

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

        fs::perms perms(static_cast<fs::perms>(file->mode));

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
        if (offset < filelength) std::cout << std::endl;
        else break;

        file = reinterpret_cast<fileheader*>(contents.data() + offset);
    }
}