#pragma once

#include <QFontMetrics>
#include <QHash>
#include <shared_mutex>
#include "../buffer/TextBuffer.h"
#include "../buffer/TextCursor.h"
#include "../parser/StateMachine.h"
//#include "../render/IRenderData.h"
#include "ITerminal.h"
//#include "ITerminalControl.h"
//#include "ITerminalInput.h"

namespace PresudoMicrosoft::Console::VirtualTerminal {

  class Terminal final : public ITerminal {
   public:
    Terminal(int row, int column);
    ~Terminal(){};

    //    void SetTerminalInput(ITerminalInput* input);
    //    void SetRenderTarget(IRenderTarget* renderTarget);
    //    void SetTerminalControl(ITerminalControl* control);

    void SetFont(const QFont& font);
    const TextBuffer* GetTextBuffer() const;

    // Write goes through the parser
    void Write(const QString& str);

    //    void UserResize(const Size& newSize);
    void ResizeColumn(int column);
    void ResizeRow(int row);

    QString GetText(const Coord& start, const Coord& end);

    [[nodiscard]] std::shared_lock<std::shared_mutex> LockForReading();
    [[nodiscard]] std::unique_lock<std::shared_mutex> LockForWriting();

   private:
    QStringList _GraphemeSplit(const QString& str);
    int _CalcWidth(const QString& uch);

    void _ScrollUp(int n);
    void _ScrollDown(int n);

    // notify update
    void TriggerRedraw();

   private:
    std::unique_ptr<StateMachine> _stateMachine;

    std::shared_mutex _readWriteLock;

    std::unique_ptr<TextBuffer> _buffer;
    int _row;
    int _column;

    bool _autoWrap = true;
    ScrollMargin _margin;

    QFontMetrics _fm = QFontMetrics(QFont("SF Mono", 13));
    QHash<QString, int> _uch2width;

    //    IRenderTarget* _renderTarget = nullptr;
    //    ITerminalControl* _control = nullptr;
    //    ITerminalInput* _input = nullptr;

    // ITerminal interface
   public:
    void Print(const wchar_t wch) override;
    void PrintString(const std::wstring_view string) override;
    void Execute(const wchar_t wch) override;

    int GetRow() const override;
    int GetColumn() const override;
    Coord GetCursorPos() const override;
    bool SetCursorPos(const Coord& pos) override;
    ScrollMargin GetScrollMargin() const override;

    bool PrivateScrollUp(int distance) override;
    bool PrivateScrollDown(int distance) override;

    bool PrivateIsVtInputEnabled() const override;
    TextCursor* PrivateGetTextCursor() const override;
    //    bool PrivateGetTextAttributes(TextAttribute& attrs) const override;
    //    bool PrivateSetTextAttributes(const TextAttribute& attrs) override;
    bool SetConsoleWindowInfo(const bool absolute,
                              const ScrollMargin& window) override;
    bool PrivateSetCursorKeysMode(const bool applicationMode) override;
    bool PrivateSetKeypadMode(const bool applicationMode) override;
    bool PrivateEnableWin32InputMode(const bool win32InputMode) override;
    bool PrivateSetAnsiMode(const bool ansiMode) override;
    bool PrivateSetScreenMode(const bool reverseMode) override;
    bool PrivateSetAutoWrapMode(const bool wrapAtEOL) override;
    bool PrivateShowCursor(const bool show) override;
    bool PrivateAllowCursorBlinking(const bool enable) override;
    bool PrivateSetScrollingRegion(const QMargins& scrollMargins) override;
    bool PrivateWarningBell() override;
    bool PrivateGetLineFeedMode() const override;
    bool PrivateLineFeed(const bool withReturn) override;
    bool PrivateReverseLineFeed() override;
    bool SetConsoleTitleW(const std::wstring_view title) override;
    bool PrivateUseAlternateScreenBuffer() override;
    bool PrivateUseMainScreenBuffer() override;
    bool PrivateEnableVT200MouseMode(const bool enabled) override;
    bool PrivateEnableUTF8ExtendedMouseMode(const bool enabled) override;
    bool PrivateEnableSGRExtendedMouseMode(const bool enabled) override;
    bool PrivateEnableButtonEventMouseMode(const bool enabled) override;
    bool PrivateEnableAnyEventMouseMode(const bool enabled) override;
    bool PrivateEnableAlternateScroll(const bool enabled) override;
    bool PrivateEraseCell(const Coord& start, const Coord& end) override;
    bool PrivateEraseSaveLines() override;
    bool PrivateRefreshWindow() override;
    bool SetConsoleOutputCP(const unsigned int codepage) override;
    bool GetConsoleOutputCP(unsigned int& codepage) override;
    bool PrivateSuppressResizeRepaint() override;
    bool IsConsolePty() const override;
    bool DeleteLines(const size_t count) override;
    bool InsertLines(const size_t count) override;
    bool MoveToBottom() const override;
    bool PrivateGetColorTableEntry(const size_t index,
                                   COLORREF& value) const override;
    bool PrivateSetColorTableEntry(const size_t index,
                                   const COLORREF value) const override;
    bool PrivateSetDefaultForeground(const COLORREF value) const override;
    bool PrivateSetDefaultBackground(const COLORREF value) const override;
    bool PrivateFillRegion(const Coord startPosition, const size_t fillLength,
                           const wchar_t fillChar,
                           const bool standardFillAttrs) override;
    bool PrivateScrollRegion(const ScrollMargin scrollRect,
                             const std::optional<ScrollMargin> clipRect,
                             const Coord destinationOrigin,
                             const bool standardFillAttrs) override;
  };

}  // namespace PresudoMicrosoft::Console::VirtualTerminal
