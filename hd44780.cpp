#include "mbed.h"

#include "hd44780.h"

HD44780::HD44780(DigitalOut &E, DigitalOut &RS, DigitalOut &RW, BusInOut &data) // :_data(data), _E(E), _RS(RS), _RW(RW){
{
    _data = &data;
    _E = &E;
    _RW = &RW;
    _RS = &RS;
}

void HD44780::reset()
{
    _data->output();

    *_E = 0;
    *_RS = 0;
    *_RW = 0;

    *_data = 0x03;

    toggleEnable();
    wait(RESET_WAIT);

    toggleEnable();
    wait(RESET_WAIT);

    toggleEnable();
    wait(RESET_WAIT);
}

void HD44780::init(unsigned char function_set, unsigned char display_control, unsigned char entry_mode)
{
    reset();

    //Initialize the display
    *_data = 0x2;
    toggleEnable();
    wait(RESET_WAIT);

    //Function set
    *_data = (0x20 | function_set) >> 4;
    toggleEnable();
    *_data = 0x20 | function_set;
    toggleEnable();
    wait(RESET_WAIT);

    //Turn off the display
    *_data = 0x08 >> 4;
    toggleEnable();
    *_data = 0x08;
    toggleEnable();
    wait(RESET_WAIT);

    //Clear the display
    *_data = 0x01 >> 4;
    toggleEnable();
    *_data = 0x01;
    toggleEnable();
    wait(RESET_WAIT);

    //entry mode set
    *_data = (0x04 | entry_mode) >> 4;
    toggleEnable();
    *_data = 0x04 | entry_mode;
    toggleEnable();
    wait(RESET_WAIT);

    //Turn on the display
    *_data = (0x08 | display_control) >> 4;
    toggleEnable();
    *_data = 0x08 | display_control;
    toggleEnable();
    wait(RESET_WAIT);
}

void HD44780::setCursorToPosition(unsigned char row, unsigned char column)
{
    unsigned char address = row * 0x40 + column;
    unsigned char command = 0x80 | address;

    *_RS = 0;
    *_RW = 0;

    _data->output();

    *_data = command >> 4;
    toggleEnable();

    *_data = command;
    toggleEnable();

    waitForCompletion();
}

void HD44780::writeCommand(unsigned char command)
{
    *_RS = 0;

    writeByte(command);
}

void HD44780::writeCharacter(unsigned char character)
{
    *_RS = 1;

    writeByte(character);
}

void HD44780::writeByte(unsigned char value)
{
    *_RW = 0;
    _data->output();

    *_data = value >> 4;
    toggleEnable();

    *_data = value;
    toggleEnable();

    waitForCompletion();
}

unsigned char HD44780::readByte()
{
    unsigned char data;

    *_RW = 1;
    _data->input();

    *_E = 1;
    wait(ENABLE_WAIT);

    data = *_data << 4;

    *_E = 0;
    wait(ENABLE_WAIT);

    *_E = 1;
    wait(ENABLE_WAIT);

    data |= *_data;

    *_E = 0;
    wait(ENABLE_WAIT);

    return data;
}

unsigned char HD44780::readStatus()
{
    *_RS = 0;

    return readByte();
}

void HD44780::clear()
{
    *_RS = 0;
    *_RW = 0;

    _data->output();

    *_data = 0x01 >> 4;
    toggleEnable();

    *_data = 0x01;
    toggleEnable();

    waitForCompletion();
}

void HD44780::print(char *str)
{
    unsigned char byte;
    unsigned int i = 0;

    do {
        byte = str[i];
        if (byte != '\0') {
            writeCharacter(byte);
        }
        i++;
    } while(byte != '\0');
}

void HD44780::toggleEnable()
{
    wait(ENABLE_WAIT);
    *_E = 1;
    wait(ENABLE_WAIT);
    *_E = 0;
    wait(ENABLE_WAIT);
}

void HD44780::waitForCompletion()
{
    while((readStatus() && 0x80) == 0x80) {}
}