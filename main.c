#include <stdio.h>
#include <unistd.h>

#include "utils.h"
#include "graphics.h"
#include "iso_font.h"

#define FB_INTERFACE "/dev/fb0"


int main(void)
{

    FRAMEBUFFER_t display; 
    RECT_CP_t* buf; 
    int retval; 

    retval = init_framebuffer(&display, FB_INTERFACE); 
    if (retval)
        return 1; 
        
    
    buf = copy_rect(&display, 0, 0, 100, 100); 

    int i; 
    for (i = 0; i < 100; i += 10)
        write_rect_alpha(&display, buf, i, i, 127); 


    RECT_CP_free(buf); 
    free_framebuffer(&display); 

    return 0; 
}
