#include "TextBuffer.h"

#include <QDebug>

using namespace PresudoMicrosoft::Console;

TextBuffer::TextBuffer(const TextBuffer::TextBufferType type, int row,
                       int column)
    : _type(type),
      _storage{},
      _cursor(1, 1),
      _resizeState{_cursor, 0},
      _flagY(0),
      _size{row, column},
      _margin{1, row, 1, column} {
  // initialize ROWs
  for (int i = 0; i < row; ++i) {
    _storage.emplace_back(column);
  }
}

void TextBuffer::SetRenderTarget(IRenderTarget* renderTarget) {
  _renderTarget = renderTarget;
  _cursor.SetRenderTarget(renderTarget);
}

void TextBuffer::SetTerminalControl(ITerminalControl* control) {
  _control = control;
}

bool TextBuffer::isCursor(const Coord& coord) const {
  const Coord& pos = _cursor.GetPosition();
  if (pos.x == coord.x && pos.y + _flagY == coord.y) {
    return true;
  } else {
    return false;
  }
}

void TextBuffer::Insert(const UAttrCharCell& ucell, const bool wrap) {
  Q_UNUSED(ucell)
  Q_UNUSED(wrap)
  Coord pos = _cursor.GetPosition();
  if (pos.x > _margin.right) {
  }
}

void TextBuffer::Replace(const UAttrCharCell& ucell, const bool wrap) {
  const Coord& pos = _cursor.GetPosition();
  int x = pos.x, y = pos.y;
  Rect rect{x, y, x, y};
  int w = ucell.uch().width();
  if (x > _margin.right || x + w > _margin.right + 1) {
    if (wrap) {
      x = 1;
      y += 1;
      if (y > _margin.bottom) {
        y = _margin.bottom;
        ScrollUp(1);
      }
    } else {
      x = _margin.right;
    }
  }

  Row& row = GetRowByOffset(y);
  row.replace(x, ucell.createCharRowCell(), ucell.createAttrRowCell());
  x += w;
  _cursor.SetPosition(x, y);

  rect.x2 = x;
  rect.y2 = y;
  if (_renderTarget) _renderTarget->TriggerRedraw(rect);
  if (_control) _control->NotifyHightChange(_storage.size());
}

void TextBuffer::FillScreen(const UAttrCharCell& cell) {
  for (int i = 1; i <= _size.row; i++) {
    GetRowByOffset(i).fill(cell.createCharRowCell(), cell.createAttrRowCell());
  }
}

void TextBuffer::InsertEmptyLine() {
  const Coord& pos = _cursor.GetPosition();
  int x = pos.x, y = pos.y;
  y = y + 1;
  if (y > _margin.bottom) {
    y = _margin.bottom;
    ScrollUp(1);
  }
  x = _margin.right + 1;
  _cursor.SetPosition(x, y);
}

void TextBuffer::ExtendLine() {
  _storage.emplace_back(_size.column);
  _flagY += 1;
  _cursor.SetYPosition(_cursor.GetPosition().y - 1);
}

void TextBuffer::ScrollUp(int n) {
  int t = qMin(n, _margin.bottom - _margin.top + 1);
  while (t > 0) {
    Row row(_size.column);
    _storage.insert(_storage.begin() + (_margin.bottom + _flagY), row);

    if (_type == TextBufferType::Main && _margin.top == 1) {
      _flagY += 1;
    } else {
      _storage.erase(_storage.begin() + (_margin.top + _flagY - 1));
    }

    t--;
  }
}

void TextBuffer::ScrollDown(int n) {
  int t = qMin(n, _margin.bottom - _margin.top + 1);
  while (t > 0) {
    Row row(_size.column);
    _storage.insert(_storage.begin() + (_margin.top + _flagY - 1), row);

    _storage.erase(_storage.begin() + (_margin.bottom + _flagY));

    t--;
  }
}

void TextBuffer::InsertLine(int n) {
  const Coord& pos = _cursor.GetPosition();
  int t = qMin(n, _margin.bottom - pos.y + 1);
  while (t > 0) {
    Row row(_size.column);
    _storage.insert(_storage.begin() + (pos.y + _flagY - 1), row);

    _storage.erase(_storage.begin() + (_margin.bottom + _flagY));

    t--;
  }
}

