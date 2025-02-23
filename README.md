<div align="center">
  <h1>classparse</h1>
  Simple and easy to use Java classfile parser.
</div>

## Getting Started
As of now, the library itself is not yet published to any repository. You will need to build it manually.

### Building

Luckily, building the project is very easy! The only thing you will need is `make`, and follow these steps:
1) Clone the repository with `git`
2) Navigate to the directory
3) Run `make` or `make all`(they do the same thing). This will build the library. The library will appear in the `build/` directory.
4) Optionally run `make install` to copy the library & header files into `/usr/include/` and `/usr/lib/`.
5) You're now good to go! Don't forget to link with `libclassparse`!

## Examples
Examples can be found inside the `examples/` directory, you can compile all of them using the `make` command.
Please note that it's recommended to run `make` and `make install` in the root directory to assure that the linkage of the library will succceed!
These are some of the most commonly used functions:

1) `ClassFile *ReadFromStream(FILE *stream);` will read the given classfile from C-style stream.
2) `void FreeClassFile(ClassFile *cf);` should be always used to free the resources allocated by reading.
3) `Method *GetMethodByName(ClassFile *cf, const char *name);` etc. can be used as utilities to make your life easier.

The rest of them can be found in the [classparse.h](src/classparse.h)

## Contributing
The library is pretty easy to understand, so if you have any recommendations, feel free to submit your changes as PR, I will gladly marge them!

## License

Licensed under the [MIT License](LICENSE).
