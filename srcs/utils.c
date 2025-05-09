#include "utils.h"


void print_help()
{
    printf("Usage: ./fbecho <option>\n");
    printf("Option available: \n"); 
    printf("\t-h : print this message.\n"); 
    printf("\t-i : print screen information.\n"); 
    printf("\t-t <str> : Show the str on the screen.\n\n");
    return;  
}