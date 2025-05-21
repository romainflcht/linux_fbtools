#include <stdio.h>
#include <unistd.h>

#include "utils.h"
#include "graphics.h"
#include "iso_font.h"

#define FB_INTERFACE "/dev/fb0"

// Cursor row and col coordinates. 
uint_t NOR; 
uint_t NOC; 


int main(void)
{

    FRAMEBUFFER_t display; 
    int retval; 

    retval = init_framebuffer(&display, FB_INTERFACE); 
    if (retval)
        return 1; 
    
    
    NOR = (display.vinfo.yres / ISO_CHAR_HEIGHT) - 1; 
    NOC = 0; 

    fill_screen(&display, BLACK); 

    // put_text(&display, "printing text test: \nLorem ipsum dolor sit amet, consectetur adipiscing elit. Suspendisse tincidunt risus neque, in pretium ante condimentum id. Nunc gravida semper purus, in commodo velit volutpat eget\n", WHITE, BLACK); 
    sleep(3); 

    draw_piet_mondrian(&display); 
    sleep(3); 

    fill_screen(&display, BLACK); 
    put_text(&display, "bye :)", WHITE, BLACK); 

    free_framebuffer(&display); 
    return 0; 
}
