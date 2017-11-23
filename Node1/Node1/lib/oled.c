/*
 * oled.c
 *
 * Created: 27.09.2017 17:22:07
 *  Author: eddas, robbas og arras
 */ 


#define F_CPU 4915200UL
#include <util/delay.h>
#include "oled.h"
#include <stdlib.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "sram.h"

volatile char *OLEDC_ptr = (uint8_t *) 0x1000; // Command char* before
volatile char *OLEDD_ptr = (uint8_t *) 0x1200; // Data char* before

const unsigned char PROGMEM font[95][5];

void oled_init(){
	oled_writeCommand(0xae);        // Display  off
	oled_writeCommand(0xa1);        // Segment  remap
	oled_writeCommand(0xda);        // Common  pads  hardware:  alternative
	oled_writeCommand(0x12);
	oled_writeCommand(0xc8);        // Common output scan direction:com63~com0
	oled_writeCommand(0xa8);        // Multiplex  ration  mode:63
	oled_writeCommand(0x3f);
	oled_writeCommand(0xd5);        // Display divide ratio/osc. freq. mode
	oled_writeCommand(0x80);
	oled_writeCommand(0x81);        // Contrast control
	oled_writeCommand(0x50);
	oled_writeCommand(0xd9);        // Set pre-charge period
	oled_writeCommand(0x21);
	oled_writeCommand(0x20);        // Set Memory Addressing Mode
	oled_writeCommand(0x02);
	oled_writeCommand(0xdb);        // VCOM deselect level mode
	oled_writeCommand(0x30);
	oled_writeCommand(0xad);        // Master configuration
	oled_writeCommand(0x00);
	oled_writeCommand(0xa4);        // Out follows RAM content
	oled_writeCommand(0xa6);        // Set normal display
	oled_writeCommand(0xaf);        // Display on
	oled_reset();
}

void oled_reset(){
	for(int line = 0; line < 8; line++){
		oled_clearLine(line);
	}
}

void oled_clearLine(uint8_t line){
	oled_writeCommand(0xb0 + line); // Selecting row/page
	for(int byte = 0; byte < 128; byte++){
		OLEDD_ptr[0] = 0x00; // Clear current column
	}
}

void oled_bright(){
	for(int line = 0; line < 8; line++){
		oled_writeCommand(0xb0+line);
		for(int byte = 0; byte < 128; byte++){
			OLEDD_ptr[0] = 0xff;
		}
	}
}

void oled_writeCommand(uint8_t command){
	OLEDC_ptr[0] = command;
}

void oled_writeChar(char* str){
	uint8_t FONT_SIZE=5;
	for(uint8_t _col=0; _col < FONT_SIZE; _col++){
			*OLEDD_ptr = (pgm_read_byte(&(font[*str-32][_col])));
	}
}

void oled_writeCharInv(char* str){
	uint8_t FONT_SIZE=5;
	for(uint8_t _col=0; _col < FONT_SIZE; _col++){
		*OLEDD_ptr = ~(pgm_read_byte(&(font[*str-32][_col])));
	}
}




void oled_writeFromStartOnLine(int line){
		*OLEDC_ptr = 0xB0 + line;
		*OLEDC_ptr = 0x00;
		*OLEDC_ptr = 0x10;
}

void oled_writeString(char* str, int size, int inv){
	if (!inv){
		for (int i = 0; i < size; i++){
			oled_writeChar(str+i);
		}
	}
	else{
		for (int i = 0; i < size; i++){
			oled_writeCharInv(str+i);
		}
	}
	
	
}

