// keyboard.c -- basic ps2 keyboard driver

#include "keyboard.h"
#include "isr.h"
#include "monitor.h"

u16int key_press;

u8int shift_state =0;

char lowercase[256] = {0, '?', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', '?', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', '?', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', '?', '*', '?', ' '};
char uppercase[256] = {0, '?', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'T', 'I', 'O', 'P', '{', '}', '\n', '?', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', '?', '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', '?', '*', '?', ' '};
 
static void keyboard_callback(registers_t regs)
{
	key_press = inb(0x60);
	char new_char;
	
	switch(key_press) {
		case 0x2a:
			shift_state = 1;
			break;
		
		case 0xAA:
			shift_state = 0;
			break;
			
		default:
			if(key_press & 0x80) {
				
			} else {
			new_char = (shift_state ? uppercase:lowercase)[key_press];
			monitor_put(new_char);
			}
	}
}

void init_keyboard()
{
	// Register keyboard interrupt
	register_interrupt_handler(IRQ1, &keyboard_callback);
}
