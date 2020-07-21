#include "TextRow.h"

TextRow::TextRow(int column) : _cells(column) {}

void TextRow::ReplaceCell(int index, const UChar &uch, const TextAttribute &attr) {
  DeleteCell(index);
  _cells[index - 1] = TextCell(uch, attr);
  int w = uch.width();
  if (w > 1) {
    TextCell *parent = &_cells[index - 1];
    for (int i = 1; i < w; i++) {
      DeleteCell(index + i);
      _cells[index + i - 1] = TextCell(parent);
    }
  }
}

void TextRow::DeleteCell(int index) {
  const TextCell &cell = _cells.at(index - 1);
  if (cell.isNull()) {
    return;
  }
  if (cell.isNormal()) {
    int w = cell.uch().width();
    for (int i = 0; i < w; i++) {
      _cells[index + i - 1].SetNull();
    }
  }
  if (cell.isExtend()) {
    int px = _FindParentIndex(cell.parent()) + 1;
    if (px != -1) {
      DeleteCell(px);
    }
  }
}

const TextCell &TextRow::GetCell(int index) const {
  return _cells.at(index - 1);
}

TextCell &TextRow::GetCell(int index) { return _cells[index - 1]; }

void TextRow::SetWraped(bool enabled) { _wraped = enabled; }

bool TextRow::isWraped() const { return _wraped; }

bool TextRow::isEmptyRow() const {
  for (size_t i = 0; i < _cells.size(); i++) {
    if (!_cells.at(i).isNull()) {
      return false;
    }
  }
  return true;
}

int TextRow::GetEffectLength() const {
  int len = _cells.size();
  while (len > 0 && GetCell(len).isNull()) {
    len -= 1;
  }
  return len;
}

int TextRow::_FindParentIndex(TextCell *parent) {
  for (size_t i = 0; i < _cells.size(); i++) {
    if (parent == &_cells.at(i)) {
      return i;
    }
  }
  return -1;
}
