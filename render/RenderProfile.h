#pragma once

#include <QFont>
#include <QMargins>

namespace NexTerm::Render {

  struct RenderProfile {
    QFont font;
    int row;
    int column;
    int flagY;
    QMargins margin;
  };

}  // namespace NexTerm::Render
