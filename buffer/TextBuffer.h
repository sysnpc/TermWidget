#pragma once

#include <QVector>
#include <deque>
#include <optional>
#include <vector>
#include "../terminal/ITerminalControl.h"
#include "Cursor.h"
#include "IRenderTarget.h"
#include "Row.h"
#include "UAttrCharCell.h"

using namespace PresudoMicrosoft::Console::Render;

class TextBuffer final {
 public:
  enum TextBufferType { Main, Alternate };

  TextBuffer(const TextBufferType type, int row, int column);
  TextBuffer(const TextBuffer& a) = delete;

  void SetRenderTarget(IRenderTarget* renderTarget);
  void SetTerminalControl(ITerminalControl* control);

  inline bool isMain() const { return _type == TextBufferType::Main; }
  inline bool isAlternate() const { return _type == TextBufferType::Alternate; }
  inline int LineCount() const { return _storage.size(); }

  inline Cursor& GetCursor() noexcept { return _cursor; }
  inline const Cursor& GetCursor() const noexcept { return _cursor; }
  bool isCursor(const Coord& coord) const;

  inline int GetFlagY() const { return _flagY; }
  inline void AdjustFlagY(int n) {
    _flagY = _flagY + n;
    _cursor.SetYPosition(_cursor.GetPosition().y - n);
  }

  void Insert(const UAttrCharCell& ucell, const bool wrap = true);
  void Replace(const UAttrCharCell& ucell, const bool wrap = true);
  void FillScreen(const UAttrCharCell& cell);
  void InsertEmptyLine();
  void ExtendLine();

  void ScrollUp(int n);
  void ScrollDown(int n);

  void InsertLine(int n);
  void DeleteLine(int n);

  void CursorUp(int n);
  void CursorDown(int n);
  void CursorForward(int n);
  void CursorBackward(int n);
  void SetCursor(int x, int y);
  void SetCursorX(int x, bool clampInMargins = true);
  void SetMargin(int left, int top, int right, int bottom);
  void SetTopBottomMargin(int top, int bottom);

  void CR();
  void LF(bool withReturn);
  void ReverseLF();

  void Erase(Coord st, Coord ed);
  void Erase(int st, int count);

  Coord GetWordStart(Coord target, const QString& wordDelimiters);
  Coord GetWordEnd(Coord target, const QString& wordDelimiters);

  QString GetText(Coord start, Coord end);

  Row& GetRowByOffset(const int index);
  const Row& GetRow(const int index) const;

  int OffsetY() const;

  bool IsWithInPos(Coord pos) const;

  TextBuffer* ResizeColumn(int column);
  int ResizeRow(int row);

 private:
  int GetNotEmptyLineCount();

  void ResizeSaveCursor();
  void ResizeRestoreCursor();

 private:
  TextBufferType _type;
  std::vector<Row> _storage;
  Cursor _cursor;

  struct ResizeSaveState {
    Cursor cursor;
    int flagY;
  };
  ResizeSaveState _resizeState;

  int _flagY = 0;

  Size _size;
  Margin _margin;

  IRenderTarget* _renderTarget = nullptr;
  ITerminalControl* _control = nullptr;
};