void TextBuffer::DeleteLine(int n) {
  const Coord& pos = _cursor.GetPosition();
  int t = qMin(n, _margin.bottom - pos.y + 1);
  while (t > 0) {
    Row row(_size.column);
    _storage.insert(_storage.begin() + (_margin.bottom + _flagY), row);

    _storage.erase(_storage.begin() + (pos.y + _flagY - 1));

    t--;
  }
}

void TextBuffer::CursorUp(int n) {
  const Coord& pos = _cursor.GetPosition();
  int y = std::clamp(pos.y - n, _margin.top, _margin.bottom);
  _cursor.SetYPosition(y);
}

void TextBuffer::CursorDown(int n) {
  const Coord& pos = _cursor.GetPosition();
  int y = std::clamp(pos.y + n, _margin.top, _margin.bottom);
  _cursor.SetYPosition(y);
}

void TextBuffer::CursorForward(int n) {
  const Coord& pos = _cursor.GetPosition();
  int x = std::clamp(pos.x + n, _margin.left, _margin.right);
  _cursor.SetXPosition(x);
}

void TextBuffer::CursorBackward(int n) {
  const Coord& pos = _cursor.GetPosition();
  int x = std::clamp(pos.x - n, _margin.left, _margin.right);
  _cursor.SetXPosition(x);
}

void TextBuffer::SetCursor(int x, int y) {
  x = std::clamp(x, 1, _margin.right);
  y = std::clamp(y, 1, _margin.bottom);
  _cursor.SetPosition(x, y);
}

void TextBuffer::SetCursorX(int x, bool clampInMargins) {
  if (clampInMargins) {
    x = std::clamp(x, _margin.left, _margin.right);
  } else {
    x = std::clamp(x, 1, _size.column);
  }
  _cursor.SetXPosition(x);
}

void TextBuffer::SetMargin(int left, int top, int right, int bottom) {
  _margin.left = left;
  _margin.top = top;
  _margin.right = right;
  _margin.bottom = bottom;
}

void TextBuffer::SetTopBottomMargin(int top, int bottom) {
  // TODO:: top < bottom
  _margin.top = top;
  _margin.bottom = bottom;
}

void TextBuffer::ResizeSaveCursor() {
  _resizeState.cursor = _cursor;
  _resizeState.flagY = _flagY;
}

void TextBuffer::ResizeRestoreCursor() {
  _cursor = _resizeState.cursor;
  _flagY = _resizeState.flagY;
}

void TextBuffer::CR() { _cursor.SetXPosition(1); }

void TextBuffer::LF(bool withReturn) {
  const Coord& pos = _cursor.GetPosition();

  GetRowByOffset(pos.y).setWraped(true);

  int y = pos.y + 1;

  if (withReturn) {
    _cursor.SetXPosition(1);
  }
  if (y > _margin.bottom) {
    _cursor.SetYPosition(_margin.bottom);
    ScrollUp(1);
  } else {
    _cursor.SetYPosition(y);
  }
  // TODO: need adjust
  if (_control) _control->NotifyHightChange(_storage.size());
}

void TextBuffer::ReverseLF() {
  const Coord& pos = _cursor.GetPosition();
  int y = pos.y - 1;
  if (y < _margin.top) {
    _cursor.SetYPosition(_margin.top);
    ScrollDown(1);
  } else {
    _cursor.SetYPosition(y);
  }
}

void TextBuffer::Erase(Coord st, Coord ed) {
  st.x = qMin(_size.column, st.x);
  while ((st.y < ed.y) || (st.y == ed.y && st.x <= ed.x)) {
    Row& row = GetRowByOffset(st.y);
    row.ClearCell(st.x);

    st.x += 1;
    if (st.x > _size.column) {
      st.x = 1;
      row.setWraped(false);
      st.y += 1;
    }
  }
}

void TextBuffer::Erase(int st, int count) {
  _storage.erase(_storage.begin() + st, _storage.begin() + st + count);
}

