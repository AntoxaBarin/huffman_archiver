# huffman_archiver
### Simple file archiver using Huffman encoding.

### Build archiver
```shell
$ mkdir build && cd build
$ cmake ..
$ cmake --build .
```
### Tests
[Doctest](https://github.com/doctest/doctest) used for testing.
To run tests:
```shell
$ ./huffman_test 
```

### Usage:
There are several flags:
* `-c` compress text file
* `-u` uncompress binary file
* `-f <path>`, `--file <path>` name of input file
* `-o <path>`, `--output <path>` name of output file

To encode text file
```shell
$ ./huffman_archiver -c -f toCompress.txt -o compressed.bin
```
To decode binary file
```shell
$ ./huffman_archiver -u -f compressed.bin -o decompressed.bin
```

Example:
```
$ ./huffman_archiver -c -f myfile.txt -o result.bin
15678 
6172
482
```
15678 - size of input file in bytes, 6172 - size of compressed data without metadata in bytes,
482 - size of metadata in bytes.
