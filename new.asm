org 0x0000

start:
  lds 0x8000
  lda 0x0000 # hey this is a noice comment
scr_preloop:
  ldx 0x6000
  ldb 0x1770
  mda
  set
  ada 0x0001
  jmp scr_preloop
inf_loop:
  jmp inf_loop