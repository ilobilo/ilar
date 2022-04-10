#include <filesystem>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

#include <fs.hpp>

size_t filesize(std::string path)
{
    return std::filesystem::file_size(path);
}

std::string filename(std::string path)
{
    return std::filesystem::path(path).filename();
}

bool fileexists(std::string path)
{
    return std::filesystem::exists(path);
}

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
    if (std::string(argv[1]) == "write")
    {
        if (argc < 3)
        {
            usage();
            return EINVAL;
        }
        std::ofstream image(argv[2], std::ios::binary);

        for (int i = 3; i < argc; i++)
        {
            if (fileexists(argv[i]) == false)
            {
                std::cout << "File \"" << argv[i] << "\" does not exist!" << std::endl;
                return ENOENT;
            }

            size_t size = filesize(argv[i]);
            std::string name = filename(argv[i]);
            std::string contents = filecontents(argv[i]);
            if (contents.front() != '\0') size++;

            fileheader file;
            std::strncpy(file.name, name.c_str(), NAME_LENGTH);
            file.length = size;

            image.write(reinterpret_cast<char*>(&file), sizeof(fileheader));
            image.write(contents.c_str(), size);
        }

        image.close();
    }
    else if (std::string(argv[1]) == "read")
    {
        if (argc < 3)
        {
            usage();
            return EINVAL;
        }
        if (fileexists(argv[2]) == false)
        {
            std::cout << "File \"" << argv[2] << "\" does not exist!" << std::endl;
            return ENOENT;
        }

        std::string contents = filecontents(argv[2]);
        size_t filelength = filesize(argv[2]);
        size_t offset = 0;

        while (offset < filelength)
        {
            fileheader *file = reinterpret_cast<fileheader*>(contents.data() + offset);
            std::cout << "Name: " << file->name << ", Size: " << file->length << std::endl;

            offset += sizeof(fileheader);
            std::cout << "Contents: " << std::endl << contents.data() + offset << std::endl;
            offset += file->length;
            if (offset < filelength) std::cout << std::endl;
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