#include "TextColor.h"

#include <QHash>
#include "Config/Preferences.h"

TextColor::TextColor(const TextColor::ColorTag tag)
    : QColor(_tagToColor(tag)) {}

TextColor::TextColor(const QColor &color) : QColor(color) {}

QColor TextColor::_tagToColor(const TextColor::ColorTag &tag) {
  switch (tag) {
    case TEXT:
      return appconf->profile()->foreground();
    case BACKGROUND:
      return appconf->profile()->background();
    case Black:
      return appconf->profile()->black();
    case Red:
      return appconf->profile()->red();
    case Green:
      return appconf->profile()->green();
    case Yellow:
      return appconf->profile()->yellow();
    case Blue:
      return appconf->profile()->blue();
    case Magenta:
      return appconf->profile()->magenta();
    case Cyan:
      return appconf->profile()->cyan();
    case White:
      return appconf->profile()->white();
    case Bright_Black:
      return appconf->profile()->brightBlack();
    case Bright_Red:
      return appconf->profile()->brightRed();
    case Bright_Green:
      return appconf->profile()->brightGreen();
    case Bright_Yellow:
      return appconf->profile()->brightYellow();
    case Bright_Blue:
      return appconf->profile()->brightBlue();
    case Bright_Magenta:
      return appconf->profile()->brightMagenta();
    case Bright_Cyan:
      return appconf->profile()->brightCyan();
    case Bright_White:
      return appconf->profile()->brightWhite();
    default:
      return Qt::transparent;
  }
}
