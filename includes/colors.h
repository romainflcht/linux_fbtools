/* colors.h */
#ifndef _COLOR_H_
#define _COLOR_H_

#include <stdint.h>


/* 16b color RGB 565 */
#define BLACK  0x0000
#define DRED   0x7800
#define DGREE  0x03E0
#define DBLUE  0x000F
#define DYELL  0x7BE0
#define DMAGE  0x780F
#define DCYAN  0x03EF
#define DGREY  0x39E7
#define RED    0xF800
#define GREEN  0x07E0
#define BLUE   0x001F
#define YELLOW 0xFFE0
#define MAGEN  0xF81F
#define CYAN   0x07FF
#define GREY   0x7BEF
#define WHITE  0xFFFF

// * _ TYPES DEFINITIONS _______________________________________________________
typedef uint16_t COLOR_t; 

// * _ FUNCTIONS _______________________________________________________________
unsigned short palette(int c); 

// * Return a color from r, g, b value. (r: 5 bits, g: 6bits, b: 5bits). 
// * @param: r: 5 bits red value. 
// * @param: g: 6 bits green value. 
// * @param: b: 5 bits blue value. 
// * @return: the 16 bits color.  
COLOR_t color_from_rgb(uint8_t r, uint8_t g, uint8_t b); 

// * Blend two 16 bits color together to create transparency effect. 
// * @param: src  : the color of the pixel where the blended color will be 
// *                applied.
// * @param: dst  : the color of the pixel that we want to blend. 
// * @param: alpha: the value of transparency that we want to apply. (0 to 255). 
// * @return: the 16 bits blended color. 
COLOR_t blend_16bits_color(COLOR_t src, COLOR_t dst, uint8_t alpha);

#endif