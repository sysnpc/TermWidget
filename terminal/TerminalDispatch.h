#pragma once

#include <QStringList>
#include "ITerminal.h"
#include "TermDispatch.h"

using namespace PresudoMicrosoft::Console::VirtualTerminal;
using namespace NexTerm::Console::Buffer;
using namespace PresudoMicrosoft::Console::VirtualTerminal::DispatchTypes;

class TerminalDispatch : public TermDispatch {
 public:
  TerminalDispatch(ITerminal* terminal);

  // ITermDispatch interface
 public:
  void Execute(const wchar_t wchControl) override;
  void Print(const wchar_t wchPrintable) override;
  void PrintString(const std::wstring_view string) override;
  void CursorUp(const size_t distance) override;
  void CursorDown(const size_t distance) override;
  void CursorForward(const size_t distance) override;
  void CursorBackward(const size_t distance) override;
  void CursorNextLine(const size_t distance) override;
  void CursorPrevLine(const size_t distance) override;
  void CursorHorizontalPositionAbsolute(const size_t column) override;
  void VerticalLinePositionAbsolute(const size_t line) override;
  void HorizontalPositionRelative(const size_t distance) override;
  void VerticalPositionRelative(const size_t distance) override;
  void CursorPosition(const size_t line, const size_t column) override;
  void CursorSaveState() override;
  void CursorRestoreState() override;
  void CursorVisibility(const bool isVisible) override;
  void InsertCharacter(const size_t count) override;
  void DeleteCharacter(const size_t count) override;
  void ScrollUp(const size_t distance) override;
  void ScrollDown(const size_t distance) override;
  void InsertLine(const size_t distance) override;
  void DeleteLine(const size_t distance) override;
  void SetColumns(const size_t columns) override;
  void SetCursorKeysMode(const bool applicationMode) override;
  void SetKeypadMode(const bool applicationMode) override;
  void EnableCursorBlinking(const bool enable) override;
  void SetScreenMode(const bool reverseMode) override;
  void SetOriginMode(const bool relativeMode) override;
  void SetAutoWrapMode(const bool wrapAtEOL) override;
  void SetTopBottomScrollingMargins(const size_t topMargin,
                                    const size_t bottomMargin) override;
  void WarningBell() override;
  void CarriageReturn() override;
  void LineFeed(const LineFeedType lineFeedType) override;
  void ReverseLineFeed() override;
  void SetWindowTitle(std::wstring_view title) override;
  void UseAlternateScreenBuffer() override;
  void UseMainScreenBuffer() override;
  void HorizontalTabSet() override;
  void ForwardTab(const size_t numTabs) override;
  void BackwardsTab(const size_t numTabs) override;
  void TabClear(const size_t clearType) override;
  void EnableDECCOLMSupport(const bool enabled) override;
  void EnableVT200MouseMode(const bool enabled) override;
  void EnableUTF8ExtendedMouseMode(const bool enabled) override;
  void EnableSGRExtendedMouseMode(const bool enabled) override;
  void EnableButtonEventMouseMode(const bool enabled) override;
  void EnableAnyEventMouseMode(const bool enabled) override;
  void EnableAlternateScroll(const bool enabled) override;
  void EnableBracketedPasteMode(const bool enabled) override;
  void SetColorTableEntry(const size_t tableIndex,
                          const unsigned long color) override;
  void SetDefaultForeground(const unsigned long color) override;
  void SetDefaultBackground(const unsigned long color) override;
  void EraseInDisplay(const EraseType eraseType) override;
  void EraseInLine(const EraseType eraseType) override;
  void EraseCharacters(const size_t numChars) override;
  void SetGraphicsRendition(
      const std::basic_string_view<GraphicsOptions> options) override;
  void SetPrivateModes(
      const std::basic_string_view<PrivateModeParams> params) override;
  void ResetPrivateModes(
      const std::basic_string_view<PrivateModeParams> params) override;
  void DeviceStatusReport(const AnsiStatusType statusType) override;
  void DeviceAttributes() override;
  void DesignateCharset(const wchar_t wchCharset) override;
  void SoftReset() override;
  void HardReset() override;
  void ScreenAlignmentPattern() override;
  void SetCursorStyle(const CursorStyle cursorStyle) override;
  void SetCursorColor(const unsigned long color) override;
  void WindowManipulation(
      const WindowManipulationType function,
      const std::basic_string_view<size_t> parameters) override;

 private:
  ITerminal* _terminal;

  TextColor _foreColor;
  TextColor _backColor;
  TextStyles _styles;

 private:
  QStringList _GraphemeSplit(const QString& str);
  void _WriteBuffer(const std::wstring_view& stringView);

  TextColor _RGBColor(const std::basic_string_view<GraphicsOptions> options);

  bool _SetResettPrivateMode(const PrivateModeParams param, bool enabled);
};