QString TextBuffer::GetText(Coord start, Coord end) {
  QString str = "";
  int maxX = _size.column;
  int maxY = _storage.size();
  start.x = std::clamp(start.x, 1, maxX);
  start.y = std::clamp(start.y, 1, maxY);
  end.x = std::clamp(end.x, 1, maxX);
  end.y = std::clamp(end.y, 1, maxY);
  while ((start.y < end.y) || (start.y == end.y && start.x <= end.x)) {
    const Row& row = GetRow(start.y);
    const CharRowCell& charCell = row.GetCharRow().at(start.x);
    if (charCell.isNormal()) {
      str.append(charCell.uch().udata());
    } else {
      str.append(" ");
    }
    start.x += 1;
    if (start.x > _size.column) {
      if (row.isWraped()) {
        str.append("\r\n");
      }
      start.x = 1;
      start.y += 1;
    }
  }
  return str;
}

Row& TextBuffer::GetRowByOffset(const int index) {
  const int offsetIndex = _flagY + index;
  return _storage.at(offsetIndex - 1);
}

const Row& TextBuffer::GetRow(const int index) const {
  return _storage.at(index - 1);
}

int TextBuffer::OffsetY() const {
  const Coord& pos = _cursor.GetPosition();
  return pos.y + _flagY;
}

bool TextBuffer::IsWithInPos(Coord pos) const {
  int h = _storage.size();
  if (pos.x > 0 && pos.x <= _size.column && pos.y > 0 && pos.y <= h) {
    return true;
  }
  return false;
}

TextBuffer* TextBuffer::ResizeColumn(int column) {
  // TODO:: lock write
  TextBuffer* buffer = new TextBuffer(_type, _size.row, column);
  const Coord& pos = _cursor.GetPosition();

  int h = this->GetNotEmptyLineCount();

  for (int i = 1; i <= h; i++) {
    const Row& row = GetRow(i);
    const CharRow& charRow = row.GetCharRow();

    if (i < pos.y + _flagY) {
      if (!charRow.IsEmpty()) {
        int ed = charRow.GetLastNotNullIndex();
        for (int j = 1; j <= ed; j++) {
          const CharRowCell& charCell = charRow.at(j);
          if (charCell.isNormal()) {
            const AttrRowCell& attrCell = row.GetAttrRow().at(j);
            buffer->Replace(UAttrCharCell(charCell, attrCell));
          } else if (charCell.isNull()) {
            buffer->CursorForward(1);
          } else {
          }
        }
        if (row.isWraped()) buffer->LF(true);
      } else {
        if (row.isWraped()) {
          buffer->LF(true);
        } else {
          buffer->InsertEmptyLine();
        }
      }

    } else {
      for (int j = 1; j < pos.x; j++) {
        const CharRowCell& charCell = charRow.at(j);
        if (charCell.isNormal()) {
          const AttrRowCell& attrCell = row.GetAttrRow().at(j);
          buffer->Replace(UAttrCharCell(charCell, attrCell));
        } else if (charCell.isNull()) {
          buffer->CursorForward(1);
        } else {
        }
      }
    }
  }

  if (int(_storage.size()) > _size.row) {
    int dh = _storage.size() - h;
    int tdh = buffer->LineCount() - buffer->GetNotEmptyLineCount();
    int t = dh - tdh;
    //    qDebug() << _storage.size() << h << dh
    //             << buffer->LineCount() - buffer->GetNotEmptyLineCount() <<
    //             tdh << t
    //             << buffer->GetFlagY();
    while (t > 0) {
      buffer->ExtendLine();
      t = t - 1;
    }
  }
  buffer->SetRenderTarget(_renderTarget);
  buffer->SetTerminalControl(_control);
  return buffer;
}

int TextBuffer::ResizeRow(int row) {
  int df = 0;
  if (row > _size.row) {
    df = row - _size.row;
    if (row > int(_storage.size())) {
      int dr = row - _storage.size();
      for (int i = 0; i < dr; i++) {
        _storage.push_back(_size.column);
        df -= 1;
      }
    } else {
    }
  } else {
    df = _size.row - row;
    int h = this->GetNotEmptyLineCount();
    int dh = _storage.size() - h;
    int dr = _size.row - row;
    int t = qMin(dh, dr);
    while (t > 0) {
      _storage.pop_back();
      t = t - 1;
      df -= 1;
    }
  }
  _size.row = row;
  _margin.bottom = row;
  return df;
}

int TextBuffer::GetNotEmptyLineCount() {
  int h = _storage.size();
  int cy = _cursor.GetPosition().y + _flagY;
  while (h > cy && _storage[h - 1].GetCharRow().IsEmpty()) {
    h = h - 1;
  }
  return h;
}
