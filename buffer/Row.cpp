#include "Row.h"

Row::Row(const int rowWidth)
    : _rowWidth{rowWidth},
      _charRow{rowWidth},
      _attrRow{rowWidth},
      _wraped{false} {}

void Row::replace(int x, const CharRowCell &charCell,
                  const AttrRowCell &attrCell) {
  _charRow.ClearCell(x);
  const CharRowCell &cc = _charRow.replace(x, charCell);
  int w = cc.uch().width();
  if (w > 1) {
    int t = 1;
    while (t < w) {
      _charRow.ClearCell(x + t);
      _charRow.replace(x + t, CharRowCell(&cc));
      t += 1;
    }
  }

  _attrRow.replace(x, attrCell);
}

void Row::insert(int x, const CharRowCell &charCell,
                 const AttrRowCell &attrCell) {
  Q_UNUSED(x)
  Q_UNUSED(charCell)
  Q_UNUSED(attrCell)
}

void Row::fill(const CharRowCell &charCell, const AttrRowCell &attrCell) {
  for (int i = 1; i <= _rowWidth; i++) {
    _charRow.replace(i, charCell);
    _attrRow.replace(i, attrCell);
  }
}

void Row::ClearCell(int x) {
  _charRow.ClearCell(x);
  int w = _charRow.at(x).width();
  int t = 0;
  while (t < w) {
    _attrRow.ClearCell(x + t);
    t += 1;
  }
}
