#include "Terminal.h"

#include <QApplication>
#include <QDebug>
#include <QTextBoundaryFinder>
#include <QtMath>
#include "../parser/OutputStateMachineEngine.h"
#include "../tools/til/at.h"
#include "TerminalDispatch.h"

using namespace PresudoMicrosoft::Console::VirtualTerminal;

Terminal::Terminal(int row, int column) {
  _row = row;
  _column = column;
  _margin = {1, 1, column, row};

  auto dispatch = std::make_unique<TerminalDispatch>(this);
  auto engine = std::make_unique<OutputStateMachineEngine>(std::move(dispatch));
  _stateMachine = std::make_unique<StateMachine>(std::move(engine));
  _buffer = std::make_unique<TextBuffer>(row, column, TextBuffer::Main);
}

// void Terminal::SetTerminalInput(ITerminalInput* input) { _input = input; }

// void Terminal::SetRenderTarget(IRenderTarget* renderTarget) {
//  _renderTarget = renderTarget;
//  //  _buffer->SetRenderTarget(renderTarget);
//}

// void Terminal::SetTerminalControl(ITerminalControl* control) {
//  _control = control;
//  //  _buffer->SetTerminalControl(control);
//}

void Terminal::SetFont(const QFont& font) {
  _fm = QFontMetrics(font);
  _uch2width.clear();
}

const TextBuffer* Terminal::GetTextBuffer() const { return _buffer.get(); }

void Terminal::Write(const QString& str) {
  //  qDebug() << str;
  wchar_t arr[str.size()];
  int len = str.toWCharArray(arr);
  _stateMachine->ProcessString(std::wstring_view(arr, len));
}

void Terminal::ResizeColumn(int column) {
  std::unique_ptr<TextBuffer> buffer =
      std::make_unique<TextBuffer>(_row, column, TextBuffer::Main);
  auto cursor = buffer->GetCursor();
  ScrollMargin margin{1, 1, column, _row};

  auto _cursor = _buffer->GetCursor();
  auto ry = _cursor->Ry();
  for (int i = 1; i <= ry; i++) {
    auto row = _buffer->GetRow(i);
    auto len = i == ry ? _cursor->X() - 1 : row.GetEffectLength();
    for (int j = 1; j <= len; j++) {
      auto cell = row.GetCell(j);
      if (cell.isNormal() || cell.isNull()) {
        // append cell
        int x = cursor->X();
        int y = cursor->Y();
        int w = cell.uch().width();

        if (x > margin.Right || x + w > margin.Right + 1) {
          if (_autoWrap) {
            x = 1;
            y += 1;
            if (y > margin.Bottom) {
              y = margin.Bottom;
              buffer->InsertRow(cursor->FlagY() + margin.Bottom);
              cursor->ScrollUp(1);
            }
          } else {
            x = margin.Right;
          }
        }
        cursor->X(x);
        cursor->Y(y);
        buffer->Append(cell);
        cursor->X(x + w);
      }
    }
    if (row.isWraped()) {
      auto cursor = buffer->GetCursor();
      buffer->SetRowWraped(true);
      int y = cursor->Y() + 1;
      cursor->X(1);
      if (y > margin.Bottom) {
        y = margin.Bottom;
        buffer->InsertRow(cursor->FlagY() + margin.Bottom);
        cursor->ScrollUp(1);
      }
      cursor->Y(y);
    }
  }

  _column = column;
  _margin = margin;
  _buffer = std::move(buffer);
}

void Terminal::ResizeRow(int row) {
  ScrollMargin margin{1, 1, _column, row};
  auto cursor = _buffer->GetCursor();
  if (row > _row) {
    int dr = row - _row;
    int flagY = cursor->FlagY();
    int y = cursor->Y();
    if (flagY >= dr) {
      cursor->FlagY(flagY - dr);
      cursor->Y(y + dr);
    } else {
      dr = dr - flagY;
      cursor->FlagY(0);
      cursor->Y(y + flagY);
      _buffer->AppendRow(dr);
    }
  } else {
    int dr = _row - row;
    int nr = _buffer->GetHeight() - _buffer->GetEffectiveRowHeight();
    if (dr <= nr) {
      _buffer->PopBackRow(dr);
    } else {
      _buffer->PopBackRow(nr);
      dr = dr - nr;
      int flagY = cursor->FlagY();
      int y = cursor->Y();
      cursor->FlagY(flagY + dr);
      cursor->Y(y - dr);
    }
  }
  _margin = margin;
  _row = row;
}

