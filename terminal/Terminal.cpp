#include "Terminal.h"

#include <QApplication>
#include <QTextBoundaryFinder>
#include "../parser/OutputStateMachineEngine.h"
#include "TerminalDispatch.h"

using namespace PresudoMicrosoft::Terminal::Core;

Terminal::Terminal(int row, int column) {
  auto dispatch = std::make_unique<TerminalDispatch>(this);
  auto engine = std::make_unique<OutputStateMachineEngine>(std::move(dispatch));
  _stateMachine = std::make_unique<StateMachine>(std::move(engine));

  _buffer = std::make_unique<TextBuffer>(TextBuffer::Main, row, column);
  _size = Size{row, column};
}

void Terminal::SetTerminalInput(ITerminalInput* input) { _input = input; }

void Terminal::SetRenderTarget(IRenderTarget* renderTarget) {
  _renderTarget = renderTarget;
  _buffer->SetRenderTarget(renderTarget);
}

void Terminal::SetTerminalControl(ITerminalControl* control) {
  _control = control;
  _buffer->SetTerminalControl(control);
}

void Terminal::Write(const QString& str) {
  //  qDebug() << str;
  wchar_t arr[str.size()];
  int len = str.toWCharArray(arr);
  _stateMachine->ProcessString(std::wstring_view(arr, len));
}

void Terminal::UserResize(const Size& newSize) {
  //  if (_buffer->isMain()) {
  //    std::unique_ptr<TextBuffer> newBuffer = _buffer->NewBuffer(newSize);
  //    _buffer.swap(newBuffer);
  //  }
}

QString Terminal::GetText(const Coord& start, const Coord& end) {
  if (start.x == 0 && start.y == 0 && end.x == 0 && end.y == 0) {
    return "";
  } else {
    return _buffer->GetText(start, end);
  }
}

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
