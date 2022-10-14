# ILAR
Simple archive format

### TODO
- [x] Regular files
- [x] Directories
- [x] Symlinks
- [x] File modes
- [x] Create
- [x] List
- [x] Extract
- [x] GZIP
- [x] BZIP2
- [x] LZMA

### Dependencies
* Meson
* Ninja
* C++ compiler
* Boost iostreams (libboost-iostreams-dev on debian based distros)

### Building and running
* ``meson builddir``
* ``ninja -C builddir``
* Create ILAR archive: (To use compression, add .gz, .bz2 or .xz to file name or use -t/--type. Possible values are gz, bz2 and xz)
```
./builddir/ilar -c/--create -t/--type {gz|bz2|xz} -f/--file myarchive.ilar.ext myfile.txt mydir mysymlink.pdf
```
* Extract ILAR archive:
```
./builddir/ilar -x/--extract -f/--file myarchive.ilar -d/--directory myoutdir/
```
* List files in ILAR archive:
```
./builddir/ilar -l/--list -f/--file myarchive.ilar
```
* Show help
```
./builddir/ilar -h/--help
```
* Show version
```
./builddir/ilar -v/--version
```

### Specifications
ILAR archive consists of chainloaded file headers and contents. First file header starts at offset 0x00. To get contents of that file, add ``sizeof(fileheader)`` (274 bytes) to header offset. To get next file header, add another ``file->size`` bytes.\
``name`` and ``link`` are null terminated
```
1st file header -> 1st file contents -> 2nd file header -> 2nd file contents
```
File header structure:
```c
#define PATH_LENGTH 128
#define ILAR_SIGNATURE "ILAR"

struct fileheader
{
    char signature[5]; // Should be ILAR_SIGNATURE
    char name[PATH_LENGTH]; // File name with full path
    char link[PATH_LENGTH]; // Contains relative path to symlinked file
    uint64_t size; // Size of file contents in bytes
    uint8_t type; // File type
    uint32_t mode; // File modes
} __attribute__((packed));
```
File types:
```c
enum filetypes
{
    ILAR_REGULAR = 0,
    ILAR_DIRECTORY = 1,
    ILAR_SYMLINK = 2
};
```