const unsigned char PROGMEM font[95][5] = {
	{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000}, //   
	{0b00000000,0b00000000,0b01011110,0b00000000,0b00000000}, // ! 
	{0b00000000,0b00001110,0b00000000,0b00001110,0b00000000}, // " 
	{0b00101000,0b01111100,0b00101000,0b01111100,0b00101000}, // # 
	{0b00001000,0b01010100,0b01111100,0b01010100,0b00100000}, // $ 
	{0b00100110,0b00010000,0b00001000,0b01100100,0b00000000}, // % 
	{0b00101000,0b01010100,0b00101000,0b01000000,0b00000000}, // & 
	{0b00000000,0b00000000,0b00001110,0b00000000,0b00000000}, // ' 
	{0b00000000,0b00111100,0b01000010,0b00000000,0b00000000}, // ( 
	{0b00000000,0b01000010,0b00111100,0b00000000,0b00000000}, // ) 
	{0b00000000,0b01010100,0b00111000,0b01010100,0b00000000}, // * 
	{0b00010000,0b00010000,0b01111100,0b00010000,0b00010000}, // + 
	{0b00000000,0b10000000,0b01100000,0b00100000,0b00000000}, // , 
	{0b00010000,0b00010000,0b00010000,0b00010000,0b00000000}, // - 
	{0b00000000,0b01100000,0b01100000,0b00000000,0b00000000}, // . 
	{0b00100000,0b00010000,0b00001000,0b00000100,0b00000000}, // / 
	{0b00000000,0b00111100,0b01000010,0b00111100,0b00000000}, // 0 
	{0b00000000,0b01000100,0b01111110,0b01000000,0b00000000}, // 1 
	{0b01000100,0b01100010,0b01010010,0b01001100,0b00000000}, // 2 
	{0b00100010,0b01001010,0b01001010,0b00110110,0b00000000}, // 3 
	{0b00011000,0b00010100,0b01111110,0b00010000,0b00000000}, // 4 
	{0b00101110,0b01001010,0b01001010,0b00110010,0b00000000}, // 5 
	{0b00111100,0b01001010,0b01001010,0b00110000,0b00000000}, // 6 
	{0b00000010,0b01100010,0b00011010,0b00000110,0b00000000}, // 7 
	{0b00110100,0b01001010,0b01001010,0b00110100,0b00000000}, // 8 
	{0b00001100,0b01010010,0b01010010,0b00111100,0b00000000}, // 9 
	{0b00000000,0b01101100,0b01101100,0b00000000,0b00000000}, // : 
	{0b10000000,0b01101100,0b00101100,0b00000000,0b00000000}, // ; 
	{0b00000000,0b00010000,0b00101000,0b01000100,0b00000000}, // < 
	{0b00101000,0b00101000,0b00101000,0b00101000,0b00000000}, // = 
	{0b00000000,0b01000100,0b00101000,0b00010000,0b00000000}, // > 
	{0b00000000,0b00000100,0b01010010,0b00001100,0b00000000}, // ? 
	{0b00111100,0b01000010,0b01011010,0b00011100,0b00000000}, // @ 
	{0b01111100,0b00010010,0b00010010,0b01111100,0b00000000}, // A 
	{0b01111110,0b01001010,0b01001010,0b00110100,0b00000000}, // B 
	{0b00111100,0b01000010,0b01000010,0b00100100,0b00000000}, // C 
	{0b01111110,0b01000010,0b01000010,0b00111100,0b00000000}, // D 
	{0b01111110,0b01001010,0b01001010,0b01000010,0b00000000}, // E 
	{0b01111110,0b00001010,0b00001010,0b00000010,0b00000000}, // F 
	{0b00111100,0b01000010,0b01010010,0b01110100,0b00000000}, // G 
	{0b01111110,0b00001000,0b00001000,0b01111110,0b00000000}, // H 
	{0b00000000,0b01000010,0b01111110,0b01000010,0b00000000}, // I 
	{0b00100000,0b01000000,0b01000000,0b00111110,0b00000000}, // J 
	{0b01111110,0b00011000,0b00100100,0b01000010,0b00000000}, // K 
	{0b01111110,0b01000000,0b01000000,0b01000000,0b00000000}, // L 
	{0b01111110,0b00001100,0b00001100,0b01111110,0b00000000}, // M 
	{0b01111110,0b00001100,0b00110000,0b01111110,0b00000000}, // N 
	{0b00111100,0b01000010,0b01000010,0b00111100,0b00000000}, // O 
	{0b01111110,0b00010010,0b00010010,0b00001100,0b00000000}, // P 
	{0b00111100,0b01100010,0b01000010,0b10111100,0b00000000}, // Q 
	{0b01111110,0b00010010,0b00110010,0b01001100,0b00000000}, // R 
	{0b00100100,0b01001010,0b01010010,0b00100100,0b00000000}, // S 
	{0b00000000,0b00000010,0b01111110,0b00000010,0b00000000}, // T 
	{0b00111110,0b01000000,0b01000000,0b00111110,0b00000000}, // U 
	{0b00011110,0b01100000,0b01100000,0b00011110,0b00000000}, // V 
	{0b01111110,0b00110000,0b00110000,0b01111110,0b00000000}, // W 
	{0b01100110,0b00011000,0b00011000,0b01100110,0b00000000}, // X 
	{0b00000000,0b00001110,0b01110000,0b00001110,0b00000000}, // Y 
	{0b01100010,0b01010010,0b01001010,0b01000110,0b00000000}, // Z 
	{0b00000000,0b01111110,0b01000010,0b01000010,0b00000000}, // [ 
	{0b00000100,0b00001000,0b00010000,0b00100000,0b00000000}, // "\" 
	{0b00000000,0b01000010,0b01000010,0b01111110,0b00000000}, // ] 
	{0b00000000,0b00000100,0b00000010,0b00000100,0b00000000}, // ^ 
	{0b01000000,0b01000000,0b01000000,0b01000000,0b00000000}, // _ 
	{0b00000000,0b00000010,0b00000100,0b00000000,0b00000000}, // ` 
	{0b00110000,0b01001000,0b00101000,0b01111000,0b00000000}, // a 
	{0b01111110,0b01001000,0b01001000,0b00110000,0b00000000}, // b 
	{0b00110000,0b01001000,0b01001000,0b00000000,0b00000000}, // c 
	{0b00110000,0b01001000,0b01001000,0b01111110,0b00000000}, // d 
	{0b00110000,0b01101000,0b01011000,0b00010000,0b00000000}, // e 
	{0b00010000,0b01111100,0b00010010,0b00000100,0b00000000}, // f 
	{0b01010000,0b10101000,0b10101000,0b10011000,0b00000000}, // g 
	{0b01111110,0b00001000,0b00001000,0b01110000,0b00000000}, // h 
	{0b00000000,0b01001000,0b01111010,0b01000000,0b00000000}, // i 
	{0b00000000,0b01000000,0b10000000,0b01111010,0b00000000}, // j 
	{0b01111110,0b00010000,0b00101000,0b01000000,0b00000000}, // k 
	{0b00000000,0b01000010,0b01111110,0b01000000,0b00000000}, // l 
	{0b01111000,0b00010000,0b00011000,0b01110000,0b00000000}, // m 
	{0b01111000,0b00001000,0b00001000,0b01110000,0b00000000}, // n 
	{0b00110000,0b01001000,0b01001000,0b00110000,0b00000000}, // o 
	{0b11111000,0b01001000,0b01001000,0b00110000,0b00000000}, // p 
	{0b00110000,0b01001000,0b01001000,0b11111000,0b00000000}, // q 
	{0b01111000,0b00001000,0b00001000,0b00010000,0b00000000}, // r 
	{0b01010000,0b01011000,0b01101000,0b00101000,0b00000000}, // s 
	{0b00001000,0b00111110,0b01001000,0b01000000,0b00000000}, // t 
	{0b00111000,0b01000000,0b01000000,0b01111000,0b00000000}, // u 
	{0b00000000,0b00111000,0b01000000,0b00111000,0b00000000}, // v 
	{0b01111000,0b01100000,0b01100000,0b01111000,0b00000000}, // w 
	{0b01001000,0b00110000,0b00110000,0b01001000,0b00000000}, // x 
	{0b00011000,0b10100000,0b01000000,0b00111000,0b00000000}, // y 
	{0b01001000,0b01101000,0b01011000,0b01001000,0b00000000}, // z 
	{0b00000000,0b00001000,0b00111100,0b01000010,0b00000000}, // 
	{0b00000000,0b00000000,0b01111110,0b00000000,0b00000000}, // | 
	{0b00000000,0b01000010,0b00111100,0b00001000,0b00000000}, // 
	{0b00000100,0b00000010,0b00000100,0b00000010,0b00000000}, // ~ 
};

