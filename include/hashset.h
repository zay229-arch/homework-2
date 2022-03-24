/* Modify this prehash function to work with a template type T instead of a char*
unsigned long prehash(unsigned char *str) {
    unsigned long h = 5381;
    int c;
    while (c = *str++) { 
	    h = ((h << 5) + h) + c;
    }
    return h;
}*/
