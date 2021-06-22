#ifndef __SGCPU_H__
#define __SGCPU_H__

#include <stdint.h>

#include "sgcpu_data.h"

// A: A accumulator(16-bit), for math 'n stuff
// B: B accumulator(16-bit), for MOAR math 'n stuff
// D: Data register(16-bit), for byte division things and memory loading?
// H: High half of data register(8-bit), cos byte division is noice
// L: Not-high half of data register(8-bit), cos byte division is still noice

// X: X index register(16-bit), for memory copying and pointer magic
// Y: Y index register(16-bit), for EVEN MOAR memory copying and pointer magic
// SP: Stack Pointer(16-bit), for ermm... stack
// IP: Instruction Pointer(16-bit), cos knowing where you are in memory is a good idea

// T: Temp. register(16-bit), used internally by microcode(specially for storing IP)
// R: Random and weird Temp. register(16-bit), used internally by microcode(specially for memory reads/writes)

typedef struct sg_regs_t sg_regs_t;
typedef struct sg_func_t sg_func_t;

struct sg_regs_t {
  uint16_t a, b, d;
  uint16_t x, y, sp, ip;
  uint16_t t, r;

  uint8_t curr, step;

  uint64_t sleep, total;
};

struct sg_func_t {
  void (*write)(uint16_t ptr, uint8_t val);
  uint8_t (*read)(uint16_t ptr);
};

static void sg_step(sg_func_t func, sg_regs_t *regs) {
  if (regs->step == 0)
    regs->curr = func.read(regs->ip);

  if (regs->curr >= SG_CNT)
    return;

  uint32_t code = sg_ucode_arr[(regs->curr << 3) | regs->step];
  regs->step = (regs->step + 1) & 7;

  if (!(code & 0xFFFF)) regs->sleep++;
  regs->total++;

  uint16_t bus = 0x0000;

  if ((code & SG_FLG_AXZ) && regs->a)
    return;
  if ((code & SG_FLG_BXZ) && regs->b)
    return;

  if ((code & SG_FLG_AXN) && !regs->a)
    return;
  if ((code & SG_FLG_BXN) && !regs->b)
    return;

  if ((code & SG_FLG_CRY) && regs->a >= regs->b)
    return;
  if ((code & SG_FLG_ZRO) && regs->a != regs->b)
    return;

  if ((code & SG_FLG_NCR) && regs->a < regs->b)
    return;
  if ((code & SG_FLG_NZR) && regs->a == regs->b)
    return;

  switch (code & SG_MSK_OUT) {
    case SG_AXO:
      bus |= regs->a;
      break;
    case SG_BXO:
      bus |= regs->b;
      break;
    case SG_DXO:
      bus |= regs->d;
      break;
    case SG_DHO:
      bus |= regs->d & 0xFF;
      break;
    case SG_DLO:
      bus |= (regs->d >> 8) & 0xFF;
      break;
    case SG_TXO:
      bus |= regs->t;
      break;
    case SG_THO:
      bus |= regs->t & 0xFF;
      break;
    case SG_TLO:
      bus |= (regs->t >> 8) & 0xFF;
      break;
    case SG_RXO:
      bus |= regs->r;
      break;
    case SG_RHO:
      bus |= regs->r & 0xFF;
      break;
    case SG_RLO:
      bus |= (regs->r >> 8) & 0xFF;
      break;
    case SG_XIO:
      bus |= regs->x;
      break;
    case SG_YIO:
      bus |= regs->y;
      break;
    case SG_SPO:
      bus |= regs->sp;
      break;
    case SG_IPO:
      bus |= regs->ip;
      break;
  }

  switch (code & SG_MSK_ALU) {
    case SG_ADD:
      bus |= regs->a + regs->b;
      break;
    case SG_SUB:
      bus |= regs->a - regs->b;
      break;
    case SG_AND:
      bus |= regs->a & regs->b;
      break;
    case SG_ORR:
      bus |= regs->a | regs->b;
      break;
    case SG_XOR:
      bus |= regs->a ^ regs->b;
      break;
    case SG_SHR:
      bus |= regs->a >> 1;
      break;
    case SG_NEG:
      bus |= regs->a ^ 0xFFFF;
      break;
  }

  if (code & SG_REA)
    bus |= func.read(regs->ip);

  switch (code & SG_MSK_INP) {
    case SG_AXI:
      regs->a = bus;
      break;
    case SG_BXI:
      regs->b = bus;
      break;
    case SG_DXI:
      regs->d = bus;
      break;
    case SG_DHI:
      regs->d = (regs->d & 0xFF) | (bus << 8);
      break;
    case SG_DLI:
      regs->d = (regs->d & 0xFF00) | (bus & 0xFF);
      break;
    case SG_TXI:
      regs->t = bus;
      break;
    case SG_THI:
      regs->t = (regs->t & 0xFF) | (bus << 8);
      break;
    case SG_TLI:
      regs->t = (regs->t & 0xFF00) | (bus & 0xFF);
      break;
    case SG_RXI:
      regs->r = bus;
      break;
    case SG_RHI:
      regs->r = (regs->r & 0xFF) | (bus << 8);
      break;
    case SG_RLI:
      regs->r = (regs->r & 0xFF00) | (bus & 0xFF);
      break;
    case SG_XII:
      regs->x = bus;
      break;
    case SG_YII:
      regs->y = bus;
      break;
    case SG_SPI:
      regs->sp = bus;
      break;
    case SG_IPI:
      regs->ip = bus;
      break;
  }

  if (code & SG_WRI)
    func.write(regs->ip, (uint8_t)(bus & 0xFF));

  switch (code & SG_MSK_INC) {
    case SG_DEX:
      regs->x--;
      break;
    case SG_DEY:
      regs->y--;
      break;
    case SG_DSP:
      regs->sp--;
      break;
    case SG_INX:
      regs->x++;
      break;
    case SG_INY:
      regs->y++;
      break;
    case SG_ISP:
      regs->sp++;
      break;
    case SG_IIP:
      regs->ip++;
      break;
  }
}

#endif