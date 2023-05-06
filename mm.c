#include "mm.h"
#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include <unistd.h> /*for getpagesize*/
#include <sys/mman.h>

static vm_page_for_families_t *first_vm_page_for_families = NULL;
static size_t SYSTEM_PAGE_SIZE = 0;

void mm_init(){
    SYSTEM_PAGE_SIZE = getpagesize();
    
    printf("Init: VM Page size = %lu\n\n", SYSTEM_PAGE_SIZE);
}

//Function to request vm page from kernel
static void *mm_get_new_vm_page_from_kernel(int units){
    char *vm_page = mmap(
        0,
        units * SYSTEM_PAGE_SIZE,
        PROT_READ|PROT_WRITE|PROT_EXEC,
        MAP_ANON|MAP_PRIVATE,
        0, 0
    );

    if(vm_page == MAP_FAILED){
        printf("Error: VM Page allocation Failed\n");
        return NULL;
    }

    memset(vm_page, 0, units * SYSTEM_PAGE_SIZE);
    
    return (void *)vm_page;
}

static void mm_return_vm_page_to_kernel(void *vm_page, int units){
    if(munmap(vm_page, units * SYSTEM_PAGE_SIZE))
        printf("Error: Could not munmap VM page to kernel");
}

void mm_instantiate_new_page_family(char *struct_name, uint32_t struct_size){
	
	vm_page_family_t *vm_page_family_curr = NULL;
	vm_page_for_families_t *new_vm_page_for_families = NULL;
	
	if(struct_size > SYSTEM_PAGE_SIZE){
		
		printf("Error : %s() structure %s Size exceeds system page size\n", __FUNCTION__, struct_name);
		return;
	}
	
	if(!first_vm_page_for_families){
		
		first_vm_page_for_families = (vm_page_for_families_t *)mm_get_new_vm_page_from_kernel(1);
		first_vm_page_for_families->next = NULL;
		printf("First virtual memory page for families is created,\n");
		
		strncpy(first_vm_page_for_families->vm_page_family[0].struct_name, struct_name, MM_MAX_STRUCT_NAME);
		first_vm_page_for_families->vm_page_family[0].struct_size = struct_size;
		printf("Virtual memory to %s is allocated\n", first_vm_page_for_families->vm_page_family[0].struct_name);
			
		return;
	}
	
	uint32_t count = 0;
	
	ITERATE_PAGE_FAMILIES_BEGIN(first_vm_page_for_families, vm_page_family_curr) {
	
		if(strncmp(vm_page_family_curr->struct_name, struct_name, MM_MAX_STRUCT_NAME) != 0){
			count++;
			continue;
		}
		
		assert(0);
	} ITERATE_PAGE_FAMILIES_END(first_vm_page_for_families, vm_page_familiy_curr);
	
	if(count == MAX_FAMILIES_PER_VM_PAGE){
		
		new_vm_page_for_families = (vm_page_for_families_t *)mm_get_new_vm_page_from_kernel(1);
		new_vm_page_for_families->next = first_vm_page_for_families;
		first_vm_page_for_families = new_vm_page_for_families;
		vm_page_family_curr = &first_vm_page_for_families->vm_page_family[0];
		
		printf("Another virtual memory page for families is created,\n");
	}
	
	strncpy(vm_page_family_curr->struct_name, struct_name, MM_MAX_STRUCT_NAME);
	vm_page_family_curr->struct_size  = struct_size;
	//vm_page_family_curr->first_page = NULL;
	
	printf("Virtual memory to %s is allocated\n", vm_page_family_curr->struct_name);
}

/*int main(){

    mm_init();
    printf("VM Page size = %lu\n", SYSTEM_PAGE_SIZE);
    void *addr1 = mm_get_new_vm_page_from_kernel(1);
    void *addr2 = mm_get_new_vm_page_from_kernel(1);
    printf("page 1 = %p, page 2 = %p\n", addr1, addr2);
    return 0;
}*/
