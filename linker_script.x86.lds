OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(main)
SECTIONS
{
  . = 0x1400;
  _text = .;			/* Text and read-only data */
  .text : {
    example/cmain.o(.text) 
	*(.text)
	}
  .rodata : { *(.rodata) }

  .data : {			/* Data */
	*(.data)
	}

  __bss_start = .;
  .bss (NOLOAD): {
	*(.bss)
	}
  _end = . ;

}