// void Terminal::UserResize(const Size& newSize) {
//  if (_buffer->isMain()) {
//    std::unique_ptr<TextBuffer> newBuffer = _buffer->NewBuffer(newSize);
//    _buffer.swap(newBuffer);
//  }
//}

QString Terminal::GetText(const Coord& start, const Coord& end) {
  //  if (start.x == 0 && start.y == 0 && end.x == 0 && end.y == 0) {
  //    return "";
  //  } else {
  //    return _buffer->GetText(start, end);
  //  }
}

QStringList Terminal::_GraphemeSplit(const QString& str) {
  QTextBoundaryFinder tbf(QTextBoundaryFinder::Grapheme, str);
  QStringList strs;
  int pre = 0;
  int next = tbf.toNextBoundary();
  while (next != -1) {
    strs.append(str.mid(pre, next - pre));
    pre = next;
    next = tbf.toNextBoundary();
  }
  return strs;
}

int Terminal::_CalcWidth(const QString& uch) {
  int w = _uch2width.value(uch);
  if (w == 0) {
    w = qCeil(_fm.horizontalAdvance(uch) / (_fm.averageCharWidth() * 1.0));
    _uch2width[uch] = w;
  }
  return w;
}

void Terminal::_ScrollUp(int n) {
  int t = qMin(n, _margin.Bottom - _margin.Top + 1);
  auto cursor = _buffer->GetCursor();
  while (t > 0) {
    _buffer->InsertRow(cursor->FlagY() + _margin.Bottom);

    if (_buffer->isMain() && _margin.Top == 1) {
      cursor->ScrollUp(1);
    } else {
      _buffer->DeleteRow(cursor->FlagY() + _margin.Top - 1);
    }

    t--;
  }
}

void Terminal::_ScrollDown(int n) {
  int t = qMin(n, _margin.Bottom - _margin.Top + 1);
  auto cursor = _buffer->GetCursor();
  while (t > 0) {
    _buffer->InsertRow(cursor->FlagY() + _margin.Top - 1);
    _buffer->DeleteRow(cursor->FlagY() + _margin.Bottom);

    t--;
  }
}

