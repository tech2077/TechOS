// main.c -- Kernel entry point

#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "keyboard.h"

int main(struct multiboot *mboot_ptr)
{
	init_descriptor_tables();
	
	monitor_clear();
	monitor_write("Hello From Kernel");
	
	asm volatile("sti");
	init_keyboard();

	return 0xDEADBABA;

}
