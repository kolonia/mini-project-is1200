/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int mytime = 0x5957;
int timeoutcount = 0;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}
/* Lab-specific initialization goes here */
void labinit( void )
{
  //TRISE &= ~0xff;
    volatile int* trise = (volatile int*) 0xbf886100;
	*trise &= 0xffffff00;
	TRISF |= 0x00000002;
	
	
	TRISD |= 0x00000fe0;
	
	T2CON = 0x70;
	PR2 = 50000;
	TMR2 = 0;
	T2CONSET = 0x8050;
    return;
}
int num = 567;
int rand(int n){
	return (num%n) + 1;
}
int count = 0;
short sequence[96];
char level = 0;
int idiot = 2;
void add_instruction(){
	
	if(level == 0){
		sequence[count] = idiot;
	}
	else{
		sequence[count] = rand(4);
	}
	count = count + 1;
}
int increaseSpeed = 0;
void display_instructions(){
	int i=0;
	int y;
	if((level == 2) && (increaseSpeed < 30)){
		increaseSpeed = increaseSpeed + 5;
	}
	while( i < count ){
		for(y = 0; y <32; y++){
			display_update();
			display_image((4 - sequence[i])*32, y, icon);
			delay(30 - increaseSpeed);

		}
		
		i = i + 1;
		
	}
}


