#include "TextBuffer.h"

TextBuffer::TextBuffer(int row, int column, TextBuffer::TextBufferType type)
    : _row(row), _column(column), _type(type) {
  _cursor = std::make_unique<TextCursor>();
  // initialize ROWs
  for (int i = 0; i < row; ++i) {
    _rows.emplace_back(column);
  }
}

TextCursor* TextBuffer::GetCursor() { return _cursor.get(); }

bool TextBuffer::isDrawCell(int x, int y) const {
  if (y <= int(_rows.size()) && x <= _column) {
    const TextRow& row = GetRow(y);
    if (row.GetCell(x).isNormal()) {
      return true;
    }
  }
  return false;
}

const TextRow& TextBuffer::GetRow(int index) const {
  return _rows.at(index - 1);
}

TextRow& TextBuffer::GetRow(int index) { return _rows[index - 1]; }

const TextCell& TextBuffer::GetCell(int x, int y) const {
  return GetRow(y).GetCell(x);
}

void TextBuffer::Append(const TextCell& cell) {
  if (cell.isNormal()) {
    TextRow& row = _rows.at(_cursor->Ry() - 1);
    row.ReplaceCell(_cursor->X(), cell.uch(), cell.attr());
  } else if (cell.isNull()) {
    auto x = _cursor->X();
    _cursor->X(x + 1);
  } else {
  }
}

int TextBuffer::GetHeight() const { return _rows.size(); }

int TextBuffer::GetEffectiveRowHeight() const {
  int h = _rows.size();
  while (_rows.at(h - 1).isEmptyRow()) {
    h -= 1;
  }
  return h;
}

void TextBuffer::SetRowWraped(bool enabled) {
  _rows[_cursor->Ry() - 1].SetWraped(enabled);
}

void TextBuffer::InsertRow(int index) {
  TextRow row(_column);
  _rows.insert(_rows.begin() + index, row);
}

void TextBuffer::DeleteRow(int index) { _rows.erase(_rows.begin() + index); }

void TextBuffer::DeleteRow(int st, int ed) {
  _rows.erase(_rows.begin() + st, _rows.begin() + ed);
}

void TextBuffer::AppendRow(int n) {
  for (int i = 0; i < n; i++) {
    _rows.emplace_back(_column);
  }
}

void TextBuffer::PopBackRow(int n) {
  for (int i = 0; i < n; i++) {
    _rows.pop_back();
  }
}

void TextBuffer::InsertUChar(const UChar& uchar) {}

void TextBuffer::ReplaceUChar(const UChar& uchar) {
  TextRow& row = _rows.at(_cursor->Ry() - 1);
  row.ReplaceCell(_cursor->X(), uchar, _cursor->Attr());
}

// void TextBuffer::FillScreen(const UAttrCharCell& cell) {
//  for (int i = 1; i <= _size.row; i++) {
//    GetRowByOffset(i).fill(cell.createCharRowCell(),
//    cell.createAttrRowCell());
//  }
//}

// void TextBuffer::InsertEmptyLine() {
//  const Coord& pos = _cur.GetPosition();
//  int x = pos.x, y = pos.y;
//  y = y + 1;
//  if (y > _margin.bottom) {
//    y = _margin.bottom;
//    ScrollUp(1);
//  }
//  x = _margin.right + 1;
//  _cur.SetPosition(x, y);
//}

// void TextBuffer::ExtendLine() {
//  _storage.emplace_back(_size.column);
//  _flagY += 1;
//  _cur.SetYPosition(_cur.GetPosition().y - 1);
//}

// void TextBuffer::InsertLine(int n) {
//  const Coord& pos = _cur.GetPosition();
//  int t = qMin(n, _margin.bottom - pos.y + 1);
//  while (t > 0) {
//    Row row(_size.column);
//    _storage.insert(_storage.begin() + (pos.y + _flagY - 1), row);

//    _storage.erase(_storage.begin() + (_margin.bottom + _flagY));

//    t--;
//  }
//}

// void TextBuffer::DeleteLine(int n) {
//  const Coord& pos = _cur.GetPosition();
//  int t = qMin(n, _margin.bottom - pos.y + 1);
//  while (t > 0) {
//    Row row(_size.column);
//    _storage.insert(_storage.begin() + (_margin.bottom + _flagY), row);

