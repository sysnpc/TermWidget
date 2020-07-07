#pragma once

#include "../parser/ITermDispatch.h"
#include "../tracing/TermDispatchTracing.h"

using namespace PresudoMicrosoft::Console::VirtualTerminal;

class TermDispatch : public ITermDispatch {
 public:
  TermDispatch() : _trace{TermDispatchTracing()} {}

  // ITermDispatch interface

  void Execute(const wchar_t wchControl) override {
    _trace.TraceUnuseDispatch(QString("%1: %2").arg("Execute").arg(wchControl));
  }
  void Print(const wchar_t wchPrintable) override {
    _trace.TraceUnuseDispatch(QString("%1: %2").arg("Print").arg(wchPrintable));
  }
  void PrintString(const std::wstring_view string) override {
    _trace.TraceUnuseDispatch(QString("%1: %2")
                                  .arg("PrintString")
                                  .arg(QString::fromWCharArray(string.data())));
  }
  void CursorUp(const size_t distance) override {
    _trace.TraceUnuseDispatch(QString("%1: %2").arg("CursorUp").arg(distance));
  }
  void CursorDown(const size_t distance) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("CursorDown").arg(distance));
  }
  void CursorForward(const size_t distance) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("CursorForward").arg(distance));
  }
  void CursorBackward(const size_t distance) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("CursorBackward").arg(distance));
  }
  void CursorNextLine(const size_t distance) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("CursorNextLine").arg(distance));
  }
  void CursorPrevLine(const size_t distance) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("CursorPrevLine").arg(distance));
  }
  void CursorHorizontalPositionAbsolute(const size_t column) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("CursorHorizontalPositionAbsolute").arg(column));
  }
  void VerticalLinePositionAbsolute(const size_t line) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("VerticalLinePositionAbsolute").arg(line));
  }
  void HorizontalPositionRelative(const size_t distance) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("HorizontalPositionRelative").arg(distance));
  }
  void VerticalPositionRelative(const size_t distance) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("VerticalPositionRelative").arg(distance));
  }
  void CursorPosition(const size_t line, const size_t column) override {
    _trace.TraceUnuseDispatch(
        QString("%1: (%2, %3)").arg("CursorPosition").arg(line).arg(column));
  }
  void CursorSaveState() override {
    _trace.TraceUnuseDispatch("CursorSaveState");
  }
  void CursorRestoreState() override {
    _trace.TraceUnuseDispatch("CursorRestoreState");
  }
  void CursorVisibility(const bool isVisible) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("CursorVisibility").arg(isVisible));
  }
  void InsertCharacter(const size_t count) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("InsertCharacter").arg(count));
  }
  void DeleteCharacter(const size_t count) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("DeleteCharacter").arg(count));
  }
  void ScrollUp(const size_t distance) override {
    _trace.TraceUnuseDispatch(QString("%1: %2").arg("ScrollUp").arg(distance));
  }
  void ScrollDown(const size_t distance) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("ScrollDown").arg(distance));
  }
  void InsertLine(const size_t distance) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("InsertLine").arg(distance));
  }
  void DeleteLine(const size_t distance) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("DeleteLine").arg(distance));
  }
  void SetColumns(const size_t columns) override {
    _trace.TraceUnuseDispatch(QString("%1: %2").arg("SetColumns").arg(columns));
  }
  void SetCursorKeysMode(const bool applicationMode) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("SetCursorKeysMode").arg(applicationMode));
  }
  void SetKeypadMode(const bool applicationMode) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("SetKeypadMode").arg(applicationMode));
  }
  void EnableCursorBlinking(const bool enable) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("EnableCursorBlinking").arg(enable));
  }
  void SetScreenMode(const bool reverseMode) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("SetScreenMode").arg(reverseMode));
  }
  void SetOriginMode(const bool relativeMode) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("SetOriginMode").arg(relativeMode));
  }
  void SetAutoWrapMode(const bool wrapAtEOL) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("SetAutoWrapMode").arg(wrapAtEOL));
  }
  void SetTopBottomScrollingMargins(const size_t topMargin,
                                    const size_t bottomMargin) override {
    _trace.TraceUnuseDispatch(QString("%1: top-%2, bottom-%3")
                                  .arg("SetTopBottomScrollingMargins")
                                  .arg(topMargin)
                                  .arg(bottomMargin));
  }
  void WarningBell() override { _trace.TraceUnuseDispatch("WarningBell"); }
  void CarriageReturn() override {
    _trace.TraceUnuseDispatch("CarriageReturn");
  }
  void LineFeed(const DispatchTypes::LineFeedType lineFeedType) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("LineFeed").arg(int(lineFeedType)));
  }
  void ReverseLineFeed() override {
    _trace.TraceUnuseDispatch("ReverseLineFeed");
  }
  void SetIconAndWindowTitle(std::wstring_view title) override {
    _trace.TraceUnuseDispatch(QString("%1: %2")
                                  .arg("SetIconAndWindowTitle")
                                  .arg(QString::fromWCharArray(title.data())));
  }
  void SetIconName(std::wstring_view icon) override {
    _trace.TraceUnuseDispatch(QString("%1: %2")
                                  .arg("SetIconName")
                                  .arg(QString::fromWCharArray(icon.data())));
  }
  void SetWindowTitle(std::wstring_view title) override {
    _trace.TraceUnuseDispatch(QString("%1: %2")
                                  .arg("SetWindowTitle")
                                  .arg(QString::fromWCharArray(title.data())));
  }
  void UseAlternateScreenBuffer() override {
    _trace.TraceUnuseDispatch("UseAlternateScreenBuffer");
  }
  void UseMainScreenBuffer() override {
    _trace.TraceUnuseDispatch("UseMainScreenBuffer");
  }
  void HorizontalTabSet() override {
    _trace.TraceUnuseDispatch("HorizontalTabSet");
  }
  void ForwardTab(const size_t numTabs) override {
    _trace.TraceUnuseDispatch(QString("%1: %2").arg("ForwardTab").arg(numTabs));
  }
  void BackwardsTab(const size_t numTabs) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("BackwardsTab").arg(numTabs));
  }
  void TabClear(const size_t clearType) override {
    _trace.TraceUnuseDispatch(QString("%1: %2").arg("TabClear").arg(clearType));
  }
  void EnableDECCOLMSupport(const bool enabled) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("EnableDECCOLMSupport").arg(enabled));
  }
  void EnableVT200MouseMode(const bool enabled) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("EnableVT200MouseMode").arg(enabled));
  }
  void EnableUTF8ExtendedMouseMode(const bool enabled) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("EnableUTF8ExtendedMouseMode").arg(enabled));
  }
  void EnableSGRExtendedMouseMode(const bool enabled) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("EnableSGRExtendedMouseMode").arg(enabled));
  }
  void EnableButtonEventMouseMode(const bool enabled) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("EnableButtonEventMouseMode").arg(enabled));
  }
  void EnableAnyEventMouseMode(const bool enabled) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("EnableAnyEventMouseMode").arg(enabled));
  }
  void EnableAlternateScroll(const bool enabled) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("EnableAlternateScroll").arg(enabled));
  }
  void EnableBracketedPasteMode(const bool enabled) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("EnableBracketedPasteMode").arg(enabled));
  }
  void SetColorTableEntry(const size_t tableIndex,
                          const unsigned long color) override {
    _trace.TraceUnuseDispatch(QString("%1: %2 | %3")
                                  .arg("SetColorTableEntry")
                                  .arg(tableIndex)
                                  .arg(color));
  }
  void SetDefaultForeground(const unsigned long color) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("SetDefaultForeground").arg(color));
  }
  void SetDefaultBackground(const unsigned long color) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("SetDefaultBackground").arg(color));
  }
  void EraseInDisplay(const DispatchTypes::EraseType eraseType) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("EraseInDisplay").arg(int(eraseType)));
  }
  void EraseInLine(const DispatchTypes::EraseType eraseType) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("EraseInLine").arg(int(eraseType)));
  }
  void EraseCharacters(const size_t numChars) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("EraseCharacters").arg(numChars));
  }
  void SetGraphicsRendition(
      const std::basic_string_view<DispatchTypes::GraphicsOptions> options)
      override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("SetGraphicsRendition").arg(options.at(0)));
  }
  void SetPrivateModes(
      const std::basic_string_view<DispatchTypes::PrivateModeParams> params)
      override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("SetPrivateModes").arg(int(params.at(0))));
  }
  void ResetPrivateModes(
      const std::basic_string_view<DispatchTypes::PrivateModeParams> params)
      override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("ResetPrivateModes").arg(int(params.at(0))));
  }
  void DeviceStatusReport(
      const DispatchTypes::AnsiStatusType statusType) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("DeviceStatusReport").arg(int(statusType)));
  }
  void DeviceAttributes() override {
    _trace.TraceUnuseDispatch("DeviceAttributes");
  }
  void DesignateCharset(const wchar_t wchCharset) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("DesignateCharset").arg(wchCharset));
  }
  void SoftReset() override { _trace.TraceUnuseDispatch("SoftReset"); }
  void HardReset() override { _trace.TraceUnuseDispatch("HardReset"); }
  void ScreenAlignmentPattern() override {
    _trace.TraceUnuseDispatch("ScreenAlignmentPattern");
  }
  void SetCursorStyle(const DispatchTypes::CursorStyle cursorStyle) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("SetCursorStyle").arg(int(cursorStyle)));
  }
  void SetCursorColor(const unsigned long color) override {
    _trace.TraceUnuseDispatch(
        QString("%1: %2").arg("SetCursorColor").arg(color));
  }
  void WindowManipulation(
      const DispatchTypes::WindowManipulationType function,
      const std::basic_string_view<size_t> parameters) override {
    _trace.TraceUnuseDispatch(QString("%1: %2 | %3")
                                  .arg("WindowManipulation")
                                  .arg(function)
                                  .arg(parameters.at(0)));
  }

 protected:
  TermDispatchTracing _trace;
};
