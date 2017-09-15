# GCIDE-CLI
Command line tool for searching the GCIDE

## Synopsis
Search for words in the GCIDE and write them to standard output. This can be
from the command line, standard input, or a file where each line is searched for
as a word. It is possible to use any combination of these methods at once. It is
also possible to specify what information to output.

## Installation
### Dependencies
The libxml++ and glibmm libraries are required for the program to run. CMake is
used for building and installing.

### CMake
Standard CMake installation:

```bash
mkdir build
cd build
cmake ..
make
make install
```

The build directory may be anywhere, in-tree as well. Use
`-DCMAKE_INSTALL_PREFIX=/usr` to control installation location. Root privileges
are required for the `make install` command.

## Usage
See the gcide-cli man page:
```bash
man gcide-cli
```

In short, use the `--stdin` flag to read words from standard input and the
`--file` flag to specify a file. Options such as `--word` exist to write the
word in addition to the definition. Remaining options are also interpreted as
words to define. For example:
```bash
echo "word1" | gcide-cli --file "my_words.txt" --stdin "additional_word"
```

## License
GPLv3