//    _storage.erase(_storage.begin() + (pos.y + _flagY - 1));

//    t--;
//  }
//}

// void TextBuffer::ResizeSaveCursor() {
//  _resizeState.cursor = _cur;
//  _resizeState.flagY = _flagY;
//}

// void TextBuffer::ResizeRestoreCursor() {
//  _cur = _resizeState.cursor;
//  _flagY = _resizeState.flagY;
//}

// void TextBuffer::CR() { _cur.SetXPosition(1); }

// void TextBuffer::LF(bool withReturn) {
//  const Coord& pos = _cur.GetPosition();

//  GetRowByOffset(pos.y).setWraped(true);

//  int y = pos.y + 1;

//  if (withReturn) {
//    _cur.SetXPosition(1);
//  }
//  if (y > _margin.bottom) {
//    _cur.SetYPosition(_margin.bottom);
//    ScrollUp(1);
//  } else {
//    _cur.SetYPosition(y);
//  }
//  // TODO: need adjust
//  if (_control) _control->NotifyHightChange(_storage.size());
//}

// void TextBuffer::ReverseLF() {
//  const Coord& pos = _cur.GetPosition();
//  int y = pos.y - 1;
//  if (y < _margin.top) {
//    _cur.SetYPosition(_margin.top);
//    ScrollDown(1);
//  } else {
//    _cur.SetYPosition(y);
//  }
//}

// QString TextBuffer::GetText(Coord start, Coord end) {
//  QString str = "";
//  int maxX = _size.column;
//  int maxY = _storage.size();
//  start.x = std::clamp(start.x, 1, maxX);
//  start.y = std::clamp(start.y, 1, maxY);
//  end.x = std::clamp(end.x, 1, maxX);
//  end.y = std::clamp(end.y, 1, maxY);
//  while ((start.y < end.y) || (start.y == end.y && start.x <= end.x)) {
//    const Row& row = GetRow(start.y);
//    const CharRowCell& charCell = row.GetCharRow().at(start.x);
//    if (charCell.isNormal()) {
//      str.append(charCell.uch().udata());
//    } else {
//      str.append(" ");
//    }
//    start.x += 1;
//    if (start.x > _size.column) {
//      if (row.isWraped()) {
//        str.append("\r\n");
//      }
//      start.x = 1;
//      start.y += 1;
//    }
//  }
//  return str;
//}

// Row& TextBuffer::GetRowByOffset(const int index) {
//  const int offsetIndex = _flagY + index;
//  return _storage.at(offsetIndex - 1);
//}

// const Row& TextBuffer::GetRow(const int index) const {
//  return _storage.at(index - 1);
//}

// int TextBuffer::OffsetY() const {
//  const Coord& pos = _cur.GetPosition();
//  return pos.y + _flagY;
//}

// bool TextBuffer::IsWithInPos(Coord pos) const {
//  int h = _storage.size();
//  if (pos.x > 0 && pos.x <= _size.column && pos.y > 0 && pos.y <= h) {
//    return true;
//  }
//  return false;
//}

// int TextBuffer::ResizeRow(int row) {
//  int df = 0;
//  if (row > _size.row) {
//    df = row - _size.row;
//    if (row > int(_storage.size())) {
//      int dr = row - _storage.size();
//      for (int i = 0; i < dr; i++) {
//        _storage.push_back(_size.column);
//        df -= 1;
//      }
//    } else {
//    }
//  } else {
//    df = _size.row - row;
//    int h = this->GetNotEmptyLineCount();
//    int dh = _storage.size() - h;
//    int dr = _size.row - row;
//    int t = qMin(dh, dr);
//    while (t > 0) {
//      _storage.pop_back();
//      t = t - 1;
//      df -= 1;
//    }
//  }
//  _size.row = row;
//  _margin.bottom = row;
//  return df;
//}

// int TextBuffer::GetNotEmptyLineCount() {
//  int h = _storage.size();
//  int cy = _cur.GetPosition().y + _flagY;
//  while (h > cy && _storage[h - 1].GetCharRow().IsEmpty()) {
//    h = h - 1;
//  }
//  return h;
//}
