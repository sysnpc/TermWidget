#pragma once

#include <QColor>

class TextColor {
 public:
  enum ColorTag {
    // default
    WindowText,
    Window,

    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,

    Bright_Black,
    Bright_Red,
    Bright_Green,
    Bright_Yellow,
    Bright_Blue,
    Bright_Magenta,
    Bright_Cyan,
    Bright_White,

    Custom_Rgb,
  };

 public:
  TextColor(ColorTag tag);
  ColorTag tag() const;

 private:
  ColorTag _tag;
};
