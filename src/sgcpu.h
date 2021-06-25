#ifndef __SGCPU_H__
#define __SGCPU_H__

#include <stdint.h>

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

// Nothing!
#define SG_NOP (uint32_t)(0b000000000000000000000000)

// Conditionals(not part of the microcode itself, just for generation)
#define SG_FLG_AXZ (uint32_t)(0b100000000000000000000000)
#define SG_FLG_BXZ (uint32_t)(0b010000000000000000000000)
#define SG_FLG_AXN (uint32_t)(0b001000000000000000000000)
#define SG_FLG_BXN (uint32_t)(0b000100000000000000000000)
#define SG_FLG_CRY (uint32_t)(0b000010000000000000000000)
#define SG_FLG_ZRO (uint32_t)(0b000001000000000000000000)
#define SG_FLG_NCR (uint32_t)(0b000000100000000000000000)
#define SG_FLG_NZR (uint32_t)(0b000000010000000000000000)

// Affect on rising edge
#define SG_AXO (uint32_t)(0b000000000001000000000000)
#define SG_BXO (uint32_t)(0b000000000010000000000000)
#define SG_DXO (uint32_t)(0b000000000011000000000000)
#define SG_DHO (uint32_t)(0b000000000100000000000000)
#define SG_TXO (uint32_t)(0b000000000101000000000000)
#define SG_RXO (uint32_t)(0b000000000110000000000000)
#define SG_RHO (uint32_t)(0b000000000111000000000000)
#define SG_XIO (uint32_t)(0b000000001000000000000000)
#define SG_YIO (uint32_t)(0b000000001001000000000000)
#define SG_SPO (uint32_t)(0b000000001010000000000000)
#define SG_IPO (uint32_t)(0b000000001011000000000000)
#define SG_SW1 (uint32_t)(0b000000001100000000000000)
#define SG_SW2 (uint32_t)(0b000000001101000000000000)
#define SG_SW3 (uint32_t)(0b000000001110000000000000)
#define SG_SW4 (uint32_t)(0b000000001111000000000000)

// Affect on rising edge
#define SG_AXI (uint32_t)(0b000000000000000100000000)
#define SG_BXI (uint32_t)(0b000000000000001000000000)
#define SG_DXI (uint32_t)(0b000000000000001100000000)
#define SG_DHI (uint32_t)(0b000000000000010000000000)
#define SG_DLI (uint32_t)(0b000000000000010100000000)
#define SG_TXI (uint32_t)(0b000000000000011000000000)
#define SG_THI (uint32_t)(0b000000000000011100000000)
#define SG_TLI (uint32_t)(0b000000000000100000000000)
#define SG_RXI (uint32_t)(0b000000000000100100000000)
#define SG_RHI (uint32_t)(0b000000000000101000000000)
#define SG_RLI (uint32_t)(0b000000000000101100000000)
#define SG_XII (uint32_t)(0b000000000000110000000000)
#define SG_YII (uint32_t)(0b000000000000110100000000)
#define SG_SPI (uint32_t)(0b000000000000111000000000)
#define SG_IPI (uint32_t)(0b000000000000111100000000)

// Affect on rising edge
#define SG_WRI (uint32_t)(0b000000000000000010000000)
#define SG_REA (uint32_t)(0b000000000000000001000000)

// Affect on rising edge
#define SG_ADD (uint32_t)(0b000000000000000000001000)
#define SG_SUB (uint32_t)(0b000000000000000000010000)
#define SG_AND (uint32_t)(0b000000000000000000011000)
#define SG_ORR (uint32_t)(0b000000000000000000100000)
#define SG_XOR (uint32_t)(0b000000000000000000101000)
#define SG_SHR (uint32_t)(0b000000000000000000110000)
#define SG_NEG (uint32_t)(0b000000000000000000111000)

// Affect on falling edge
#define SG_DEX (uint32_t)(0b000000000000000000000001)
#define SG_DEY (uint32_t)(0b000000000000000000000010)
#define SG_DSP (uint32_t)(0b000000000000000000000011)
#define SG_INX (uint32_t)(0b000000000000000000000100)
#define SG_INY (uint32_t)(0b000000000000000000000101)
#define SG_ISP (uint32_t)(0b000000000000000000000110)
#define SG_IIP (uint32_t)(0b000000000000000000000111)

// Masks
#define SG_MSK_OUT (uint32_t)(0b000000001111000000000000)
#define SG_MSK_INP (uint32_t)(0b000000000000111100000000)
#define SG_MSK_ALU (uint32_t)(0b000000000000000000111000)
#define SG_MSK_INC (uint32_t)(0b000000000000000000000111)

// Instruction count
#define SG_CNT 0xC0

typedef struct sg_regs_t sg_regs_t;
typedef struct sg_func_t sg_func_t;

typedef struct sg_instr_t sg_instr_t;
typedef struct sg_label_t sg_label_t;

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

struct sg_instr_t {
  const char *name;
  const char *arg;
  int chg_data, chg_code;
};

struct sg_label_t {
  char name[16];
  uint16_t ptr;
};

extern const uint32_t sg_ucode_arr[];
extern const sg_instr_t sg_instr_arr[];

void sg_asm(const char *code, uint8_t *buffer, uint16_t *size);
void sg_step(sg_func_t func, sg_regs_t *regs);

#endif
