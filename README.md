# ldshim

This is a simple shim for avoiding the system's global dynamic linker (e.g., `/lib64/ld-linux-x86-64.so.2`) to maintain path agonism.

## Why?

Packaging dynamically linked binaries that are fully *path agonistic* requires that we override paths to avoid system-wide paths.

For shared libraries, this can be done by defining the ELF `rpath` to a relative path using `$ORIGIN`. For example: if 
`rpath = $ORIGIN/../lib`, the dynamic linker will look for binaries at `<location of binary>/../lib/`. This allows for bundling
dynamically linked libraries in a package.

However, the dynamic linker path is placed in the `.interp` section of the ELF header. This value *must be an absolute path*, so
we cannot use the technique used for shared libraries! This problem is addressed with `ldshim`, which manually invokes the dynamic 
linker from a relative path, preventing the system from using the ELF-specified dynamic linker. 

## Building

Use `make` to build with GCC. This will create a static `ldshim` binary.

## Usage

1. Determine a binary that you want to shim, for example `bin/bash`
2. Move the actual, dynamically linked binary to `dynbin/bash`
3. Place a copy of ldshim at `bin/bash`
4. Place a dynamic linker at `lib/ld-linux-x86-64.so.2`
5. Running the ldshim at `bin/bash` will invoke `dynbin/bash` using the dynamic linker at `lib/ld-linux-x86-64.so.2`
