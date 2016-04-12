#ifndef _MMU_H_
#define _MMU_H_


/*
 * This file contains:
 *
 *	Part 1.  ARM definitions.
 *	Part 2.  Our conventions.
 *	Part 3.  Our helper functions.
 */

/*
 * Part 1.  ARM definitions.
 */
#define BY2PG		4096		// bytes to a page
#define PDMAP		(4*1024*1024)	// bytes mapped by a page directory entry
#define PGSHIFT		12
#define PDSHIFT		20		// log2(PDMAP)
#define PDX(va)		((((u_long)(va))>>20) & 0x0FFF)
#define PTX(va)		((((u_long)(va))>>12) & 0x0FF)
#define PDE_ADDR(pde)   ((u_long)(pde)&~0x3FF)
#define PTE_ADDR(pte)	((u_long)(pte)&~0xFFF)

// page number field of address
#define PPN(va)		(((u_long)(va))>>12)
#define VPN(va)		PPN(va)

#define VA2PFN(va)		(((u_long)(va)) & 0xFFFFF000 ) // va 2 PFN for EntryLo0/1
#define PTE2PT		1024
//$#define VA2PDE(va)		(((u_long)(va)) & 0xFFC00000 ) // for context

/* Page Table/Directory Entry flags
 *   these are defined by the hardware
 *   bits[1:0] identify the descriptor type.
 *   PDE_* means the first-level descriptor
 *   PTE_* means the second-level descriptor
 */
#define PDE_DS          0x01  // Page Table
#define PTE_SP		0x02  // Small Page
#define INVALID		0x00  // INVALID for both PTE and PDE

/*
 * Part 2.  Our conventions.
 */

/*
 o     4G ----------->  +----------------------------+------------0x100000000
 a                      .                            .                           |
 a                      |~~~~~~~~~~~~~~~~~~~~~~~~~~~~|                           |
 a                      |                            |                           |
 o    KERNBASE   -----> +----------------------------+                           |
 o                      | Interrupts & Exception     |     	                \|/
 a     0 ------------>  +----------------------------+ -----------------------------
 o
*/

#define KERNBASE 0x8000

#define KSTACKTOP (VPT-0x100)
#define KSTKSIZE (8*BY2PG)
#define KLIM 0x4000000

#define UVPT (ULIM - PDMAP)
#define UPAGES (UVPT - PDMAP)
#define UENVS (UPAGES - PDMAP)




#ifndef __ASSEMBLER__

/*
 * Part 3.  Our helper functions.
 */
#include "types.h"
void bcopy(const void *, void *, size_t);
void bzero(void *, size_t);

extern char bootstacktop[], bootstack[];

extern u_long npage;

typedef u_long Pde;
typedef u_long Pte;

extern volatile Pte* vpt[];
extern volatile Pde* vpd[];

#define PADDR(kva)						\
({								\
	u_long a = (u_long) (kva);				\
	if (a >= KLIM)					\
		panic("PADDR called with invalid kva %08lx", a);\
	(a);						\
})


// translates from physical address to kernel virtual address
#define KADDR(pa)						\
({								\
	u_long ppn = PPN(pa);					\
	if (ppn >= npage)					\
		panic("KADDR called with invalid pa %08lx", (u_long)pa);\
	(pa);					\
})

#define assert(x)	\
	do {	if (!(x)) panic("assertion failed: %s", #x); } while (0)

#endif //!__ASSEMBLER__
#endif // !_MMU_H_
