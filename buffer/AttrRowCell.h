#pragma once

#include "TextColor.h"
#include "TextStyle.h"

using namespace NexTerm::Console::Buffer;

class AttrRowCell final {
 public:
  AttrRowCell(const TextColor& foreColor = TextColor::WindowText,
              const TextColor& backColor = TextColor::Window,
              const TextStyles& styles = TextStyle::Normal);

  inline const TextColor& foreColor() const { return _foreColor; }
  inline const TextColor& backColor() const { return _backColor; }
  inline const TextStyles& styles() const { return _styles; }
  inline bool isBlink() const { return _styles.testFlag(TextStyle::Blink); }

 private:
  TextColor _foreColor;
  TextColor _backColor;
  TextStyles _styles;
};
