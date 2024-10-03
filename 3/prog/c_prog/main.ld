/* Script for -z combreloc */
/* Copyright (C) 2014-2023 Free Software Foundation, Inc.
   Copying and distribution of this script, with or without modification,
   are permitted in any medium without royalty provided the copyright
   notice and this notice are preserved.  */
OUTPUT_FORMAT("elf32-littleriscv")
OUTPUT_ARCH(riscv)

/* need to control RAM configuration */
MEMORY
{
  itcm(rwx)  : ORIGIN = 0x4000, LENGTH = 0x1000
  dtcm(rwx)  : ORIGIN = 0x8000, LENGTH = 0x1000
}

ENTRY(_start)
SEARCH_DIR("/opt/riscv/riscv64-unknown-elf/lib");
SECTIONS
{
  /* variables accessd from code */
  PROVIDE (__stack_top = ORIGIN(dtcm) + LENGTH(dtcm));
  PROVIDE (__uart_base_addr = 0x2000);

  .init   : { KEEP (*(SORT_NONE(.init)))}
  .text   : {*(.text)}
  .rodata : { *(.rodata) }

  /* Adjust the address for the data segment.  We want to adjust up to
     the same address within the page on the next page up.  */
  . = DATA_SEGMENT_ALIGN (CONSTANT (MAXPAGESIZE), CONSTANT (COMMONPAGESIZE));

  __DATA_BEGIN__ = .;
  .data   : { *(.data) }

  __bss_start = .;
  .bss    : { *(.bss) }
  __BSS_END__ = .;

  /* need for global pointer relaxations */
  __global_pointer$ = MAX(__DATA_BEGIN__ + 0x800, __BSS_END__ - 0x800);

  . = DATA_SEGMENT_END (.);
}
