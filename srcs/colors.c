#include "colors.h"


unsigned short palette(int c)
{
  switch(c) 
  {
    case 0:
      return BLACK;
    case 1:
      return DRED;
    case 2:
      return DGREE;
    case 3:
      return DBLUE;
    case 4:
      return DYELL;
    case 5:
      return DMAGE;
    case 6:
      return DCYAN;
    case 7:
      return DGREY;
    case 8:
      return RED;
    case 9:
      return GREEN;
    case 10:
      return BLUE;
    case 11:
      return YELLOW;
    case 12:
      return MAGEN;
    case 13:
      return CYAN;
    case 14:
      return GREY;
    default:
      return WHITE;
  }
}


// * Return a color from r, g, b value. (r: 5 bits, g: 6bits, b: 5bits). 
// * @param: r: 5 bits red value (0 - 31). 
// * @param: g: 6 bits green value (0 - 63). 
// * @param: b: 5 bits blue value (0 - 31). 
// * @return: the 16 bits color.  
COLOR_t color_from_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    COLOR_t color; 

    color = 0; 
    if (r >= 32 || g >= 64 || b >= 32)
        return 0; 

    color |= r << 11; 
    color |= g << 5; 
    color |= b; 

    return color; 
}


// * Blend two 16 bits color together to create transparency effect. 
// * @param: src  : the color of the pixel where the blended color will be 
// *                applied.
// * @param: dst  : the color of the pixel that we want to blend. 
// * @param: alpha: the value of transparency that we want to apply. (0 to 255). 
// * @return: the 16 bits blended color. 
COLOR_t blend_16bits_color(COLOR_t src, COLOR_t dst, uint8_t alpha) 
{
    // Get r, g, b, from src and dest. 
    uint8_t src_r = (src >> 11) & 0x1F;
    uint8_t src_g = (src >> 5) & 0x3F;
    uint8_t src_b = src & 0x1F;

    uint8_t dst_r = (dst >> 11) & 0x1F;
    uint8_t dst_g = (dst >> 5) & 0x3F;
    uint8_t dst_b = dst & 0x1F;

    // Blend the src and dst color together.  
    uint8_t out_r = (dst_r * (255 - alpha) + src_r * alpha) / 255;
    uint8_t out_g = (dst_g * (255 - alpha) + src_g * alpha) / 255;
    uint8_t out_b = (dst_b * (255 - alpha) + src_b * alpha) / 255;

    return (out_r << 11) | (out_g << 5) | out_b;
}