#include "TerminalDispatch.h"

#include <QTextBoundaryFinder>
#include "Config/Preferences.h"

TerminalDispatch::TerminalDispatch(ITerminal *terminal)
    : _terminal{terminal},
      _foreColor(appconf->profile()->foreground()),
      _backColor(appconf->profile()->background()),
      _styles(TextStyle::Normal) {}

void TerminalDispatch::Execute(const wchar_t wchControl) {
  _WriteBuffer({&wchControl, 1});
}

void TerminalDispatch::Print(const wchar_t wchPrintable) {
  _WriteBuffer({&wchPrintable, 1});
}

void TerminalDispatch::PrintString(const std::wstring_view string) {
  _WriteBuffer(string);
}

void TerminalDispatch::CursorUp(const size_t distance) {
  //  TermDispatch::CursorUp(distance);
  _terminal->CursorUp(distance);
}

void TerminalDispatch::CursorDown(const size_t distance) {
  //  TermDispatch::CursorDown(distance);
  _terminal->CursorDown(distance);
}

void TerminalDispatch::CursorForward(const size_t distance) {
  //  TermDispatch::CursorForward(distance);
  _terminal->CursorForward(distance);
}

void TerminalDispatch::CursorBackward(const size_t distance) {
  //  TermDispatch::CursorBackward(distance);
  _terminal->CursorBackward(distance);
}

void TerminalDispatch::CursorNextLine(const size_t distance) {
  TermDispatch::CursorNextLine(distance);
}

void TerminalDispatch::CursorPrevLine(const size_t distance) {
  TermDispatch::CursorPrevLine(distance);
}

void TerminalDispatch::CursorHorizontalPositionAbsolute(const size_t column) {
  //  TermDispatch::CursorHorizontalPositionAbsolute(column);
  _terminal->CursorColumn(column);
}

void TerminalDispatch::VerticalLinePositionAbsolute(const size_t line) {
  TermDispatch::VerticalLinePositionAbsolute(line);
}

void TerminalDispatch::HorizontalPositionRelative(const size_t distance) {
  TermDispatch::HorizontalPositionRelative(distance);
}

void TerminalDispatch::VerticalPositionRelative(const size_t distance) {
  TermDispatch::VerticalPositionRelative(distance);
}

void TerminalDispatch::CursorPosition(const size_t line, const size_t column) {
  //  TermDispatch::CursorPosition(line, column);
  _terminal->SetCursorPosition(column, line);
}

void TerminalDispatch::CursorSaveState() { TermDispatch::CursorSaveState(); }

void TerminalDispatch::CursorRestoreState() {
  TermDispatch::CursorRestoreState();
}

void TerminalDispatch::CursorVisibility(const bool isVisible) {
  TermDispatch::CursorVisibility(isVisible);
}

void TerminalDispatch::InsertCharacter(const size_t count) {
  TermDispatch::InsertCharacter(count);
}

void TerminalDispatch::DeleteCharacter(const size_t count) {
  TermDispatch::DeleteCharacter(count);
}

void TerminalDispatch::ScrollUp(const size_t distance) {
  TermDispatch::ScrollUp(distance);
}

void TerminalDispatch::ScrollDown(const size_t distance) {
  TermDispatch::ScrollDown(distance);
}

void TerminalDispatch::InsertLine(const size_t distance) {
  //  TermDispatch::InsertLine(distance);
  _terminal->InsertLine(distance);
}

void TerminalDispatch::DeleteLine(const size_t distance) {
  //  TermDispatch::DeleteLine(distance);
  _terminal->DeleteLine(distance);
}

void TerminalDispatch::SetColumns(const size_t columns) {
  //  TermDispatch::SetColumns(columns);
  _terminal->ResizeColumn(columns);
}

void TerminalDispatch::SetCursorKeysMode(const bool applicationMode) {
  //  TermDispatch::SetCursorKeysMode(applicationMode);
  _terminal->SetCursorKeysMode(applicationMode);
}

void TerminalDispatch::SetKeypadMode(const bool applicationMode) {
  //  TermDispatch::SetKeypadMode(applicationMode);
  _terminal->SetKeypadMode(applicationMode);
}

