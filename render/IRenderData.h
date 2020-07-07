#pragma once

#include "../buffer/IRenderTarget.h"

class TextBuffer;

using namespace PresudoMicrosoft::Console::Render;

namespace NexTerm::Render {

  class IRenderData {
   public:
    virtual const TextBuffer& GetTextBuffer() const noexcept = 0;
  };

}  // namespace NexTerm::Render
