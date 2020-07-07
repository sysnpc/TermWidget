#include "AttrRow.h"

AttrRow::AttrRow(int rowWidth) : _data(rowWidth, AttrRowCell()) {}

void AttrRow::replace(int x, const AttrRowCell &attrCell) {
  _data[x - 1] = attrCell;
}

void AttrRow::ClearCell(int x) { _data[x - 1] = AttrRowCell(); }
