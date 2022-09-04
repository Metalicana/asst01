CC=arm-none-eabi-gcc
ARCH=cortex-m4
TARGET = target
OBJECT = object
MAPDIR = mapfiles
FPU = -mfloat-abi=softfp -mfpu=fpv4-sp-d16
CFLAGS=-c -mcpu=$(ARCH) -mthumb $(FPU) -Wall -std=gnu11 -O0
LDFLAGS= -nostdlib -nostartfiles -nodefaultlibs -fno-exceptions -mcpu=$(ARCH) -mthumb $(FPU) -T stm32_ls.ld -Wl,-Map=$(MAPDIR)/asst01.map -O0

STDIODIR = ../kstdio
SRC_STDIO = $(wildcard $(STDIODIR)/*.c)
STDIO_OBJECTS = $(patsubst $(STDIODIR)/%.c, $(OBJECT)/%.o,$(SRC_STDIO))

MAINDIR = ../kmain
SRC_MAIN = $(wildcard $(MAINDIR)/*.c)
MAIN_OBJECTS = $(patsubst $(MAINDIR)/%.c, $(OBJECT)/%.o,$(SRC_MAIN))

DEVDIR = ../arch/stm32f446re/dev
SRC_DEV = $(wildcard $(DEVDIR)/*.c)
DEV_OBJECTS = $(patsubst $(DEVDIR)/%.c, $(OBJECT)/%.o,$(SRC_DEV))

SYSDIR = ../arch/stm32f446re/sys
SRC_SYS = $(wildcard $(SYSDIR)/*.c)
SYS_OBJECTS = $(patsubst $(SYSDIR)/%.c, $(OBJECT)/%.o,$(SRC_SYS))

SYSCALLSDIR = ../syscalls
SRC_SYSCALLS = $(wildcard $(SYSCALLSDIR)/*.c)
SYSCALLS_OBJECTS = $(patsubst $(SYSCALLSDIR)/%.c, $(OBJECT)/%.o,$(SRC_SYSCALLS))

all: asst01.elf

# kstdio set
$(STDIO_OBJECTS): $(OBJECT)/%.o: $(STDIODIR)/%.c
	$(info in kstdio set)
	$(CC) $(CFLAGS) -o $@ $^

# kmain set
$(MAIN_OBJECTS): $(OBJECT)/%.o: $(MAINDIR)/%.c
	$(info in kmain set)
	$(CC) $(CFLAGS) -o $@ $^

# arch/board/dev set
$(DEV_OBJECTS): $(OBJECT)/%.o: $(DEVDIR)/%.c
	$(info in arch/board/dev set $(SRC_DEV))
	$(CC) $(CFLAGS) -o $@ $^

# arch/board/sys set
$(SYS_OBJECTS): $(OBJECT)/%.o: $(SYSDIR)/%.c
	$(info in arch/board/sys set)
	$(CC) $(CFLAGS) -o $@ $^

# arch/board/syscalls set
$(SYSCALLS_OBJECTS): $(OBJECT)/%.o: $(SYSCALLSDIR)/%.c
	$(info in syscalls set)
	$(CC) $(CFLAGS) -o $@ $^
	
asst01.elf: $(STDIO_OBJECTS) $(MAIN_OBJECTS) $(DEV_OBJECTS) $(SYS_OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $(TARGET)/$@

clean:
	rm -rf $(OBJECT)/*.o $(MAPDIR)/*.map $(TARGET)/* *.o *.elf

load:
	openocd -f board/stm32f4discovery.cfg

debug:
	arm-none-eabi-gdb

flash:
	monitor flash write_image erase target/asst01.elf