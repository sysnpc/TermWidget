#include "AttrRowCell.h"

#include "TextColor.h"

AttrRowCell::AttrRowCell(const TextColor& foreColor, const TextColor& backColor,
                         const TextStyles& styles)
    : _foreColor{foreColor}, _backColor(backColor), _styles{styles} {}
