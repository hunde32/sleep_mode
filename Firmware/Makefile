MCU = atmega328p
CC = avr-gcc
OBJCOPY = avr-objcopy
CFLAGS = -Wall -Os -mmcu=$(MCU)

TARGET = main
SRC = main.c

all: $(TARGET).hex

$(TARGET).elf: $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

clean:
	rm -f $(TARGET).elf $(TARGET).hex
