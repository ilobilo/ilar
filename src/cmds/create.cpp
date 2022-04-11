#include <cmds/create.hpp>
#include <cstring>
#include <fs.hpp>

std::string filecontents(fs::path path)
{
    std::ostringstream buf;
    std::ifstream input(path, std::ios::binary);
    buf << input.rdbuf();
    input.close();
    return buf.str();
}

std::string filecontents(std::string path)
{
    std::ostringstream buf;
    std::ifstream input(fs::path(path), std::ios::binary);
    buf << input.rdbuf();
    input.close();
    return buf.str();
}

void createreg(std::string parent, fs::path path, std::ofstream &image)
{
    size_t size = fs::file_size(path);
    std::string name = parent + "/" + path.filename().string();
    std::string contents = filecontents(path);

    fileheader file;
    std::strncpy(file.magic, ILFS_MAGIC, 5);
    std::strncpy(file.name, name.c_str(), NAME_LENGTH);
    file.size = size;
    file.type = ILFS_REGULAR;
    file.mode = static_cast<uint64_t>(fs::status(path).permissions());

    image.write(reinterpret_cast<char*>(&file), sizeof(fileheader));
    image.write(contents.c_str(), size);
}

void createdir(std::string parent, fs::path path, std::ofstream &image)
{
    std::string name = parent + "/" + path.filename().string();
    std::string contents = filecontents(path);

    fileheader file;
    std::strncpy(file.magic, ILFS_MAGIC, 5);
    std::strncpy(file.name, name.c_str(), NAME_LENGTH);
    file.size = 0;
    file.type = ILFS_DIRECTORY;
    file.mode = static_cast<uint32_t>(fs::status(path).permissions());

    image.write(reinterpret_cast<char*>(&file), sizeof(fileheader));

    for (const auto &entry : fs::directory_iterator(path))
    {
        createfile(parent + "/" + path.filename().string(), entry, image);
    }
}

void createfile(std::string parent, fs::path path, std::ofstream &image)
{
    switch (fs::status(path).type())
    {
        case fs::file_type::regular:
            createreg(parent, path, image);
            break;
        case fs::file_type::directory:
            createdir(parent, path, image);
            break;
        case fs::file_type::symlink:
            // TODO
            break;
        default:
            std::cout << path.c_str() << ": Unknown file type!" << std::endl;
            break;
    }
}