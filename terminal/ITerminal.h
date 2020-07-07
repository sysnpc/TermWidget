#pragma once

#include <vector>
#include "../buffer/UAttrCharCell.h"
#include "../parser/DispatchTypes.h"

using namespace PresudoMicrosoft::Console::VirtualTerminal::DispatchTypes;

class ITerminal {
 public:
  virtual void WriteCell(const std::vector<UAttrCharCell>& ucells) = 0;

  virtual int CalcWidth(const QString& str) = 0;

  virtual void CarriageReturn() = 0;
  virtual void LineFeed(const LineFeedType lineFeedType) = 0;
  virtual void ReverseLineFeed() = 0;

  virtual void InsertLine(int n) = 0;
  virtual void DeleteLine(int n) = 0;

  virtual void CursorUp(int distance) = 0;
  virtual void CursorDown(int distance) = 0;
  virtual void CursorForward(int distance) = 0;
  virtual void CursorBackward(int distance) = 0;
  virtual void CursorColumn(int column) = 0;
  virtual void SetCursorPosition(int x, int y) = 0;

  virtual void EraseDisplayToEnd() = 0;
  virtual void EraseDisplayToBegin() = 0;
  virtual void EraseDisplayAll() = 0;
  virtual void EraseDisplaySaveLines() = 0;
  virtual void EraseLineToRight() = 0;
  virtual void EraseLineToLeft() = 0;
  virtual void EraseLineAll() = 0;

  virtual void SetCursorKeysMode(bool enabled) = 0;
  virtual void SetKeypadMode(bool enabled) = 0;

  virtual void WarningBell() = 0;

  virtual void ResizeColumn(int column) = 0;

  virtual void SetWindowTitle(const QString& title) = 0;
  virtual void SetTopBottomMargin(int top, int bottom) = 0;

  virtual void UseAlternateBuffer() = 0;
  virtual void UseMainBuffer() = 0;

  virtual void SendDeviceAttributes(const QString& str) = 0;
  virtual void ScreenAlignmentPattern() = 0;

  virtual void BracketedPasteMode(bool enabled) = 0;
};
