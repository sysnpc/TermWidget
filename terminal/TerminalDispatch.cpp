#include "TerminalDispatch.h"

#include <QtGlobal>

#include "../tools/til/at.h"

using namespace PresudoMicrosoft::Console::VirtualTerminal;

TerminalDispatch::TerminalDispatch(ITerminal* pTerm) : _pTerm(pTerm) {}

void TerminalDispatch::PrintString(const std::wstring_view string) {
  if (_termOutput.NeedToTranslate()) {
    std::wstring buffer;
    buffer.reserve(string.size());
    for (auto& wch : string) {
      buffer.push_back(_termOutput.TranslateKey(wch));
    }
    _pTerm->PrintString(buffer);
  } else {
    _pTerm->PrintString(string);
  }
}

void TerminalDispatch::Print(const wchar_t wchPrintable) {
  const auto wchTranslated = _termOutput.TranslateKey(wchPrintable);
  // By default the DEL character is meant to be ignored in the same way as a
  // NUL character. However, it's possible that it could be translated to a
  // printable character in a 96-character set. This condition makes sure that
  // a character is only output if the DEL is translated to something else.
  if (wchTranslated != AsciiChars::DEL) {
    _pTerm->Print(wchTranslated);
  }
}

bool TerminalDispatch::CursorUp(const size_t distance) {
  return _CursorMovePosition(Offset::Backward(distance), Offset::Unchanged(),
                             true);
}

bool TerminalDispatch::CursorDown(const size_t distance) {
  return _CursorMovePosition(Offset::Forward(distance), Offset::Unchanged(),
                             true);
}

bool TerminalDispatch::CursorForward(const size_t distance) {
  return _CursorMovePosition(Offset::Unchanged(), Offset::Forward(distance),
                             true);
}

bool TerminalDispatch::CursorBackward(const size_t distance) {
  return _CursorMovePosition(Offset::Unchanged(), Offset::Backward(distance),
                             true);
}

bool TerminalDispatch::CursorNextLine(const size_t distance) {
  return _CursorMovePosition(Offset::Forward(distance), Offset::Absolute(1),
                             true);
}

bool TerminalDispatch::CursorPrevLine(const size_t distance) {
  return _CursorMovePosition(Offset::Backward(distance), Offset::Absolute(1),
                             true);
}

bool TerminalDispatch::CursorHorizontalPositionAbsolute(const size_t column) {
  return _CursorMovePosition(Offset::Unchanged(), Offset::Absolute(column),
                             false);
}

bool TerminalDispatch::VerticalLinePositionAbsolute(const size_t line) {
  return _CursorMovePosition(Offset::Absolute(line), Offset::Unchanged(),
                             false);
}

bool TerminalDispatch::HorizontalPositionRelative(const size_t distance) {
  return _CursorMovePosition(Offset::Unchanged(), Offset::Forward(distance),
                             false);
}

bool TerminalDispatch::VerticalPositionRelative(const size_t distance) {
  return _CursorMovePosition(Offset::Forward(distance), Offset::Unchanged(),
                             false);
}

bool TerminalDispatch::CursorPosition(const size_t line, const size_t column) {
  return _CursorMovePosition(Offset::Absolute(line), Offset::Absolute(column),
                             false);
}

bool TerminalDispatch::CursorSaveState() {}

bool TerminalDispatch::CursorRestoreState() {}

// Routine Description:
// - DECTCEM - Sets the show/hide visibility status of the cursor.
// Arguments:
// - fIsVisible - Turns the cursor rendering on (TRUE) or off (FALSE).
// Return Value:
// - True if handled successfully. False otherwise.
bool TerminalDispatch::CursorVisibility(const bool isVisible) {
  // This uses a private API instead of the public one, because the public API
  //      will set the cursor shape back to legacy.
  return _pTerm->PrivateShowCursor(isVisible);
}

bool TerminalDispatch::EraseInDisplay(const EraseType eraseType) {
  auto pos = _pTerm->GetCursorPos();
  auto row = _pTerm->GetRow();
  auto column = _pTerm->GetColumn();

  Coord st{1, 1}, mid(pos), ed{column, row};
  switch (eraseType) {
    case EraseType::ToEnd:
      _pTerm->PrivateEraseCell(mid, ed);
      break;
    case EraseType::FromBeginning:
      _pTerm->PrivateEraseCell(st, mid);
      break;
    case EraseType::All:
      _pTerm->PrivateEraseCell(st, ed);
      _pTerm->SetCursorPos(Coord{1, 1});
      break;
    case EraseType::Scrollback:
      _pTerm->PrivateEraseSaveLines();
      break;
  }
  return true;
}

