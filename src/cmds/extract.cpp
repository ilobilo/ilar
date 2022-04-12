#include <cmds/create.hpp>
#include <header.hpp>
#include <cstring>

#include <sys/stat.h>

namespace cmds
{
    void extract(std::string path, std::string dir)
    {
        std::string contents = filecontents(path);
        fileheader *file = reinterpret_cast<fileheader*>(contents.data());
        std::string fullpath;
        size_t offset = 0;

        while (true)
        {
            if (std::strcmp(file->signature, ILFS_SIGNATURE))
            {
                std::cout << "Error: File signature incorrect!" << std::endl;
                break;
            }

            fullpath = dir + file->name;
            fs::remove_all(fullpath);

            switch (file->type)
            {
                case ILFS_REGULAR:
                {
                    std::ofstream newfile(fullpath, std::ios::binary);
                    newfile.write(contents.c_str() + offset + sizeof(fileheader), file->size);
                    newfile.close();
                    fs::permissions(fullpath, fs::perms(file->mode));
                    break;
                }
                case ILFS_DIRECTORY:
                    fs::create_directories(fullpath);
                    fs::permissions(fullpath, fs::perms(file->mode));
                    break;
                case ILFS_SYMLINK:
                    fs::create_symlink(file->link, fullpath);
                    break;
            }

            offset += sizeof(fileheader) + file->size;
            if (offset >= fs::file_size(path)) break;

            file = reinterpret_cast<fileheader*>(contents.data() + offset);
        }
    }
}