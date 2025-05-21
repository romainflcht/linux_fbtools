#include "graphics.h"
#include "iso_font.h"


// * Initialize the framebuffer structure with file descriptor, total size of 
// * the pixel array, display information and the memory map address of the 
// * framebuffer. 
// * @param: *fb  : the structure to initialize. 
// * @param: *path: path of the peripheral (usually /dev/fb0). 
// * @return: 1 in case of an error, 0 otherwise.  
int init_framebuffer(FRAMEBUFFER_t* fb, const char *path)
{ 
    // Open the framebuffer peripheral. 
    fb->fd = -1; 
    fb->fd = open(path, O_RDWR);
    if (fb->fd < 0)
    {
        printf("\x1b[1;31m~[ERROR] Opening %s failed.\x1b[0m\n", path); 
        return 1; 
    }
    
    // Fetch informations about the framebuffer. 
    ioctl(fb->fd, FBIOGET_VSCREENINFO, &(fb->vinfo)); 
    
    // Calculate the total size that need to be allocated (Divide the bits per 
    // pixel by 8 to convert into bytes per pixel, in this case 16 bits color 
    // is 2 bytes color). 
    fb->fb_total_bytes_size = fb->vinfo.xres * fb->vinfo.yres * (fb->vinfo.bits_per_pixel / 8); 
    
    // Map the framebuffer to another memory address to write on it that the 
    // kernel choose. 
    fb->screen = mmap(0, fb->fb_total_bytes_size, PROT_READ | PROT_WRITE, MAP_SHARED, fb->fd, 0);
    if (!fb->screen)
    {
        printf("~[ERROR] Mapping video memory failed.\n"); 
        return 1; 
    }

    return 0; 
}


// * Free the memory used by the FRAMEBUFFER_t structure. 
// * @param: *fb: the structure to initialize. 
void free_framebuffer(FRAMEBUFFER_t* fb)
{
    // Check if the memory map is allocated and free it after. 
    if (fb->screen)
    {
        munmap(fb->screen, fb->fb_total_bytes_size);
        fb->screen = NULL; 
    }    
    
    // Check if there is a file descriptor and close it. 
    if (fb->fd >= 0)
    {
        close(fb->fd);
        fb->fd = -1; 
    }

    return; 
}


// * Display useful information about the screen for debug. 
// * @param: *vinfo: structure that contains every screen info. 
void display_info(FRAMEBUFFER_t* fb)
{
    printf("~DISPLAY INFORMATION: \n"); 
    printf("\t-width : %d\n", fb->vinfo.xres); 
    printf("\t-height: %d\n", fb->vinfo.yres); 
    printf("\t-bbp   : %d\n", fb->vinfo.bits_per_pixel);
    return;  
}


// ! __ GRAPHIC FUNCTIONS ______________________________________________________
// * Clear the display. 
// * @param: *fb: the structure to initialize. 
void fill_screen(FRAMEBUFFER_t* fb, COLOR_t color)
{
    uint_t i; 

    i = 0; 
    while (i < fb->vinfo.xres * fb->vinfo.yres)
    {
        fb->screen[i] = color; 
        i++; 
    }
}


// * Draw a pixel at a specific location. 
// * @param: *fb  : The framebuffer where the pixel will be drawn. 
// * @param: x    : x location of the pixel. 
// * @param: y    : y location of the pixel. 
// * @param: color: color of the pixel. 
void draw_pixel(FRAMEBUFFER_t* fb, uint_t x, uint_t y, COLOR_t color)
{
    // Check x and y boundary. 
    if (x > fb->vinfo.xres)
        return; 

    else if (y > fb->vinfo.yres)
        return; 

    // Set the color to the pixel memory address. 
    fb->screen[y * fb->vinfo.xres + x] = color; 
    return; 
}


// * Return the color value of a specific pixel on screen. 
// * @param: *fb: The framebuffer where the pixel will be fetch. 
// * @param: x  : x location of the pixel. 
// * @param: y  : y location of the pixel. 
// * @return: the color value of the pixel. 
COLOR_t get_pixel_color(FRAMEBUFFER_t* fb, uint_t x, uint_t y)
{
    if (x > fb->vinfo.xres)
        return -1; 

    else if (y > fb->vinfo.yres)
        return -1;
        
    return fb->screen[y * fb->vinfo.xres + x];
}