char correct = 1;
void check_input(){
	int i = 0;
	while( i < count){
		volatile int check_btns = getbtns(); 
		while(check_btns == 0){
			num = num + 1;
			check_btns = getbtns();
		}
		
		delay(300);
		if( (check_btns & 0x1) && (sequence[i] == 1)){
			
		}
		else if( ((check_btns >> 1) & 0x1) && (sequence[i] == 2)){
			
		}
		else if( ((check_btns >> 2) & 0x1) && (sequence[i] == 3)){
			
		}
		else if( ((check_btns >> 3) & 0x1) && (sequence[i] == 4)){
			
		}
		else {
			display_string(0, "GAME OVER");
			display_update();
			correct = 0;
			delay(1000);
			break;
		}
		display_string(0, "");
		display_update();
		i = i + 1;
		check_btns = 0;
	}
}	
/*	int n = 0;
	char correct = 1; //check in main funct	
	while( n < count ){	
		display_string(n, "       lol");
		display_update();
			
		volatile int check_btns = getbtns(); 
		int gtbtns = 0;
		while(1){
			num = num + 1; //global variable for random
			check_btns = getbtns(); 
			if(check_btns){				
				gtbtns = check_btns; //avoid fast players :D
				break;
			} 
		}
		if(((gtbtns & 0x1)) && (sequence[n] == 1) ){
			display_string(0, "bt1 is pressed");
		}
		else if(((gtbtns >> 1) & 0x1) && (sequence[n] == 2)){
			display_string(0, "bt2 is pressed");
		//	display_string(0, "correct");
		}
		else if(((gtbtns >> 2) & 0x1) && (sequence[n] == 3)){
			display_string(0, "bt3 is pressed");
			//display_string(0, "correct");
		}
		else if(((gtbtns >> 3) & 0x1) && (sequence[n] == 4)){ 
			display_string(0, "bt4 is pressed");
			
		}
		else{
			display_string(0, "wrong");
			display_update();
			correct = 0; 
			//	count = 0;
				//break;
		}
			
		n = n + 1;
	}	
}
/* This function is called repetitively from the main program */
void labwork( void )
{ 	char option = 1;
	volatile int btns = getbtns(); 
	while((btns & 0x1) == 0){
		btns = getbtns();
		if((btns >> 1) & 0x3){
			option = option + 1;
			delay(250);
		}
		option = option % 2;
		if(option == 1){
			display_string(0, "      MENU");
			display_string(2,"     oStart");
			display_string(3,"   High score");
		}
		else{
			display_string(0, "      MENU");
			display_string(2,"      Start");
			display_string(3,"  oHigh score");
		}
		display_update();
	}
	delay(250);
	display_string(0, "");
	display_string(1,"");
	display_string(2,"");
	display_string(3,"");
	display_update();
	if(option == 1){
		btns = getbtns();
		while((btns & 0x1) == 0){
			btns = getbtns();
			if((btns >> 1) & 0x1){
				level = level + 1;
				delay(250);
			}
			if((btns >> 2) & 0x1){
				if(level > 0){
					level = level - 1;
				}
				else{
					level = 2;
				}
				delay(250);
			}
			level = level % 3;
			if(level == 0){
				display_string(0, "     LEVEL:");
				display_string(1,"    oIdiot");
				display_string(2,"     Normal");
				display_string(3," Not for Losers");
				display_update();
			}
			else if(level == 1){
				display_string(0, "     LEVEL:");
				display_string(1,"     Idiot");
				display_string(2,"    oNormal");
				display_string(3," Not for Losers");
				display_update();
			}
			else{
				display_string(0, "     LEVEL:");
				display_string(1,"     Idiot");
				display_string(2,"     Normal");
				display_string(3,"oNot for Losers");
				display_update();
			}
			display_update();
		}
		display_string(0, "");
		display_string(1,"");
		display_string(2,"");
		display_string(3,"");
		display_update();
		while(correct){
			display_update();
			delay(1000);
			add_instruction();
			if( level == 2){
				add_instruction();
			}
			display_instructions();
			check_input();
			num = num + 1;
		}
		level = 0;
		option = 1;
		correct = 1;
		count = 0;
		display_string(0, "");
		display_string(1,"");
		display_string(2,"");
		display_string(3,"");
		display_update();
		
	}
	/*for(y=0; y<=32; y++){
		if(IFS(0)){
			IFS(0) = 0;
			if(timeoutcount == 10){
				display_update();
				display_image(y, icon);
				timeoutcount = -2;
			}	  
			timeoutcount = timeoutcount + 2;
		}
	}
	
	
	//display_image(y, icon);
	//display_update();
	/* 
	char sequence[100];
	static char *p = sequence;
	*sequence = '1';
	p++;
	*p = '2';
	for (int i=0; i<0;i++) 
		sequence[i]=9;
//	sequence = sequence + 1;
	*sequence = '2';
	sequence = sequence + 1;
	*sequence = '3';
	sequence = sequence - 2;
	//sequence = sequence - 4;
	
  while(*sequence != 0x0){		
	  if(IFS(0)){
		IFS(0) = 0;
		if(timeoutcount == 10){
		if(*sequence == '1'){display_string(0, "Button 1");}
		if(*sequence == '2'){display_string(0, "Button 2");}
		if(*sequence == '3'){display_string(0, "Button 3");}
		if(*sequence == '4'){display_string(0, "Button 4");}
		sequence = sequence + 1;
		//time2string( textstring, mytime );
		//display_string( 0, "yolo" );
		display_update();
		//tick( &mytime );
		volatile int*  porte = (volatile int*) 0xbf886110;
		*porte= *porte +1;
		timeoutcount = -2;
		}	  
		timeoutcount = timeoutcount + 2;
	  }
  }
  
    delay(1000);
	display_string(0, "start");
    display_update();
	
	void check_input(){
	sequence = sequence - count;
	volatile int getbtns = getbtns(); 
	int gtbtns = 0;
	char correct = 1; //check in main funct
	while(*sequence != 0x0){	
		while(1){
			num = num + 1; //global variable for random
			if(getbtns){
				gtbtns = getbtns; //avoid fast players :D
				break;
			} 
		}
		if((gtbtns & 0x1) && *sequence == 1 ){
		}
		else if((gtbtns >> 1) & 0x1 && *sequence == 2){
		}
		else if((gtbtns >> 2) & 0x1 && *sequence == 3){
		}
		else if((gtbtns >> 3) & 0x1 && *sequence == 4){ 
		}
		else{
			correct = 0; 
			break;
		}
		sequence = sequence + 1;
	}
}
	
	
	//delay(10000);
	*/
  
}
