#pragma once

#include <QObject>
#include <memory>
#include "IRenderTarget.h"
#include "Types.h"

using namespace PresudoMicrosoft::Console::Render;

class Cursor final {
 public:
  static const unsigned int s_InvertCursorColor = INVALID_COLOR;
  // the following values are used to create the textmode cursor.
  static constexpr unsigned int CURSOR_SMALL_SIZE =
      25;  // large enough to be one pixel on a six pixel font

  Cursor(int x, int y, IRenderTarget* parentBuffer = nullptr) noexcept;

  void SetRenderTarget(IRenderTarget* renderTarget);

  ~Cursor();

  // No Copy. It will copy the timer handle. Bad news.
  //  Cursor(const Cursor&) = delete;
  //  Cursor& operator=(const Cursor&) & = delete;

  //  Cursor(Cursor&&) = default;
  //  Cursor& operator=(Cursor&&) & = delete;

  bool HasMoved() const noexcept;
  bool IsVisible() const noexcept;
  bool IsOn() const noexcept;
  bool IsBlinkingAllowed() const noexcept;
  bool IsDouble() const noexcept;
  bool IsConversionArea() const noexcept;
  bool IsPopupShown() const noexcept;
  bool GetDelay() const noexcept;
  int GetSize() const noexcept;
  inline const Coord& GetPosition() const noexcept { return _pos; }

  CursorType GetType() const noexcept;
  bool IsUsingColor() const noexcept;
  unsigned long GetColor() const noexcept;

  void StartDeferDrawing() noexcept;
  void EndDeferDrawing() noexcept;

  void SetHasMoved(const bool fHasMoved) noexcept;
  void SetIsVisible(const bool fIsVisible) noexcept;
  void SetIsOn(const bool fIsOn) noexcept;
  void SetBlinkingAllowed(const bool fIsOn) noexcept;
  void SetIsDouble(const bool fIsDouble) noexcept;
  void SetIsConversionArea(const bool fIsConversionArea) noexcept;
  void SetIsPopupShown(const bool fIsPopupShown) noexcept;
  void SetDelay(const bool fDelay) noexcept;
  void SetSize(const int ulSize) noexcept;
  void SetStyle(const int ulSize, const unsigned long color,
                const CursorType type) noexcept;

  void SetPosition(int NewX, int NewY) noexcept;
  void SetPosition(const Coord cPosition) noexcept;
  void SetXPosition(const int NewX) noexcept;
  void SetYPosition(const int NewY) noexcept;
  void IncrementXPosition(const int DeltaX) noexcept;
  void IncrementYPosition(const int DeltaY) noexcept;
  void DecrementXPosition(const int DeltaX) noexcept;
  void DecrementYPosition(const int DeltaY) noexcept;

  void CopyProperties(const Cursor& OtherCursor) noexcept;

  void DelayEOLWrap(const Coord coordDelayedAt) noexcept;
  void ResetDelayEOLWrap() noexcept;
  Coord GetDelayedAtPosition() const noexcept;
  bool IsDelayedEOLWrap() const noexcept;

  void SetColor(const unsigned int color) noexcept;
  void SetType(const CursorType type) noexcept;

 private:
  IRenderTarget* _renderTarget;

  // TODO: separate the rendering and text placement

  // NOTE: If you are adding a property here, go add it to CopyProperties.

  Coord _pos;  // current position on screen (in screen buffer coords).

  bool _fHasMoved;
  bool _fIsVisible;  // whether cursor is visible (set only through the API)
  bool _fIsOn;       // whether blinking cursor is on or not
  bool _fIsDouble;   // whether the cursor size should be doubled
  bool _fBlinkingAllowed;   // Whether or not the cursor is allowed to blink at
                            // all. only set through VT (^[[?12h/l)
  bool _fDelay;             // don't blink scursor on next timer message
  bool _fIsConversionArea;  // is attached to a conversion area so it doesn't
                            // actually need to display the cursor.
  bool _fIsPopupShown;  // if a popup is being shown, turn off, stop blinking.

  bool _fDelayedEolWrap;  // don't wrap at EOL till the next char comes in.
  Coord _coordDelayedAt;  // coordinate the EOL wrap was delayed at.

  bool _fDeferCursorRedraw;  // whether we should defer redrawing the cursor or
                             // not
  bool _fHaveDeferredCursorRedraw;  // have we been asked to redraw the cursor
                                    // while it was being deferred?

  int _ulSize;

  void _RedrawCursor() noexcept;
  void _RedrawCursorAlways() noexcept;

  CursorType _cursorType;
  //  bool _fUseColor;
  unsigned long _color;
};
