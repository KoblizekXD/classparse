<div align="center">
  <h1>classparse</h1>
  C based Java classfile parser. Simple and easy.
</div>

## Getting Started
Using classparse is easy, refer to the building section on guide how to install.
Basic usage is covered in the examples section.

## Installation and Building

To build and install the library globally:

```bash
make TARGET=linux       # Build for Linux
sudo make install       # Install header and shared library
```

### Build Targets

| Command | Description |
|---------|-------------|
| `make` or `make dev` | Development build (sanitizers, debug symbols, profiling) |
| `make prod` | Production build (optimized with `-O3` and LTO) |

### Platform Selection

Append `TARGET=<platform>` to any build command:

| Target | Description | Output Extension |
|--------|-------------|------------------|
| `TARGET=linux` | Linux shared library | `.so` |
| `TARGET=win32` | Windows DLL | `.dll` |
| `TARGET=wasm` | WebAssembly module | `.wasm` |

Examples:
```bash
make dev TARGET=wasm    # Dev build for WebAssembly
make prod TARGET=win32  # Optimized Windows build
```

### Standalone mode
*Enable by defining the `STANDALONE` macro or setting STANDALONE=1 in Makefile*

Classparse supports standalone mode, which will use minimal amount of functions from the standard C library.
This is optimal for things like custom operating systems or any non-libc compliant systems.
Heres a list of `extern`'d functions, which needs to be present:

1) `void *malloc(size_t size)`, `void free(void *ptr)` - required for memory allocations
2) `int strcmp(const char *s1, const char *s2)`
3) `char *strcpy(char *dest, const char *src)`
4) `char *strchr(const char *s, int c)`, `char *strrchr(const char *s, int c)`
5) `size_t strlen(const char *ptr)`
6) `void *memcpy(void *dest, const void *src)`

We tried to strip it down as much as possible, but some of these are just necessity :)

### Build Configuration

#### Development Mode (`make dev`)
- Compiler: `-std=gnu11`
- Flags: `-g -Wall -Wextra -fsanitize=address -fno-omit-frame-pointer -pg`
- No optimizations

#### Production Mode (`make prod`)
- Compiler: `-std=gnu11` 
- Flags: `-O3 -flto -Wall -Wextra`
- Full optimizations enabled

## Examples
Examples can be found inside the `examples/` directory, you can compile all of them using the `make` command.
Please note that it's recommended to run `make` and `make install` in the root directory to assure that the linkage of the library will succceed!
These are some of the most commonly used functions:

1) `ClassFile *ReadFromStream(FILE *stream);` will read the given classfile from C-style stream.
2) `void FreeClassFile(ClassFile *cf);` should be always used to free the resources allocated by reading.
3) `Method *GetMethodByName(ClassFile *cf, const char *name);` etc. can be used as utilities to make your life easier.

The rest of them can be found in the [classparse.h](src/classparse.h)

## Contributing
The library is pretty easy to understand, so if you have any recommendations, feel free to submit your changes as PR, I will gladly merge them!

## License

Licensed under the [MIT License](LICENSE).
