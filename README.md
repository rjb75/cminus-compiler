# CPSC 411 Project

This project contains the code for the CPSC 411 project.

## Assignment 1: Scanner

The first assignment is to complete a scanner for the C-Minus programming language that reads in a source file and produces a listing of each token one by one, annotated with the token kind, and it's location in the source.

### Building the Scanner

Utilizing the included make file in the root directory the scanner can be built by running the following command

```bash
$ make scanner-build
```

### Running the Scanner

To run the scanner on a file just give the filename as an argument. By default the output will be stored in `scanner.out`.

```bash
$ ./scanner test.cminus
```

To print the output to console, enable debug mode with the `-d` flag

```bash
$ ./scanner -d test.cminus
```

To print the output to a custom file, specify a file with the `-o` flag

```bash
$ ./scanner -o test.out test.cminus
```

### Testing the Scanner

The scanner is currently ran against pre-determined test files and outputs. The test cases are located in `/tests/scanner`. Any `.cminus` file located in this folder will tested against as long as it has a result file following the format of `.cminus.res` with the same name. The output of each test will have the `cminus.tst.res` format.

Scanner tests can be ran as follows

```bash
$ make scanner-test
```
