;----------  loader and kernel  ----------
LOADER_START_SECTOR equ 0x2
LOADER_BASE_ADDR equ 0x900
LOADER_MAGIC_ADDR equ 0x00
LOADER_MAGIC equ 0x1106
LOADER_SECTOR_NUM_ADDR equ 0x02
LOADER_ENTRY_ADDR equ 0x04

PAGE_DIR_TABLE_PADDR equ 0x0009_0000

KERNEL_START_SECTOR equ 0x09
KERNEL_BIN_TOTAL_SECTOR equ 200
KERNEL_BIN_BASE_PADDR equ 0x70000

KERNEL_ENTRY_POINT equ 0xc000_1500


;----------  GDT descriptor attributes  ----------
DESC_G_4K equ (1b<<23)
DESC_D_32 equ (1<<22)
DESC_L_NOT_64 equ (0b<<21)
DESC_AVL equ (0b<<20)

DESC_P equ (1b<<15)

DESC_DPL_0 equ (00b<<13)
DESC_DPL_1 equ (01b<<13)
DESC_DPL_2 equ (10b<<13)
DESC_DPL_3 equ (11b<<13)

DESC_S_CODE equ (1b<<12)
DESC_S_DATA equ DESC_S_CODE
DESC_S_SYS equ (0b<<12)

DESC_TYPE_CODE equ (1000b<<8)
DESC_TYPE_DATA equ (0010b<<8)

DESC_CODE_HIGH4 equ (0x00<<24) | DESC_G_4K | DESC_D_32 | \
                    DESC_L_NOT_64 | DESC_AVL | (1111b<<16) | \
                    DESC_P | DESC_DPL_0 | DESC_S_CODE | \
                    DESC_TYPE_CODE | 0x00

DESC_DATA_HIGH4 equ (0x00<<24) | DESC_G_4K | DESC_D_32 | \
                    DESC_L_NOT_64 | DESC_AVL | (1111b<<16) | \
		    DESC_P | DESC_DPL_0 | DESC_S_DATA | \
		    DESC_TYPE_DATA | 0x00

DESC_VIDEO_HIGH4 equ (0x00<<24) | DESC_G_4K | DESC_D_32 | \
                    DESC_L_NOT_64 | DESC_AVL | (0000b<<16) | \
		    DESC_P | DESC_DPL_0 | DESC_S_DATA | \
		    DESC_TYPE_DATA | 0x0b

;----------  GDT selector attributes ----------
RPL0 equ 00b
RPL1 equ 01b
RPL2 equ 10b
RPL3 equ 11b

TI_GDT equ (0b<<2)
TI_LDT equ (1b<<2)

;----------  virtual page attributes ----------
PG_P equ 1b
PG_RDWR equ 10b
PG_RDOY equ 00b
PG_S equ 000b
PG_U equ 100b

;----------  ELF32  ----------

e_phentsize equ 42
e_phoff equ 28
e_phnum equ 44

p_type equ 0
p_offset equ 4
p_vaddr equ 8
p_filesz equ 16

PT_NULL equ 0
