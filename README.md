# sgcpu
## seg's WONDELFUL, INCREDIBLE and POWERFUL 16-bit CPU (: 
Not really ):

This is just a tiny emulated ISA and CPU with an assembler, a simple debugger with disassembly and a test program. This was made really quickly (originally) in the span of 4 days, and it's by no means perfect, with a lot of ugly code, random microcode typos and errors that then cause huge errors when running test programs, etc.

## Wait, why is there a huuuge microcode table there?
Good question! The thing is, I actually plan on building it in real life using 74-series logic gates and some additional memory ICs on a PCB I haven't designed yet, I just don't know when am I finally going to do it...

## Can you give more information about the architecture?
For that, you can check the `instr.md` file, although it's not documented and it may be really hard to understand what's going on there. You also have a basic explanation of the registers in `sgcpu.h`.

## License?
Well, I'm using the MIT license, as this is just a tiny educative CPU for people to learn how real and modern CPUs work and maybe also how much I suck at coding? Also, I'll try to keep updating this project and, if you want to, you can also contribute and help me design it!