const unsigned int byggern[1024] PROGMEM = {
	0xFF, 0x01, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD,
	0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0x7D, 0x7D, 0x7D,
	0x7D, 0x7D, 0x7D, 0xFD, 0xFD, 0x7D, 0x7D, 0x7D, 0xFD, 0xFD,
	0xFD, 0x7D, 0x7D, 0x7D, 0xFD, 0xFD, 0x7D, 0x7D, 0x7D, 0x7D,
	0x7D, 0xFD, 0xFD, 0xFD, 0xFD, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D,
	0xFD, 0xFD, 0xFD, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0xFD,
	0xFD, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0xFD, 0xFD, 0xFD,
	0x7D, 0x7D, 0xFD, 0xFD, 0xFD, 0x7D, 0x7D, 0xFD, 0xFD, 0xFD,
	0xFD, 0xFD, 0xFD, 0xFD, 0x7D, 0x7D, 0x7D, 0x7D, 0xFD, 0xFD,
	0xFD, 0x7D, 0x7D, 0x7D, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0x7D,
	0x7D, 0xFD, 0xFD, 0xFD, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D,
	0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD,
	0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0x01, 0xFF, 0xFF, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x80, 0x92, 0x92, 0x92,
	0x80, 0x84, 0xFF, 0xFF, 0xFE, 0xFC, 0x80, 0x81, 0x80, 0xFC,
	0xFE, 0xFF, 0xFF, 0xC0, 0x80, 0x9E, 0x9E, 0x96, 0x94, 0xC4,
	0xF7, 0xFF, 0xC0, 0x80, 0x9E, 0x9E, 0x96, 0x94, 0xC4, 0xF7,
	0xFF, 0x80, 0x80, 0x92, 0x92, 0x92, 0x92, 0xFF, 0xFF, 0x80,
	0x80, 0xF2, 0xF2, 0xC2, 0x80, 0x98, 0xBF, 0xFF, 0x80, 0x80,
	0xF8, 0xE1, 0xC7, 0x80, 0x80, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xBF, 0x9C, 0x8C, 0x86, 0x90, 0x98, 0x9D, 0xF3, 0xC0, 0x8C,
	0x9E, 0x8C, 0x80, 0xF3, 0xFF, 0xF9, 0xFC, 0x80, 0x80, 0xFF,
	0xFF, 0xFF, 0xFE, 0x8E, 0x82, 0xF0, 0xFC, 0xFE, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF,
	0xFF, 0xFF, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0xFF, 0x3F, 0x3F,
	0x3F, 0x3F, 0x7F, 0xFF, 0xFF, 0x3F, 0x3F, 0x3F, 0x3F, 0x7F,
	0xFF, 0xFF, 0xFF, 0x3F, 0x3F, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0x7F,
	0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0xFF, 0x3F, 0xBF,
	0x3F, 0x7F, 0x3F, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
	0xE0, 0xE0, 0xE5, 0xE5, 0xE5, 0xFF, 0xE0, 0xE0, 0xE7, 0xE7,
	0xF0, 0xFF, 0xFF, 0xE0, 0xE0, 0xE7, 0xE7, 0xF0, 0xF8, 0xE7,
	0xE1, 0xF0, 0xF2, 0xF0, 0xE0, 0xE7, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x9F, 0x1F,
	0x4F, 0x07, 0x87, 0x03, 0x03, 0x07, 0x03, 0x57, 0x07, 0x23,
	0x07, 0x03, 0x81, 0xDC, 0x49, 0x65, 0xC0, 0x98, 0x40, 0xB0,
	0x29, 0x74, 0x43, 0x42, 0x74, 0x15, 0x10, 0x3F, 0x85, 0xA4,
	0x89, 0xA6, 0x29, 0x36, 0x91, 0x06, 0x4B, 0x0F, 0x1F, 0x7F,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x07,
	0x47, 0x07, 0x3F, 0xFF, 0xFF, 0x07, 0x07, 0x27, 0x27, 0x07,
	0x8F, 0xFF, 0x07, 0x07, 0xFF, 0xFF, 0x07, 0x07, 0xFF, 0xFF,
	0xFF, 0xFF, 0x07, 0x07, 0xE7, 0xE7, 0x0F, 0xBF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD, 0xFF, 0xE6, 0x51,
	0x14, 0x00, 0x40, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00,
	0x00, 0xF8, 0xF8, 0x7D, 0x28, 0x38, 0x34, 0x3E, 0x2C, 0x18,
	0x10, 0x3E, 0x0F, 0x00, 0x1F, 0x1F, 0x04, 0x0E, 0x0F, 0x0B,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
	0x07, 0x1B, 0x5F, 0xBF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF,
	0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFC, 0xFC, 0xFE, 0xFE, 0xFE,
	0xFC, 0xFC, 0xFF, 0xFC, 0xFC, 0xFF, 0xFC, 0xFC, 0xFC, 0xFF,
	0xFC, 0xFC, 0xFF, 0xFF, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFF,
	0xFC, 0xFC, 0xFC, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE2,
	0x80, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00,
	0x80, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0xA0, 0x80, 0xA0,
	0x80, 0x40, 0xD8, 0xC0, 0xF8, 0xE0, 0xE0, 0xE4, 0xE0, 0xFC,
	0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x80, 0xE4, 0x84, 0x80, 0x91,
	0xFF, 0xC1, 0x88, 0x9C, 0x9C, 0x88, 0xC1, 0xFF, 0x80, 0x80,
	0x94, 0x90, 0xC9, 0xFF, 0xFF, 0x80, 0x80, 0xFF, 0xFF, 0x80,
	0x80, 0xF1, 0xC7, 0x80, 0x80, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFE, 0xFF, 0xFF, 0xFC, 0xFE, 0xFC, 0xFE, 0xFE, 0xFC, 0xFA,
	0xFC, 0xF8, 0xFD, 0xFC, 0xFB, 0xFC, 0xFF, 0xFC, 0xFE, 0xFE,
	0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x80, 0xBF, 0xBF,
	0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF,
	0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF,
	0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF,
	0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF,
	0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF,
	0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF,
	0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF,
	0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF,
	0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF,
	0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF,
	0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF,
	0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF,
	0xBF, 0xBF, 0x80, 0xFF
};

