#include <create.hpp>
#include <cstring>
#include <fs.hpp>

void usage()
{
    std::cout << "Usage:" << std::endl;
    std::cout << " list -- List files in image" << std::endl;
    std::cout << "   Example: ilfs list myimage.ilfs" << std::endl;
    std::cout << " create -- Create image from files" << std::endl;
    std::cout << "   Example: ilfs create myimage.ilfs myfile.txt myfile2.txt" << std::endl;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        usage();
        return EINVAL;
    }
    if (!std::strcmp(argv[1], "create"))
    {
        if (argc < 3)
        {
            usage();
            return EINVAL;
        }

        std::remove(argv[2]);
        std::ofstream image(argv[2], std::ios::binary);

        for (int i = 3; i < argc; i++)
        {
            if (fs::exists(argv[i]) == false)
            {
                std::cout << "File \"" << argv[i] << "\" does not exist!" << std::endl;
                image.close();
                std::remove(argv[2]);
                return ENOENT;
            }

            fs::path filepath(argv[i]);
            createfile("", filepath, image);
        }

        image.close();
    }
    else if (!std::strcmp(argv[1], "list"))
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

        std::string contents = filecontents(fs::path(argv[2]));
        size_t filelength = fs::file_size(argv[2]);
        fileheader *file = reinterpret_cast<fileheader*>(contents.data());
        size_t offset = 0;

        while (offset < filelength && !std::strcmp(file->magic, ILFS_MAGIC))
        {
            std::cout << "Name: " << file->name << ", Size: " << file->length << ", Type: ";
            switch (file->type)
            {
                case ILFS_REGULAR:
                    std::cout << "Regular" << std::endl;
                    break;
                case ILFS_DIRECTORY:
                    std::cout << "Directory" << std::endl;
                    break;
                default:
                    std::cout << "Unknown" << std::endl;
                    break;
            }
            offset += sizeof(fileheader) + file->length;

            file = reinterpret_cast<fileheader*>(contents.data() + offset);
        }
    }
    else
    {
        std::cout << "Unknown command line option \"" << argv[1] << "\"" << std::endl;
        usage();
        return EINVAL;
    }

    return 0;
}