bool TerminalDispatch::EraseInLine(const EraseType eraseType) {
  auto pos = _pTerm->GetCursorPos();
  auto column = _pTerm->GetColumn();
  Coord st{1, pos.y}, mid(pos), ed{column, pos.y};
  switch (eraseType) {
    case EraseType::FromBeginning:
      _pTerm->PrivateEraseCell(st, mid);
      break;
    case EraseType::ToEnd:
      _pTerm->PrivateEraseCell(mid, ed);
      break;
    case EraseType::All:
      _pTerm->PrivateEraseCell(st, ed);
      break;
    default:
      return false;
      break;
  }
  return true;
}

bool TerminalDispatch::EraseCharacters(const size_t numChars) {}

bool TerminalDispatch::InsertCharacter(const size_t count) {}

bool TerminalDispatch::DeleteCharacter(const size_t count) {}

bool TerminalDispatch::SetGraphicsRendition(
    const std::basic_string_view<GraphicsOptions> options) {
  TextAttribute attr;
  auto cursor = _pTerm->PrivateGetTextCursor();

  if (options.size() > 0) {
    auto opt = til::at(options, 0);
    switch (opt) {
      case Off:
        cursor->SetDefaultForeground();
        cursor->SetDefaultBackground();
        cursor->SetDefaultStyles();
        break;
      case ForegroundDefault:
        cursor->SetDefaultForeground();
        break;
      case BackgroundDefault:
        cursor->SetDefaultBackground();
        break;
      case BoldBright:
        cursor->SetBold(true);
        break;
      case RGBColorOrFaint:
        cursor->SetFaint(true);
        break;
      case NotBoldOrFaint:
        cursor->SetBold(false);
        cursor->SetFaint(false);
        break;
      case Italics:
        cursor->SetItalic(true);
        break;
      case NotItalics:
        cursor->SetItalic(false);
        break;
      case BlinkOrXterm256Index:
        cursor->SetBlinking(true);
        break;
      case Steady:
        cursor->SetBlinking(false);
        break;
      case Invisible:
        cursor->SetInvisible(true);
        break;
      case Visible:
        cursor->SetInvisible(false);
        break;
      case CrossedOut:
        cursor->SetCrossedOut(true);
        break;
      case DoublyUnderlined:
        cursor->SetDoublyUnderlined(true);
        break;
      case NotCrossedOut:
        cursor->SetCrossedOut(false);
        break;
      case Negative:
        cursor->SetReverseVideo(true);
        break;
      case Positive:
        cursor->SetReverseVideo(false);
        break;
      case Underline:
        cursor->SetUnderlined(true);
        break;
      case NoUnderline:
        cursor->SetUnderlined(false);
        break;
      case Overline:
        cursor->SetOverlined(true);
        break;
      case NoOverline:
        cursor->SetOverlined(false);
        break;
      case ForegroundBlack:
        cursor->SetIndexedForeground(TextColor::Black);
        break;
      case ForegroundBlue:
        cursor->SetIndexedForeground(TextColor::Blue);
        break;
      case ForegroundGreen:
        cursor->SetIndexedForeground(TextColor::Green);
        break;
      case ForegroundCyan:
        cursor->SetIndexedForeground(TextColor::Cyan);
        break;
      case ForegroundRed:
        cursor->SetIndexedForeground(TextColor::Red);
        break;
      case ForegroundMagenta:
        cursor->SetIndexedForeground(TextColor::Magenta);
        break;
      case ForegroundYellow:
        cursor->SetIndexedForeground(TextColor::Yellow);
        break;
      case ForegroundWhite:
        cursor->SetIndexedForeground(TextColor::White);
        break;
      case BackgroundBlack:
        cursor->SetIndexedBackground(TextColor::Black);
        break;
      case BackgroundBlue:
        cursor->SetIndexedBackground(TextColor::Blue);
        break;
      case BackgroundGreen:
        cursor->SetIndexedBackground(TextColor::Green);
        break;
      case BackgroundCyan:
        cursor->SetIndexedBackground(TextColor::Cyan);
        break;
      case BackgroundRed:
        cursor->SetIndexedBackground(TextColor::Red);
        break;
      case BackgroundMagenta:
        cursor->SetIndexedBackground(TextColor::Magenta);
        break;
      case BackgroundYellow:
        cursor->SetIndexedBackground(TextColor::Yellow);
        break;
      case BackgroundWhite:
        cursor->SetIndexedBackground(TextColor::White);
        break;
      case BrightForegroundBlack:
        cursor->SetIndexedForeground(TextColor::Bright_Black);
        break;
      case BrightForegroundBlue:
        cursor->SetIndexedForeground(TextColor::Bright_Blue);
        break;
      case BrightForegroundGreen:
        cursor->SetIndexedForeground(TextColor::Bright_Green);
        break;
      case BrightForegroundCyan:
        cursor->SetIndexedForeground(TextColor::Bright_Cyan);
        break;
      case BrightForegroundRed:
        cursor->SetIndexedForeground(TextColor::Bright_Red);
        break;
      case BrightForegroundMagenta:
        cursor->SetIndexedForeground(TextColor::Bright_Magenta);
        break;
      case BrightForegroundYellow:
        cursor->SetIndexedForeground(TextColor::Bright_Yellow);
        break;
      case BrightForegroundWhite:
        cursor->SetIndexedForeground(TextColor::Bright_White);
        break;
      case BrightBackgroundBlack:
        cursor->SetIndexedBackground(TextColor::Bright_Black);
        break;
      case BrightBackgroundBlue:
        cursor->SetIndexedBackground(TextColor::Bright_Blue);
        break;
      case BrightBackgroundGreen:
        cursor->SetIndexedBackground(TextColor::Bright_Green);
        break;
      case BrightBackgroundCyan:
        cursor->SetIndexedBackground(TextColor::Bright_Cyan);
        break;
      case BrightBackgroundRed:
        cursor->SetIndexedBackground(TextColor::Bright_Red);
        break;
      case BrightBackgroundMagenta:
        cursor->SetIndexedBackground(TextColor::Bright_Magenta);
        break;
      case BrightBackgroundYellow:
        cursor->SetIndexedBackground(TextColor::Bright_Yellow);
        break;
      case BrightBackgroundWhite:
        cursor->SetIndexedBackground(TextColor::Bright_White);
        break;
      case ForegroundExtended:
        //        i += _SetRgbColorsHelper(options.subspan(i + 1), attr, true);
        break;
      case BackgroundExtended:
        //        i += _SetRgbColorsHelper(options.subspan(i + 1), attr, false);
        break;
    }
  }

  return true;
}

