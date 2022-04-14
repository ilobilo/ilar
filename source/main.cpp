// Copyright (C) 2022  ilobilo

#include <tools/compress.hpp>
#include <cmds/extract.hpp>
#include <cmds/create.hpp>
#include <tools/misc.hpp>
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
    std::cout << "   To use compression, add .gz, .xz or .bz2 to image file name" << std::endl;
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

        std::stringstream strstream;
        file2strstream(argv[2], strstream);

        decompress(strstream);
        cmds::list(strstream);
    }
    else if (!std::strcmp(argv[1], "create"))
    {
        if (argc < 3)
        {
            usage();
            errno = EINVAL;
            return EXIT_FAILURE;
        }

        std::stringstream strstream;
        for (int i = 3; i < argc; i++)
        {
            if (fs::exists(argv[i]) == false)
            {
                std::cout << "File \"" << argv[i] << "\" does not exist!" << std::endl;
                std::remove(argv[2]);
                errno = ENOENT;
                return EXIT_FAILURE;
            }

            std::string pathstr(argv[i]);
            if (pathstr.back() == '/') pathstr.pop_back();

            fs::path path(pathstr);
            cmds::create("", path, strstream);
        }

        comp_type comp = comp_type::NONE;
        fs::path path(argv[2]);

        if (path.extension() == ".gz") comp = comp_type::GZIP;
        else if (path.extension() == ".bz2") comp = comp_type::BZIP2;
        else if (path.extension() == ".xz") comp = comp_type::LZMA;

        compress(strstream, comp);

        fs::remove_all(argv[2]);
        strstream2ofstream(strstream, argv[2]);
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

        std::stringstream strstream;
        file2strstream(argv[2], strstream);

        decompress(strstream);
        cmds::extract(strstream, dir);
    }
    else if (!std::strcmp(argv[1], "help")) usage();
    else
    {
        std::cout << "Unknown command line option \"" << argv[1] << "\"" << std::endl;
        usage();
        errno = EINVAL;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}