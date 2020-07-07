#pragma once

#include "Types.h"

namespace PresudoMicrosoft::Console::Render {
  class IRenderTarget {
   public:
    virtual ~IRenderTarget() = 0;

   protected:
    IRenderTarget() = default;
    IRenderTarget(const IRenderTarget&) = default;
    IRenderTarget(IRenderTarget&&) = default;
    IRenderTarget& operator=(const IRenderTarget&) = default;
    IRenderTarget& operator=(IRenderTarget&&) = default;

   public:
    virtual void TriggerRedraw(const Rect& rect) = 0;
    virtual void TriggerRedraw(int x1, int y1, int x2, int y2) = 0;
    virtual void TriggerRedrawCursor(const Coord* const pcoord) = 0;

    virtual void TriggerRedrawAll() = 0;
    virtual void TriggerTeardown() = 0;

    virtual void TriggerSelection() = 0;
    virtual void TriggerScroll() = 0;
    virtual void TriggerScroll(const Coord* const pcoordDelta) = 0;
    virtual void TriggerCircling() = 0;
    virtual void TriggerTitleChange() = 0;
  };

  inline IRenderTarget::~IRenderTarget() {}

}  // namespace PresudoMicrosoft::Console::Render
