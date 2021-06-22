// seg's CPU toolkit, by (surprisingly) segfaultdev!

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "sgcpu.h"

uint8_t *sgtk_mem;

void sgtk_write(uint16_t ptr, uint8_t val) {
  // printf("[SGTK] Written 0x%02X to address 0x%04X\n", val, ptr);
  if (ptr >= 0x8000) sgtk_mem[ptr & 0xFFFF] = val;
}

uint8_t sgtk_read(uint16_t ptr) {
  // printf("[SGTK] Reading from address 0x%04X\n", ptr);
  return sgtk_mem[ptr & 0xFFFF];
}

int main(int argc, char const *argv[]) {
  sg_func_t func = (sg_func_t){
    sgtk_write,
    sgtk_read
  };

  sg_regs_t regs;
  regs.ip = 0, regs.step = 0, regs.sleep = 0, regs.total = 0;

  sgtk_mem = malloc(65536);

  for (;;) {
    if (!regs.step) {
      uint8_t curr = sgtk_read(regs.ip);

      uint16_t next = (uint16_t)(sgtk_read(regs.ip + 1)) | ((uint16_t)(sgtk_read(regs.ip + 2)) << 8);

      if (!sg_instr_arr[curr].arg) {
        printf("[SGTK] %02X     : %s\n", curr, sg_instr_arr[curr].name);
      } else if (!strcmp(sg_instr_arr[curr].arg, "$ptr")) {
        printf("[SGTK] %02X %04X: %s $0x%04X\n", curr, next, sg_instr_arr[curr].name, next);
      } else if (!strcmp(sg_instr_arr[curr].arg, "imm")) {
        printf("[SGTK] %02X %04X: %s 0x%04X\n", curr, next, sg_instr_arr[curr].name, next);
      } else {
        printf("[SGTK] %02X     : %s %s\n", curr, sg_instr_arr[curr].name, sg_instr_arr[curr].arg);
      }

      // printf("[SGTK] A=0x%04X, B=0x%04X, D=0x%04X, X=0x%04X, Y=0x%04X, SP=0x%04X, IP=0x%04X\n", regs.a, regs.b, regs.d, regs.x, regs.y, regs.sp, regs.ip);
      // printf("[SGTK] %llu/%llu cycles sleep (%f)\n\n", regs.sleep, regs.total, (float)(regs.sleep) / (float)(regs.total));
    }

    sg_step(func, &regs);

    // usleep(10000);
  }

  free(sgtk_mem);

  return 0;
}