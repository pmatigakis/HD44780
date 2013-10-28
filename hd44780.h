#include "mbed.h"

#ifndef _HD44780_H_
#define _HD44780_H_

#define LINES_2 0x08
#define LINE_1 0x00

#define FONT_5BY10 0x04
#define FONT_5BY8 0x00

#define DISPLAY_ON 0x04

#define CURSOR_VISIBLE 0x02

#define CHARACTER_BLINKING 0x01

#define INCREMENT 0x02
#define DECREMENT 0x00

#define RESET_WAIT 0.05
#define ENABLE_WAIT 0.0005

class HD44780
{
private:
    BusInOut *_data;
    DigitalOut *_E;
    DigitalOut *_RS;
    DigitalOut *_RW;
    void reset();
    void writeCommand(unsigned char command);
    void writeCharacter(unsigned char character);
    void writeByte(unsigned char value);
    unsigned char readStatus();
    unsigned char readByte();
    void toggleEnable();
    void waitForCompletion();

public:
    HD44780(DigitalOut &E, DigitalOut &RS, DigitalOut &RW, BusInOut &data);
    void init(unsigned char function_set, unsigned char display_control, unsigned char entry_mode);
    void setCursorToPosition(unsigned char row, unsigned char column);
    void clear();
    void print(char *str);
};

#endif