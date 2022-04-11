#include <filesystem>
#include <iostream>
#include <fstream>
#include <cstring>

#include <fs.hpp>
namespace fs = std::filesystem;

std::string filecontents(std::string path)
{
    std::ostringstream buf;
    std::ifstream input(path.c_str(), std::ios::binary);
    buf << input.rdbuf();
    input.close();
    return buf.str();
}

void usage()
{
    std::cout << "Usage:" << std::endl;
    std::cout << " read -- List files in image" << std::endl;
    std::cout << "   Example: ilfs read myimage.ilfs" << std::endl;
    std::cout << " write -- Create image from files" << std::endl;
    std::cout << "   Example: ilfs write myimage.ilfs myfile.txt myfile2.txt" << std::endl;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        usage();
        return EINVAL;
    }
    if (!std::strcmp(argv[1], "write"))
    {
        if (argc < 3)
        {
            usage();
            return EINVAL;
        }
        std::ofstream image(argv[2], std::ios::binary);

        for (int i = 3; i < argc; i++)
        {
            if (fs::exists(argv[i]) == false)
            {
                std::cout << "File \"" << argv[i] << "\" does not exist!" << std::endl;
                return ENOENT;
            }

            size_t size = fs::file_size(argv[i]);
            std::string name = fs::path(argv[i]).filename();
            std::string contents = filecontents(argv[i]);
            if (contents.front() != '\0') size++;

            fileheader file;
            std::strncpy(file.magic, ILFS_MAGIC, 5);
            std::strncpy(file.name, name.c_str(), NAME_LENGTH);
            file.length = size;

            image.write(reinterpret_cast<char*>(&file), sizeof(fileheader));
            image.write(contents.c_str(), size);
        }

        image.close();
    }
    else if (!std::strcmp(argv[1], "read"))
    {
        if (argc < 3)
        {
            usage();
            return EINVAL;
        }
        if (fs::exists(argv[2]) == false)
        {
            std::cout << "File \"" << argv[2] << "\" does not exist!" << std::endl;
            return ENOENT;
        }

        std::string contents = filecontents(argv[2]);
        size_t filelength = fs::file_size(argv[2]);
        fileheader *file = reinterpret_cast<fileheader*>(contents.data());
        size_t offset = 0;

        while (offset < filelength && !std::strcmp(file->magic, ILFS_MAGIC))
        {
            std::cout << "Name: " << file->name << ", Size: " << file->length << std::endl;

            offset += sizeof(fileheader);
            std::cout << "Contents: " << std::endl << contents.data() + offset << std::endl;
            offset += file->length;
            if (offset < filelength && *(contents.data() + offset - 2) != '\n') std::cout << std::endl;

            file = reinterpret_cast<fileheader*>(contents.data() + offset);
        }
    }
    else
    {
        std::cout << "Unknown command line option " << argv[1] << std::endl;
        usage();
        return EINVAL;
    }

    return 0;
}