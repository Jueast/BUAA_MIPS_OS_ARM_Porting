#include "mmu.h"

#include "pmap.h"

#include "printf.h"

#include "error.h"







/* These variables are set by mips_detect_memory() */

u_long maxpa;            /* Maximum physical address */

u_long npage;            /* Amount of memory(in pages) */

u_long basemem;          /* Amount of base memory(in bytes) */

u_long extmem;           /* Amount of extended memory(in bytes) */



Pde* boot_pgdir;



struct Page *pages;

static u_long freemem;



static struct Page_list page_free_list;	/* Free list of physical pages */



void arm_detect_memory()

{

	

	basemem = 64*1024*1024;

	extmem = 0;

	maxpa = basemem;



	npage = maxpa / BY2PG;



	printf("Physical memory: %dK available, ", (int)(maxpa/1024));

	printf("base = %dK, extended = %dK\n", (int)(basemem/1024), (int)(extmem/1024));

}



static void * alloc(u_int n, u_int align, int clear)

{

   //end is defined in mips_vm_init()

	extern char end[]; 	

	u_long alloced_mem;

	// initialize freemem if this is the first time

	if(freemem == 0){
		freemem =(u_long)end;
	}	
	freemem = (u_long)(freemem + (align - 1)) & ~(u_long)(align-1);	
	alloced_mem = freemem;

	///////////////////////////////////////////////////////////////////

	// Your code here:

	//	Step 1: round freemem up to be aligned properly

	//	Step 2: save current value of freemem as allocated chunk

	//	Step 3: increase freemem to record allocation

	//	Step 4: clear allocated chunk if necessary

	//	Step 5: return allocated chunk
	freemem += n;


        if(clear){
	  bzero((void *)alloced_mem, n);
	}
	//how to return allocated chunk?? 

	//save the allocated address to an globe variable

	return (void *)alloced_mem;

	//A?????????????????

}





//

// Given pgdir, the current page directory base,

// walk the 2-level page table structure to find

// the Pte for virtual address va.  Return a pointer to it.

//

// If there is no such directory page:

//	- if create == 0, return 0.

//	- otherwise allocate a new directory page and install it.

//

// This function is abstracting away the 2-level nature of

// the page directory for us by allocating new page tables

// as needed.

// 

// Boot_pgdir_walk cannot fail.  It's too early to fail.

// 

static Pte* boot_pgdir_walk(Pde *pgdir, u_long va, int create)

{

	//v?????????????????

	Pde *targetPde;

	Pte *pageTable;

	targetPde = (Pde *)(&pgdir[PDX(va)]);

	pageTable = KADDR( PDE_ADDR(*targetPde) );
	
	if( *targetPde == 0){

		if(create == 0)

			return 0;

		else{

		//create a new directory page,or allocating new page tables

		//the page size equal to a page table struct size 4096 = 4 * 1024 = 4 * 2^10
		*targetPde = PADDR(alloc(sizeof(struct Page), BY2PG, 1)) | PDE_PT;
		pageTable = KADDR(PTE_ADDR(*targetPde));	





		//what is the function of PTE_R

		//the entries of page directory and page table store physical address
		

		}

	}

	

	return (Pte *)(&pageTable[PTX(va)]);



		



	//A?????????????????

}

//

// Map [va, va+size) of virtual address space to physical [pa, pa+size)

// size should be smaller than 4M;

// in the page table rooted at pgdir.  Size is a multiple of BY2PG.

// Use permission bits perm|PTE_V for the entries.

//

void boot_map_segment(Pde *pgdir, u_long va, u_long size, u_long pa, int perm)

{

	//v?????????????????

	int i, nToMap;
	
	u_int PERM =  PTE_SP;

	Pde *targetPde;

	Pte *pageTable;
	for(;size > 0;size -= 256 * BY2PG){
		targetPde = (Pde *)(&pgdir[PDX(va)]);
		nToMap = size / BY2PG;
		pageTable = KADDR(PDE_ADDR(*targetPde));
	
		for(i = 0; i < nToMap ; i++){
			pageTable[PTX(va)+i] = (Pte *)((pa + i * BY2PG) | PERM);
		}
		va += 256 * BY2PG;
	}
			











	//A?????????????????

}



// Set up a two-level page table:

// 

// This function only sets up the kernel part of the address space

// (ie. addresses >= UTOP).  The user part of the address space

// will be setup later.

//

void arm_vm_init()

