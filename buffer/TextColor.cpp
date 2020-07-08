#include "TextColor.h"

TextColor::TextColor(ColorTag tag) : _tag(tag) {}

TextColor::ColorTag TextColor::tag() const { return _tag; }
