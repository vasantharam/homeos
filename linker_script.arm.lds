OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(main)
SECTIONS
{
  . = 0x10000;
  . = ALIGN(4);
  _text = .;			/* Text and read-only data */
  .text : {
    start.o(.text) 
	*(.text)
    *(.fixup)
    *(.gnu.warning)
	}
  .text.lock : { *(.text.lock) }
  .rodata : { *(.rodata) }
  .kstrtab :  { *(.kstrtab) }
  . = ALIGN(4);
  _etext = .;

  .data : {			/* Data */
	*(.data)
	}
  _edata = .;
  . = ALIGN(4);

  . = ALIGN(8192);
  .data.init_task : { *(.data.init_task) }

  . = ALIGN(4096);
  __init_begin = .;
  .text.init : { *(.text.init) }
  .data.init : { *(.data.init) }

  . = ALIGN(4096);
  __init_end = .;

  . = ALIGN(4);
  __bss_start = .;
  .bss (NOLOAD) : {
	*(.bss)
	}
  _end = . ;

}
