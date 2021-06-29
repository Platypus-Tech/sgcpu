org 0x0000

start:
  lds 0x5000
  lda 0x0000
scr_preloop:
  ldx 0x6000
  ldb 0x1770
scr_loop:
  jbz scr_end
  sbb 0x0001
  mdb
  ada d
  tad
  stl $x
  tad
  inx
  jmp scr_loop
scr_end:
  ada 0x0001
  jmp scr_preloop