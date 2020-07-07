#pragma once

#include <shared_mutex>
#include "../buffer/TextBuffer.h"
#include "../parser/StateMachine.h"
#include "../render/IRenderData.h"
#include "ITerminal.h"
#include "ITerminalControl.h"
#include "ITerminalInput.h"

using namespace PresudoMicrosoft::Console::VirtualTerminal;
using namespace PresudoMicrosoft::Console::Render;

using namespace NexTerm::Render;

namespace PresudoMicrosoft::Terminal::Core {

  class Terminal final : public ITerminal, public IRenderData {
   public:
    Terminal(int row, int column);
    ~Terminal(){};

    void SetTerminalInput(ITerminalInput* input);
    void SetRenderTarget(IRenderTarget* renderTarget);
    void SetTerminalControl(ITerminalControl* control);

    // Write goes through the parser
    void Write(const QString& str);

    void UserResize(const Size& newSize);

    QString GetText(const Coord& start, const Coord& end);

    [[nodiscard]] std::shared_lock<std::shared_mutex> LockForReading();
    [[nodiscard]] std::unique_lock<std::shared_mutex> LockForWriting();

   private:
    std::unique_ptr<StateMachine> _stateMachine;

    std::shared_mutex _readWriteLock;

    std::unique_ptr<TextBuffer> _buffer;
    std::unique_ptr<TextBuffer> _save_buffer;
    Size _size;

    IRenderTarget* _renderTarget = nullptr;
    ITerminalControl* _control = nullptr;
    ITerminalInput* _input = nullptr;

    // ITerminal interface
   public:
    void WriteCell(const std::vector<UAttrCharCell>& ucells) override;
    int CalcWidth(const QString& str) override;
    void CarriageReturn() override;
    void LineFeed(const LineFeedType lineFeedType) override;
    void ReverseLineFeed() override;
    void CursorUp(int distance) override;
    void CursorDown(int distance) override;
    void CursorForward(int distance) override;
    void CursorBackward(int distance) override;
    void CursorColumn(int column) override;
    void SetCursorPosition(int x, int y) override;
    void InsertLine(int n) override;
    void DeleteLine(int n) override;

    void EraseDisplayToEnd() override;
    void EraseDisplayToBegin() override;
    void EraseDisplayAll() override;
    void EraseDisplaySaveLines() override;
    void EraseLineToRight() override;
    void EraseLineToLeft() override;
    void EraseLineAll() override;

    void SetCursorKeysMode(bool enabled) override;
    void SetKeypadMode(bool enabled) override;

    void WarningBell() override;

    void ResizeColumn(int column) override;
    int ResizeRow(int row);

    void SetWindowTitle(const QString& title) override;
    void SetTopBottomMargin(int top, int bottom) override;

    void UseMainBuffer() override;
    void UseAlternateBuffer() override;

    void SendDeviceAttributes(const QString& str) override;
    void ScreenAlignmentPattern() override;

    void BracketedPasteMode(bool enabled) override;

    // IRenderData interface
   public:
    const TextBuffer& GetTextBuffer() const noexcept override;
    TextBuffer& GetTextBuffer() noexcept;
  };

}  // namespace PresudoMicrosoft::Terminal::Core
