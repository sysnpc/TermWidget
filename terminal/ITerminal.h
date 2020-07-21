#pragma once

#include <QMargins>
#include <deque>
#include <optional>
#include <string>
#include <string_view>
#include "../buffer/TextCursor.h"
#include "../common/precomp.h"

namespace PresudoMicrosoft::Console::VirtualTerminal {

  struct ScrollMargin {
    int Left;
    int Top;
    int Right;
    int Bottom;
  };

  struct ScreenBufferInfo {
    int row;
    int column;
    Coord _cursor;
  };

  class ITerminal {
   public:
    virtual ~ITerminal() = default;
    virtual void Print(const wchar_t wch) = 0;
    // These characters need to be mutable so that they can be processed by the
    // TerminalInput translater.
    virtual void PrintString(const std::wstring_view string) = 0;
    virtual void Execute(const wchar_t wch) = 0;

    virtual int GetRow() const = 0;
    virtual int GetColumn() const = 0;
    virtual Coord GetCursorPos() const = 0;
    virtual bool SetCursorPos(const Coord& pos) = 0;
    virtual ScrollMargin GetScrollMargin() const = 0;

    virtual bool PrivateIsVtInputEnabled() const = 0;

    virtual TextCursor* PrivateGetTextCursor() const = 0;
    //    virtual bool PrivateGetTextAttributes(TextAttribute& attrs) const = 0;
    //    virtual bool PrivateSetTextAttributes(const TextAttribute& attrs) = 0;
    /*
        virtual bool PrivateWriteConsoleInputW(
            std::deque<std::unique_ptr<IInputEvent>>& events,
            size_t& eventsWritten) = 0;
    */
    virtual bool SetConsoleWindowInfo(const bool absolute,
                                      const ScrollMargin& window) = 0;
    virtual bool PrivateSetCursorKeysMode(const bool applicationMode) = 0;
    virtual bool PrivateSetKeypadMode(const bool applicationMode) = 0;
    virtual bool PrivateEnableWin32InputMode(const bool win32InputMode) = 0;

    virtual bool PrivateSetAnsiMode(const bool ansiMode) = 0;
    virtual bool PrivateSetScreenMode(const bool reverseMode) = 0;
    virtual bool PrivateSetAutoWrapMode(const bool wrapAtEOL) = 0;

    virtual bool PrivateShowCursor(const bool show) = 0;
    virtual bool PrivateAllowCursorBlinking(const bool enable) = 0;

    virtual bool PrivateSetScrollingRegion(const QMargins& scrollMargins) = 0;
    virtual bool PrivateWarningBell() = 0;
    virtual bool PrivateGetLineFeedMode() const = 0;
    virtual bool PrivateLineFeed(const bool withReturn) = 0;
    virtual bool PrivateReverseLineFeed() = 0;
    virtual bool SetConsoleTitleW(const std::wstring_view title) = 0;
    virtual bool PrivateUseAlternateScreenBuffer() = 0;
    virtual bool PrivateUseMainScreenBuffer() = 0;

    virtual bool PrivateEnableVT200MouseMode(const bool enabled) = 0;
    virtual bool PrivateEnableUTF8ExtendedMouseMode(const bool enabled) = 0;
    virtual bool PrivateEnableSGRExtendedMouseMode(const bool enabled) = 0;
    virtual bool PrivateEnableButtonEventMouseMode(const bool enabled) = 0;
    virtual bool PrivateEnableAnyEventMouseMode(const bool enabled) = 0;
    virtual bool PrivateEnableAlternateScroll(const bool enabled) = 0;

    virtual bool PrivateEraseCell(const Coord& start, const Coord& end) = 0;
    virtual bool PrivateEraseSaveLines() = 0;
    /*
    virtual bool SetCursorStyle(const CursorType style) = 0;
    virtual bool SetCursorColor(const COLORREF color) = 0;
    virtual bool PrivatePrependConsoleInput(
        std::deque<std::unique_ptr<IInputEvent>>& events,
        size_t& eventsWritten) = 0;
    virtual bool PrivateWriteConsoleControlInput(const KeyEvent key) = 0;
*/
    virtual bool PrivateRefreshWindow() = 0;

    virtual bool SetConsoleOutputCP(const unsigned int codepage) = 0;
    virtual bool GetConsoleOutputCP(unsigned int& codepage) = 0;

    virtual bool PrivateSuppressResizeRepaint() = 0;
    virtual bool IsConsolePty() const = 0;

    virtual bool DeleteLines(const size_t count) = 0;
    virtual bool InsertLines(const size_t count) = 0;

    virtual bool MoveToBottom() const = 0;

    virtual bool PrivateGetColorTableEntry(const size_t index,
                                           COLORREF& value) const = 0;
    virtual bool PrivateSetColorTableEntry(const size_t index,
                                           const COLORREF value) const = 0;
    virtual bool PrivateSetDefaultForeground(const COLORREF value) const = 0;
    virtual bool PrivateSetDefaultBackground(const COLORREF value) const = 0;

    virtual bool PrivateFillRegion(const Coord startPosition,
                                   const size_t fillLength,
                                   const wchar_t fillChar,
                                   const bool standardFillAttrs) = 0;

    virtual bool PrivateScrollRegion(const ScrollMargin scrollRect,
                                     const std::optional<ScrollMargin> clipRect,
                                     const Coord destinationOrigin,
                                     const bool standardFillAttrs) = 0;

    virtual bool PrivateScrollUp(int distance) = 0;
    virtual bool PrivateScrollDown(int distance) = 0;
  };
}  // namespace PresudoMicrosoft::Console::VirtualTerminal
