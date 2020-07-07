#include "ITerminalInput.h"

#include <QDebug>

// See
// http://invisible-island.net/xterm/ctlseqs/ctlseqs.html#h2-PC-Style-Function-Keys
//    For the source for these tables.
// Also refer to the values in terminfo for kcub1, kcud1, kcuf1, kcuu1, kend,
// khome.
//   the 'xterm' setting lists the application mode versions of these sequences.
static QHash<Qt::Key, QString> s_cursorKeysNormalMapping{
    {Qt::Key_Up, "\x1b[A"},    {Qt::Key_Down, "\x1b[B"},
    {Qt::Key_Right, "\x1b[C"}, {Qt::Key_Left, "\x1b[D"},
    {Qt::Key_Home, "\x1b[H"},  {Qt::Key_End, "\x1b[F"},
};

static QHash<Qt::Key, QString> s_cursorKeysApplicationMapping{
    {Qt::Key_Up, "\x1bOA"},    {Qt::Key_Down, "\x1bOB"},
    {Qt::Key_Right, "\x1bOC"}, {Qt::Key_Left, "\x1bOD"},
    {Qt::Key_Home, "\x1bOH"},  {Qt::Key_End, "\x1bOF"},
};

static QHash<Qt::Key, QString> s_keypadNumericMapping{
    {Qt::Key_Tab, "\x09"},       {Qt::Key_Backspace, "\x7f"},
    {Qt::Key_Pause, "\x1a"},     {Qt::Key_Escape, "\x1b"},
    {Qt::Key_Insert, "\x1b[2~"}, {Qt::Key_Delete, "\x1b[3~"},
    {Qt::Key_PageUp, "\x1b[5~"}, {Qt::Key_PageDown, "\x1b[6~"},
    {Qt::Key_F1, "\x1bOP"},  // also \x1b[11~, PuTTY uses \x1b\x1b[A
    {Qt::Key_F2, "\x1bOQ"},  // also \x1b[12~, PuTTY uses \x1b\x1b[B
    {Qt::Key_F3, "\x1bOR"},  // also \x1b[13~, PuTTY uses \x1b\x1b[C
    {Qt::Key_F4, "\x1bOS"},  // also \x1b[14~, PuTTY uses \x1b\x1b[D
    {Qt::Key_F5, "\x1b[15~"},    {Qt::Key_F6, "\x1b[17~"},
    {Qt::Key_F7, "\x1b[18~"},    {Qt::Key_F8, "\x1b[19~"},
    {Qt::Key_F9, "\x1b[20~"},    {Qt::Key_F10, "\x1b[21~"},
    {Qt::Key_F11, "\x1b[23~"},   {Qt::Key_F12, "\x1b[24~"},
};

