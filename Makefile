# S: Source directory
# D: Destination directory

# First, export the source and destination directories
# export S=path/to/source
# export D=path/to/destination

S ?= ./
D ?= $(S)

# Compilator and options compilation for ARM Cortex-M4
ARM_CC = arm-uclinuxeabi-gcc
ARM_CFLAGS = -march=armv7-m -mtune=cortex-m4 -mlittle-endian -mthumb -Os -ffast-math \
             -ffunction-sections -fdata-sections -Wl,--gc-sections -fno-common \
             --param max-inline-insns-single=1000 -Wl,-elf2flt=-s -Wl,-elf2flt=16384 

# Linker options
LDFLAGS = 

# Conditional macro definitions
DEFINES = # -DWITH_TEXT

CFLAGS += $(DEFINES)
ARM_CFLAGS += $(DEFINES)


SRCS = $(wildcard $(S)/*.c)
TARGETS = $(patsubst $(S)/%.c,$(S)/%,$(SRCS))
TARGETS_EMBEDDED = $(patsubst $(S)/%.c,$(D)/_embedded/%,$(SRCS))

all : $(TARGETS) $(TARGETS_EMBEDDED)

$(S)/% : $(S)/%.c
	@echo "Compiling for PC: $< -> $@"
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(D)/_embedded/% : $(S)/%.c
	@echo "Compiling for embedded: $< -> $@"
	@mkdir -p $(D)/_embedded
	$(ARM_CC) $(ARM_CFLAGS) -o $@ $< $(LDFLAGS)

clean :
	@echo "Cleaning generated binaries: $(TARGETS) $(TARGETS_EMBEDDED)"
	@rm -f $(TARGETS) $(TARGETS_EMBEDDED)
	@echo "Cleaning generated directory: $(D)/_embedded"
	@rm -rf $(D)/_embedded
