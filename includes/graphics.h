#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "colors.h"


extern unsigned int NOR; 
extern unsigned int NOC; 


// * __ DEFINITIONS ____________________________________________________________
#define FRAMEBUFFER_t struct framebuffer_t
#define RECT_CP_t struct rect_cp_t
#define uint_t unsigned int


// * __ STRUCTURE DEFINITIONS __________________________________________________
struct framebuffer_t
{
    int                         fd;
    int                         fb_total_bytes_size; 
    COLOR_t*                    screen; 
    struct fb_var_screeninfo    vinfo;
}; 


struct rect_cp_t
{
    uint_t    w; 
    uint_t    h;
    uint_t    size; 
    COLOR_t*  buf; 
}; 


// * __ FUNCTIONS ______________________________________________________________

// * Initialize the framebuffer structure with file descriptor, total size of 
// * the pixel array, display information and the memory map address of the 
// * framebuffer. 
// * @param: *fb  : the structure to initialize. 
// * @param: *path: path of the peripheral (usually /dev/fb0). 
// * @return: 1 in case of an error, 0 otherwise.  
int init_framebuffer(FRAMEBUFFER_t* fb, const char *path); 

// * Free the memory used by the FRAMEBUFFER_t structure. 
// * @param: *fb  : the structure to initialize. 
void free_framebuffer(FRAMEBUFFER_t* fb); 

// * Display useful information about the screen for debug. 
// * @param: *vinfo: structure that contains every screen info. 
void display_info(FRAMEBUFFER_t* fb); 

// ! __ GRAPHIC FUNCTIONS ______________________________________________________

// * Clear the display. 
// * @param: *fb: the structure to initialize. 
void fill_screen(FRAMEBUFFER_t* fb, COLOR_t color); 

// * Draw a pixel at a specific location. 
// * @param: *fb  : The framebuffer where the pixel will be drawn. 
// * @param: x    : x location of the pixel. 
// * @param: y    : y location of the pixel. 
// * @param: color: color of the pixel. 
void draw_pixel(FRAMEBUFFER_t* fb, uint_t x, uint_t y, COLOR_t color); 


// * Return the color value of a specific pixel on screen. 
// * @param: *fb: The framebuffer where the pixel will be fetch. 
// * @param: x  : x location of the pixel. 
// * @param: y  : y location of the pixel. 
// * @return: the color value of the pixel. 
COLOR_t get_pixel_color(FRAMEBUFFER_t* fb, uint_t x, uint_t y); 


// * Draw a ACSII character on the screen at position x, y. 
// * @param: c      : the char to draw. 
// * @param: x      : x position where to the draw the char. 
// * @param: y      : y position where to the draw the char. 
// * @param: fgcolor: color of the letter. 
// * @param: bgcolor: color of the background of the char. 
void print_char_coord(FRAMEBUFFER_t* fb, char c, uint_t x, uint_t y, 
               COLOR_t fgcolor, COLOR_t bgcolor); 


// * Draw a ACSII character on the screen aligned on the char grid. 
// * @param: c      : the char to draw. 
// * @param: row    : row index where to draw the char. 
// * @param: col    : column index position where to draw the char. 
// * @param: fgcolor: color of the letter. 
// * @param: bgcolor: color of the background of the char. 
void print_char_grid(FRAMEBUFFER_t* fb, char c, uint_t row, uint_t col, 
    COLOR_t fgcolor, COLOR_t bgcolor); 


// * Put a char on the screen at global cursor position. 
// * @param: c      : the char to draw. 
// * @param: fgcolor: color of the letter. 
// * @param: bgcolor: color of the background of the char. 
void put_char(FRAMEBUFFER_t* fb, char c, COLOR_t fgcolor, COLOR_t bgcolor); 


// * Copy the screen to offset it by one char height, creating a scrolling. 
// * @param: fb: FRAMEBUFFER_t where the screen will be scrolled.  
void scroll_screen(FRAMEBUFFER_t* fb);


// * Draw a ACSII a string on the screen at position x, y. 
// * @param: str  : the char to draw. 
// * @param: x    : x position the draw the char. 
// * @param: y    : y position the draw the char. 
// * @param: color: color of the pixel. 
void print_str_coord(FRAMEBUFFER_t* fb, char* str, uint_t x, uint_t y, 
                 COLOR_t fgcolor, COLOR_t bgcolor); 