// * Draw a ACSII character on the screen at position x, y. 
// * @param: c      : the char to draw. 
// * @param: x      : x position where to the draw the char. 
// * @param: y      : y position where to the draw the char. 
// * @param: fgcolor: color of the letter. 
// * @param: bgcolor: color of the background of the char. 
void print_char_coord(FRAMEBUFFER_t* fb, char c, uint_t x, uint_t y, 
               COLOR_t fgcolor, COLOR_t bgcolor)
{
    unsigned char* char_addr; 
    unsigned char current_byte; 
    int i; 
    int j; 

    // Get the address of the first byte of the character. 
    char_addr = ISO_FONT + (c * ISO_CHAR_HEIGHT); 

    for (i = 0; i < ISO_CHAR_HEIGHT; i++)
    {
        // Get the first byte value of the character (the first line). 
        current_byte = *char_addr;

        for (j = 0; j < ISO_CHAR_WIDTH; j++)
        {
            // Check each bits of the bytes and draw a pixel according to zeros 
            // and ones. 
            if (current_byte & 0x01)
                draw_pixel(fb, x + j, y + i, fgcolor); 
            
            else
                draw_pixel(fb, x + j, y + i, bgcolor); 
            
            // Offset the byte by one to see the next bit. 
            current_byte = current_byte >> 1; 

        }

        // Increment the address to get all 16 lines. 
        char_addr++; 
    }

    return; 
}


// * Draw a ACSII character on the screen aligned on the char grid. 
// * @param: c      : the char to draw. 
// * @param: row    : row index where to draw the char. 
// * @param: col    : column index position where to draw the char. 
// * @param: fgcolor: color of the letter. 
// * @param: bgcolor: color of the background of the char. 
void print_char_grid(FRAMEBUFFER_t* fb, char c, uint_t row, uint_t col, 
                COLOR_t fgcolor, COLOR_t bgcolor)
{
    int x; 
    int y; 

    x = row * ISO_CHAR_WIDTH; 
    y = col * ISO_CHAR_HEIGHT; 

    print_char_coord(fb, c, x, y, fgcolor, bgcolor); 
    return; 
}


// * Put a char on the screen at global cursor position. 
// * @param: c      : the char to draw. 
// * @param: fgcolor: color of the letter. 
// * @param: bgcolor: color of the background of the char. 
void put_char(FRAMEBUFFER_t* fb, char c, COLOR_t fgcolor, COLOR_t bgcolor)
{
    int x; 
    int y; 

    x = NOC * ISO_CHAR_WIDTH; 
    y = NOR * ISO_CHAR_HEIGHT; 

    // 
    if (c != '\n')
        print_char_coord(fb, c, x, y, fgcolor, bgcolor); 

    else
    {
        NOC = 0; 
        NOR += 1; 
    }

    // Increment the cursor position one char to the right. 
    NOC++; 
    if (NOC >= (fb->vinfo.xres / ISO_CHAR_WIDTH))
    {
        // If off screen on the right, reset the row position and increment 
        // the col position. 
        NOC = 0; 
        NOR++; 
    }

    if (NOR >= (fb->vinfo.yres / ISO_CHAR_HEIGHT))
    {
        // If off screen at the bottom, set cursor position one line higher and
        // scroll the screen. 
        NOR--;
        scroll_screen(fb); 
    }

    return; 
}


// * Copy the screen to offset it by one char height, creating a scrolling. 
// * @param: fb: FRAMEBUFFER_t where the screen will be scrolled.  
void scroll_screen(FRAMEBUFFER_t* fb)
{
    void* scroll_rect; 
    int max_x; 
    int max_y; 

    max_x = fb->vinfo.xres; 
    max_y = fb->vinfo.yres; 

    // Copy and write the screen one char width to the top. 
    scroll_rect = copy_rect(fb, 0, ISO_CHAR_HEIGHT, max_x, max_y); 
    if (!scroll_rect)
        return; 

    write_rect(fb, scroll_rect, 0, 0); 

    // Clear the last char line at the bottom of the screen. 
    draw_rect(fb, 0, max_y - ISO_CHAR_HEIGHT, max_y, ISO_CHAR_HEIGHT, BLACK); 
    
    // Free ressources. 
    RECT_CP_free(scroll_rect); 
    return; 
}


// * Draw a ACSII a string on the screen at position x, y. 
// * @param: str  : the char to draw. 
// * @param: x    : x position the draw the char. 
// * @param: y    : y position the draw the char. 
// * @param: color: color of the pixel. 
void print_str_coord(FRAMEBUFFER_t* fb, char* str, uint_t x, uint_t y,
                 COLOR_t fgcolor, COLOR_t bgcolor)
{
    int i;
    uint_t row; 
    uint_t line; 

    // Check if str in sot empty, x and y boundary. 
    if (!str)
        return; 
    
    else if (x > fb->vinfo.xres)
        return; 

    else if (y > fb->vinfo.yres)
        return; 

    row = x; 
    line = y; 

    // Loop through each char of the string and draw it to the screen, stop the
    // drawing if the string is too long. 
    i = 0; 
    while (str[i])
    {
        print_char_coord(fb, str[i], row, line, fgcolor, bgcolor); 

        row += ISO_CHAR_WIDTH; 

        if (row > fb->vinfo.xres)
        {
            row = x; 
            line += ISO_CHAR_HEIGHT; 
        }
        
        i++; 
    }
}