{


	extern char end[];


	Pde* pgdir;

	u_int n;

	



	//v?????????????????

	pgdir = (Pde *)alloc(BY2PG,BY2PG,1);

	printf("to memory %x for struct page directory.\n",freemem);

	//mCONTEXT = (int)(*pgdir);




	boot_pgdir = (Pde *)pgdir;

	pages = (struct Page*)alloc(sizeof(struct Page)*npage,BY2PG,1);

	printf("to memory %x for struct Pages.\n",freemem);

	//create pgdir entry for all kernel virtual memory



	boot_pgdir_walk(pgdir,0,1);

	boot_map_segment(pgdir,0,PDMAP,0,0);

	//before this print information cannot be printed

	printf("mips_vm_init:boot_pgdir is %x\n",boot_pgdir);



	printf("pmap.c:\t mips vm init success\n");


	

	//A?????????????????

}











//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// --------------------------------------------------------------

// Tracking of physical pages.

// -

//static void page_initpp(struct Page *pp);

void

page_init(void)

{

	// The exaple code here marks all pages as free.

	// However this is not truly the case.  What memory is free?

	//  1) Mark page 0 as in use(for good luck) 

	//  2) Mark the rest of base memory as free.

	//  3) Then comes the IO hole [IOPHYSMEM, EXTPHYSMEM) => mark it as in use

	//     So that it can never be allocated.      

	//  4) Then extended memory(ie. >= EXTPHYSMEM):

	//     ==> some of it's in use some is free. Where is the kernel?

	//     Which pages are used for page tables and other data structures?    

	//

	// Change the code to reflect this.

	//v?????????????????

	int i;
        struct Page * currentPage;
	LIST_INIT (&page_free_list);
	freemem = ROUND(freemem, BY2PG);
	for(i=freemem/BY2PG;i<npage;i++){
	   currentPage =(struct Page *)((u_long)pages + (u_long)i *(u_long) sizeof(struct Page));
	   LIST_INSERT_HEAD(&page_free_list, currentPage, pp_link);
	}
	
	// TODO: mark used pages as used.




	//A?????????????????

	

}



//

// Initialize a Page structure.

//

static void

page_initpp(struct Page *pp)

{

	bzero(pp, sizeof(*pp));

}





//

// Allocates a physical page.

//

// *pp -- is set to point to the Page struct of the newly allocated

// page

//

// RETURNS 

//   0 -- on success

//   -E_NO_MEM -- otherwise 

//

// Hint: use LIST_FIRST, LIST_REMOVE, page_initpp()

// Hint: pp_ref should not be incremented 

int

page_alloc(struct Page **pp)

{

	// Fill this function in

	//v?????????????????

	//pick the head of the free list

	//then remove the first(head) element of the page free list :page_free_list

	if(LIST_FIRST(&page_free_list) != 0){

		//see page_check to see why use **pp here

	        (*pp) = LIST_FIRST(&page_free_list);
		LIST_REMOVE(*pp, pp_link);
		page_initpp(*pp);
		//(*pp)->pp_ref++;
		bzero(page2pa(*pp), BY2PG);		

//page initialization
			

		return 0;

	}

	

	//if know the address of the allocated Page ,pp pointed to .

	//then we can get the index of pp in Page arrays:pages.

	//then we can caculate the real physical address range of pp.

	//A?????????????????

	

		

	

	return -E_NO_MEM;

}





//

// Return a page to the free list.

// (This function should only be called when pp->pp_ref reaches 0.)

//

void

page_free(struct Page *pp)

{

	// Fill this function in

	//v?????????????????

	//when to free a page ,just insert it to the page_free_list.

	

	



	

	LIST_INSERT_HEAD(&page_free_list,pp,pp_link);

	//A?????????????????

}







void

page_decref(struct Page *pp)

{

	if (--pp->pp_ref == 0)

		page_free(pp);

}





//

// This is boot_pgdir_walk with a different allocate function.

// Unlike boot_pgdir_walk, pgdir_walk can fail, so we have to

// return pte via a pointer parameter.

//

// Stores address of page table entry in *pte.

// Stores 0 if there is no such entry or on error.

// 

// RETURNS: 

//   0 on success

//   -E_NO_MEM, if page table couldn't be allocated

//





int

pgdir_walk(Pde *pgdir, u_long va, int create, Pte **ppte)