#define FREE_SPACE_SRAM 500

void oled_writePic(){
	oled_writeFromStartOnLine(0);
	volatile char *ext_ram = (char *) 0x1800;							// Start address for the SRAM
	for(int line = 0; line < 8; line++){
		oled_writeCommand(0xb0+line);
		for(int byte = 0; byte < 128; byte++){
			OLEDD_ptr[0] = ext_ram[(128*line + byte)+FREE_SPACE_SRAM];	// ext_ram[line+byte+EMPTY_SPACE_SRAM];
		}
	}
}

void oled_fillSram(){
	volatile char *ext_ram = (char *) 0x1800;							// Start address for the SRAM
	for (int i = 0; i < 1024; i++){
		ext_ram[i+FREE_SPACE_SRAM] = ~pgm_read_byte(&(byggern[i]));
	}
}

void oled_printGame(int score,int lives){
	oled_writeFromStartOnLine(0);
	oled_writeString(" Game running!!  ",16,0);
	oled_writeFromStartOnLine(3);
	for (int i = 0; i < lives; i++){
		oled_writeString(" <3 ",4,0);
	}
	oled_writeFromStartOnLine(5);
	oled_writeString("Score: ",7,0);
	oled_printNumber(score);	
}

void oled_printGameTraining(int counter){
	oled_writeFromStartOnLine(0);
	oled_writeString(" Training mode  ",15,0);
	oled_writeFromStartOnLine(4);
	oled_writeString("Time left: ",11,0);
	oled_printNumber(counter);
}

