#include "sgcpu.h"

#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void sg_asm(const char *code, uint8_t *buffer, uint16_t *size) {
  *size = 0;
  if (!code || !buffer)
    return;

  const char *old_code = code;

  uint16_t org = 0x0000;

  sg_label_t labels[128];
  int label_cnt = 0;

  char prev[200] = {0}, curr[200] = {0};
  int length = 0;

  char *end_ptr;
  uint16_t num;

  int in_string = 0;

  while (*code) {
    if (*code == '#') {
      while (*code && *code != '\n') code++;
    }
    
    if (*code == '"') {
      in_string = !in_string;
    } else if (!in_string) {
      if (isspace(*code)) {
        if (length) {
          if (!strcmp(prev, "ORG")) {
            num = (uint16_t)(strtol(curr, &end_ptr, 0));
            if (end_ptr - curr < length)
              continue;

            org = num;
          } else if (!strcmp(prev, "ALIGN")) {
            num = (uint16_t)(strtol(curr, &end_ptr, 0));
            if (end_ptr - curr < length)
              continue;

            if (*size % num) {
              num = num - (*size % num);

              *size += num;
              org += num;
            }
          } else if (!strcmp(prev, "DB")) {
            org++, (*size)++;
          } else if (!strcmp(prev, "DW")) {
            org += 2, *size += 2;
          } else if (!strcmp(prev, "STR")) {
            org += length + 1, *size += length + 1;
          } else if (curr[length - 1] == ':') {
            curr[length - 1] = '\0';

            strcpy(labels[label_cnt].name, curr);
            labels[label_cnt++].ptr = org;
          } else {
            for (int i = 0; i < SG_CNT; i++) {
              if (sg_instr_arr[i].arg) {
                if (strcmp(sg_instr_arr[i].name, prev))
                  continue;

                if (!strcmp(sg_instr_arr[i].arg, "imm")) {
                  org += 3, *size += 3;
                  break;
                } else {
                  if (strcmp(sg_instr_arr[i].arg, curr))
                    continue;

                  org++, (*size)++;
                  break;
                }
              } else {
                if (strcmp(sg_instr_arr[i].name, curr))
                  continue;

                org++, (*size)++;
                break;
              }
            }
          }

          memcpy(prev, curr, 200);
        }

        length = 0;
      } else {
        curr[length++] = toupper(*code);
      }
    } else if (*code == '\\') {
      code++;

      if (*code == 'n')
        curr[length++] = '\n';
    } else {
      curr[length++] = *code;
    }

    curr[length] = '\0';
    code++;
  }

  memset(prev, 0x00, 200);
  memset(curr, 0x00, 200);

  code = old_code;
  length = 0;
  *size = 0;
  in_string = 0;

  while (*code) {
    if (*code == '"') {
      in_string = !in_string;
    } else if (!in_string) {
      if (isspace(*code)) {
        if (length) {
          if (!strcmp(prev, "ALIGN")) {
            num = (uint16_t)(strtol(curr, &end_ptr, 0));
            if (end_ptr - curr < length)
              continue;

            if (*size % num) {
              num = num - (*size % num);
              *size += num;
            }
          } else if (!strcmp(prev, "DB")) {
            num = (uint16_t)(strtol(curr, &end_ptr, 0));
            if (end_ptr - curr < length)
              continue;

            buffer[(*size)++] = (uint8_t)(num);
          } else if (!strcmp(prev, "DW")) {
            num = (uint16_t)(strtol(curr, &end_ptr, 0));
            if (end_ptr - curr < length)
              continue;

            buffer[(*size)++] = (uint8_t)(num >> 0);
            buffer[(*size)++] = (uint8_t)(num >> 8);
          } else if (!strcmp(prev, "STR")) {
            for (int i = 0; i <= length; i++)
              buffer[(*size)++] = curr[i];
          } else {
            for (int i = 0; i < SG_CNT; i++) {
              if (sg_instr_arr[i].arg) {
                if (strcmp(sg_instr_arr[i].name, prev))
                  continue;

                if (!strcmp(sg_instr_arr[i].arg, "imm")) {
                  num = (uint16_t)(strtol(curr, &end_ptr, 0));

                  if (end_ptr - curr < length) {
                    for (int j = 0; j < label_cnt; j++) {
                      if (!strcmp(curr, labels[j].name)) {
                        num = labels[j].ptr;

                        goto yay_found_a_label;
                      }
                    }

                    continue;
                  }

                yay_found_a_label:
                  buffer[(*size)++] = (uint8_t)(i);
                  buffer[(*size)++] = (uint8_t)(num >> 0);
                  buffer[(*size)++] = (uint8_t)(num >> 8);

                  break;
                } else {
                  if (strcmp(sg_instr_arr[i].arg, curr))
                    continue;
                  buffer[(*size)++] = (uint8_t)(i);

                  break;
                }
              } else {
                if (strcmp(sg_instr_arr[i].name, curr))
                  continue;
                buffer[(*size)++] = (uint8_t)(i);

                break;
              }
            }
          }

          memcpy(prev, curr, 200);
        }

        length = 0;
      } else {
        curr[length++] = toupper(*code);
      }
    } else if (*code == '\\') {
      code++;

      if (*code == 'n')
        curr[length++] = '\n';
    } else {
      curr[length++] = *code;
    }

    curr[length] = '\0';
    code++;
  }
}

void sg_step(sg_func_t func, sg_regs_t *regs) {
  if (regs->step == 0)
    regs->curr = func.read(regs->ip);

  if (regs->curr >= SG_CNT)
    return;

  uint32_t code = sg_ucode_arr[(regs->curr << 3) | regs->step];
  regs->step = (regs->step + 1) & 7;

  if (!(code & 0xFFFF))
    regs->sleep++;
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
    bus |= (regs->d >> 8) & 0xFF;
    break;
  case SG_TXO:
    bus |= regs->t;
    break;
  case SG_RXO:
    bus |= regs->r;
    break;
  case SG_RHO:
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
  case SG_SW1:
    bus |= 0xFFF8;
    break;
  case SG_SW2:
    bus |= 0xFFFA;
    break;
  case SG_SW3:
    bus |= 0xFFFC;
    break;
  case SG_SW4:
    bus |= 0xFFFE;
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