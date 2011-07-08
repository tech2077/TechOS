// monitor.c -- define monitor interface functions

#include "monitor.h"

u16int	*video_memory = (u16int *)0xB8000;
u8int	cursor_x = 0, cursor_y = 0;

static void move_cursor()
{
	// screen is 80 characters wide and 25 high
	u16int cursorLocation = cursor_y * 80 + cursor_x;
	outb(0x3D4, 14);			// Tell VGA we are setting high cursor byte
	outb(0x3D5, cursorLocation >> 8);	// Send high byte
	outb(0x3D4, 15);			// Tell VGA controller we are setting low byte
	outb(0x3D5, cursorLocation);		// Send low byte
}

static void scroll()
{
	// Create space character with default color attibutes.
	u8int attributeByte = (0 /*black*/ << 4) | (2 /*green*/ & 0x0F);
	u16int blank = (0x20 /*space*/ | (attributeByte << 8));

	// Row 25 is the end, so we must scroll up
	if(cursor_y >= 25) {
		// Move the current text chunk that makes up the screen back
		// in the buffer by a line
		int i;
		for(i = 0*80; i < 24*80; i++) {
			video_memory[i] = video_memory[i+80];
		}

		// The last line is blanked
		for(i = 24*80; i < 25*80; i++) {
			video_memory[i] = blank;
		}

		cursor_y = 24;
	}
}

void monitor_put(char c)
{
	// Background is black and foreground is green
	u8int backColor = 0;
	u8int foreColor = 2;

	// The attribute byte consists of two 4 bit nibbles, lower being foreground color,
	// higher being background color
	u8int attributeByte =(backColor << 4 | foreColor & 0x0F);

	// The attribute byte is the top byte of the word sent to the VGA controller
	u16int attribute = attributeByte << 8;
	u16int *location;

	// Handle backspace by moving cursor back one character
	if(c == 0x08 && cursor_x)
		cursor_x--;

	// Handle a tab by increasing cursor_x by 8 where it is divisible by 8
	else if(c == 0x09)
		cursor_x = (cursor_x+8) & ~(8-1);

	// Handle carriage return
	else if(c == '\r')
		cursor_x = 0;

	// Handle newline
	else if(c == '\n') {
		cursor_x = 0;
		cursor_y++;
	}

	else if(c >= ' ') {
		location = video_memory + (cursor_y*80 + cursor_x);
		*location = c | attribute;
		cursor_x++;
	}

	// Check if newline is needed because reached end of screen
	if(cursor_x >= 80) {
		cursor_x = 0;
		cursor_y++;
	}

	scroll();

	move_cursor();
}

// Clears the screen, by copying lots of spaces to the framebuffer.
void monitor_clear()
{
	// Make an attribute byte for the default colours
	u8int attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
	u16int blank = 0x20 /* space */ | (attributeByte << 8);

	int i;
	for(i = 0; i < 80*25; i++) {
		video_memory[i] = blank;
	}

	// Move hardware cursor to start
	cursor_x = 0;
	cursor_y = 0;
	move_cursor();
}

// Outputs a null-terminated ASCII string to the monitor.
void monitor_write(char *c)
{
   int i = 0;
   while (c[i]) 
   {
       monitor_put(c[i++]);
   }
}

// Outputs a hexidecimal number as string
void monitor_write_hex(u32int n)
{
    s32int tmp;

    monitor_write("0x");

    char noZeroes = 1;

    int i;
    for (i = 28; i > 0; i -= 4)
    {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0)
        {
            continue;
        }
    
        if (tmp >= 0xA)
        {
            noZeroes = 0;
            monitor_put (tmp-0xA+'a' );
        }
        else
        {
            noZeroes = 0;
            monitor_put( tmp+'0' );
        }
    }
  
    tmp = n & 0xF;
    if (tmp >= 0xA)
    {
        monitor_put (tmp-0xA+'a');
    }
    else
    {
        monitor_put (tmp+'0');
    }

}

// Outputs decimal as string
void monitor_write_dec(u32int n)
{

    if (n == 0)
    {
        monitor_put('0');
        return;
    }

    s32int acc = n;
    char c[32];
    int i = 0;
    while (acc > 0)
    {
        c[i] = '0' + acc%10;
        acc /= 10;
        i++;
    }
    c[i] = 0;

    char c2[32];
    c2[i--] = 0;
    int j = 0;
    while(i >= 0)
    {
        c2[i--] = c[j++];
    }
    monitor_write(c2);

}