bool TerminalDispatch::DeviceStatusReport(
    const DispatchTypes::AnsiStatusType statusType) {}

bool TerminalDispatch::DeviceAttributes() {}

bool TerminalDispatch::Vt52DeviceAttributes() {}

// Routine Description:
// - SU - Pans the window DOWN by given distance (distance new lines appear at
// the bottom of the screen) Arguments:
// - distance - Distance to move
// Return Value:
// - True if handled successfully. False otherwise.
bool TerminalDispatch::ScrollUp(const size_t distance) {
  return _pTerm->PrivateScrollUp(distance);
}

// Routine Description:
// - SD - Pans the window UP by given distance (distance new lines appear at the
// top of the screen) Arguments:
// - distance - Distance to move
// Return Value:
// - True if handled successfully. False otherwise.
bool TerminalDispatch::ScrollDown(const size_t distance) {
  return _pTerm->PrivateScrollDown(distance);
}

bool TerminalDispatch::InsertLine(const size_t distance) {}

bool TerminalDispatch::DeleteLine(const size_t distance) {}

bool TerminalDispatch::SetColumns(const size_t columns) {}

bool TerminalDispatch::SetPrivateModes(
    const std::basic_string_view<DispatchTypes::PrivateModeParams> params) {}

bool TerminalDispatch::ResetPrivateModes(
    const std::basic_string_view<DispatchTypes::PrivateModeParams> params) {}

bool TerminalDispatch::SetCursorKeysMode(const bool applicationMode) {}

bool TerminalDispatch::SetKeypadMode(const bool applicationMode) {}

bool TerminalDispatch::EnableWin32InputMode(const bool win32InputMode) {}

bool TerminalDispatch::EnableCursorBlinking(const bool enable) {}

bool TerminalDispatch::SetAnsiMode(const bool ansiMode) {}

bool TerminalDispatch::SetScreenMode(const bool reverseMode) {}

bool TerminalDispatch::SetOriginMode(const bool relativeMode) noexcept {}

bool TerminalDispatch::SetAutoWrapMode(const bool wrapAtEOL) {
  return _pTerm->PrivateSetAutoWrapMode(wrapAtEOL);
}

bool TerminalDispatch::SetTopBottomScrollingMargins(const size_t topMargin,
                                                    const size_t bottomMargin) {
  
}

bool TerminalDispatch::WarningBell() { return _pTerm->PrivateWarningBell(); }

// Routine Description:
// - CR - Performs a carriage return.
//    Moves the cursor to the leftmost column.
// Arguments:
// - None
// Return Value:
// - True if handled successfully. False otherwise.
bool TerminalDispatch::CarriageReturn() {
  return _CursorMovePosition(Offset::Unchanged(), Offset::Absolute(1), true);
}

