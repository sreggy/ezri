/* Linker Script for rboot */

MEMORY
{
  dport0_0_seg :                        org = 0x3FF00000, len = 0x10
  dram0_0_seg :                         org = 0x3FFE8000, len = 0x14000
  iram1_0_seg :                         org = 0x40100000, len = 0x8000
  irom0_0_seg :                         org = 0x40200010 + (0x00002000 & 0xFFFFF), len = 1M - (0x00002000 & 0xFFFFF) - 0x10
}

INCLUDE "common.ld"