void oled_printNumber(int number){
	if(number<0){
		oled_writeString("-",1,0);
		number=-number;
	}
	const int maxNumbDig=4;
	char dig[maxNumbDig];
	int asciiNumberOffset = 48;
	int numbDig=maxNumbDig;
	for (int i = 0; i < maxNumbDig; i++){
		dig[maxNumbDig-i-1]=(char) number%10 + asciiNumberOffset;
		number=number/10;
		if (number==0){
			numbDig=i+1;
			break;
		}
	}
	oled_writeString(dig+(maxNumbDig-numbDig),numbDig,0);
}

void oled_printNumberInverted(int number){
	if(number<0){
		oled_writeString("-",1,1);
		number=-number;
	}
	const int maxNumbDig=4;
	char dig[maxNumbDig];
	int asciiNumberOffset = 48;
	int numbDig=maxNumbDig;
	for (int i = 0; i < maxNumbDig; i++){
		dig[maxNumbDig-i-1]=(char) number%10 + asciiNumberOffset;
		number=number/10;
		if (number==0){
			numbDig=i+1;
			break;
		}
	}
	oled_writeString(dig+(maxNumbDig-numbDig),numbDig,1);
}



const unsigned char gameOver [] PROGMEM=
{
	0x00, 0x80, 0xE0, 0xF0, 0xF0, 0x30, 0x30, 0x30, 0x70, 0x70,
	0x60, 0x00, 0x00, 0x00, 0x80, 0xF0, 0xF0, 0x70, 0xF0, 0xF0,
	0xE0, 0x80, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
	0x80, 0x00, 0xC0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0x00, 0x00,
	0x30, 0xF0, 0xF0, 0xF0, 0x30, 0x30, 0x30, 0x70, 0x70, 0x70,
	0x00, 0x00, 0x0D, 0x1F, 0x1F, 0x3F, 0x30, 0x32, 0x3A, 0x3E,
	0x1E, 0x1E, 0x00, 0x00, 0x1C, 0x1F, 0x1F, 0x17, 0x06, 0x17,
	0x1F, 0x1F, 0x1F, 0x18, 0x10, 0x00, 0x1F, 0x1F, 0x1F, 0x10,
	0x07, 0x1F, 0x1F, 0x0F, 0x01, 0x1D, 0x1F, 0x1F, 0x1F, 0x10,
	0x00, 0x10, 0x1F, 0x1F, 0x1F, 0x19, 0x19, 0x19, 0x19, 0x1C,
	0x1C, 0x00, 0x00, 0xD0, 0xF8, 0xFC, 0xFC, 0x0C, 0x0C, 0x0C,
	0xFC, 0xFC, 0xFC, 0xF0, 0x00, 0x0C, 0x3C, 0xFC, 0xFC, 0xFC,
	0x84, 0x00, 0xCC, 0xFC, 0xFC, 0x3C, 0x0C, 0x00, 0x0C, 0xFC,
	0xFC, 0xFC, 0xFC, 0x4C, 0xCC, 0xCC, 0x5C, 0x1C, 0x1C, 0x00,
	0x00, 0x0C, 0xFC, 0xFC, 0xFC, 0xEC, 0xCC, 0xCC, 0xEC, 0xFC,
	0x7C, 0x38, 0x00, 0x00, 0x01, 0x0F, 0x0F, 0x0F, 0x0C, 0x0C,
	0x0C, 0x0F, 0x0F, 0x0F, 0x03, 0x00, 0x00, 0x00, 0x01, 0x0F,
	0x0F, 0x0F, 0x0E, 0x0F, 0x0F, 0x01, 0x00, 0x00, 0x00, 0x0C,
	0x0F, 0x0F, 0x0F, 0x0F, 0x0C, 0x0C, 0x0C, 0x0F, 0x0E, 0x0F,
	0x00, 0x00, 0x0C, 0x0F, 0x0F, 0x0F, 0x0C, 0x00, 0x00, 0x03,
	0x0F, 0x0F, 0x0C, 0x00
};

