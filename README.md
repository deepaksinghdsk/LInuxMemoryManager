# LinuxMemoryManager
This is a dyanamic memory manager, mm.c is has all the major memory management code, uapi_mm is the API between the Memory manager (mm.c) and the user space applications (testapp.c) which require heap memory to be allocated to its variables.

testapp.c will call the macros of uapi_mm.h to allocate memory to its struct variables dynamically, then uapi_mm.h calls the functions whose definitions are in mm.c, and then mm.c uses mmap function to allocated and munmap function to deallocate dynamic memory.

Linked list data structure is being used by the Memory manager to allocate memory, "vm_page_for_families" is created which is of size allocated by system and then in it multiple "vm_page_family" are stored which are the name and size of the struct which has to be allocated dynamic memory. And the next is "vm_page_for_families" points to the next page which also has multiple "vm_page_family". 

<image align="center" width="700" src="./screenshots/2.png">

<image align="center" width="700" src="./screenshots/1.png">

<br><br>
<image align="center" width="700" src="./screenshots/6.png">

<br><br>
<image align="center" width="700" src="./screenshots/3.png">
<image align="center" width="700" src="./screenshots/4.png">
<image align="center" width="700" src="./screenshots/5.png">


