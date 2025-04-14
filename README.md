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

### Maintenance Tasks

| Command | Description |
|---------|-------------|
| `make clean` | Remove all build artifacts |
| `sudo make install` | Install Linux library and headers (requires `TARGET=linux`) |
| `sudo make uninstall` | Remove installed files |

## Build Configuration

### Development Mode (`make dev`)
- Compiler: `-std=gnu11`
- Flags: `-g -Wall -Wextra -fsanitize=address -fno-omit-frame-pointer -pg`
- No optimizations

### Production Mode (`make prod`)
- Compiler: `-std=gnu11` 
- Flags: `-O3 -flto -Wall -Wextra`
- Full optimizations enabled

## Directory Structure
```
project/
├── bin/       # Final binaries
├── build/     # Intermediate objects
└── src/       # Source files
```

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
