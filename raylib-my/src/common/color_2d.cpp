#include "color_2d.h"

Color2D::Color2D(unsigned char r_, unsigned char g_, unsigned char b_, unsigned char a_):
  r { r_ },
  g { g_ },
  b { b_ },
  a { a_ }
{}

Color2D::Color2D():
  r { 0 },
  g { 0 },
  b { 0 },
  a { 255 }
{}

Color2D Color2D::Black() {
  return Color2D(0, 0, 0, 255);
}

Color2D Color2D::White() {
  return Color2D(255, 255, 255, 255);
}

Color2D Color2D::Red() {
  return Color2D(255, 0, 0, 255);
}

Color2D Color2D::Green() {
  return Color2D(0, 255, 0, 255);
}

Color2D Color2D::Blue() {
  return Color2D(0, 0, 255, 255);
}

Color2D Color2D::Yellow() {
  return Color2D(255, 255, 0, 255);
}

Color2D Color2D::Gray() {
  return Color2D(130, 130, 130, 255);
}

Color2D Color2D::DarkGray() {
  return Color2D(80, 80, 80, 255);
}

Color2D Color2D::LightGray() {
  return Color2D(200, 200, 200, 255);
}

Color2D Color2D::Magenta() {
  return Color2D(255, 0, 255, 255);
}

Color2D Color2D::MenuBackground() {
  return Color2D(245, 245, 245, 255);
}

Color2D Color2D::MenuRowBackground() {
  return Color2D(232, 232, 232, 255);
}

Color2D Color2D::MenuRowSelected() {
  return Color2D(200, 225, 245, 255);
}

Color2D Color2D::MenuRowHoverBorder() {
  return Color2D(120, 120, 120, 255);
}

Color2D Color2D::MenuText() {
  return Color2D(25, 25, 25, 255);
}
