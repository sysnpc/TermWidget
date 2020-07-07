#pragma once

#include "AttrRowCell.h"
#include "CharRowCell.h"

class UAttrCharCell {
 public:
  UAttrCharCell(const UChar& uch,
                const TextColor& foreColor = TextColor(TextColor::TEXT),
                const TextColor& backColor = TextColor(TextColor::BACKGROUND),
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