// * Put a string on the screen at global cursor position. 
// * @param: c      : the char to draw. 
// * @param: fgcolor: color of the letter. 
// * @param: bgcolor: color of the background of the char. 
void put_text(FRAMEBUFFER_t* fb, char* str, COLOR_t fgcolor, COLOR_t bgcolor)
{
    int i; 

    // Put a char until the we meet a '\0'. 
    i = 0; 
    while (str[i])
    {
        put_char(fb, str[i], fgcolor, bgcolor); 
        i++; 
    }    
    
    return; 
}

// * Copy a rectangle of pixel from the screen into a buffer. 
// * @param: *fb: The framebuffer where the area will be copied. 
// * @param: x0 : x coordinate of the top-left corner of the copied area. 
// * @param: y0 : y coordinate of the top-left corner of the copied area.
// * @param: x1 : x coordinate of the bottom-right corner of the copied area.
// * @param: y1 : y coordinate of the bottom-right corner of the copied area.
// * @return: The pointer to the buffer containing the screen data copied. 
void* copy_rect(FRAMEBUFFER_t* fb, uint_t x0, uint_t y0, 
                uint_t x1, uint_t y1)
{
    RECT_CP_t* cp; 
    uint_t x; 
    uint_t y; 
    
    // Allocate space for the RECT_CT_t structure that hold size info and the 
    // buffer itself. 
    cp = malloc(sizeof(RECT_CP_t)); 
    if (!cp)
        return NULL; 
    
    // Initialise all the size of the buffer. 
    cp->w = x1 - x0; 
    cp->h = y1 - y0; 
    cp->size = (x1 - x0) * (y1 - y0); 
    
    // Allocate the buffer that will store screen data. 
    cp->buf = malloc(sizeof(COLOR_t) * cp->size); 
    if (!cp->buf)
    {
        RECT_CP_free(cp); 
        return NULL; 
    }
    
    // Copy the area contained in x0;y0 - x1;y1 into the buffer. 
    for (y = y0; y < y1; y++)
    {
        for (x = x0; x < x1; x++)
        cp->buf[(y - y0) * (x1 - x0) + (x - x0)] = get_pixel_color(fb, x, y); 
    }
    
    return (void*)cp; 
}


// * Free the RECT_CP_t structure created in copy_rect function. 
// * @param: *cp: The structure that will be freed. 
void RECT_CP_free(RECT_CP_t* cp)
{
    if (!cp)
        return; 

    if (cp->buf)
    {
        free(cp->buf); 
        cp->buf = NULL; 
    }

    free(cp); 
    return; 
}


// * Paste a rectangle of pixel previously copied. 
// * @param: *fb: The framebuffer where the area will be pasted. 
// * @param: buf: The RECT_CP_t buffer that contains data that will be drawn. 
// * @param: x  : x coordinate of the top-left corner where the copy will be 
// *              drawn. 
// * @param: y  : y coordinate of the top-left corner where the copy will be 
// *              drawn. 
void write_rect(FRAMEBUFFER_t* fb, void* buf, uint_t x, uint_t y)
{
    RECT_CP_t* cp; 
    uint_t i; 
    uint_t j; 

    // Cast the void buffer. 
    cp = (RECT_CP_t*)buf; 
    i = 0; 
    j = 0; 

    // Draw screen data onto the screen. 
    while (i < cp->size)
    {
        if (i % cp->w == 0 && i != 0)
            j++;

        draw_pixel(fb, x + (i % cp->w), y + j, cp->buf[i]); 
        i++; 
    }

    return; 
}


// * Paste a rectangle of pixel previously copied with transparency. 
// * @param: *fb  : The framebuffer where the area will be pasted. 
// * @param: buf  : The RECT_CP_t buffer that contains data that will be drawn. 
// * @param: x    : x coordinate of the top-left corner where the copy will be 
// *                drawn. 
// * @param: y    : y coordinate of the top-left corner where the copy will be 
// *                drawn. 
// * @param: alpha: the value of transparency we want to apply (0 to 255). 
void write_rect_alpha(FRAMEBUFFER_t* fb, void* buf, uint_t x, 
                      uint_t y, uint8_t alpha)
{
    RECT_CP_t*      cp; 
    COLOR_t         src; 
    COLOR_t         dst; 
    COLOR_t         blended; 
    uint_t    i; 
    uint_t    j; 

    // Cast the void buffer. 
    cp = (RECT_CP_t*)buf; 
    i = 0; 
    j = 0; 

    // Draw screen data onto the screen. 
    while (i < cp->size)
    {
        if (i % cp->w == 0 && i != 0)
            j++;

        src = get_pixel_color(fb, x + (i % cp->w), y + j); 
        dst = cp->buf[i]; 
        blended = blend_16bits_color(src, dst, alpha); 

        draw_pixel(fb, x + (i % cp->w), y + j, blended); 
        i++; 
    }

    return; 
}



