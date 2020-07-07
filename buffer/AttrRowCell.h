#pragma once

#include "TextColor.h"
#include "TextStyle.h"

using namespace NexTerm::Console::Buffer;

class AttrRowCell final {
 public:
  AttrRowCell(const TextColor& foreColor = TextColor::TEXT,
              const TextColor& backColor = TextColor::BACKGROUND,
              const TextStyles& styles = TextStyle::Normal);

  inline const QColor& foreColor() const { return _foreColor; }
  inline const QColor& backColor() const { return _backColor; }
  inline const TextStyles& styles() const { return _styles; }
  inline bool isBlink() const { return _styles.testFlag(TextStyle::Blink); }

 private:
  TextColor _foreColor;
  TextColor _backColor;
  TextStyles _styles;
};
