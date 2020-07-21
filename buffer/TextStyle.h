#pragma once

#include <QFlags>

enum class TextStyle : unsigned short {
  Normal = 0x00,
  Bold = 0x01,
  Italics = 0x02,
  Blinking = 0x04,
  Invisible = 0x08,
  CrossedOut = 0x10,
  // TODO:GH#2916 add support for these to the parser as well.
  Underlined =
      0x20,  // _technically_ different from LVB_UNDERSCORE, see TODO:GH#2915
  DoublyUnderlined =
      0x40,  // Included for completeness, but not currently supported.
  Faint = 0x80,

  Inverse = 0x100,  // fore <-> back color
  Overlined = 0x200,
};

Q_DECLARE_FLAGS(TextStyles, TextStyle);
Q_DECLARE_OPERATORS_FOR_FLAGS(TextStyles);