void TerminalDispatch::EnableCursorBlinking(const bool enable) {
  TermDispatch::EnableCursorBlinking(enable);
}

void TerminalDispatch::SetScreenMode(const bool reverseMode) {
  TermDispatch::SetScreenMode(reverseMode);
}

void TerminalDispatch::SetOriginMode(const bool relativeMode) {
  TermDispatch::SetOriginMode(relativeMode);
}

void TerminalDispatch::SetAutoWrapMode(const bool wrapAtEOL) {
  TermDispatch::SetAutoWrapMode(wrapAtEOL);
}

void TerminalDispatch::SetTopBottomScrollingMargins(const size_t topMargin,
                                                    const size_t bottomMargin) {
  //  TermDispatch::SetTopBottomScrollingMargins(topMargin, bottomMargin);
  _terminal->SetTopBottomMargin(topMargin, bottomMargin);
}

void TerminalDispatch::WarningBell() {
  //  TermDispatch::WarningBell();
  _terminal->WarningBell();
}

void TerminalDispatch::CarriageReturn() {
  //  TermDispatch::CarriageReturn();
  _terminal->CarriageReturn();
}

void TerminalDispatch::LineFeed(const LineFeedType lineFeedType) {
  //  TermDispatch::LineFeed(lineFeedType);
  _terminal->LineFeed(lineFeedType);
}

void TerminalDispatch::ReverseLineFeed() {
  //  TermDispatch::ReverseLineFeed();
  _terminal->ReverseLineFeed();
}

void TerminalDispatch::SetWindowTitle(std::wstring_view title) {
  //  TermDispatch::SetWindowTitle(title);
  QString str = QString::fromWCharArray(title.data(), title.size());
  _terminal->SetWindowTitle(str);
}

void TerminalDispatch::UseAlternateScreenBuffer() {
  //  TermDispatch::UseAlternateScreenBuffer();
  _terminal->UseAlternateBuffer();
}

void TerminalDispatch::UseMainScreenBuffer() {
  //  TermDispatch::UseMainScreenBuffer();
  _terminal->UseMainBuffer();
}

void TerminalDispatch::HorizontalTabSet() { TermDispatch::HorizontalTabSet(); }

void TerminalDispatch::ForwardTab(const size_t numTabs) {
  TermDispatch::ForwardTab(numTabs);
}

void TerminalDispatch::BackwardsTab(const size_t numTabs) {
  TermDispatch::BackwardsTab(numTabs);
}

void TerminalDispatch::TabClear(const size_t clearType) {
  TermDispatch::TabClear(clearType);
}

void TerminalDispatch::EnableDECCOLMSupport(const bool enabled) {
  TermDispatch::EnableDECCOLMSupport(enabled);
}

void TerminalDispatch::EnableVT200MouseMode(const bool enabled) {
  TermDispatch::EnableVT200MouseMode(enabled);
}

void TerminalDispatch::EnableUTF8ExtendedMouseMode(const bool enabled) {
  TermDispatch::EnableUTF8ExtendedMouseMode(enabled);
}

void TerminalDispatch::EnableSGRExtendedMouseMode(const bool enabled) {
  TermDispatch::EnableSGRExtendedMouseMode(enabled);
}

void TerminalDispatch::EnableButtonEventMouseMode(const bool enabled) {
  TermDispatch::EnableButtonEventMouseMode(enabled);
}

void TerminalDispatch::EnableAnyEventMouseMode(const bool enabled) {
  TermDispatch::EnableAnyEventMouseMode(enabled);
}

void TerminalDispatch::EnableAlternateScroll(const bool enabled) {
  TermDispatch::EnableAlternateScroll(enabled);
}

void TerminalDispatch::EnableBracketedPasteMode(const bool enabled) {
  //  TermDispatch::EnableBracketedPasteMode(enabled);
  _terminal->BracketedPasteMode(enabled);
}

void TerminalDispatch::SetColorTableEntry(const size_t tableIndex,
                                          const unsigned long color) {
  TermDispatch::SetColorTableEntry(tableIndex, color);
}

void TerminalDispatch::SetDefaultForeground(const unsigned long color) {
  TermDispatch::SetDefaultForeground(color);
}

