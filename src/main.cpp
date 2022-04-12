#include <cmds/extract.hpp>
#include <cmds/create.hpp>
#include <cmds/list.hpp>
#include <header.hpp>
#include <cstring>
#include <cerrno>

void usage()
{
    std::cout << "Usage:" << std::endl;
    std::cout << " list -- List files in image" << std::endl;
    std::cout << "   Example: ilar list myimage.ilar" << std::endl;
    std::cout << " create -- Create image from files" << std::endl;
    std::cout << "   Example: ilar create myimage.ilar myfile.txt myfile2.txt mydir" << std::endl;
    std::cout << " extract -- List files in image" << std::endl;
    std::cout << "   Example: ilar extract myimage.ilar outdir" << std::endl;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        usage();
        errno = EINVAL;
        return EXIT_FAILURE;
    }

    if (!std::strcmp(argv[1], "list"))
    {
        if (argc < 3)
        {
            usage();
            errno = EINVAL;
            return EXIT_FAILURE;
        }
        if (fs::exists(argv[2]) == false)
        {
            std::cout << "File \"" << argv[2] << "\" does not exist!" << std::endl;
            errno = ENOENT;
            return EXIT_FAILURE;
        }
        if (fs::is_regular_file(argv[2]) == false)
        {
            std::cout << "\"" << argv[2] << "\" is not a regular file!" << std::endl;
            errno = EINVAL;
            return EXIT_FAILURE; // ENOTREG ?
        }

        cmds::list(argv[2]);
    }
    else if (!std::strcmp(argv[1], "create"))
    {
        if (argc < 3)
        {
            usage();
            errno = EINVAL;
            return EXIT_FAILURE;
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
                errno = ENOENT;
                return EXIT_FAILURE;
            }

            fs::path path(argv[i]);
            cmds::create("", path, image);
        }

        image.close();
    }
    else if (!std::strcmp(argv[1], "extract"))
    {
        if (argc < 3)
        {
            usage();
            errno = EINVAL;
            return EXIT_FAILURE;
        }

        if (fs::exists(argv[2]) == false)
        {
            std::cout << "File \"" << argv[2] << "\" does not exist!" << std::endl;
            errno = ENOENT;
            return EXIT_FAILURE;
        }

        std::string dir;
        if (argc < 4) dir = fs::current_path().string();
        else
        {
            dir = argv[3];
            if (fs::exists(dir) == false)
            {
                std::cout << "Directory \"" << dir << "\" does not exist!" << std::endl;
                errno = ENOENT;
                return EXIT_FAILURE;
            }
            if (fs::is_directory(dir) == false)
            {
                std::cout << "\"" << dir << "\" is not a directory!" << std::endl;
                errno = ENOTDIR;
                return EXIT_FAILURE;
            }
        }

        cmds::extract(argv[2], dir);
    }
    else
    {
        std::cout << "Unknown command line option \"" << argv[1] << "\"" << std::endl;
        usage();
        errno = EINVAL;
        return EXIT_FAILURE;
    }

    return 0;
}