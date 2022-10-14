// Copyright (C) 2022  ilobilo

#include <conflict/conflict.hpp>
#include <tools/compress.hpp>

#include <cmds/extract.hpp>
#include <cmds/create.hpp>
#include <cmds/list.hpp>

#include <header.hpp>

#include <filesystem>
#include <algorithm>
#include <fstream>

namespace fs = std::filesystem;

std::vector<std::string_view> files;
std::string_view directory;
std::string_view archive;

uint64_t compression = 0;
uint64_t flags = 0;

const auto parser = conflict::parser
{
    conflict::option { { 'h', "help", "Show help" }, flags, (1 << 0) },
    conflict::option { { 'v', "version", "Show version" }, flags, (1 << 1) },
    conflict::option { { 'l', "list", "List files in archive" }, flags, (1 << 2) },
    conflict::option { { 'c', "create", "Create an archive" }, flags, (1 << 3) },
    conflict::option { { 'x', "extract", "Extract files from an archive" }, flags, (1 << 4) },

    conflict::string_option { { 'f', "file", "ILAR archive" }, "filename", archive },
    conflict::string_option { { 'd', "dir", "Output directory" }, "dirname", directory },

    conflict::choice { { 't', "type", "Compression type (Detected automatically)" }, conflict::choice_mode::replace, compression,
        conflict::flag { { "gz" }, 1 },
        conflict::flag { { "bz2" }, 2 },
        conflict::flag { { "xz" }, 3 }
    }
};

void usage()
{
    std::cout << "Usage:\n";
    std::cout << "    ilar -h/--help\n";
    std::cout << "    ilar -v/--version\n";
    std::cout << "    ilar -l/--list -f/--file myimage.ilar\n";
    std::cout << "    ilar -c/--create -t/--type {gz|bz2|xz} -f/--file myimage.ilar myfile.txt myfile2.txt mydir\n";
    std::cout << "    ilar -x/--extract -f/--file myimage.ilar -d/--directory outdir\n";
}

bool parse_flags()
{
    if (flags & (1 << 0))
    {
        usage();
        std::cout << "\nOptions:\n";
        parser.print_help();
        return true;
    }
    else if (flags & (1 << 1))
    {
        std::cout << "ilar v0.1\n";
        return true;
    }

    return false;
}

auto main(int argc, char **argv) -> int
{
    parser.apply_defaults();
    conflict::default_report(parser.parse(argc - 1, argv + 1, files));

    if (parse_flags())
        return EXIT_SUCCESS;

    if (archive.empty())
    {
        usage();
        return EXIT_FAILURE;
    }

    if (flags & (1 << 2)) // List
    {
        if (files.size() != 0)
        {
            usage();
            return EXIT_FAILURE;
        }

        if (fs::exists(archive) == false)
        {
            std::cerr << "File '" << archive << "' does not exist!" << std::endl;
            return EXIT_FAILURE;
        }

        if (fs::is_regular_file(archive) == false)
        {
            std::cerr << "'" << archive << "' is not a regular file!" << std::endl;
            return EXIT_FAILURE;
        }

        std::stringstream buffer;
        {
            std::ifstream file(archive.data(), std::ios::binary);
            buffer << file.rdbuf();
        }

        decompress(buffer);
        cmds::list(buffer);
    }
    else if (flags & (1 << 3)) // Create
    {
        if (files.size() == 0)
        {
            usage();
            return EXIT_FAILURE;
        }

        if (fs::exists(archive) == true && fs::is_regular_file(archive) == false)
        {
            std::cerr << "'" << archive << "' is not a regular file!" << std::endl;
            return EXIT_FAILURE;
        }

        std::stringstream buffer;

        for (auto file : files)
        {
            // TODO: Remove this?
            if (fs::equivalent(file, archive))
                continue;

            if (fs::exists(file) == false)
            {
                std::cerr << "File '" << file << "' does not exist!" << std::endl;
                return EXIT_FAILURE;
            }

            if (file.starts_with("/"))
                file.remove_prefix(1);

            cmds::create("", file, buffer);
        }

        auto comp = comp_type(compression);
        fs::path path(archive);

        if (comp == comp_type::none)
        {
            if (path.extension() == ".gz")
                comp = comp_type::gzip;
            else if (path.extension() == ".bz2")
                comp = comp_type::bzip2;
            else if (path.extension() == ".xz")
                comp = comp_type::lzma;
        }

        compress(buffer, comp);
        {
            std::ofstream file(archive.data(), std::ios::binary | std::ios::trunc);
            file << buffer.rdbuf();
        }
    }
    else if (flags & (1 << 4)) // Extract
    {
        if (files.size() != 0)
        {
            usage();
            return EXIT_FAILURE;
        }

        if (fs::exists(archive) == false)
        {
            std::cerr << "File '" << archive << "' does not exist!" << std::endl;
            return EXIT_FAILURE;
        }

        if (fs::is_regular_file(archive) == false)
        {
            std::cerr << "'" << archive << "' is not a regular file!" << std::endl;
            return EXIT_FAILURE;
        }

        std::string outdir { directory };
        if (outdir.empty() == true)
            outdir = ".";

        if (fs::exists(outdir) == false)
            fs::create_directory(outdir);

        if (fs::is_directory(outdir) == false)
        {
            std::cerr << "'" << outdir << "' is not a directory!" << std::endl;
            return EXIT_FAILURE;
        }

        std::stringstream buffer;
        {
            std::ifstream file(archive.data(), std::ios::binary);
            buffer << file.rdbuf();
        }

        decompress(buffer);
        cmds::extract(buffer, outdir);
    }
    else
    {
        usage();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}