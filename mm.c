#include <stdio.h>
#include <memory.h>
#include <unistd.h>     // for getpagesize()
#include <sys/mman.h>   // for mmap()

static size_t SYSTEM_PAGE_SIZE = 0;


//  Init program
void mm_init(){

    SYSTEM_PAGE_SIZE = getpagesize();   // Get the page size of my system
}



//  Request VM Page from kernal
static void* mm_get_new_vm_page_from_kernal(int units){

    char* vm_page = mmap(
        0,
        units * SYSTEM_PAGE_SIZE,
        PROT_READ|PROT_WRITE|PROT_EXEC,
        MAP_ANON|MAP_PRIVATE,
        0, 0);

    if(vm_page == MAP_FAILED){
        printf("Error : VM Page allocation Failed\n");
        return NULL;
    }

    memset(vm_page, 0, units * SYSTEM_PAGE_SIZE);
    return (void*)vm_page;
} 

//  Return VM Page to kernal
static void mm_return_vm_page_to_kernal(void *vm_page, int units){

    if(munmap(vm_page, units * SYSTEM_PAGE_SIZE)) {
        printf("ERROR : Could not munmap VM PAGE to kernal\n");
    }
}

int main(int argc, char** argv){

    mm_init();
    printf("VM Page size = %lu\n", SYSTEM_PAGE_SIZE);
    void* addr1 = mm_get_new_vm_page_from_kernal(1);
    void* addr2 = mm_get_new_vm_page_from_kernal(1);
    printf("page 1 = %p, page 2 = %p\n", addr1, addr2);
    return 0;
}
