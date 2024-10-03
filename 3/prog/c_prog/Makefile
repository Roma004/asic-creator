RISCV_CROSS_BIN = /home/roma004/work/x-tools/riscv32-unknown-elf/bin

CC = $(RISCV_CROSS_BIN)/riscv32-unknown-elf-gcc
GDB = $(RISCV_CROSS_BIN)/riscv32-unknown-elf-gdb
OBJDUMP = $(RISCV_CROSS_BIN)/riscv32-unknown-elf-objdump
OBJCOPY = $(RISCV_CROSS_BIN)/riscv32-unknown-elf-objcopy

BUILD_DIR = ./build

ELF = $(BUILD_DIR)/test.elf
ELF_DUMP = $(BUILD_DIR)/test_dump.elf
BIN = $(BUILD_DIR)/test.bin

LD_FILE = main.ld
ASM_FILES = crt0.s
C_FILES = main.c

C_FALGS = -ffreestanding -nostartfiles -mcmodel=medany -march=rv32i -mabi=ilp32

LD_FLAGS = -Wl,-T,$(LD_FILE) -Wl,--gc-sections


.PHONY: build
build:
	@mkdir -p $(BUILD_DIR)
	$(CC) $(C_FALGS) $(LD_FLAGS) $(ASM_FILES) $(C_FILES) -o $(ELF)
	$(OBJDUMP) -d $(ELF) > $(ELF_DUMP)
	$(OBJCOPY) -R .got -R .got.plt -O binary $(ELF) $(BIN)

