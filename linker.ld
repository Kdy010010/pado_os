ENTRY(p_kstart)

SECTIONS {
  . = 1M;
  .text   : { *(.text*) }
  .rodata : { *(.rodata*) }
  .data   : { *(.data*) }
  .bss    : { *(.bss COMMON*) }
}
