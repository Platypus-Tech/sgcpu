org 0x0000

start:
  lds 0x8000
  ldx 0x4001
  ldl $x
  jsr print_hex
  ldx string
  jsr print_str
  hlt

print_hex:
  ldx 0x4002
  ldh 0x00
  mad
  ana 0x000F
  ldb 0x000A
  jls low_dec
  ada 0x0007
low_dec:
  ada 0x0030
  tad
  sla
  sla
  sla
  sla
  ana 0x0F00
  ldb 0x0A00
  jls high_dec
  ada 0x0700
high_dec:
  ada 0x3000
  ora d
  mda
  sth $x
  stl $x
  ret

print_str:
  ldy 0x4002
  ldh 0x00
str_loop:
  ldl $x
  mad
  jaz str_end
  stl $y
  inx
  jmp str_loop
str_end:
  ret

string:
  str "*32K RAM found\n"