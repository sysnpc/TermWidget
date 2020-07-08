#pragma once

#include "AttrRowCell.h"
#include "CharRowCell.h"

class UAttrCharCell {
 public:
  UAttrCharCell(const UChar& uch,
                const TextColor& foreColor = TextColor(TextColor::WindowText),
                const TextColor& backColor = TextColor(TextColor::Window),
                const TextStyles styles = TextStyle::Normal);
  UAttrCharCell(const CharRowCell& charCell, const AttrRowCell& attrCell);

  inline const UChar& uch() const { return _uch; }

  CharRowCell createCharRowCell() const;
  AttrRowCell createAttrRowCell() const;

 private:
  UChar _uch;
  TextColor _foreColor;
  TextColor _backColor;
  TextStyles _styles;
};
