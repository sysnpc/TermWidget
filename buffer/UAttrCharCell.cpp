#include "UAttrCharCell.h"

UAttrCharCell::UAttrCharCell(const UChar& uch, const TextColor& foreColor,
                             const TextColor& backColor,
                             const TextStyles styles)
    : _uch(uch),
      _foreColor(foreColor),
      _backColor(backColor),
      _styles(styles) {}

UAttrCharCell::UAttrCharCell(const CharRowCell& charCell,
                             const AttrRowCell& attrCell)
    : _uch(charCell.uch()),
      _foreColor(attrCell.foreColor()),
      _backColor(attrCell.backColor()),
      _styles(attrCell.styles()) {}

CharRowCell UAttrCharCell::createCharRowCell() const {
  return CharRowCell(_uch);
}

AttrRowCell UAttrCharCell::createAttrRowCell() const {
  return AttrRowCell(_foreColor, _backColor, _styles);
}
