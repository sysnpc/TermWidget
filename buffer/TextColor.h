#pragma once

#include <QColor>

class TextColor : public QColor {
 public:
  enum ColorTag {
    // default
    TEXT,
    BACKGROUND,

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
  };

 public:
  TextColor(const ColorTag tag);
  TextColor(const QColor &color);

 private:
  QColor _tagToColor(const ColorTag &tag);
};
