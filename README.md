# ILAR
## Simple archive format

### TODO
- [x] Regular files
- [x] Directories
- [x] Symlinks
- [x] File modes
- [x] Create image
- [x] List files in image
- [x] Extract files from image
- [x] GZIP, BZIP2 and LZMA support

### Dependencies to build
* Clang++ (For custom compiler specify CPP=compiler when running make)
* Make
* Boost iostreams (libboost-iostreams-dev on debian based distros)
* Linux or WSL

### Building and usage
* Compile the code with ```make```. Executable will be available in bin/ folder
* Create ILAR archive:
```
bin/ilar create myarchive.ilar myfile.txt mydir mysymlink.pdf
```
* To use compression add .gz, .bz2 or .xz to archive file name
* Extract ILAR archive:
```
bin/ilar extract myarchive.ilar myoutdir/
```
* List files in ILAR archive:
```
bin/ilar list myarchive.ilar
```
* Show help
```
bin/ilar help
```

### Specifications
ILAR archive consists of chainloaded file headers and contents. First file header starts at offset 0x00. To get contents of that file, add ```sizeof(fileheader)``` (0x112 or 274 bytes) to header offset. To get next file header, add another ```file->size``` bytes
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
