#include "TextCursor.h"

TextCursor::TextCursor() : _x(1), _y(1) {}

int TextCursor::X() { return _x; }

void TextCursor::X(int n) { _x = n; }

int TextCursor::Y() { return _y; }

int TextCursor::Ry() { return _flagY + _y; }

void TextCursor::Y(int n) { _y = n; }

int TextCursor::FlagY() { return _flagY; }

void TextCursor::FlagY(int y) { _flagY = y; }

void TextCursor::ScrollUp(int n) { _flagY += n; }

void TextCursor::ScrollDown(int n) { _flagY -= n; }

const TextAttribute &TextCursor::Attr() { return _attr; }

void TextCursor::SetDefaultForeground() {
  _attr.SetForeColor(TextColor::WindowText);
}

void TextCursor::SetDefaultBackground() {
  _attr.SetBackColor(TextColor::Window);
}

void TextCursor::SetDefaultStyles() { _attr.SetStyles(TextStyle::Normal); }

void TextCursor::SetBold(bool enabled) {
  _attr.SetStyles(TextStyle::Bold, enabled);
}

void TextCursor::SetFaint(bool enabled) {
  _attr.SetStyles(TextStyle::Faint, enabled);
}

void TextCursor::SetItalic(bool enabled) {
  _attr.SetStyles(TextStyle::Italics, enabled);
}

void TextCursor::SetBlinking(bool enabled) {
  _attr.SetStyles(TextStyle::Blinking, enabled);
}

void TextCursor::SetInvisible(bool enabled) {
  _attr.SetStyles(TextStyle::Invisible, enabled);
}

void TextCursor::SetCrossedOut(bool enabled) {
  _attr.SetStyles(TextStyle::CrossedOut, enabled);
}

void TextCursor::SetReverseVideo(bool enabled) {
  _attr.SetStyles(TextStyle::Inverse, enabled);
}

void TextCursor::SetUnderlined(bool enabled) {
  _attr.SetStyles(TextStyle::Underlined, enabled);
}

void TextCursor::SetDoublyUnderlined(bool enabled) {
  _attr.SetStyles(TextStyle::DoublyUnderlined, enabled);
}

void TextCursor::SetOverlined(bool enabled) {
  _attr.SetStyles(TextStyle::Overlined, enabled);
}

void TextCursor::SetIndexedForeground(TextColor color) {
  _attr.SetForeColor(color);
}

void TextCursor::SetIndexedBackground(TextColor color) {
  _attr.SetBackColor(color);
}
