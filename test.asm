org 0x0000

start:
  lds 0x8000
  jsr print_str
echo_loop:
  ldx 0x4000
  ldl $x
  stl $x
  jmp echo_loop

print_str:
  ldx string
  ldy 0x4000
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