void TerminalDispatch::SetDefaultBackground(const unsigned long color) {
  TermDispatch::SetDefaultBackground(color);
}

void TerminalDispatch::EraseInDisplay(const EraseType eraseType) {
  //  TermDispatch::EraseInDisplay(eraseType);
  switch (eraseType) {
    case EraseType::ToEnd:
      _terminal->EraseDisplayToEnd();
      break;
    case EraseType::FromBeginning:
      _terminal->EraseDisplayToBegin();
      break;
    case EraseType::All:
      _terminal->EraseDisplayAll();
      break;
    case EraseType::Scrollback:
      _terminal->EraseDisplaySaveLines();
      break;
  }
}

void TerminalDispatch::EraseInLine(const EraseType eraseType) {
  //  TermDispatch::EraseInLine(eraseType);
  switch (eraseType) {
    case EraseType::ToEnd:
      _terminal->EraseLineToRight();
      break;
    case EraseType::FromBeginning:
      _terminal->EraseLineToLeft();
      break;
    case EraseType::All:
      _terminal->EraseLineAll();
      break;
    default:
      TermDispatch::EraseInLine(eraseType);
      break;
  }
}

void TerminalDispatch::EraseCharacters(const size_t numChars) {
  TermDispatch::EraseCharacters(numChars);
}