{

	// Fill this function in

	//v?????????????????

	Pde *targetPde;

	Pte *pageTable;
	struct Page * pageTablePage = 0;
	targetPde = (Pde *)(&pgdir[PDX(va)]);

	pageTable = (Pte *)KADDR(PDE_ADDR(*targetPde));
	
	*ppte = 0;
	if(*targetPde == 0){
		if(create == 0)
			return -E_NO_MEM;
		else{
		    if(page_alloc(&pageTablePage) == 0){
			pageTablePage->pp_ref++;
 		    	*targetPde = page2pa(pageTablePage) | PDE_PT;
		    	pageTable = (Pte *)KADDR(PDE_ADDR(*targetPde));     	
		    } else{
			return -E_NO_MEM;
		    }
			


		}
	}
			//init the newly allocated physical page,used as Page Table. 

	*ppte = (Pte *)(&pageTable[PTX(va)]);		



	

	
	
	//printf("pgdir_walk succeed.allocated page is %x\n",pageTable);



	//A?????????????????

        return 0;

}






//

// Map the physical page 'pp' at virtual address 'va'.

// The permissions (the low 12 bits) of the page table

//  entry should be set to 'perm|PTE_P'.

//

// Details

//   - If there is already a page mapped at 'va', it is page_remove()d.

//   - If necesary, on demand, allocates a page table and inserts it into 'pgdir'.

//   - pp->pp_ref should be incremented if the insertion succeeds

//

// RETURNS: 

//   0 on success

//   -E_NO_MEM, if page table couldn't be allocated

//

// Hint: The TA solution is implemented using

//   pgdir_walk() and and page_remove().

//

/*int

page_insert(Pde *pgdir, struct Page *pp, u_long va, u_int perm) 

{

	// Fill this function in

	//v?????????????????

	Pte *ptEntry;

	struct Page *oldPage;

	if(pgdir_walk(pgdir,va,1,&ptEntry) != 0){

		return -E_NO_MEM;

	}

	if(*ptEntry != 0){

		oldPage = pa2page(*ptEntry);

		page_decref(oldPage);

	}

	*ptEntry = page2pa(pp)|perm|PTE_V;

	pp->pp_ref++;

	//A?????????????????

	printf("%x page_insert succeed!\n",pp);

	return 0;

}*/



int
page_insert(Pde *pgdir, struct Page *pp, u_long va, u_int perm) 

{	// Fill this function in

	u_int PERM;	

	Pte *pgtable_entry;

	PERM = PTE_SP;

	

        pgdir_walk(pgdir, va, 0, &pgtable_entry);
	

	if(pgtable_entry!=0 &&(*pgtable_entry & PTE_SP) == PTE_SP) {

		if(pa2page(*pgtable_entry)!=pp) {
			page_remove(pgdir,va);
		}
		else
		{

			*pgtable_entry = (page2pa(pp)|PERM);			

			return 0;

		}
	
	}
	if(pgdir_walk(pgdir, va, 1, &pgtable_entry)!=0)

		return -E_NO_MEM;// panic("page insert wrong.\n");

	*pgtable_entry = (page2pa(pp)|PERM);

	pp->pp_ref++;

	return 0;

}







//return the Page which va map to.

struct Page*

page_lookup(Pde *pgdir, u_long va, Pte **ppte)

{

	struct Page *ppage;

	Pte *pte;



        pgdir_walk(pgdir, va, 0, &pte);

       	if(pte==0) return 0;

       	if((*pte & PTE_SP)==0) return 0;       //the page is not in memory.

       	ppage = pa2page(*pte);

	if(ppte) *ppte = pte;

	return ppage;

}





//

// Unmaps the physical page at virtual address 'va'.

//

// Details:

//   - The ref count on the physical page should decrement.

//   - The physical page should be freed if the refcount reaches 0.

//   - The pg table entry corresponding to 'va' should be set to 0.

//     (if such a PTE exists)

//   - The TLB must be invalidated if you remove an entry from

//	   the pg dir/pg table.

//

// Hint: The TA solution is implemented using

//  pgdir_walk(), page_free(), tlb_invalidate()

//



void

page_remove(Pde *pgdir, u_long va) 

{

	// Fill this function in

	//v?????????????????

	Pte * ppte;

	u_long pa;

	

	pa = va2pa(pgdir,va);

	//PTX is error,should use PPN(pa),PTX just have 12 bit valid number

	//#define PTX(va)		((((u_long)(va))>>12) & 0x03FF)

	//#define PPN(va)		(((u_long)(va))>>12)

	//page_decref(&pages[PTX(pa)]);

	page_decref(pa2page(pa));



	pgdir_walk(pgdir,va,0, &ppte);

	*ppte = 0;



	//A?????????????????

	return;

}







void

page_check(void)

