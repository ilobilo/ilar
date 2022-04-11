#include <cmds/extract.hpp>
#include <cmds/create.hpp>
#include <cmds/list.hpp>
#include <cstring>
#include <fs.hpp>

void usage()
{
    std::cout << "Usage:" << std::endl;
    std::cout << " list -- List files in image" << std::endl;
    std::cout << "   Example: ilfs list myimage.ilfs" << std::endl;
    std::cout << " create -- Create image from files" << std::endl;
    std::cout << "   Example: ilfs create myimage.ilfs myfile.txt myfile2.txt mydir" << std::endl;
    std::cout << " extract -- List files in image" << std::endl;
    std::cout << "   Example: ilfs extract myimage.ilfs outdir" << std::endl;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        usage();
        return EINVAL;
    }

    if (!std::strcmp(argv[1], "list"))
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
        if (fs::is_regular_file(argv[2]) == false)
        {
            std::cout << "\"" << argv[2] << "\" is not a regular file!" << std::endl;
            return EINVAL; // ENOTREG ?
        }

        listfiles(argv[2]);
    }
    else if (!std::strcmp(argv[1], "create"))
    {
        if (argc < 3)
        {
            usage();
            return EINVAL;
        }

        fs::remove_all(argv[2]);
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
    else if (!std::strcmp(argv[1], "extract"))
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

        std::string dir;
        if (argc < 4)
        {
            dir = fs::current_path().string();
            if (fs::exists(dir) == false)
            {
                std::cout << "Directory \"" << dir << "\" does not exist!" << std::endl;
                return ENOENT;
            }
            if (fs::is_directory(dir) == false)
            {
                std::cout << "\"" << dir << "\" is not a directory!" << std::endl;
                return ENOTDIR;
            }
        }
        else dir = argv[3];

        extract(argv[2], dir);
    }
    else
    {
        std::cout << "Unknown command line option \"" << argv[1] << "\"" << std::endl;
        usage();
        return EINVAL;
    }

    return 0;
}