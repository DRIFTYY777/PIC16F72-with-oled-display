/*
 * File:   main.c
 * Author: Driftyy_777
 *
 * Created on 30 June, 2024, 11:21 AM
 */

#include <xc.h>
#include "font.c"

// CONFIG BITS
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 12000000 // Define the oscillator frequency as 12 MHz

/* Display Defines*/
#define OLED_ADDR 0x78 // SSD1306 I2C address for write
#define SDA RC4
#define SCL RC3
#define SDA_TRIS TRISC4
#define SCL_TRIS TRISC3

/* Display Functions*/
void command(unsigned char comm);
void oled_init();
void clrScreen();
void sendData(unsigned char dataB);
void startBit(void);
void stopBit(void);
void clock(void);
void drawChar(char data, uint8_t y, char c);
void drawNumber(unsigned int number, unsigned char y, unsigned char x_start);
void drawString(const char *str, unsigned char y, unsigned char x_start);

/* bit bang I2C methods */
void command(unsigned char comm)
{
    startBit();
    sendData(OLED_ADDR); // address
    sendData(0x00);
    sendData(comm); // command code
    stopBit();
}

void sendData(unsigned char dataB)
{
    for (unsigned char b = 0; b < 8; b++)
    {
        // SDA = (dataB >> (7 - b)) % 2;
        SDA = (dataB >> (7 - b)) & 0x01;
        clock();
    }
    SDA_TRIS = 1; // SDA input
    clock();
    __delay_us(4);
    SDA_TRIS = 0; // SDA output
}

void startBit(void)
{
    SDA = 0;
    __delay_us(4);
    SCL = 0;
}
void stopBit(void)
{
    SCL = 1;
    __delay_us(4);
    SDA = 1;
}
void clock(void)
{
    __delay_us(4);
    SCL = 1;
    __delay_us(4);
    SCL = 0;
    __delay_us(4);
}

void drawChar(char data, uint8_t y, char x)
{
    unsigned char i, line, btm, top; //

    command(0x20); // vert mode
    command(0x01);
    command(0x21);       // col addr
    command(13 * x);     // col start
    command(13 * x + 9); // col end
    command(0x22);       // 0x22
    command(y);          // Page start
    command(y + 1);      // Page end
    startBit();
    sendData(OLED_ADDR); // address
    sendData(0x40);
    for (i = 0; i < 5; i++)
    {
        line = font[5 * (data - 32) + i]; // <-- FIXED
        btm = 0;
        top = 0;
        // expend char
        if (line & 64)
        {
            btm += 192;
        }
        if (line & 32)
        {
            btm += 48;
        }
        if (line & 16)
        {
            btm += 12;
        }
        if (line & 8)
        {
            btm += 3;
        }
        if (line & 4)
        {
            top += 192;
        }
        if (line & 2)
        {
            top += 48;
        }
        if (line & 1)
        {
            top += 12;
        }
        sendData(top); // top page
        sendData(btm); // second page
        sendData(top);
        sendData(btm);
    }
    stopBit();
    command(0x20); // horizontal mode
    command(0x00);
}

void drawNumber(unsigned int number, unsigned char y, unsigned char x_start)
{
    unsigned char digits[5];
    unsigned char i = 0;

    // Extract digits (up to 5 digits for safety)
    do
    {
        digits[i++] = number % 10;
        number /= 10;
    } while (number > 0 && i < 5);

    // Draw digits in reverse order (most significant digit first)
    for (unsigned char j = 0; j < i; j++)
    {
        drawChar(digits[i - j - 1] + '0', y, x_start + j); // <-- add '0'
    }
}

void oled_init()
{
    command(0xAE); // DISPLAYOFF
    command(0x8D); // CHARGEPUMP *
    command(0x14); // 0x14-pump on
    command(0x20); // MEMORYMODE
    command(0x0);  // 0x0=horizontal, 0x01=vertical, 0x02=page
    command(0xA1); // SEGREMAP * A0/A1=top/bottom
    command(0xC8); // COMSCANDEC * C0/C8=left/right
    command(0xDA); // SETCOMPINS *
    command(0x22); // 0x22=4rows for 128x32, 0x12=8rows for 128x64
    command(0x81); // SETCONTRAST
    command(0xFF); // 0x8F
    // next settings are set by default
    command(0xD5); // SETDISPLAYCLOCKDIV
    command(0x80);
    command(0xA8); // SETMULTIPLEX
    command(0x3F); // 0x1F
    command(0xD3); // SETDISPLAYOFFSET
    command(0x0);
    command(0x40); // SETSTARTLINE
    command(0xD9); // SETPRECHARGE
    command(0xF1);
    command(0xDB); // SETVCOMDETECT
    command(0x40);
    command(0xA4); // DISPLAYALLON_RESUME
    command(0xA6); // NORMALDISPLAY
    command(0xAF); // DISPLAYON
}

void clrScreen() // fill screen with 0
{
    unsigned char y, i;

    for (y = 0; y < 8; y++)
    {
        command(0x21);  // col addr
        command(0);     // col start
        command(127);   // col end
        command(0x22);  // 0x22
        command(y);     // Page start
        command(y + 1); // Page end
        startBit();
        sendData(OLED_ADDR); // address
        sendData(0x40);
        for (i = 0; i < 128; i++)
        {
            sendData(0x00);
        }
        stopBit();
    }
}

void drawString(const char *str, unsigned char y, unsigned char x_start)
{
    unsigned char i = 0;
    while (str[i] != '\0' && i < 20) // Limit to 20 characters
    {
        drawChar(str[i], y, x_start + i);
        i++;
    }
}

void inBuildLed()
{
    TRISA1 = 0; // Set RA1 as output for LED
    RA1 = 0;    // Ensure it's LOW initially
}

void blink()
{
    RA1 = 1; // LED ON
    __delay_ms(50);
    RA1 = 0; // LED OFF
    __delay_ms(50);
}




void main(void)
{

    TRISC3 = 0;
    TRISC4 = 0;

    SCL = 1;
    SDA = 1;
    __delay_ms(500);
    oled_init();
    clrScreen(); // clear screen

    drawString("Init", 0, 0); // Draw "Init"


    __delay_ms(1000);
    clrScreen(); // clear screen

    drawString("Hello World", 0, 0);
    drawNumber(123, 2, 0);
    
    while (1)
    {
        
    }
}