void TerminalDispatch::SetGraphicsRendition(
    const std::basic_string_view<DispatchTypes::GraphicsOptions> options) {
  //  TermDispatch::SetGraphicsRendition(options);
  GraphicsOptions opt = GraphicsOptions::Off;
  if (!options.empty()) {
    opt = options.at(0);
    switch (opt) {
      case Off:
        _foreColor = TextColor::TEXT;
        _backColor = TextColor::BACKGROUND;
        _styles = TextStyle::Normal;
        break;
      case BoldBright:
        _styles.setFlag(TextStyle::Bold, true);
        break;
        // The 2 and 5 entries here are for BOTH the extended graphics options,
        // as well as the Faint/Blink options.
      case RGBColorOrFaint:  // 2 is also Faint, decreased intensity (ISO 6429).
        _styles.setFlag(TextStyle::Faint, true);
        break;
      case Italics:
        _styles.setFlag(TextStyle::Italics, true);
        break;
      case Underline:
        _styles.setFlag(TextStyle::Underlined, true);
        break;
      case BlinkOrXterm256Index:  // 5 is also Blink (appears as Bold).
        _styles.setFlag(TextStyle::Blink, true);
        break;
      case Negative:  // inverse
        _styles.setFlag(TextStyle::Inverse, true);
        break;
      case Invisible:
        _styles.setFlag(TextStyle::Invisible, true);
        break;
      case CrossedOut:
        _styles.setFlag(TextStyle::CrossedOut, true);
        break;
      case DoublyUnderlined:
        _styles.setFlag(TextStyle::DoublyUnderlined, true);
        break;
      case UnBold:
        _styles.setFlag(TextStyle::Bold, false);
        _styles.setFlag(TextStyle::Faint, false);
        break;
      case NotItalics:
        _styles.setFlag(TextStyle::Italics, false);
        break;
      case NoUnderline:
        _styles.setFlag(TextStyle::Underlined, false);
        break;
      case Steady:  // _not_ blink
        _styles.setFlag(TextStyle::Blink, false);
        break;
      case Positive:  // _not_ inverse
        _styles.setFlag(TextStyle::Inverse, false);
        break;
      case Visible:  // _not_ invisible
        _styles.setFlag(TextStyle::Invisible, false);
        break;
      case NotCrossedOut:
        _styles.setFlag(TextStyle::CrossedOut, false);
        break;
      case ForegroundBlack:
        _foreColor = TextColor::Black;
        break;
      case ForegroundRed:
        _foreColor = TextColor::Red;
        break;
      case ForegroundGreen:
        _foreColor = TextColor::Green;
        break;
      case ForegroundYellow:
        _foreColor = TextColor::Yellow;
        break;
      case ForegroundBlue:
        _foreColor = TextColor::Blue;
        break;
      case ForegroundMagenta:
        _foreColor = TextColor::Magenta;
        break;
      case ForegroundCyan:
        _foreColor = TextColor::Cyan;
        break;
      case ForegroundWhite:
        _foreColor = TextColor::White;
        break;
      case ForegroundExtended:
        _foreColor = _RGBColor(options);
        break;
      case ForegroundDefault:
        _foreColor = TextColor::TEXT;
        break;
      case BackgroundBlack:
        _backColor = TextColor::Black;
        break;
      case BackgroundRed:
        _backColor = TextColor::Red;
        break;
      case BackgroundGreen:
        _backColor = TextColor::Green;
        break;
      case BackgroundYellow:
        _backColor = TextColor::Yellow;
        break;
      case BackgroundBlue:
        _backColor = TextColor::Blue;
        break;
      case BackgroundMagenta:
        _backColor = TextColor::Magenta;
        break;
      case BackgroundCyan:
        _backColor = TextColor::Cyan;
        break;
      case BackgroundWhite:
        _backColor = TextColor::White;
        break;
      case BackgroundExtended:
        _backColor = _RGBColor(options);
        break;
      case BackgroundDefault:
        _backColor = TextColor::BACKGROUND;
        break;
      case BrightForegroundBlack:
        _foreColor = TextColor::Bright_Black;
        break;
      case BrightForegroundRed:
        _foreColor = TextColor::Bright_Red;
        break;
      case BrightForegroundGreen:
        _foreColor = TextColor::Bright_Green;
        break;
      case BrightForegroundYellow:
        _foreColor = TextColor::Bright_Yellow;
        break;
      case BrightForegroundBlue:
        _foreColor = TextColor::Bright_Blue;
        break;
      case BrightForegroundMagenta:
        _foreColor = TextColor::Bright_Magenta;
        break;
      case BrightForegroundCyan:
        _foreColor = TextColor::Bright_Cyan;
        break;
      case BrightForegroundWhite:
        _foreColor = TextColor::Bright_White;
        break;
      case BrightBackgroundBlack:
        _backColor = TextColor::Bright_Black;
        break;
      case BrightBackgroundRed:
        _backColor = TextColor::Bright_Red;
        break;
      case BrightBackgroundGreen:
        _backColor = TextColor::Bright_Green;
        break;
      case BrightBackgroundYellow:
        _backColor = TextColor::Bright_Yellow;
        break;
      case BrightBackgroundBlue:
        _backColor = TextColor::Bright_Blue;
        break;
      case BrightBackgroundMagenta:
        _backColor = TextColor::Bright_Magenta;
        break;
      case BrightBackgroundCyan:
        _backColor = TextColor::Bright_Cyan;
        break;
      case BrightBackgroundWhite:
        _backColor = TextColor::Bright_White;
        break;
      default:
        TermDispatch::SetGraphicsRendition(options);
        break;
    }
  }
}

void TerminalDispatch::SetPrivateModes(
    const std::basic_string_view<DispatchTypes::PrivateModeParams> params) {
  //  TermDispatch::SetPrivateModes(params);
  if (!params.empty()) {
    PrivateModeParams param = params.at(0);
    bool success = _SetResettPrivateMode(param, true);
    if (!success) {
      TermDispatch::SetPrivateModes(params);
    }
  }
}

void TerminalDispatch::ResetPrivateModes(
    const std::basic_string_view<DispatchTypes::PrivateModeParams> params) {
  //  TermDispatch::ResetPrivateModes(params);
  if (!params.empty()) {
    PrivateModeParams param = params.at(0);
    bool success = _SetResettPrivateMode(param, false);
    if (!success) {
      TermDispatch::ResetPrivateModes(params);
    }
  }
}

void TerminalDispatch::DeviceStatusReport(
    const DispatchTypes::AnsiStatusType statusType) {
  TermDispatch::DeviceStatusReport(statusType);
}

void TerminalDispatch::DeviceAttributes() {
  //  TermDispatch::DeviceAttributes();
  // See: http://vt100.net/docs/vt100-ug/chapter3.html#DA
  _terminal->SendDeviceAttributes("\x1b[?1;0c");
}

