ENTRY(reset_handler)

MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 64K
    RAM  (xrw) : ORIGIN = 0x20000000, LENGTH = 8K
}

/* stack */
_estack = 0x20002000;

SECTIONS
{
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))    /* Startup code */
        . = ALIGN(4);
    } >FLASH

    .text :
    {
        . = ALIGN(4);
 
        *(.text .text.*)
        /**(.rodata)
        *(.rodata*)*/

        KEEP (*(.init))
        KEEP (*(.fini))

        . = ALIGN(4);
        _etext = .;
        _sidata = _etext;
    } >FLASH
/*
	.rodata : AT ( _etext )
	{
		. = ALIGN(4);

		*(.rodata)
		*(.rodata*)

		. = ALIGN(4);
		_erodata = . ;
        _sidata = _erodata;
	} >RAM
*/
    .data  : AT ( _sidata )
    {
        . = ALIGN(4);
        _sdata = . ;
        _data = . ;

        *(.data)
        *(.data.*)
        *(.rodata)
        *(.rodata*)

        . = ALIGN(4);
        _edata = . ;
    } >RAM

    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        _bss = .;

        *(.bss)
        *(.bss.*)
        *(COMMON)

        . = ALIGN(4);
        _ebss = . ;
    } >RAM

    PROVIDE ( end = _ebss );
    PROVIDE ( _end = _ebss );

/*    __exidx_start = .;
    __exidx_end = .;*/

}