// Routine Description:
// - IND/NEL - Performs a line feed, possibly preceded by carriage return.
//    Moves the cursor down one line, and possibly also to the leftmost column.
// Arguments:
// - lineFeedType - Specify whether a carriage return should be performed as
// well. Return Value:
// - True if handled successfully. False otherwise.
bool TerminalDispatch::LineFeed(const LineFeedType lineFeedType) {
  switch (lineFeedType) {
    case DispatchTypes::LineFeedType::DependsOnMode:
      return _pTerm->PrivateLineFeed(_pTerm->PrivateGetLineFeedMode());
    case DispatchTypes::LineFeedType::WithoutReturn:
      return _pTerm->PrivateLineFeed(false);
    case DispatchTypes::LineFeedType::WithReturn:
      return _pTerm->PrivateLineFeed(true);
    default:
      return false;
  }
}

bool TerminalDispatch::ReverseLineFeed() {}

bool TerminalDispatch::SetWindowTitle(const std::wstring_view title) {}

bool TerminalDispatch::UseAlternateScreenBuffer() {}

bool TerminalDispatch::UseMainScreenBuffer() {}

bool TerminalDispatch::HorizontalTabSet() {}

bool TerminalDispatch::ForwardTab(const size_t numTabs) {}

bool TerminalDispatch::BackwardsTab(const size_t numTabs) {}

bool TerminalDispatch::TabClear(const size_t clearType) {}

bool TerminalDispatch::DesignateCodingSystem(const wchar_t codingSystem) {}

bool TerminalDispatch::Designate94Charset(
    const size_t gsetNumber, const std::pair<wchar_t, wchar_t> charset) {}

bool TerminalDispatch::Designate96Charset(
    const size_t gsetNumber, const std::pair<wchar_t, wchar_t> charset) {}

bool TerminalDispatch::LockingShift(const size_t gsetNumber) {}

bool TerminalDispatch::LockingShiftRight(const size_t gsetNumber) {}

bool TerminalDispatch::SingleShift(const size_t gsetNumber) {}

bool TerminalDispatch::SoftReset() {}

bool TerminalDispatch::HardReset() {}

bool TerminalDispatch::ScreenAlignmentPattern() {}

bool TerminalDispatch::EnableDECCOLMSupport(const bool enabled) noexcept {}

bool TerminalDispatch::EnableVT200MouseMode(const bool enabled) {}

bool TerminalDispatch::EnableUTF8ExtendedMouseMode(const bool enabled) {}

bool TerminalDispatch::EnableSGRExtendedMouseMode(const bool enabled) {}

bool TerminalDispatch::EnableButtonEventMouseMode(const bool enabled) {}

bool TerminalDispatch::EnableAnyEventMouseMode(const bool enabled) {}

bool TerminalDispatch::EnableAlternateScroll(const bool enabled) {}

bool TerminalDispatch::SetCursorStyle(
    const DispatchTypes::CursorStyle cursorStyle) {}

bool TerminalDispatch::SetCursorColor(const COLORREF cursorColor) {}

bool TerminalDispatch::SetClipboard(const std::wstring_view content) noexcept {}

bool TerminalDispatch::SetColorTableEntry(const size_t tableIndex,
                                          const DWORD color) {}

bool TerminalDispatch::SetDefaultForeground(const DWORD color) {}

bool TerminalDispatch::SetDefaultBackground(const DWORD color) {}

bool TerminalDispatch::WindowManipulation(
    const DispatchTypes::WindowManipulationType function,
    const std::basic_string_view<size_t> parameters) {}

// Routine Description:
// - Generalizes cursor movement to a specific position, which can be absolute
// or relative. Arguments:
// - rowOffset - The row to move to
// - colOffset - The column to move to
// - clampInMargins - Should the position be clamped within the scrolling
// margins Return Value:
// - True if handled successfully. False otherwise.
bool TerminalDispatch::_CursorMovePosition(
    const TerminalDispatch::Offset rowOffset,
    const TerminalDispatch::Offset colOffset, const bool clampInMargins) const {
  Coord pos = _pTerm->GetCursorPos();
  int x = pos.x, y = pos.y;
  int row = _pTerm->GetRow(), column = _pTerm->GetColumn();
  ScrollMargin margin = _pTerm->GetScrollMargin();

  if (colOffset.IsAbsolute) {
    x = 0;
  }
  if (clampInMargins) {
    x = std::clamp(x + colOffset.Value, margin.Left, margin.Right);
    y = std::clamp(y + rowOffset.Value, margin.Top, margin.Bottom);

  } else {
    x = std::clamp(x + colOffset.Value, 1, column);
    y = std::clamp(y + rowOffset.Value, 1, row);
  }
  const Coord newPos = {x, y};
  return _pTerm->SetCursorPos(newPos);
}

bool TerminalDispatch::_ScrollMovement(
    const TerminalDispatch::ScrollDirection dir, const size_t distance) const {}
