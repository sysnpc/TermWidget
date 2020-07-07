#pragma once

#include <string_view>
#include "DispatchTypes.h"

namespace PresudoMicrosoft::Console::VirtualTerminal {

  class ITermDispatch {
   public:
    virtual ~ITermDispatch() = 0;

    virtual void Execute(const wchar_t wchControl) = 0;
    virtual void Print(const wchar_t wchPrintable) = 0;
    virtual void PrintString(const std::wstring_view string) = 0;

    virtual void CursorUp(const size_t distance) = 0;        // CUU
    virtual void CursorDown(const size_t distance) = 0;      // CUD
    virtual void CursorForward(const size_t distance) = 0;   // CUF
    virtual void CursorBackward(const size_t distance) = 0;  // CUB, BS
    virtual void CursorNextLine(const size_t distance) = 0;  // CNL
    virtual void CursorPrevLine(const size_t distance) = 0;  // CPL
    virtual void CursorHorizontalPositionAbsolute(
        const size_t column) = 0;  // HPA, CHA
    virtual void VerticalLinePositionAbsolute(const size_t line) = 0;    // VPA
    virtual void HorizontalPositionRelative(const size_t distance) = 0;  // HPR
    virtual void VerticalPositionRelative(const size_t distance) = 0;    // VPR
    virtual void CursorPosition(const size_t line,
                                const size_t column) = 0;            // CUP, HVP
    virtual void CursorSaveState() = 0;                              // DECSC
    virtual void CursorRestoreState() = 0;                           // DECRC
    virtual void CursorVisibility(const bool isVisible) = 0;         // DECTCEM
    virtual void InsertCharacter(const size_t count) = 0;            // ICH
    virtual void DeleteCharacter(const size_t count) = 0;            // DCH
    virtual void ScrollUp(const size_t distance) = 0;                // SU
    virtual void ScrollDown(const size_t distance) = 0;              // SD
    virtual void InsertLine(const size_t distance) = 0;              // IL
    virtual void DeleteLine(const size_t distance) = 0;              // DL
    virtual void SetColumns(const size_t columns) = 0;               // DECCOLM
    virtual void SetCursorKeysMode(const bool applicationMode) = 0;  // DECCKM
    virtual void SetKeypadMode(
        const bool applicationMode) = 0;  // DECKPAM, DECKPNM
    virtual void EnableCursorBlinking(const bool enable) = 0;  // ATT610
    virtual void SetScreenMode(const bool reverseMode) = 0;    // DECSCNM
    virtual void SetOriginMode(const bool relativeMode) = 0;   // DECOM
    virtual void SetAutoWrapMode(const bool wrapAtEOL) = 0;    // DECAWM
    virtual void SetTopBottomScrollingMargins(
        const size_t topMargin, const size_t bottomMargin) = 0;  // DECSTBM
    virtual void WarningBell() = 0;                              // BEL
    virtual void CarriageReturn() = 0;                           // CR
    virtual void LineFeed(const DispatchTypes::LineFeedType
                              lineFeedType) = 0;  // IND, NEL, LF, FF, VT
    virtual void ReverseLineFeed() = 0;           // RI
    virtual void SetIconAndWindowTitle(std::wstring_view title) = 0;
    virtual void SetIconName(std::wstring_view icon) = 0;      // OscIconName
    virtual void SetWindowTitle(std::wstring_view title) = 0;  // OscWindowTitle
    virtual void UseAlternateScreenBuffer() = 0;               // ASBSET
    virtual void UseMainScreenBuffer() = 0;                    // ASBRST
    virtual void HorizontalTabSet() = 0;                       // HTS
    virtual void ForwardTab(const size_t numTabs) = 0;         // CHT, HT
    virtual void BackwardsTab(const size_t numTabs) = 0;       // CBT
    virtual void TabClear(const size_t clearType) = 0;         // TBC
    virtual void EnableDECCOLMSupport(const bool enabled) = 0;         // ?40
    virtual void EnableVT200MouseMode(const bool enabled) = 0;         // ?1000
    virtual void EnableUTF8ExtendedMouseMode(const bool enabled) = 0;  // ?1005
    virtual void EnableSGRExtendedMouseMode(const bool enabled) = 0;   // ?1006
    virtual void EnableButtonEventMouseMode(const bool enabled) = 0;   // ?1002
    virtual void EnableAnyEventMouseMode(const bool enabled) = 0;      // ?1003
    virtual void EnableAlternateScroll(const bool enabled) = 0;        // ?1007
    virtual void EnableBracketedPasteMode(const bool enabled) = 0;     // ?2004
    virtual void SetColorTableEntry(
        const size_t tableIndex,
        const unsigned long color) = 0;  // OSCColorTable
    virtual void SetDefaultForeground(
        const unsigned long color) = 0;  // OSCDefaultForeground
    virtual void SetDefaultBackground(
        const unsigned long color) = 0;  // OSCDefaultBackground

    virtual void EraseInDisplay(
        const DispatchTypes::EraseType eraseType) = 0;  // ED
    virtual void EraseInLine(
        const DispatchTypes::EraseType eraseType) = 0;        // EL
    virtual void EraseCharacters(const size_t numChars) = 0;  // ECH

    virtual void SetGraphicsRendition(
        const std::basic_string_view<DispatchTypes::GraphicsOptions>
            options) = 0;  // SGR

    virtual void SetPrivateModes(
        const std::basic_string_view<DispatchTypes::PrivateModeParams>
            params) = 0;  // DECSET

    virtual void ResetPrivateModes(
        const std::basic_string_view<DispatchTypes::PrivateModeParams>
            params) = 0;  // DECRST

    virtual void DeviceStatusReport(
        const DispatchTypes::AnsiStatusType
            statusType) = 0;              // DSR, DSR-OS, DSR-CPR
    virtual void DeviceAttributes() = 0;  // DA1

    virtual void DesignateCharset(const wchar_t wchCharset) = 0;  // SCS

    virtual void SoftReset() = 0;               // DECSTR
    virtual void HardReset() = 0;               // RIS
    virtual void ScreenAlignmentPattern() = 0;  // DECALN

    // DECSCUSR
    virtual void SetCursorStyle(
        const DispatchTypes::CursorStyle cursorStyle) = 0;
    // OSCSetCursorColor, OSCResetCursorColor
    virtual void SetCursorColor(const unsigned long color) = 0;

    // DTTERM_WindowManipulation
    virtual void WindowManipulation(
        const DispatchTypes::WindowManipulationType function,
        const std::basic_string_view<size_t> parameters) = 0;
  };
  inline ITermDispatch::~ITermDispatch() {}

}  // namespace PresudoMicrosoft::Console::VirtualTerminal
