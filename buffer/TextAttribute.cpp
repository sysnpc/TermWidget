#include "TextAttribute.h"

TextAttribute::TextAttribute()
    : _fore(TextColor::WindowText),
      _back(TextColor::Window),
      _styles(TextStyle::Normal) {}

const TextColor &TextAttribute::foreColor() const { return _fore; }

const TextColor &TextAttribute::backColor() const { return _back; }

const TextStyles &TextAttribute::styles() const { return _styles; }

void TextAttribute::SetForeColor(TextColor color) { _fore = color; }

void TextAttribute::SetBackColor(TextColor color) { _back = color; }

void TextAttribute::SetStyles(TextStyle style, bool enabled) {
  _styles.setFlag(style, enabled);
}

void TextAttribute::SetStyles(TextStyle style) { _styles = style; }
