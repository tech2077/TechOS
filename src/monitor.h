// monitor.h -- defines the interface for the monitor

#ifndef MONITOR_H
#define MONITOR_H

#include "common.h"

// Write a single character out to the screen.
void monitor_put(char c);

// Clear the screen to all black.
void monitor_clear();

// Output a null-terminated ASCII string to the monitor.
void monitor_write(char *c);

// Outputs a hexidecimal number as string
void monitor_write_hex(u32int n);

// Outputs decimal as string
void monitor_write_dec(u32int n);

#endif // MONITOR_H
