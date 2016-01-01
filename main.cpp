#include "mbed.h"

#include "hd44780.h"

BusInOut data(PTE20, PTE21, PTE22, PTE23);
DigitalOut RS(PTB0);
DigitalOut RW(PTB1);
DigitalOut E(PTB2);
HD44780 display(E, RS, RW, data);

DigitalOut pin(PTD7);

int main()
{
    display.init(LINES_2 | FONT_5BY8, DISPLAY_ON | CURSOR_VISIBLE | CHARACTER_BLINKING, INCREMENT);

    display.clear();

    display.print("Hello");
    display.setCursorToPosition(1, 0);
    display.print("World");

    while(1) {
    }
}