/*

void Terminal::WriteCell(const std::vector<UAttrCharCell>& ucells) {
  Cursor& cursor = _buffer->GetCursor();

  // Defer the cursor drawing while we are iterating the string, for a better
  // performance. We can not waste time displaying a cursor event when we know
  // more text is coming right behind it.
  cursor.StartDeferDrawing();

  for (const UAttrCharCell& ucell : ucells) {
    _buffer->Replace(ucell);
  }

  cursor.EndDeferDrawing();
}

int Terminal::CalcWidth(const QString& str) { return _input->CalcWidth(str); }

void Terminal::CarriageReturn() { _buffer->CR(); }

void Terminal::LineFeed(const LineFeedType lineFeedType) {
  switch (lineFeedType) {
    case LineFeedType::WithReturn:
      _buffer->LF(true);
      break;
    case LineFeedType::WithoutReturn:
      _buffer->LF(false);
      break;
    case LineFeedType::DependsOnMode:
      _buffer->LF(true);
      break;
  }
}

void Terminal::ReverseLineFeed() { _buffer->ReverseLF(); }

void Terminal::CursorUp(int distance) { _buffer->CursorUp(distance); }

void Terminal::CursorDown(int distance) { _buffer->CursorDown(distance); }

void Terminal::CursorForward(int distance) { _buffer->CursorForward(distance); }

void Terminal::CursorBackward(int distance) {
  _buffer->CursorBackward(distance);
}

void Terminal::CursorColumn(int column) { _buffer->SetCursorX(column, false); }

void Terminal::SetCursorPosition(int x, int y) { _buffer->SetCursor(x, y); }

void Terminal::InsertLine(int n) { _buffer->InsertLine(n); }

void Terminal::DeleteLine(int n) { _buffer->DeleteLine(n); }

const TextBuffer& Terminal::GetTextBuffer() const noexcept { return *_buffer; }
TextBuffer& Terminal::GetTextBuffer() noexcept { return *_buffer; }

void Terminal::EraseDisplayToEnd() {
  const Cursor& cursor = _buffer->GetCursor();
  Coord st = cursor.GetPosition();
  Coord ed = Coord{_size.column, _size.row};
  _buffer->Erase(st, ed);
}

void Terminal::EraseDisplayToBegin() {
  const Cursor& cursor = _buffer->GetCursor();
  Coord st = Coord{1, 1};
  Coord ed = cursor.GetPosition();
  _buffer->Erase(st, ed);
}

void Terminal::EraseDisplayAll() {
  Coord st = Coord{1, 1};
  Coord ed = Coord{_size.column, _size.row};
  _buffer->Erase(st, ed);
}

void Terminal::EraseDisplaySaveLines() {
  int flagY = _buffer->GetFlagY();
  _buffer->Erase(0, flagY);
}

void Terminal::EraseLineToRight() {
  const Cursor& cursor = _buffer->GetCursor();
  Coord st = cursor.GetPosition();
  Coord ed = Coord{_size.column, st.y};
  _buffer->Erase(st, ed);
}

void Terminal::EraseLineToLeft() {
  const Cursor& cursor = _buffer->GetCursor();
  Coord ed = cursor.GetPosition();
  Coord st = Coord{1, ed.y};
  _buffer->Erase(st, ed);
}

void Terminal::EraseLineAll() {
  const Cursor& cursor = _buffer->GetCursor();
  int y = cursor.GetPosition().y;
  Coord st = {1, y};
  Coord ed = {_size.column, y};
  _buffer->Erase(st, ed);
}

void Terminal::SetCursorKeysMode(bool enabled) {
  _input->SetCursorKeysMode(enabled);
}

void Terminal::SetKeypadMode(bool enabled) { _input->SetKeypadMode(enabled); }

void Terminal::WarningBell() { _control->WarningBell(); }

void Terminal::ResizeColumn(int column) {
  if (_size.column != column) {
    if (_buffer->isMain()) {
      TextBuffer* buffer = _buffer->ResizeColumn(column);
      _buffer.reset(buffer);
    } else {
    }
  }
  _size.column = column;
}

int Terminal::ResizeRow(int row) {
  int df = 0;
  if (_size.row != row) {
    if (_buffer->isMain()) {
      df = _buffer->ResizeRow(row);
    } else {
    }
  }
  _size.row = row;
  return df;
}

void Terminal::SetWindowTitle(const QString& title) {
  _control->SetWindowTitle(title);
}
void Terminal::SetTopBottomMargin(int top, int bottom) {
  top = top == 0 ? 1 : top;
  bottom = bottom == 0 ? _size.row : bottom;
  _buffer->SetTopBottomMargin(top, bottom);
}

void Terminal::UseMainBuffer() {
  if (_save_buffer != nullptr) {
    _buffer.release();
    _buffer = std::move(_save_buffer);
  }
}

void Terminal::UseAlternateBuffer() {
  _save_buffer = std::move(_buffer);
  _control->RenderInit();
  _buffer = std::make_unique<TextBuffer>(TextBuffer::Alternate, _size.row,
                                         _size.column);
  _buffer->SetRenderTarget(_renderTarget);
  _buffer->SetTerminalControl(_control);
}

void Terminal::SendDeviceAttributes(const QString& str) {
  _control->SendDeviceAttributes(str);
}

void Terminal::ScreenAlignmentPattern() {
  _buffer->FillScreen(UAttrCharCell(UChar("E", 1)));
  _buffer->SetMargin(1, 1, _size.column, _size.row);
  _buffer->SetCursor(1, 1);
}

void Terminal::BracketedPasteMode(bool enabled) {
  _control->BracketedPasteMode(enabled);
}

void Terminal::HorizontalTabSet() {
  _InitTabStopsForWidth(_size.column);
  int x = _buffer->GetCursor().GetPosition().x;
  _tabStopColumns.at(x - 1) = true;
}

void Terminal::TabClear(const size_t clearType) {
  switch (clearType) {
    case TabClearType::ClearCurrentColumn:
      _ClearSingleTabStop();
      break;
    case TabClearType::ClearAllColumns:
      _ClearAllTabStops();
      break;
  }
}

void Terminal::ForwardTab(const size_t numTabs) {
  int x = _buffer->GetCursor().GetPosition().x;
  int y = _buffer->GetCursor().GetPosition().y;
  auto tabsPerformed = 0u;
  _InitTabStopsForWidth(_size.column);
  while (x <= _size.column && tabsPerformed < numTabs) {
    x++;
    if (til::at(_tabStopColumns, x - 1)) {
      tabsPerformed++;
    }
  }
  _buffer->SetCursor(x, y);
}

void Terminal::BackwardsTab(const size_t numTabs) {
  int x = _buffer->GetCursor().GetPosition().x;
  int y = _buffer->GetCursor().GetPosition().y;
  auto tabsPerformed = 0u;
  _InitTabStopsForWidth(_size.column);
  while (x > 1 && tabsPerformed < numTabs) {
    x--;
    if (til::at(_tabStopColumns, x - 1)) {
      tabsPerformed++;
    }
  }
  _buffer->SetCursor(x, y);
}
*/

