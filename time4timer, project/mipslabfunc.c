/* mipslabfunc.c
   This file written 2015 by F Lundevall
   Some parts are original code written by Axel Isaksson

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

/* Declare a helper function which is local to this file */
static void num32asc( char * s, int ); 

#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)

/* quicksleep:
   A simple function to create a small delay.
   Very inefficient use of computing resources,
   but very handy in some special cases. */
void quicksleep(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}

/* display_debug
   A function to help debugging.

   After calling display_debug,
   the two middle lines of the display show
   an address and its current contents.

   There's one parameter: the address to read and display.

   Note: When you use this function, you should comment out any
   repeated calls to display_image; display_image overwrites
   about half of the digits shown by display_debug.
*/   
void display_debug( volatile int * const addr )
{
  display_string( 1, "Addr" );
  display_string( 2, "Data" );
  num32asc( &textbuffer[1][6], (int) addr );
  num32asc( &textbuffer[2][6], *addr );
  display_update();
}

uint8_t spi_send_recv(uint8_t data) {
	while(!(SPI2STAT & 0x08));
	SPI2BUF = data;
	while(!(SPI2STAT & 1));
	return SPI2BUF;
}

void display_init(void) {
        DISPLAY_CHANGE_TO_COMMAND_MODE;
	quicksleep(10);
	DISPLAY_ACTIVATE_VDD;
	quicksleep(1000000);
	
	spi_send_recv(0xAE);
	DISPLAY_ACTIVATE_RESET;
	quicksleep(10);
	DISPLAY_DO_NOT_RESET;
	quicksleep(10);
	
	spi_send_recv(0x8D);
	spi_send_recv(0x14);
	
	spi_send_recv(0xD9);
	spi_send_recv(0xF1);
	
	DISPLAY_ACTIVATE_VBAT;
	quicksleep(10000000);
	
	spi_send_recv(0xA1);
	spi_send_recv(0xC8);
	
	spi_send_recv(0xDA);
	spi_send_recv(0x20);
	
	spi_send_recv(0xAF);
}

/* borrowed from lab files, modified by us to print the string in the center*/
void display_string(int line, char *s) {
	int i;
	int c = 0;
	if(line < 0 || line >= 4)
		return;
	if(!s)
		return;
	
	for(i = 0; i < 16; i++){
		if(*s) {
			c = c + 1;
			s++;
		}
	}
	s = s - c;
	for(i=0; i < ((16-c) /2) ; i++){
			textbuffer[line][i] = ' ';
	}
	for(i = ((16-c) /2); i < 16; i++){
		if(*s){
			textbuffer[line][i] = *s;
			s++;
		}
		else{
			textbuffer[line][i] = ' ';
		}
	}
}

/* power function written by us*/
int pow(int x, int y){
	int i;
	int ans = 1;
	for(i = 0; i < y; i++){
		ans = ans * x;
	}
	return ans;
}

/* borrowed from the lab files, modified by us
   to be able to move images pixel by pixel
   in x and y direction*/
void display_image(int x, int y, int height, int width, const uint8_t *data) {
	// images don't loop in y direction
	if(y > 33){
		y = 33;
	}
	// i is for the pages, j is for columns and k is for the "pages" in x direction
	int i, j, k;
	int a[32];	//transfers "marked" pixels from one page to another
	
	for(j = 0; j<32; j++){	// initialize the array to only zeros
		a[j] = 0;
	}
	
	int pg = y/8;	// the number of pages the object moves
	y = y % 8;		// number of lines in addition to pages that the object moves
	
	for(k = 0; k < 2; k++){
		for(i = 0; i < 4 - y/8; i++) { // changed the bounds to prevent overflow
			DISPLAY_CHANGE_TO_COMMAND_MODE;

			spi_send_recv(0x22);
			spi_send_recv(i + pg); // i + pg is the page we're moving to
			
			spi_send_recv(x & 0xF);
			spi_send_recv(0x10 | (((x/32 + k)*32 >> 4) & 0xF));
			
			DISPLAY_CHANGE_TO_DATA_MODE;
			// checks whether we have to transfer any pixels from one page to another
			// and sends the right data to the buffer
			if(k == 0){
				for(j = 0; j < x % 32; j++){
					spi_send_recv(0);
				}
				for(j = 0; j < 32 - x%32; j++){
					if(data[i*32 + j] < pow(2, 8- y)){ // if pg is 1 we check if the number is less than 128, if pg is 2 we check 64...
						spi_send_recv( pow(2, y) *  data[i *32 + j] + a[j]); 
						a[j] = 0;
					}
					else{
						spi_send_recv( pow(2, y)  * (data[ i*32 + j] - 128) + a[j]); // if the number is greater than the number we check, we need to change page
						a[j] = (data[i*32 + j] >> (8 - y));	
					}
				}
			}
			else{
				for(j = 32 - x%32; j<32; j++){
					if(data[i*32 + j] < pow(2, 8- y)){ // if pg is 1 we check if the number is less than 128, if pg is 2 we check 64...
						spi_send_recv( pow(2, y) *  data[i *32 + j] + a[j]); 
						a[j] = 0;
					}
					else{
						spi_send_recv( pow(2, y)  * (data[ i*32 + j] - 128) + a[j]); // if the number is greater than the number we check, we need to change page
						a[j] = (data[i*32 + j] >> (8 - y));	
					}
				}
				for(j = x%32; j < 32; j++)
					spi_send_recv(0);
			}
		}
	}
}
/* borrowed from lab files*/
void display_update(void) {
	int i, j, k;
	int c;
	for(i = 0; i < 4; i++) {
		DISPLAY_CHANGE_TO_COMMAND_MODE;
		spi_send_recv(0x22);
		spi_send_recv(i);
		
		spi_send_recv(0x0);
		spi_send_recv(0x10);
		
		DISPLAY_CHANGE_TO_DATA_MODE;
		
		for(j = 0; j < 16; j++) {
			c = textbuffer[i][j];
			if(c & 0x80)
				continue;
			
			for(k = 0; k < 8; k++)
				spi_send_recv(font[c*8 + k]);
		}
	}
}

/* Helper function, local to this file.
   Converts a number to hexadecimal ASCII digits. */
static void num32asc( char * s, int n ) 
{
  int i;
  for( i = 28; i >= 0; i -= 4 )
    *s++ = "0123456789ABCDEF"[ (n >> i) & 15 ];
}
