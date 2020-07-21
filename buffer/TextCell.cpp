#include "TextCell.h"

TextCell::TextCell() : _type(Null) {}

TextCell::TextCell(const UChar &uch, const TextAttribute &attr)
    : _type(Normal), _uch(uch), _attr(attr) {}

TextCell::TextCell(TextCell *parent) : _type(Extend), _parent(parent) {}

const UChar &TextCell::uch() const { return _uch; }

TextCell *TextCell::parent() const { return _parent; }

const TextAttribute &TextCell::attr() const { return _attr; }

bool TextCell::isNull() const { return _type == Null; }

bool TextCell::isExtend() const { return _type == Extend; }

bool TextCell::isNormal() const { return _type == Normal; }

void TextCell::SetNull() {
  _type = Null;
  _parent = nullptr;
}
