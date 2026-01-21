#pragma once

class Color2D {
public:
    unsigned char r;  // Color red value
    unsigned char g;  // Color green value
    unsigned char b;  // Color blue value
    unsigned char a;  // Color alpha value

    Color2D(unsigned char r_, unsigned char g_, unsigned char b_, unsigned char a_ = 255);
    Color2D();

    // Common colors as static methods
    static Color2D Black();
    static Color2D White();
    static Color2D Red();
    static Color2D Green();
    static Color2D Blue();
    static Color2D Yellow();
    static Color2D Gray();
    static Color2D DarkGray();
    static Color2D LightGray();
    static Color2D Magenta();
    static Color2D MenuBackground();
    static Color2D MenuRowBackground();
    static Color2D MenuRowSelected();
    static Color2D MenuRowHoverBorder();
    static Color2D MenuText();
};
