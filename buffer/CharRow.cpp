#include "CharRow.h"

CharRow::CharRow(int rowWidth) : _data(rowWidth, CharRowCell()) {}

const CharRowCell &CharRow::replace(int x, const CharRowCell &charCell) {
  _data[x - 1] = charCell;
  return _data.at(x - 1);
}

void CharRow::ClearCell(int x) {
  const CharRowCell &cell = _data.at(x - 1);
  if (cell.isNull()) {
    return;
  }
  if (cell.isNormal()) {
    int w = cell.uch().width();
    for (int i = 0; i < w; i++) {
      _data[x + i - 1] = CharRowCell::CT_Null;
    }
  }
  if (cell.isExtend()) {
    int px = _FindParentIndex(cell.parent()) + 1;
    ClearCell(px);
  }
}

bool CharRow::IsEmpty() const {
  for (size_t i = 0; i < _data.size(); i++) {
    if (_data.at(i).isNormal()) {
      return false;
    }
  }
  return true;
}

int CharRow::GetLastNotNullIndex() const {
  int index = _data.size();
  while (index > 0) {
    if (!_data[index - 1].isNull()) {
      break;
    }
    index -= 1;
  }
  return index;
}

int CharRow::_FindParentIndex(const CharRowCell *parent) {
  for (size_t i = 0; i < _data.size(); i++) {
    if (parent == &_data.at(i)) {
      return i;
    }
  }
  return -1;
}
