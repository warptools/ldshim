# ldshim

This is a simple shim for avoiding the system's global dynamic linker (e.g., `/lib64/ld-linux-x86-64.so.2`) to maintain path agonism.

## Building

Use `make` to build with GCC. This will create a static `ldshim` binary.

## Usage

1. Determine a binary that you want to shim, for example `bin/bash`
2. Move the actual, dynamically linked binary to `dynbin/bash`
3. Place a copy of ldshim at `bin/bash`
4. Place a dynamic linker at `lib/ld-linux-x86-64.so.2`
5. Running the ldshim at `bin/bash` will invoke `dynbin/bash` using the dynamic linker at `lib/ld-linux-x86-64.so.2`
