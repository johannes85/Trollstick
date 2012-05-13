/**
 * Trollstick
 * v0.1
 *
 * by DomesticHacks
 * http://domestichacks.info/
 * http://www.youtube.com/DomesticHacks
 *
 * License:
 * Creative Commons: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
 * http://creativecommons.org/licenses/by-nc-sa/3.0/
 */

#include <avr/io.h>
//#include <avr/pgmspace.h>
#include <string.h>
#include <util/delay.h>
#include "usb_keyboard.h"


#define LED_CONFIG	(DDRD |= (1<<6))
#define LED_ON		(PORTD &= ~(1<<6))
#define LED_OFF		(PORTD |= (1<<6))
#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))
#define CPU_16MHz       0x00
#define CPU_8MHz        0x01

// Hier könnt ihr die Wartezeit in Sekunden angeben
static uint16_t waitingtime = 10;

// Hier könt ihr die Sätze definieren, die gesendet werden sollen.
// Ihr müsst die Variable "stringsCount" auf die Anzahl der Sätze setzen.
static char* strings[] = {
	"Never gonna give you up",
	"never gonna let you down",
	"never gonna run around and desert you ",
	"never gonna make you cry",
	"never gonna say goodbye",
	"never gonna tell a lie and hurt you",
};
static uint8_t stringsCount = 6;


uint8_t stringIndex;
uint16_t waited;
char *currString;
char currChar;

int main(void)
{
	CPU_PRESCALE(CPU_16MHz);
	
	LED_CONFIG;	
	LED_OFF;
	
	LED_ON;
	usb_init();
	while (!usb_configured())
	_delay_ms(1000);
	LED_OFF;
	
	stringIndex = 0;
	
	while (1) {	
		
		for(waited=0;waited<waitingtime;waited++){
			_delay_ms(1000);
		}
	
		currString = strings[stringIndex];
		
		for (int a=0;a<strlen(currString);a++){
		
			currChar = currString[a];
			
			uint8_t keycode = KEY_PERIOD;
			uint8_t modifier = 0;
			
			switch(currChar){
				// Deutsch englisch Umwandlung
				//Y => Z
				case 89: 
					currChar = 90;
					break;
				//Z => Y
				case 90: 
					currChar = 89;
					break;
				//y => z
				case 121: 
					currChar = 122;
					break;
				//z => y
				case 122: 
					currChar = 121;
					break;
					
				case 32: 
					keycode = KEY_SPACE;
					break;
				case 34: 
					keycode = KEY_QUOTE;
					break;
				case 44: 
					keycode = KEY_COMMA;
					break;
				case 45: 
					keycode = KEY_MINUS;
					break;
				case 46: 
					keycode = KEY_PERIOD;
					break;
				case 48: 
					keycode = KEY_0;
					break;
				case 61: 
					keycode = KEY_EQUAL;
					break;
				case 92: 
					keycode = KEY_BACKSLASH;
					break;
			}
			
			//1-9
			if(currChar >= 49 && currChar <= 57){
				keycode = (currChar - 49) + 89;
			}
			
			//A-Z
			if(currChar >= 65 && currChar <= 90){
				modifier = KEY_SHIFT;
				keycode = (currChar - 65) + 4;
			}
			
			//a-z
			if(currChar >= 97 && currChar <= 122){
				keycode = (currChar - 97) + 4;
			}
			
			usb_keyboard_press(keycode,modifier);
		}
		usb_keyboard_press(KEY_ENTER,0);
		
		stringIndex++;
		if(stringIndex >= stringsCount){
			stringIndex = 0;
		}
	}
}





