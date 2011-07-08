#ifndef PAGING_H
#define PAGING_H

#include "common.h"
#include "isr.h"

typedef struct page
{
	u32int present	: 1;	// Page present in memory
	u32int rw	: 1;	// Read-only if clear, rw if set
	u32int user	: 1;	// Supervisor level only if clear
	u32int accessed	: 1;	// Has the page been accessed since last refresh
	u32int dirty	: 1;	// Has the page been written to since last refresh
	u32int unused	: 7;	// Zeroed section
	u32int frame	: 20;	// Frame address (shifted right 12 bits)
} page_t;

typedef struct page_table
{
	page_t pages[1024];
} page_table_t

typedef struct page_directory
{
	// Array of pointers to pagetable
	page_table_t *tables[1024];

	// Array of pointer to pagetables, but gives their physical
	// location for loaing into CR3
	u32int	tables_Physical[1024];

	// Physical address of tablesPhysical
	u32int physicalAddr;
} page_directory_t

// Initiliasation
void initialize_paging();

// Load page directory into CR3
void switch_page_directory(page_directory_t *new);

// Retrieves a pointer to requester page
// If(make), create page
page_T *get_page(u32int address, int make, page_directory_t *dir);

// Page fault handler
void page_fault(registers_t regs);