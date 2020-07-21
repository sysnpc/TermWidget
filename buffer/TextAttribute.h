#pragma once

#include "TextColor.h"
#include "TextStyle.h"

class TextAttribute {
 public:
  TextAttribute();
  const TextColor& foreColor() const;
  const TextColor& backColor() const;
  const TextStyles& styles() const;

  void SetForeColor(TextColor color);
  void SetBackColor(TextColor color);
  void SetStyles(TextStyle style, bool enabled);
  void SetStyles(TextStyle style);

 private:
  TextColor _fore;
  TextColor _back;
  TextStyles _styles;
};
