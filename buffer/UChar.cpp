#include "UChar.h"

UChar::UChar() : _uch(""), _width(1) {}

UChar::UChar(const QString& uch, int width) : _uch(uch), _width(width) {}