void Terminal::Print(const wchar_t wch) { PrintString({&wch, 1}); }

void Terminal::PrintString(const std::wstring_view string) {
  QString str = QString::fromWCharArray(string.data(), string.size());
  QStringList uchs = _GraphemeSplit(str);
  auto cursor = _buffer->GetCursor();
  for (const QString& ch : uchs) {
    int w = _CalcWidth(ch);
    UChar uch(ch, w);
    int x = cursor->X();
    int y = cursor->Y();

    if (x > _margin.Right || x + w > _margin.Right + 1) {
      if (_autoWrap) {
        x = 1;
        y += 1;
        if (y > _margin.Bottom) {
          y = _margin.Bottom;
          _ScrollUp(1);
        }
      } else {
        x = _margin.Right;
      }
    }
    cursor->X(x);
    cursor->Y(y);
    _buffer->ReplaceUChar(uch);
    cursor->X(x + w);
  }
}

void Terminal::Execute(const wchar_t wch) { PrintString({&wch, 1}); }

int Terminal::GetRow() const { return _row; }

int Terminal::GetColumn() const { return _column; }

Coord Terminal::GetCursorPos() const {
  auto cursor = _buffer->GetCursor();
  return Coord{cursor->X(), cursor->Y()};
}

bool Terminal::SetCursorPos(const Coord& pos) {
  auto cursor = _buffer->GetCursor();
  cursor->X(pos.x);
  cursor->Y(pos.y);
  return true;
}

ScrollMargin Terminal::GetScrollMargin() const { return _margin; }

bool Terminal::PrivateScrollUp(int distance) { _ScrollUp(distance); }

bool Terminal::PrivateScrollDown(int distance) { _ScrollDown(distance); }

bool Terminal::PrivateIsVtInputEnabled() const {}

TextCursor* Terminal::PrivateGetTextCursor() const {
  return _buffer->GetCursor();
}

bool Terminal::SetConsoleWindowInfo(const bool absolute,
                                    const ScrollMargin& window) {}

bool Terminal::PrivateSetCursorKeysMode(const bool applicationMode) {}

bool Terminal::PrivateSetKeypadMode(const bool applicationMode) {}

bool Terminal::PrivateEnableWin32InputMode(const bool win32InputMode) {}

