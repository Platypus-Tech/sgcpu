// seg's CPU toolkit, by (surprisingly) segfaultdev!

#include "sgcpu.h"
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

uint8_t *sgtk_mem;
struct termios t_orig, t_chan;

void enable_raw_mode() {
  tcgetattr(STDIN_FILENO, &t_orig);
  t_chan = t_orig;
  t_chan.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &t_chan);
}

void disable_raw_mode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &t_orig);
}

void handle_sint() {
  free(sgtk_mem);
  disable_raw_mode();
  exit(0);
}

void sgtk_write(uint16_t ptr, uint8_t val) {
  if (ptr >= 0x8000) {
    sgtk_mem[ptr & 0xFFFF] = val;
  } else if (ptr >= 0x4000) {
    putchar(val);
    fflush(stdout);
  }
}

uint8_t sgtk_read(uint16_t ptr) {
  // printf("[SGTK] Reading from address 0x%04X\n", ptr);
  if (ptr >= 0x4000 && ptr <= 0x7FFF) {
    char ret;
    read(STDIN_FILENO, &ret, 1);
    return ret;
  }

  return sgtk_mem[ptr & 0xFFFF];
}

int main(int argc, char const *argv[]) {
  if (argc < 2)
    return 1;

  sg_func_t func = (sg_func_t){
      sgtk_write,
      sgtk_read};

  sg_regs_t regs;
  regs.ip = 0, regs.step = 0, regs.sleep = 0, regs.total = 0;

  sgtk_mem = malloc(65536);

  uint16_t size = 0;

  if (argv[1][0] == 'a') {
    const char *src_path = NULL, *dst_path = NULL;

    for (int i = 2; i < argc; i++) {
      if (!strcmp(argv[i], "-o"))
        dst_path = argv[++i];
      else
        src_path = argv[i];
    }

    if (!src_path || !dst_path)
      return 1;

    FILE *src_file = fopen(src_path, "r");
    if (!src_file)
      return 1;

    FILE *dst_file = fopen(dst_path, "wb");
    if (!dst_file)
      return 1;

    fseek(src_file, 0, SEEK_END);
    size_t src_size = ftell(src_file);
    fseek(src_file, 0, SEEK_SET);

    char *src_buff = calloc(src_size + 2, 1);
    fread(src_buff, 1, src_size, src_file);

    src_buff[src_size] = ' ';

    sg_asm(src_buff, sgtk_mem, &size);
    fwrite(sgtk_mem, 1, size, dst_file);

    fclose(src_file);
    fclose(dst_file);
  } else if (argv[1][0] == 'r') {
    int debug_mode = 0;

    for (int i = 2; i < argc; i++) {
      if (!strcmp(argv[i], "-l")) {
        const char *rom_path = argv[++i];
        uint16_t rom_origin = strtol(argv[++i], NULL, 0);

        FILE *rom_file = fopen(rom_path, "r");
        if (!rom_file)
          return 1;

        fseek(rom_file, 0, SEEK_END);
        size_t rom_size = ftell(rom_file);
        fseek(rom_file, 0, SEEK_SET);

        fread(sgtk_mem + rom_origin, 1, rom_size, rom_file);
        fclose(rom_file);
      } else if (!strcmp(argv[i], "-d")) {
        debug_mode = 1;
      }
    }

    enable_raw_mode();
    signal(SIGINT, handle_sint);

    for (;;) {
      if (debug_mode) {
        if (!regs.step) {
          uint8_t curr = sgtk_read(regs.ip);

          uint16_t next = (uint16_t)(sgtk_read(regs.ip + 1)) | ((uint16_t)(sgtk_read(regs.ip + 2)) << 8);

          if (!sg_instr_arr[curr].arg) {
            printf("[SGTK] %04X: %02X     : %s\n", regs.ip, curr, sg_instr_arr[curr].name);
          } else if (!strcmp(sg_instr_arr[curr].arg, "$ptr")) {
            printf("[SGTK] %04X: %02X %04X: %s $0x%04X\n", regs.ip, curr, next, sg_instr_arr[curr].name, next);
          } else if (!strcmp(sg_instr_arr[curr].arg, "imm")) {
            printf("[SGTK] %04X: %02X %04X: %s 0x%04X\n", regs.ip, curr, next, sg_instr_arr[curr].name, next);
          } else {
            printf("[SGTK] %04X: %02X     : %s %s\n", regs.ip, curr, sg_instr_arr[curr].name, sg_instr_arr[curr].arg);
          }

          printf("[SGTK] A=0x%04X, B=0x%04X, D=0x%04X, X=0x%04X, Y=0x%04X, SP=0x%04X, IP=0x%04X\n", regs.a, regs.b, regs.d, regs.x, regs.y, regs.sp, regs.ip);
          printf("[SGTK] %llu/%llu cycles sleep (%f)\n\n", (long long unsigned)regs.sleep, (long long unsigned)regs.total, (float)(regs.sleep) / (float)(regs.total));
        }
      }

      sg_step(func, &regs);

      if (debug_mode)
        usleep(20000);
    }
  }

  free(sgtk_mem);

  return 0;
}