// Application mode - Some terminals support both a "Numeric" input mode, and an
// "Application" mode
//  The standards vary on what each key translates to in the various modes, so I
//  tried to make it as close to the VT220 standard as possible. The notable
//  difference is in the arrow keys, which in application mode translate to
//  "^[0A" (etc) as opposed to "^[[A" in numeric
// Some very unclear documentation at
// http://invisible-island.net/xterm/ctlseqs/ctlseqs.html also suggests
// alternate encodings for F1-4
//  which I have left in the comments on those entries as something to possibly
//  add in the future, if need be.
// It seems to me as though this was used for early numpad implementations,
// where presently numlock would enable
//  "numeric" mode, outputting the numbers on the keys, while "application" mode
//  does things like pgup/down, arrow keys, etc.
// These keys aren't translated at all in numeric mode, so I figured I'd leave
// them out of the numeric table.
static QHash<Qt::Key, QString> s_keypadApplicationMapping{
    {Qt::Key_Tab, "\x09"},       {Qt::Key_Backspace, "\x7f"},
    {Qt::Key_Pause, "\x1a"},     {Qt::Key_Escape, "\x1b"},
    {Qt::Key_Insert, "\x1b[2~"}, {Qt::Key_Delete, "\x1b[3~"},
    {Qt::Key_PageUp, "\x1b[5~"}, {Qt::Key_PageDown, "\x1b[6~"},
    {Qt::Key_F1, "\x1bOP"},  // also \x1b[11~, PuTTY uses \x1b\x1b[A
    {Qt::Key_F2, "\x1bOQ"},  // also \x1b[12~, PuTTY uses \x1b\x1b[B
    {Qt::Key_F3, "\x1bOR"},  // also \x1b[13~, PuTTY uses \x1b\x1b[C
    {Qt::Key_F4, "\x1bOS"},  // also \x1b[14~, PuTTY uses \x1b\x1b[D
    {Qt::Key_F5, "\x1b[15~"},    {Qt::Key_F6, "\x1b[17~"},
    {Qt::Key_F7, "\x1b[18~"},    {Qt::Key_F8, "\x1b[19~"},
    {Qt::Key_F9, "\x1b[20~"},    {Qt::Key_F10, "\x1b[21~"},
    {Qt::Key_F11, "\x1b[23~"},   {Qt::Key_F12, "\x1b[24~"},
    // The numpad has a variety of mappings, none of which seem standard or
    // really configurable by the OS. See
    // http://invisible-island.net/xterm/ctlseqs/ctlseqs.html#h2-PC-Style-Function-Keys
    //   to see just how convoluted this all is.
    // PuTTY uses a set of mappings that don't work in ViM without reamapping
    // them back to the numpad (see
    // http://vim.wikia.com/wiki/PuTTY_numeric_keypad_mappings#Comments) I think
    // the best solution is to just not do any for the time being. Putty also
    // provides configuration for choosing which of the 5 mappings it has
    // through the settings, which is more work than we can manage now.
    // TermKeyMap{ VK_MULTIPLY, L"\x1bOj" },     // PuTTY: \x1bOR (I believe
    // putty is treating the top row of the numpad as PF1-PF4) TermKeyMap{
    // VK_ADD, L"\x1bOk" },          // PuTTY: \x1bOl, \x1bOm (with shift)
    // TermKeyMap{ VK_SEPARATOR, L"\x1bOl" },    // ? I'm not sure which key
    // this is... TermKeyMap{ VK_SUBTRACT, L"\x1bOm" },     // \x1bOS
    // TermKeyMap{ VK_DECIMAL, L"\x1bOn" },      // \x1bOn
    // TermKeyMap{ VK_DIVIDE, L"\x1bOo" },       // \x1bOQ
    // TermKeyMap{ VK_NUMPAD0, L"\x1bOp" },
    // TermKeyMap{ VK_NUMPAD1, L"\x1bOq" },
    // TermKeyMap{ VK_NUMPAD2, L"\x1bOr" },
    // TermKeyMap{ VK_NUMPAD3, L"\x1bOs" },
    // TermKeyMap{ VK_NUMPAD4, L"\x1bOt" },
    // TermKeyMap{ VK_NUMPAD5, L"\x1bOu" }, // \x1b0E
    // TermKeyMap{ VK_NUMPAD5, L"\x1bOE" }, // PuTTY \x1b[G
    // TermKeyMap{ VK_NUMPAD6, L"\x1bOv" },
    // TermKeyMap{ VK_NUMPAD7, L"\x1bOw" },
    // TermKeyMap{ VK_NUMPAD8, L"\x1bOx" },
    // TermKeyMap{ VK_NUMPAD9, L"\x1bOy" },
    // TermKeyMap{ '=', L"\x1bOX" },      // I've also seen these codes
    // mentioned in some documentation, TermKeyMap{ VK_SPACE, L"\x1bO " }, //
    // but I wasn't really sure if they should be included or not... TermKeyMap{
    // VK_TAB, L"\x1bOI" },   // So I left them here as a reference just in
    // case.
};