bool Terminal::PrivateSetAnsiMode(const bool ansiMode) {}

bool Terminal::PrivateSetScreenMode(const bool reverseMode) {}

bool Terminal::PrivateSetAutoWrapMode(const bool wrapAtEOL) {
  _autoWrap = wrapAtEOL;
  return true;
}

bool Terminal::PrivateShowCursor(const bool show) {}

bool Terminal::PrivateAllowCursorBlinking(const bool enable) {}

bool Terminal::PrivateSetScrollingRegion(const QMargins &scrollMargins) {}

bool Terminal::PrivateWarningBell() { qApp->beep(); }

bool Terminal::PrivateGetLineFeedMode() const {
  // automatic newline （LNM）
  return true;
}

bool Terminal::PrivateLineFeed(const bool withReturn) {
  auto cursor = _buffer->GetCursor();

  _buffer->SetRowWraped(true);

  int y = cursor->Y() + 1;

  if (withReturn) {
    cursor->X(1);
  }
  if (y > _margin.Bottom) {
    y = _margin.Bottom;
    _ScrollUp(1);
  }
  cursor->Y(y);
  return true;
}

bool Terminal::PrivateReverseLineFeed() {}

bool Terminal::SetConsoleTitleW(const std::wstring_view title) {}

bool Terminal::PrivateUseAlternateScreenBuffer() {}

bool Terminal::PrivateUseMainScreenBuffer() {}

bool Terminal::PrivateEnableVT200MouseMode(const bool enabled) {}

bool Terminal::PrivateEnableUTF8ExtendedMouseMode(const bool enabled) {}

bool Terminal::PrivateEnableSGRExtendedMouseMode(const bool enabled) {}

bool Terminal::PrivateEnableButtonEventMouseMode(const bool enabled) {}

bool Terminal::PrivateEnableAnyEventMouseMode(const bool enabled) {}

bool Terminal::PrivateEnableAlternateScroll(const bool enabled) {}

bool Terminal::PrivateEraseCell(const Coord& start, const Coord& end) {
  Coord tp = start;
  auto cursor = _buffer->GetCursor();
  while ((tp.y < end.y) || (tp.y == end.y && tp.x <= end.x)) {
    TextRow& row = _buffer->GetRow(tp.y + cursor->FlagY());
    TextCell& cell = row.GetCell(tp.x);
    cell.SetNull();

    tp.x += 1;
    if (tp.x > _column) {
      tp.x = 1;
      row.SetWraped(false);
      tp.y += 1;
    }
  }
  return true;
}

bool Terminal::PrivateEraseSaveLines() {
  auto cursor = _buffer->GetCursor();
  _buffer->DeleteRow(0, cursor->FlagY());
  cursor->FlagY(0);
  return true;
}

bool Terminal::PrivateRefreshWindow() {}

bool Terminal::SetConsoleOutputCP(const unsigned int codepage) {}

bool Terminal::GetConsoleOutputCP(unsigned int &codepage) {}

bool Terminal::PrivateSuppressResizeRepaint() {}

bool Terminal::IsConsolePty() const {}

bool Terminal::DeleteLines(const size_t count) {}

bool Terminal::InsertLines(const size_t count) {}

bool Terminal::MoveToBottom() const {}

bool Terminal::PrivateGetColorTableEntry(const size_t index,
                                         COLORREF &value) const {}

bool Terminal::PrivateSetColorTableEntry(const size_t index,
                                         const COLORREF value) const {}

bool Terminal::PrivateSetDefaultForeground(const COLORREF value) const {}

bool Terminal::PrivateSetDefaultBackground(const COLORREF value) const {}

bool Terminal::PrivateFillRegion(const Coord startPosition,
                                 const size_t fillLength,
                                 const wchar_t fillChar,
                                 const bool standardFillAttrs) {}

bool Terminal::PrivateScrollRegion(const ScrollMargin scrollRect,
                                   const std::optional<ScrollMargin> clipRect,
                                   const Coord destinationOrigin,
                                   const bool standardFillAttrs) {}
