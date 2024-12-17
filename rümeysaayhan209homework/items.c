#include <stdio.h>
#include <stdlib.h>
#include "items.h"



int load_items(const char* filename) {
   
    FILE* f = fopen(filename, "r");
    if (!f) return -1;
  
    fclose(f);
    return 0;
}

void free_items() {
    
}
