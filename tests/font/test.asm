org 0x0000

start:
  lds 0x5000
  lda 0x0080
  ldx 0x0000
  ldd 0x0020
line_loop:
  ldb 0x0028
single_loop:
  tbd
  jeq halt
  tbd
  jbz single_end
  jsr print_chr
  inx
  tdx
  inx
  tdx
  sbb 0x0001
  jmp single_loop
single_end:
  tax
  ada 0x00C8
  tax
  jmp line_loop
halt:
  jmp halt

print_chr:
  psh a
  psh b
  psh d
  psh x
  psh y
  tax
  ada 0x6000
  tax
  tad
  sba 0x0020
  tad
  ldy font_arr
  tay
  ada d
  tay
  ldb 0x0006
chr_loop:
  jbz chr_end
  ldh $y
  sth $x
  tax
  ada 0x0028
  tax
  tay
  ada 0x0060
  tay
  sbb 0x0001
  jmp chr_loop
chr_end:
  pop y
  pop x
  pop d
  pop b
  pop a
  ret

include "font.inc"