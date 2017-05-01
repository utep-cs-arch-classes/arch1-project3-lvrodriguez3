CPU             	= msp430g2553
CFLAGS          	= -mmcu=${CPU} -Os -I../h -I../h

CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c

all:game.elf

game.elf: ${COMMON_OBJECTS} game.o wdt_handler.o buzzer.o
	${CC} -mmcu=${CPU}   -o $@ $^ -L../lib -lTimer -lLcd -lShape -lCircle -lp2sw

load: game.elf
	mspdebug rf2500 "prog $^"

clean:
	rm -f *.o *.elf
