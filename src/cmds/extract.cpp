#include <cmds/create.hpp>
#include <cstring>
#include <fs.hpp>
#include <sys/stat.h>

void extract(std::string path, std::string dir)
{
    std::string contents = filecontents(path);
    size_t filelength = fs::file_size(path);
    fileheader *file = reinterpret_cast<fileheader*>(contents.data());
    std::string fullpath;
    size_t offset = 0;

    while (offset < filelength && !std::strcmp(file->magic, ILFS_MAGIC))
    {
        fullpath = dir + file->name;
        fs::remove_all(fullpath);

        switch (file->type)
        {
            case ILFS_REGULAR:
            {
                std::ofstream newfile(fullpath, std::ios::binary);
                newfile.write(contents.c_str() + offset + sizeof(fileheader), file->size);
                newfile.close();
                break;
            }
            case ILFS_DIRECTORY:
                fs::create_directories(fullpath);
                break;
            case ILFS_SYMLINK:
                // TODO
                break;
        }

        fs::permissions(fullpath, static_cast<fs::perms>(file->mode));

        offset += sizeof(fileheader) + file->size;
        file = reinterpret_cast<fileheader*>(contents.data() + offset);
    }
}