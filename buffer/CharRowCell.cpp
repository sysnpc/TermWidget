#include "CharRowCell.h"

CharRowCell::CharRowCell() : _type(CT_Null) {}

// CharRowCell::CharRowCell(const QString& str) noexcept
//    : _type(CT_Normal), _uch(str), _parent(nullptr) {}

CharRowCell::CharRowCell(const UChar& uch)
    : _type(CT_Normal), _uch(uch), _parent(nullptr) {}

CharRowCell::CharRowCell(const CharRowCell* parent)
    : _type(CT_Extend), _parent(parent) {}

int CharRowCell::width() const {
  if (_type == CT_Null || _type == CT_Extend) {
    return 1;
  } else {
    return _uch.width();
  }
}
