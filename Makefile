SRCDIR = src
BUILDDIR = build
TARGET = hshb-nametag
MCU = atmega32
F_CPU = 8000000
FORMAT = ihex
OPT = s
CSTANDARD = gnu99
SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(patsubst %.c,%.o,$(subst $(SRCDIR),$(BUILDDIR),$(SOURCES)))
BOARD := 1

AVRDUDE ?= avrdude
AVRDUDE_PROGRAMMER ?= arduino
AVRDUDE_VERBOSITY ?= -vv
AVRDUDE_BAUD ?= 38400
AVRDUDE_PORT ?= /dev/ttyUSB0
AVRDUDE_FLAGS ?= -p $(MCU) -c $(AVRDUDE_PROGRAMMER) -b $(AVRDUDE_BAUD) -P $(AVRDUDE_PORT) $(AVRDUDE_VERBOSITY)
AVRDUDE_WRITE_FLASH ?= -U flash:w:$(BUILDDIR)/$(TARGET).hex
AVRDUDE_WRITE_EEPROM ?= -U eeprom:w:$(BUILDDIR)/$(TARGET).eep

CC = avr-gcc
OBJCOPY = avr-objcopy
SIZE = avr-size
NM = avr-nm
REMOVE = rm -fv

CDEFS = -DF_CPU=$(F_CPU)UL -DBoard$(BOARD)
CFLAGS = $(CDEFS)
CFLAGS += -mmcu=$(MCU)
CFLAGS += -O$(OPT)
CFLAGS += -std=$(CSTANDARD)
CFLAGS += -funsigned-char -funsigned-bitfields -fshort-enums
CFLAGS += -Wall -Wextra -Werror -Wundef -Winline -Wunused -Wno-error=array-bounds -Wno-error=sign-compare

LDFLAGS ?= -Wl,-Map=$(BUILDDIR)/$(TARGET).map,--cref -lm

all: hex eeprom size

hex: $(BUILDDIR)/$(TARGET).hex

eeprom: $(BUILDDIR)/$(TARGET)_eeprom.hex

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/$(TARGET).hex: $(BUILDDIR)/$(TARGET).elf
	$(OBJCOPY) -O ihex -j .data -j .text $(BUILDDIR)/$(TARGET).elf $(BUILDDIR)/$(TARGET).hex

$(BUILDDIR)/$(TARGET)_eeprom.hex: $(BUILDDIR)/$(TARGET).elf
	$(OBJCOPY) -O ihex -j .eeprom --change-section-lma .eeprom=1 $(BUILDDIR)/$(TARGET).elf $(BUILDDIR)/$(TARGET)_eeprom.hex

$(BUILDDIR)/$(TARGET).elf: $(OBJECTS)
	$(CC) $(LDFLAGS) -mmcu=$(MCU) $(OBJECTS) -o $(BUILDDIR)/$(TARGET).elf

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c -mmcu=$(MCU) $< -o $@

size: $(BUILDDIR)/$(TARGET).elf
	$(SIZE) --mcu=$(MCU) -C $(BUILDDIR)/$(TARGET).elf

size_detailed: $(BUILDDIR)/$(TARGET).elf
	$(NM) --size-sort --print-size $(BUILDDIR)/$(TARGET).elf

program_flash: $(BUILDDIR)/$(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)

program_eeprom: $(BUILDDIR)/$(TARGET)_eeprom.hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_EEPROM)

program_all: $(BUILDDIR)/$(TARGET).hex $(BUILDDIR)/$(TARGET)_eeprom.hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH) $(AVRDUDE_WRITE_EEPROM)

clean:
	$(REMOVE) $(BUILDDIR)/*

.PHONY: program_all program_flash program_eeprom size size_detailed clean
