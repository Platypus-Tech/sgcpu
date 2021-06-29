# sgcpu
## seg's WONDELFUL, INCREDIBLE and POWERFUL 16-bit CPU (: 
Not really ):

This is just a tiny emulated ISA and CPU with an assembler, a simple debugger with disassembly and a test program. This was made really quickly (originally) in the span of 4 days, and it's by no means perfect, with a lot of ugly code, random microcode typos and errors that then cause huge errors when running test programs, etc.

## Ok, how the heck do I run this?
```bash
# You build the toolkit and the tests by running:
sh build.sh
# (no I won't use makefiles)

# You assemble a random assembly file doing:
./sgtk asm FILENAME.asm -o OUTPUT.bin

# You run the assembled version of that random assembly file doing:
./sgtk run -l OUTPUT.bin
# Using that same command you can run the assembled tests at tests/bin.

# You can also add -d followed by the number of microseconds per cycle to the run command to see some magic debug info:
./sgtk run -l OUTPUT.bin -d 125000 # Each cycle takes 125 ms, and thus each instruction takes 1000 ms
```

## Wait, why is there a huuuge microcode table there?
Good question! The thing is, I actually plan on building it in real life using 74-series logic gates and some additional memory ICs on a PCB I haven't designed yet, I just don't know when am I finally going to do it...

## Can you give more information about the architecture?
For that, you can check the `instr.md` file, although it's not documented and it may be really hard to understand what's going on there. You also have a basic explanation of the registers in `sgcpu.h`.

## Contributing
Just 2 tiny notes:

1. Plz don't change the format ):
2. NO CLANG-FORMAT

### List of contributors:

- segfaultdev(original creator, segfaultdev@protonmail.com)
- Smart6502(serial input, INSERT ADDRESS HERE)
- aa2006(made old README.md, INSERT ADDRESS HERE)
- DamienFC(documentation, INSERT ADDRESS HERE)

## License?
Well, I'm using the MIT license, as this is just a tiny educative CPU for people to learn how real and modern CPUs work and maybe also how much I suck at coding? Also, I'll try to keep updating this project and, if you want to, you can also contribute and help me design it!