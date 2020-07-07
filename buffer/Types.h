#pragma once

enum class CursorType : unsigned int {
  Legacy = 0x0,  // uses the cursor's height value to range from underscore-like
                 // to full box
  VerticalBar = 0x1,  // A single vertical line, '|'
  Underscore = 0x2,   // a single horizontal underscore, smaller that the min
                      // height legacy cursor.
  EmptyBox = 0x3,     // Just the outline of a full box
  FullBox = 0x4       // a full box, similar to legacy with height=100%
};

struct Coord {
  int x;
  int y;
};

struct Size {
  int row;
  int column;
};

struct Margin {
  int top;
  int bottom;
  int left;
  int right;
};

struct Rect {
  int x1;
  int y1;
  int x2;
  int y2;
};

constexpr unsigned long INVALID_COLOR = 0xffffffff;
