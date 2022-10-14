// Copyright (C) 2022  ilobilo

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/bzip2.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/lzma.hpp>
#include <boost/iostreams/copy.hpp>

#include <tools/compress.hpp>
#include <filesystem>

namespace bio = boost::iostreams;
namespace fs = std::filesystem;

comp_type getcomptype(const std::stringstream &data)
{
    std::string strdata = data.str();

    if (!std::memcmp(strdata.data(), gziphdr, 2))
        return comp_type::gzip;

    if (!std::memcmp(strdata.data(), bzip2hdr, 3))
        return comp_type::bzip2;

    if (!std::memcmp(strdata.data(), lzmahdr, 6))
        return comp_type::lzma;

    return comp_type::none;
}

void compress(std::stringstream &data, comp_type comp)
{
    if (comp == comp_type::none)
        return;

    bio::filtering_streambuf<bio::input> out;

    switch (comp)
    {
        case comp_type::gzip:
            out.push(bio::gzip_compressor());
            break;
        case comp_type::bzip2:
            out.push(bio::bzip2_compressor());
            break;
        case comp_type::lzma:
            out.push(bio::lzma_compressor());
            break;
        default:
            break;
    }
    out.push(data);

    std::stringstream compressed;
    bio::copy(out, compressed);
    data = std::move(compressed);
}

void decompress(std::stringstream &data)
{
    bio::filtering_streambuf<bio::input> out;

    switch (getcomptype(data))
    {
        case comp_type::gzip:
            out.push(bio::gzip_decompressor());
            break;
        case comp_type::bzip2:
            out.push(bio::bzip2_decompressor());
            break;
        case comp_type::lzma:
            out.push(bio::lzma_decompressor());
            break;
        default:
            break;
    }
    out.push(data);

    std::stringstream decompressed;
    bio::copy(out, decompressed);
    data = std::move(decompressed);
}