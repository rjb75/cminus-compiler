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
$ ./scanner test.cm
```

To print the output to console, enable debug mode with the `-d` flag

```bash
$ ./scanner -d test.cm
```

To print the output to a custom file, specify a file with the `-o` flag

```bash
$ ./scanner -o test.out test.cm
```

### Testing the Scanner

The scanner is currently ran against pre-determined test files and outputs. The test cases are located in `/tests/scanner`. Any `.cm` file located in this folder will tested against as long as it has a result file following the format of `.cm.res` with the same name. The output of each test will have the `cm.tst.res` format.

Scanner tests can be ran as follows

```bash
$ make scanner-test
```

## Assignment 2: Parser

The second assignment is to complete a parser for the C-Minus programming language that reads in a source file and produces a text based output of the produced abstract syntax tree (AST).

This implementation of the C-Minus parser utilizes the handmade scanner from Assignment 1 in combination with Bison.

### Building the Parser

Utilizing the included make file in the root directory the parser can be built by running the following command

```bash
$ make parser-build
```

### Running the Parser

To run the scanner on a file just give the filename as an argument. By default the output will be stored in `parser.out`.

```bash
$ ./parser test.cm
```

To print the output to console, enable debug mode with the `-d` flag

```bash
$ ./parser -d test.cm
```

To print the output to a custom file, specify a file with the `-o` flag

```bash
$ ./parser -o test.out test.cm
```

### Testing the Parser

The parser is currently ran against pre-determined test files and outputs. The test cases are located in `/tests/parser`. Any `.cm` file located in this folder will tested against as long as it has a result file following the format of `.cm.res` with the same name. The output of each test will have the `cm.tst.res` format.

Parser tests can be ran as follows

```bash
$ make parser-test
```

### Resources

When implementing the parser the following resources influenced the final product:

- https://thesharpowl.github.io/posts/HAND_WRITTEN_SCANNER_WITH_BISON_C_API/
- [Compiler Construction - Principles and Practice (Kennith C. Louden)](https://www.cs.sjsu.edu/~louden/cmptext/)
- In Class Tutorials by [Melika Norouzbeygi](https://melikano.github.io/) including [Scanner/Parser Tutorial Code](https://github.com/Melikano/little-scanner-parser)

## Assignment 3: Analyzer

The third assignment is to complete a analyzer for the C-Minus programming language that reads in an abstract syntax tree (AST) and analyzes the symbols and performs type checking. If successful, an updated AST will be produced with correct types and symbols that are relative to the scope of the declared variable or function. 

This implementation builds off the parser from assignment 2.

### Building the Analyzer

Utilizing the included make file in the root directory the analyzer can be built by running the following command

```bash
$ make analyzer-build
```

### Running the Analyzer

To run the scanner on a file just give the filename as an argument. By default the output will be stored in `analyzer.out`.

```bash
$ ./analyzer test.cm
```

To print the output to console, enable debug mode with the `-d` flag
*Note:* this output will include debug info from the scanner and parser

```bash
$ ./analyzer -d test.cm
```

To print the output to a custom file, specify a file with the `-o` flag

```bash
$ ./analyzer -o test.out test.cm
```

### Testing the Analyzer

The analyzer is currently ran against pre-determined test files and outputs. The test cases are located in `/tests/analyzer`. Any `.cm` file located in this folder will tested against as long as it has a result file following the format of `.cm.res` with the same name. In cases where files do not produce a valid AST the error result will then be compared against a file with the format `.cm.err`. The output of each test will have the `cm.tst.res` format for valid `.cm` files and `cm.tst.err` for invalid ones.

Analyzer tests can be ran as follows

```bash
$ make analyzer-test
```

### Resources

When implementing the parser the following resources influenced the final product:

- In Class Tutorials by [Melika Norouzbeygi](https://melikano.github.io/)
