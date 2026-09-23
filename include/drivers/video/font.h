#pragma once
    
extern unsigned char font8x16[][16];

#define FONT_WIDTH 8
#define FONT_HEIGHT 16
#define FONT_SPACING_X 1
#define FONT_SPACING_Y 1
#define FONT_ADVANCE_X (FONT_WIDTH + FONT_SPACING_X)
#define FONT_ADVANCE_Y (FONT_HEIGHT + FONT_SPACING_Y)
#define TAB_SIZE 4
