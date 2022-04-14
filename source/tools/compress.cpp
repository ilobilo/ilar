// Copyright (C) 2022  ilobilo

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/bzip2.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/lzma.hpp>
#include <boost/iostreams/copy.hpp>
#include <tools/compress.hpp>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

comp_type getcomptype(const std::stringstream &data)
{
    std::string strdata = data.str();
    if (!std::memcmp(strdata.data(), gziphdr, 2)) return comp_type::GZIP;
    if (!std::memcmp(strdata.data(), bzip2hdr, 2)) return comp_type::BZIP2;
    if (!std::memcmp(strdata.data(), lzmahdr, 2)) return comp_type::LZMA;
    return comp_type::NONE;
}

void compress(std::stringstream &data, comp_type comp)
{
    if (comp == comp_type::NONE) return;

    std::stringstream compressed;
    bio::filtering_streambuf<bio::input> out;

    switch (comp)
    {
        case comp_type::GZIP:
            out.push(bio::gzip_compressor());
            break;
        case comp_type::BZIP2:
            out.push(bio::bzip2_compressor());
            break;
        case comp_type::LZMA:
            out.push(bio::lzma_compressor());
            break;
        default:
            break;
    }

    out.push(data);
    bio::copy(out, compressed);
    data = std::move(compressed);
}

void decompress(std::stringstream &data)
{
    std::stringstream decompressed;
    bio::filtering_streambuf<bio::input> out;

    switch (getcomptype(data))
    {
        case comp_type::GZIP:
            out.push(bio::gzip_decompressor());
            break;
        case comp_type::BZIP2:
            out.push(bio::bzip2_decompressor());
            break;
        case comp_type::LZMA:
            out.push(bio::lzma_decompressor());
            break;
        default:
            break;
    }

    out.push(data);
    bio::copy(out, decompressed);
    data = std::move(decompressed);
}