# caramel

## Building on Windows

WIP

## Building on Linux

WIP

## Building on macOS

Assuming Xcode Command Line Tools and [Homebrew](https://brew.sh/) installed already

```sh
brew bundle install
git submodule update --init --recursive
make -C limine
make
make run
```

The files inside limine.dSYM are generated macOS debug symbols. No need to commit them so hide them from Git locally
```sh
git -C limine rev-parse --git-path info/exclude | xargs -I{} sh -c 'echo "/limine.dSYM/" >> "$1"' _ {}
```

---

### Apple Silicon

Apple Silicon Macs run this x86-64 kernel using QEMU software emulation.
The `x86_64-elf-gcc` cross-compiler is needed (Apple's system compiler doesn't work)

---

Commands

- `make`: build
- `make run`: build and boot
- `make clean`: remove generated files
- `make debug`: boot QEMU paused with GDB server on port 1234

---

makefile doesn't track header dependencies
 
If you change headers run `make clean` and `make`

---