// * Put a string on the screen at global cursor position. 
// * @param: c      : the char to draw. 
// * @param: fgcolor: color of the letter. 
// * @param: bgcolor: color of the background of the char. 
void put_text(FRAMEBUFFER_t* fb, char* str, COLOR_t fgcolor, COLOR_t bgcolor); 


// * Copy a rectangle of pixel from the screen into a buffer. 
// * @param: *fb: The framebuffer where the area will be copied. 
// * @param: x0 : x coordinate of the top-left corner of the copied area. 
// * @param: y0 : y coordinate of the top-left corner of the copied area.
// * @param: x1 : x coordinate of the bottom-right corner of the copied area.
// * @param: y1 : y coordinate of the bottom-right corner of the copied area.
// * @return: The pointer to the buffer containing the screen data copied. 
void* copy_rect(FRAMEBUFFER_t* fb, uint_t x0, uint_t y0, 
                uint_t x1, uint_t y1); 


// * Free the RECT_CP_t structure created in copy_rect function. 
// * @param: *cp: The structure that will be freed.  
void RECT_CP_free(RECT_CP_t* cp); 


// * Paste a rectangle of pixel previously copied. 
// * @param: *fb: The framebuffer where the area will be pasted. 
// * @param: buf: The RECT_CP_t buffer that contains data that will be drawn. 
// * @param: x  : x coordinate of the top-left corner where the copy will be 
// *              drawn. 
// * @param: y  : y coordinate of the top-left corner where the copy will be 
// *              drawn. 
void write_rect(FRAMEBUFFER_t* fb, void* cp, uint_t x, uint_t y);


// * Paste a rectangle of pixel previously copied with transparency. 
// * @param: *fb  : The framebuffer where the area will be pasted. 
// * @param: buf  : The RECT_CP_t buffer that contains data that will be drawn. 
// * @param: x    : x coordinate of the top-left corner where the copy will be 
// *                drawn. 
// * @param: y    : y coordinate of the top-left corner where the copy will be 
// *                drawn. 
// * @param: alpha: the value of transparency we want to apply (0 to 255). 
void write_rect_alpha(FRAMEBUFFER_t* fb, void* buf, uint_t x, 
                      uint_t y, uint8_t alpha); 


// * Draw an horizontal line on screen. 
// * @param: *fb  : FRAMEBUFFER_t where the line will be rendered. 
// * @param: x    : start x coordinate. 
// * @param: y    : start y coordinate. 
// * @param: w    : width of the line. 
// * @param: color: color of the line. 
void draw_h_line(FRAMEBUFFER_t* fb, uint_t x, uint_t y, 
                 uint_t w, COLOR_t color); 


// * Draw a vertical line on screen. 
// * @param: *fb  : FRAMEBUFFER_t where the line will be rendered. 
// * @param: x    : start x coordinate. 
// * @param: y    : start y coordinate. 
// * @param: h    : height of the line. 
// * @param: color: color of the line. 
void draw_v_line(FRAMEBUFFER_t* fb, uint_t x, uint_t y, 
    uint_t h, COLOR_t color); 


// * Draw a line on the screen using two x;y coordinate. 
// * @param: *fb  : FRAMEBUFFER_t where the line will be rendered.  
// * @param: x0   : start x coordinate. 
// * @param: y0   : start y coordinate.
// * @param: x1   : end x coordinate.
// * @param: y1   : end y coordinate.
// * @param: color: color of the line. 
void draw_line(FRAMEBUFFER_t* fb, uint_t x0, uint_t y0, 
               uint_t x1, uint_t y1, COLOR_t color); 


// * Draw a filled rectangle on the screen. 
// * @param: *fb  : FRAMEBUFFER_t where the line will be rendered.  
// * @param: x    : start x coordinate. 
// * @param: y    : start y coordinate.
// * @param: w    : width of the rectangle.
// * @param: h    : height of the rectangle. 
// * @param: color: color of the rectangle. 
void draw_rect(FRAMEBUFFER_t* fb, uint_t x, uint_t y, 
               uint_t w, uint_t h, COLOR_t color); 


// * Draw the piet_mondrian style painting onto the screen. Be aware, it will 
// * clear the entire screen.
// * @param: *fb: FRAMEBUFFER_t where it will be rendered.  
void draw_piet_mondrian(FRAMEBUFFER_t* fb); 

#endif 