|    //    |    x0    |    x1    |    x2    |    x3    |    x4    |    x5    |    x6    |    x7    |    x8    |    x9    |    xA    |    xB    |    xC    |    xD    |    xE    |    xF    |
|    0x    | LDA $X   | LDB $X   | LDD $X   | LDX $X   | LDY $X   | LDH $X   | LDL $X   | LDS $X   | STA $X   | STB $X   | STD $X   | STX $X   | STY $X   | STH $X   | STL $X   | STZ $X   |
|    1x    | LDA $Y   | LDB $Y   | LDD $Y   | LDX $Y   | LDY $Y   | LDH $Y   | LDL $Y   | LDS $Y   | STA $Y   | STB $Y   | STD $Y   | STX $Y   | STY $Y   | STH $Y   | STL $Y   | STZ $Y   |
|    2x    | LDA imm  | LDB imm  | LDD imm  | LDX imm  | LDY imm  | LDH imm  | LDL imm  | LDS imm  | SRA      | SRB      | SLA      | SLB      | NGA      | NGB      | INX      | INY      |
|    3x    | ADA B    | ADB A    | SBA B    | SBB A    | ANA B    | ANB A    | ORA B    | ORB A    | EXA B    | EXB A    | PSH A    | PSH B    | PSH D    | PSH X    | PSH Y    | PSH imm  |
|    4x    | ADA D    | ADB D    | SBA D    | SBB D    | ANA D    | ANB D    | ORA D    | ORB D    | EXA D    | EXB D    | POP A    | POP B    | POP D    | POP X    | POP Y    | RET      |
|    5x    | ADA X    | ADB X    | SBA X    | SBB X    | ANA X    | ANB X    | ORA X    | ORB X    | EXA X    | EXB X    | JMP X    | JMP Y    | JMP imm  | JSR X    | JSR Y    | JSR imm  |
|    6x    | ADA Y    | ADB Y    | SBA Y    | SBB Y    | ANA Y    | ANB Y    | ORA Y    | ORB Y    | EXA Y    | EXB Y    | JAZ X    | JAZ Y    | JAZ imm  | JBZ X    | JBZ Y    | JBZ imm  |
|    7x    | ADA imm  | ADB imm  | SBA imm  | SBB imm  | ANA imm  | ANB imm  | ORA imm  | ORB imm  | EXA imm  | EXB imm  | JAN X    | JAN Y    | JAN imm  | JBN X    | JBN Y    | JBN imm  |
|    8x    | JEQ X    | JEQ Y    | JEQ imm  | JNE X    | JNE Y    | JNE imm  | MSX      | MSY      | MXS      | MYS      | JGT X    | JGT Y    | JGT imm  | JLS X    | JLS Y    | JLS imm  |
|    9x    | JCR X    | JCR Y    | JCR imm  | JNC X    | JNC Y    | JNC imm  | SW1      | SW2      | SW3      | SW4      | NOP      | SET      | MOV      | MHL      | MLH      | THL      |
|    Ax    | MAB      | MAD      | MAX      | MAY      | MBA      | MBD      | MBX      | MBY      | MDA      | MDB      | MDX      | MDY      | MXA      | MXB      | MXD      | MXY      |
|    Bx    | MYA      | MYB      | MYD      | MYX      | TAB      | TAD      | TAX      | TAY      | TBD      | TBX      | TBY      | TDX      | TDY      | TXY      | DEX      | DEY      |
|    Cx    |          |          |          |          |          |          |          |          |          |          |          |          |          |          |          |          |
|    Dx    |          |          |          |          |          |          |          |          |          |          |          |          |          |          |          |          |
|    Ex    |          |          |          |          |          |          |          |          |          |          |          |          |          |          |          |          |
|    Fx    |          |          |          |          |          |          |          |          |          |          |          |          |          |          |          |          |

HLT: Doesn't increment IP -> Halts CPU

JSR: Pushes IP AFTER increment

M--/T--: First goes the destination, then the source

$-: Value at address

0x0000 - 0x3FFF: OS ROM
0x4000 - 0x4FFF: Devices:
  - 0x4000: Current RAM bank(read and write)
  - 0x4001: Total number of banks(read only)
  - 0x4002: Serial port(read and write)
0x5000 - 0x5FFF: Fixed RAM
0x6000 - 0x7FFF: Video RAM(240x150, 4 colors + black)
0x8000 - 0xFFFF: Banked RAM