// Sequences to send when a modifier is pressed with any of these keys
// Basically, the 'm' will be replaced with a character indicating which
//      modifier keys are pressed.
static QHash<Qt::Key, QString> s_modifierKeyMapping{
    {Qt::Key_Up, "\x1b[1;mA"},     {Qt::Key_Down, "\x1b[1;mB"},
    {Qt::Key_Right, "\x1b[1;mC"},  {Qt::Key_Left, "\x1b[1;mD"},
    {Qt::Key_Home, "\x1b[1;mH"},   {Qt::Key_End, "\x1b[1;mF"},
    {Qt::Key_F1, "\x1b[1;mP"},     {Qt::Key_F2, "\x1b[1;mQ"},
    {Qt::Key_F3, "\x1b[1;mR"},     {Qt::Key_F4, "\x1b[1;mS"},
    {Qt::Key_Insert, "\x1b[2;m~"}, {Qt::Key_Delete, "\x1b[3;m~"},
    {Qt::Key_PageUp, "\x1b[5;m~"}, {Qt::Key_PageDown, "\x1b[6;m~"},
    {Qt::Key_F5, "\x1b[15;m~"},    {Qt::Key_F6, "\x1b[17;m~"},
    {Qt::Key_F7, "\x1b[18;m~"},    {Qt::Key_F8, "\x1b[19;m~"},
    {Qt::Key_F9, "\x1b[20;m~"},    {Qt::Key_F10, "\x1b[21;m~"},
    {Qt::Key_F11, "\x1b[23;m~"},   {Qt::Key_F12, "\x1b[24;m~"},
    // Ubuntu's inputrc also defines \x1b[5C, \x1b\x1bC (and D) as
    // 'forward/backward-word' mappings I believe '\x1b\x1bC' is listed because
    // the C1 ESC (x9B) gets encoded as
    //  \xC2\x9B, but then translated to \x1b\x1b if the C1 codepoint isn't
    //  supported by the current encoding
};

ITerminalInput::ITerminalInput()
    : _keypadApplicationMode{false}, _cursorKeysMode{false} {}

void ITerminalInput::SetCursorKeysMode(bool enabled) {
  _cursorKeysMode = enabled;
}

void ITerminalInput::SetKeypadMode(bool enabled) {
  _keypadApplicationMode = enabled;
}

bool ITerminalInput::HandleKeyEvent(QKeyEvent *event) {
  Qt::Key key = Qt::Key(event->key());
  //  qDebug() << event;
  //  if (event->modifiers() == Qt::MetaModifier) {
  //    switch (key) {
  //      case Qt::Key_C:
  //        qDebug() << "copy";
  //        break;
  //      case Qt::Key_V:
  //        qDebug() << "paste";
  //        break;
  //      default:
  //        break;
  //    }
  //  }

  if (event->modifiers() == Qt::ControlModifier) {
    switch (key) {
      case Qt::Key_C:
        //        this->SendSignal(SIGINT);
        this->SendString("\x03");
        return true;
      default:
        break;
    }
  }

  QString str = event->text();

  const QHash<Qt::Key, QString> &keymap = _GetKeyMapping(event);

  if (keymap.contains(key)) {
    str = keymap.value(key);
  }

  if (!str.isEmpty()) {
    this->SendString(str);
    return true;
  } else {
    return false;
  }
}

const QHash<Qt::Key, QString> &ITerminalInput::_GetKeyMapping(
    QKeyEvent *event) {
  if (_isCursorKey(event)) {
    if (_cursorKeysMode) {
      return s_cursorKeysApplicationMapping;
    } else {
      return s_cursorKeysNormalMapping;
    }
  } else {
    if (_keypadApplicationMode) {
      return s_keypadApplicationMapping;
    } else {
      return s_keypadNumericMapping;
    }
  }
}

bool ITerminalInput::_isCursorKey(QKeyEvent *event) {
  Qt::Key key = Qt::Key(event->key());
  if (key == Qt::Key_Up || key == Qt::Key_Down || key == Qt::Key_Right ||
      key == Qt::Key_Left || key == Qt::Key_Home || key == Qt::Key_End) {
    return true;
  } else {
    return false;
  }
}