// * Draw an horizontal line on screen. 
// * @param: *fb  : FRAMEBUFFER_t where the line will be rendered. 
// * @param: x    : start x coordinate. 
// * @param: y    : start y coordinate. 
// * @param: w    : width of the line. 
// * @param: color: color of the line. 
void draw_h_line(FRAMEBUFFER_t* fb, uint_t x, uint_t y, 
    uint_t w, COLOR_t color)
{
    uint_t i; 

    for (i = 0; i < w; i++)
        draw_pixel(fb, x + i, y, color); 

    return; 
}


// * Draw a vertical line on screen. 
// * @param: *fb  : FRAMEBUFFER_t where the line will be rendered. 
// * @param: x    : start x coordinate. 
// * @param: y    : start y coordinate. 
// * @param: h    : height of the line. 
// * @param: color: color of the line. 
void draw_v_line(FRAMEBUFFER_t* fb, uint_t x, uint_t y, 
    uint_t h, COLOR_t color)
{
    uint_t i; 

    for (i = 0; i < h; i++)
        draw_pixel(fb, x, y + i, color); 

    return; 
}


// * Draw a line on the screen using two x;y coordinate. 
// * @param: *fb  : FRAMEBUFFER_t where the line will be rendered.  
// * @param: x0   : start x coordinate. 
// * @param: y0   : start y coordinate.
// * @param: x1   : end x coordinate.
// * @param: y1   : end y coordinate.
// * @param: color: color of the line. 
void draw_line(FRAMEBUFFER_t* fb, uint_t x0, uint_t y0, 
               uint_t x1, uint_t y1, COLOR_t color)
{
    uint_t dx; 
    uint_t dy; 
    uint_t d; 
    uint_t y; 
    uint_t x; 

    dx = x1 - x0; 
    dy = y1 - y0; 
    d = 2 * dy - dx; 
    x = x0; 
    y = y0; 

    // Bresenham's line algorithm, see link below to understand how it's work. 
    // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm. 
    while (x < x1)
    {
        draw_pixel(fb, x, y, color); 
        if (d > 0)
        {
            y += 1;
            d -= 2 * dx; 

        }
        d += 2 * dy; 
        x++; 
    }
}


// * Draw a filled rectangle on the screen. 
// * @param: *fb  : FRAMEBUFFER_t where the line will be rendered.  
// * @param: x    : start x coordinate. 
// * @param: y    : start y coordinate.
// * @param: w    : width of the rectangle.
// * @param: h    : height of the rectangle. 
// * @param: color: color of the rectangle. 
void draw_rect(FRAMEBUFFER_t* fb, uint_t x, uint_t y, 
               uint_t w, uint_t h, COLOR_t color)
{
    uint_t i; 

    // Draw horizontal line for the height of the rectangle. 
    for (i = 0; i < h; i++)
        draw_h_line(fb, x, y + i, w, color); 

    return; 
}


// * Draw the piet_mondrian style painting onto the screen. Be aware, it will 
// * clear the entire screen.
// * @param: *fb: FRAMEBUFFER_t where it will be rendered.  
void draw_piet_mondrian(FRAMEBUFFER_t* fb)
{
    fill_screen(fb, WHITE); 

    draw_rect(fb, 35, 50, 30, 50, YELLOW);
    draw_rect(fb, 135, 50, 65, 50, RED); 
    draw_rect(fb, 135, 185, 65, 105, BLUE); 
    
    draw_v_line(fb, 35, 0, fb->vinfo.yres, BLACK); 
    draw_v_line(fb, 135, 0, fb->vinfo.yres, BLACK); 
    draw_v_line(fb, 200, 0, fb->vinfo.yres, BLACK); 
    draw_v_line(fb, 65, 50, 50, BLACK); 

    draw_h_line(fb, 0, 50, fb->vinfo.xres, BLACK); 
    draw_h_line(fb, 0, 100, fb->vinfo.xres, BLACK); 
    draw_h_line(fb, 0, 185, fb->vinfo.xres, BLACK); 
    draw_h_line(fb, 0, 290, fb->vinfo.xres, BLACK); 
    return; 
}
