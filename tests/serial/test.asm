org 0x0000

start:
  lds 0x5000
  ldx string
  jsr print_str
inf_loop:
  jmp inf_loop

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
  str "Hello, world!\n"