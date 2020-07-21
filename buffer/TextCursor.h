#pragma once

#include "TextAttribute.h"

class TextCursor {
 public:
  TextCursor();
  int X();
  void X(int n);
  int Y();
  int Ry();
  void Y(int n);
  int FlagY();
  void FlagY(int y);

  void ScrollUp(int n = 1);
  void ScrollDown(int n = 1);
  const TextAttribute& Attr();

  void SetDefaultForeground();
  void SetDefaultBackground();
  void SetDefaultStyles();
  void SetBold(bool enabled);
  void SetFaint(bool enabled);
  void SetItalic(bool enabled);
  void SetBlinking(bool enabled);
  void SetInvisible(bool enabled);
  void SetCrossedOut(bool enabled);
  void SetReverseVideo(bool enabled);
  void SetUnderlined(bool enabled);
  void SetDoublyUnderlined(bool enabled);
  void SetOverlined(bool enabled);
  void SetIndexedForeground(TextColor color);
  void SetIndexedBackground(TextColor color);

 private:
  int _x = 1;
  int _y = 1;
  int _flagY = 0;
  TextAttribute _attr;
};