void TerminalDispatch::DesignateCharset(const wchar_t wchCharset) {
  TermDispatch::DesignateCharset(wchCharset);
}

void TerminalDispatch::SoftReset() { TermDispatch::SoftReset(); }

void TerminalDispatch::HardReset() { TermDispatch::HardReset(); }

void TerminalDispatch::ScreenAlignmentPattern() {
  //  TermDispatch::ScreenAlignmentPattern();
  _terminal->ScreenAlignmentPattern();
}

void TerminalDispatch::SetCursorStyle(
    const DispatchTypes::CursorStyle cursorStyle) {
  TermDispatch::SetCursorStyle(cursorStyle);
}

void TerminalDispatch::SetCursorColor(const unsigned long color) {
  TermDispatch::SetCursorColor(color);
}

void TerminalDispatch::WindowManipulation(
    const DispatchTypes::WindowManipulationType function,
    const std::basic_string_view<size_t> parameters) {
  TermDispatch::WindowManipulation(function, parameters);
}

QStringList TerminalDispatch::_GraphemeSplit(const QString &str) {
  QTextBoundaryFinder tbf(QTextBoundaryFinder::Grapheme, str);
  QStringList strs;
  int pre = 0;
  int next = tbf.toNextBoundary();
  while (next != -1) {
    strs.append(str.mid(pre, next - pre));
    pre = next;
    next = tbf.toNextBoundary();
  }
  return strs;
}

void TerminalDispatch::_WriteBuffer(const std::wstring_view &stringView) {
  QString str = QString::fromWCharArray(stringView.data(), stringView.size());
  QStringList uchs = _GraphemeSplit(str);

  std::vector<UAttrCharCell> ucells;

  for (const QString &ch : uchs) {
    int w = _terminal->CalcWidth(ch);
    UChar uch(ch, w);
    UAttrCharCell ucell(uch, _foreColor, _backColor, _styles);
    ucells.push_back(ucell);
  }

  _terminal->WriteCell(ucells);
}

TextColor TerminalDispatch::_RGBColor(
    const std::basic_string_view<GraphicsOptions> options) {
  TermDispatch::SetGraphicsRendition(options);
  return TextColor::Black;
}

bool TerminalDispatch::_SetResettPrivateMode(const PrivateModeParams param,
                                             bool enabled) {
  switch (param) {
    case DECCKM_CursorKeysMode:
      // set - Enable Application Mode, reset - Normal mode
      SetCursorKeysMode(enabled);
      break;
    case DECCOLM_SetNumberOfColumns:
      SetColumns(enabled ? 132 : 80);
      break;
    case DECSCNM_ScreenMode:
      SetScreenMode(enabled);
      break;
    case DECOM_OriginMode:
      // The cursor is also moved to the new home position when the origin mode
      // is set or reset.
      SetOriginMode(enabled);
      break;
    case DECAWM_AutoWrapMode:
      SetAutoWrapMode(enabled);
      break;
    case ATT610_StartCursorBlink:
      EnableCursorBlinking(enabled);
      break;
    case DECTCEM_TextCursorEnableMode:
      CursorVisibility(enabled);
      break;
    case XTERM_EnableDECCOLMSupport:
      EnableDECCOLMSupport(enabled);
      break;
    case VT200_MOUSE_MODE:
      EnableVT200MouseMode(enabled);
      break;
    case BUTTON_EVENT_MOUSE_MODE:
      EnableButtonEventMouseMode(enabled);
      break;
    case ANY_EVENT_MOUSE_MODE:
      EnableAnyEventMouseMode(enabled);
      break;
    case UTF8_EXTENDED_MODE:
      EnableUTF8ExtendedMouseMode(enabled);
      break;
    case SGR_EXTENDED_MODE:
      EnableSGRExtendedMouseMode(enabled);
      break;
    case ALTERNATE_SCROLL:
      EnableAlternateScroll(enabled);
      break;
    case ASB_AlternateScreenBuffer:
      enabled ? UseAlternateScreenBuffer() : UseMainScreenBuffer();
      break;
    case BRACKETED_PASTE_MODE:
      EnableBracketedPasteMode(enabled);
      break;
    default:
      return false;
  }
  return true;
}