#define imageWidth 51
#define imageHeight 32

void oled_printGameOver(){
	oled_reset();
	oled_writeFromStartOnLine(0);
	int startPrintingWidth = 38;
	int startPrintingLine = 2;
	for(int line = 0; line < 8; line++){
		oled_writeCommand(0xb0+line);
		for(int byte = 0; byte < 128; byte++){
			if(byte < startPrintingWidth || byte > imageWidth+startPrintingWidth-1 || line < startPrintingLine || line > startPrintingLine + imageHeight/8 - 1){
				OLEDD_ptr[0] = 0x00;
			}
			else{
				OLEDD_ptr[0] = pgm_read_byte(&(gameOver[(line-startPrintingLine)*(imageWidth)+(byte-startPrintingWidth)]));
			}
		}
	}
	
}


const int maxNumbCharPrLine=25;
void oled_writeLongerText(int startLine, char* text){ //All text must end with '\0'
	char word[maxNumbCharPrLine];
	int wordLengthCounter=0;
	int textLengthCounter=0;
	int lineLengthCounter=0;
	char letter='a';
	oled_writeFromStartOnLine(startLine);
	while (letter!='\0'){
		wordLengthCounter=0;
		letter='a';
		while (letter!=' ' && letter!='\0'){
			letter=text[textLengthCounter];
			word[wordLengthCounter]=letter;
			textLengthCounter+=1;
			wordLengthCounter+=1;
		}
		if(lineLengthCounter+wordLengthCounter>maxNumbCharPrLine){
			startLine+=1;
			oled_writeFromStartOnLine(startLine);
			lineLengthCounter=0;
		}
		oled_writeString(word,wordLengthCounter,0);
		_delay_ms(20);			//Looks cool
		lineLengthCounter+=wordLengthCounter;
	}
}

void oled_printStartPage(){
	oled_reset();
	oled_writeFromStartOnLine(1);  //Printer startside
	oled_writeString("       Welcome ",15,0);
	oled_writeFromStartOnLine(2);
	oled_writeString("         to    ",15,0);
	oled_writeFromStartOnLine(3);
	oled_writeString("----PingPong Game---- ",21,0);
	oled_writeFromStartOnLine(4);
	oled_writeString("_____________________ ",21,0);
	oled_writeFromStartOnLine(7);
	oled_writeString("Next ",5,1);
}