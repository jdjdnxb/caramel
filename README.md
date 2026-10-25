# Caramel

A hobby operating system written in C23 for x86-64 and assembly (eventually going to support additional architectures).

Caramel is a monolithic kernel built from scratch, with Limine as its bootloader. It is focused on understanding OS internals and hardware support (can't lie, there is absolutely VERY little hardware support at the moment).

# Current state

- [X] Limine boot
- [x] Framebuffer and terminal
- [x] GDT and IDT initialization
- [x] ACPI table discovery
- [ ] Physical memory management
- [ ] Virtual memory management and paging
- [ ] Local APIC and I/O APIC support

# Building

### Dependencies
> NOTE!
> Package names may vary between distributions and versions. Ensure the required tools are available in your `PATH` before building.

Caramel requires the following tools:

- `x86_64-elf-gcc` — an x86-64 cross-compiler
- `nasm` — assembler
- `xorriso` — ISO image creation
- `qemu-system-x86_64` — x86-64 system emulator

You will need to build the cross-compiler yourself. For instructions, refer to a guide for building a GCC cross-compiler targeting `x86_64-elf`.

### Arch-based distributions

```bash
sudo pacman -S nasm xorriso qemu-system-x86
```

### Red Hat-based distributions

```bash
sudo dnf install nasm xorriso qemu-system-x86
```

### Debian-based distributions

```bash
sudo apt install nasm xorriso qemu-system-x86
```

### Windows

Using WSL2 is recommended. Alternatively, you can use MSYS2, provided you configure the required cross-compiler and build tools.

### macOS

With [Homebrew](https://brew.sh/):

```bash
brew install nasm xorriso qemu
```

## Running in qemu

Simply run:
```bash
make run
```
It will compile, generate the ISO and boot automatically using QEMU.

## Running in other emulators
Run:
```bash
make
```
It will compile and generate the ISO without booting with QEMU. I haven't used any other emulators, so you will have to configure them yourself.

## Cleaning
Run:
```bash
make clean
```