{

	struct Page *pp, *pp0, *pp1, *pp2;

	struct Page_list fl;



	// should be able to allocate three pages

	pp0 = pp1 = pp2 = 0;

	assert(page_alloc(&pp0) == 0);

	assert(page_alloc(&pp1) == 0);

	assert(page_alloc(&pp2) == 0);



	assert(pp0);

	assert(pp1 && pp1 != pp0);

	assert(pp2 && pp2 != pp1 && pp2 != pp0);



	// temporarily steal the rest of the free pages

	fl = page_free_list;

	//after this this page_free list must be empty!!!!

	LIST_INIT(&page_free_list);

	



	// should be no free memory

	assert(page_alloc(&pp) == -E_NO_MEM);

	printf("start page_insert\n");

	// there is no free memory, so we can't allocate a page table 

	assert(page_insert(boot_pgdir, pp1, KLIM, 0) < 0);



	// free pp0 and try again: pp0 should be used for page table

	page_free(pp0);

	//va address 0x0 pageout error "too low"

 	assert(page_insert(boot_pgdir, pp1, KLIM, 0) == 0);

 	//assert(page_insert(boot_pgdir, pp1, 0x10000, 0) == 0);

	assert(PDE_ADDR(boot_pgdir[PDX(KLIM)]) == page2pa(pp0));

	printf("va2pa(boot_pgdir, KLIM) is %x\n",va2pa(boot_pgdir, KLIM));

	printf("page2pa(pp1) is %x\n",page2pa(pp1));

	assert(va2pa(boot_pgdir, KLIM) == page2pa(pp1));

	assert(pp1->pp_ref == 1);


	// should be able to map pp2 at BY2PG because pp0 is already allocated for page table

	assert(page_insert(boot_pgdir, pp2, BY2PG+KLIM, 0) == 0);
	
	assert(va2pa(boot_pgdir, BY2PG+KLIM) == page2pa(pp2));

	assert(pp2->pp_ref == 1);

	// should be no free memory

	assert(page_alloc(&pp) == -E_NO_MEM);

//printf("why\n");

	// should be able to map pp2 at BY2PG because it's already there

	assert(page_insert(boot_pgdir, pp2, BY2PG+KLIM, 0) == 0);
	
	
	assert(va2pa(boot_pgdir, BY2PG+KLIM) == page2pa(pp2));

	assert(pp2->pp_ref == 1);

//printf("It is so unbelivable\n");

	// pp2 should NOT be on the free list

	// could happen in ref counts are handled sloppily in page_insert

	assert(page_alloc(&pp) == -E_NO_MEM);

	// should not be able to map at PDMAP because need free page for page table

	assert(page_insert(boot_pgdir, pp0, PDMAP+KLIM, 0) < 0);
	// insert pp1 at BY2PG (replacing pp2)
	assert(page_insert(boot_pgdir, pp1, BY2PG+KLIM, 0) == 0);


	// should have pp1 at both 0 and BY2PG, pp2 nowhere, ...

	assert(va2pa(boot_pgdir, KLIM) == page2pa(pp1));

	assert(va2pa(boot_pgdir, BY2PG+KLIM) == page2pa(pp1));

	// ... and ref counts should reflect this

	assert(pp1->pp_ref == 2);

	printf("pp2->pp_ref %d\n",pp2->pp_ref);

	assert(pp2->pp_ref == 0);

	printf("end page_insert\n");



	// pp2 should be returned by page_alloc

	assert(page_alloc(&pp) == 0 && pp == pp2);



	// unmapping pp1 at 0 should keep pp1 at BY2PG

	page_remove(boot_pgdir, KLIM);

	assert(va2pa(boot_pgdir, KLIM) == ~0);

	assert(va2pa(boot_pgdir, BY2PG+KLIM) == page2pa(pp1));

	assert(pp1->pp_ref == 1);

	assert(pp2->pp_ref == 0);



	// unmapping pp1 at BY2PG should free it

	page_remove(boot_pgdir, BY2PG+KLIM);

	assert(va2pa(boot_pgdir, KLIM) == ~0);

	assert(va2pa(boot_pgdir, BY2PG+KLIM) == ~0);

	assert(pp1->pp_ref == 0);

	assert(pp2->pp_ref == 0);



	// so it should be returned by page_alloc

	assert(page_alloc(&pp) == 0 && pp == pp1);



	// should be no free memory

	assert(page_alloc(&pp) == -E_NO_MEM);



	// forcibly take pp0 back

	assert(PDE_ADDR(boot_pgdir[PDX(KLIM)]) == page2pa(pp0));

	boot_pgdir[PDX(KLIM)] = 0;

	assert(pp0->pp_ref == 1);

	pp0->pp_ref = 0;



	// give free list back

	page_free_list = fl;



	// free the pages we took

	page_free(pp0);

	page_free(pp1);

	page_free(pp2);



	printf("page_check() succeeded!\n");

}









