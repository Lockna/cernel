# cernel: x64 Toy kernel

cernel is a tiny x64 toy kernel which helps us to gain a better knowledge about kernel and os development.

At this point you can work on it with Linux or WSL, the Makefile has been optimized for WSL.
Whether Windows would work natively I haven't tried yet, but feel free to include support for development on Windows.

## Build requirements
These are the tools needed for the build:
- `git` (for initial download)
- `make`
- `gcc` (for building gcc on your own)
- `qemu` (for testing)

If you're working in WSL you will also need this packages for building gcc
- `uuid-dev`
- `libfuse-dev`
- `libmpfr-dev`
- `libmpc-dev`
- `libgmp-dev`

`nasm` is getting build via our toolchain

```bash
# Clone the repository where you like
git clone https://github.com/Lockna/cernel
cd cernel
# Make the toolchain, so the kernel can be compiled with this tools
make toolchain
# Make the kernel, you don't have to provide -j
make
# You can run the kernel either with make run or make drun
make run
```

The difference between `make run` and `make drun` is, that `make drun` creates a gdb server, where you can connect to. This will look something like this

```bash
# This terminal will open qemu, which will say "Guest has not initialized the display (yet).
make drun
```

```bash
# Open a new terminal and enter gdb with the cernel.elf file loaded
cd cernel/kernel/
gdb cernel.elf
gdb> target remote :1234
gdb> break * _start
# And now you can start debugging using gdb
```

## Contact
Feel free to reach out to `Lockna#5599` or `mp#2453` on Discord

## Contributing
Contributions to code and documentation are heavily appreciated, may it be a bug fix or a improvent of the existing code or code documentation.
New features would be gladly taken over by us, as the project was created for exactly this purpose.

## License
This software is licensed unter the terms of the MIT License
