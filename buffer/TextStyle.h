#pragma once

#include <QObject>

namespace NexTerm::Console::Buffer {
  Q_NAMESPACE

  enum class TextStyle : unsigned short {
    Normal = 0x00,
    Bold = 0x01,
    Faint = 0x02,
    Italics = 0x04,
    Underlined = 0x08,
    Blink = 0x10,
    Inverse = 0x20,
    Invisible = 0x40,
    CrossedOut = 0x80,
    DoublyUnderlined = 0x100,

  };
  Q_ENUM_NS(TextStyle);
  Q_DECLARE_FLAGS(TextStyles, TextStyle);
  Q_DECLARE_OPERATORS_FOR_FLAGS(TextStyles);

}  // namespace NexTerm::Console::Buffer
