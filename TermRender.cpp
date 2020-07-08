#include "TermRender.h"

#include "buffer/TextBuffer.h"

TermRender::TermRender(TermConfig *conf, IRenderData *renderData,
                       QWidget *parent)
    : QWidget(parent),
      _conf(conf),
      _renderData(renderData),
      _start{0, 0},
      _end{0, 0} {
  this->setAutoFillBackground(true);

  QFontMetrics fm(_conf->font());
  _fontWidth = fm.maxWidth();
  _fontHeight = fm.height();

  this->setCursor(Qt::IBeamCursor);

  _timer = std::make_unique<QTimer>(this);
  QObject::connect(_timer.get(), &QTimer::timeout, this,
                   &TermRender::_BlinkUpdate);
}

void TermRender::UpdateFlagY(int flagY) {
  _flagY = flagY;
  emit this->update();
}

void TermRender::UpdateSelection(Coord start, Coord end) {
  _start = start;
  _end = end;
  //  qDebug() << QString("(%1, %2) -> (%3, %4)")
  //                  .arg(start.x)
  //                  .arg(start.y)
  //                  .arg(end.x)
  //                  .arg(end.y);
  emit this->update();
}

void TermRender::ClearSelection() {
  _start = Coord{0, 0};
  _end = Coord{0, 0};
  emit this->update();
}

void TermRender::UpdateSize() { this->resize(sizeHint()); }

void TermRender::_BlinkUpdate() {
  _isBlinkShow = !_isBlinkShow;
  //  qDebug() << _isBlinkShow;
  this->update();
}

QVector<Coord> TermRender::_calcCoords(int x1, int y1, int x2, int y2) {
  QVector<Coord> coords;
  while ((y1 < y2) || (y1 == y2 && x1 < x2)) {
    if (x1 <= _conf->column() && y1 <= _conf->row()) {
      coords.append({x1, y1});
    }

    x1 += 1;
    if (x1 > _conf->column()) {
      x1 = 1;
      y1 += 1;
    }
  }
  return coords;
}

QVector<Coord> TermRender::_rect2coords(const QRect &rect) {
  int left = _conf->margins().left();
  int top = _conf->margins().top();
  int x = rect.x(), y = rect.y(), w = rect.width(), h = rect.height();

  int x1 = qMax(0, x - left);
  int y1 = qMax(0, y - top);
  x1 = qMax(1, qCeil(x1 / (_fontWidth * 1.0)));
  y1 = qMax(1, qCeil(y1 / (_fontHeight * 1.0)));

  int x2 = qMax(x1, x + w - left);
  int y2 = qMax(y1, y + h - top);
  x2 = qMax(x1, qCeil(x2 / (_fontWidth * 1.0)));
  y2 = qMax(y1, qCeil(y2 / (_fontHeight * 1.0)));
  return _calcCoords(x1, y1, x2, y2);
}

QRect TermRender::_coord2rect(const Coord &coord) {
  int left = _conf->margins().left() + (coord.x - 1) * _fontWidth;
  int top = _conf->margins().top() + (coord.y - 1) * _fontHeight;
  return QRect(left, top, _fontWidth, _fontHeight);
}

QRegion TermRender::_coords2region(const QVector<Coord> &coords) {
  QRegion region;
  for (const Coord &coord : coords) {
    region += _coord2rect(coord);
  }
  return region;
}

void TermRender::_setPen(QPainter &p, const AttrRowCell &attrcell) {
  QPen pen = p.pen();
  QFont font = p.font();
  const TextStyles &styles = attrcell.styles();
  if (styles.testFlag(TextStyle::Blink)) {
    if (!_timer->isActive()) {
      _timer->start(600);
    }
  }
  pen.setColor(_tag2color(attrcell.foreColor()));
  if (styles.testFlag(TextStyle::Bold)) {
    font.setBold(true);
  } else {
    font.setBold(false);
  }
  p.setPen(pen);
  p.setFont(font);
}

void TermRender::_drawCursor(QPainter &p, const Coord &coord, int width) {
  p.save();
  p.setPen(QColor("#e0def4"));
  p.setBrush(QColor("#e0def4"));
  int x = (coord.x - 1) * _fontWidth + _conf->margins().left();
  int y = (coord.y - 1) * _fontHeight + _conf->margins().top();
  p.drawRect(x, y, _fontWidth * width, _fontHeight);
  p.restore();
}

void TermRender::_drawContent(QPainter &p, const Coord &coord,
                              const CharRowCell &charCell,
                              const AttrRowCell &attrCell) {
  p.save();
  if (charCell.isNormal()) {
    int x = (coord.x - 1) * _fontWidth + _conf->margins().left();
    int y = (coord.y - 1) * _fontHeight + _conf->margins().top();
    int w = charCell.uch().width() * _fontWidth;
    int h = _fontHeight;
    _setPen(p, attrCell);
    if (attrCell.isBlink()) {
      //      qDebug() << "blink:" << _isBlinkShow;
      if (_isBlinkShow) p.drawText(x, y, w, h, 0, charCell.uch().udata());
    } else {
      p.drawText(x, y, w, h, 0, charCell.uch().udata());
    }
  }
  //  if (charCell.isNull()) {
  //    p.save();
  //    int x = (coord.x - 1) * _fontWidth + _profile.margin.left();
  //    int y = (coord.y - 1) * _fontHeight + _profile.margin.top();
  //    int w = _fontWidth;
  //    int h = _fontHeight;
  //    p.setBrush(Qt::white);
  //    p.drawRect(x, y, w, h);
  //    p.restore();
  //  }
  p.restore();
}

void TermRender::_drawSelection(QPainter &p, const Coord &coord) {
  p.save();
  int x = (coord.x - 1) * _fontWidth + _conf->margins().left();
  int y = (coord.y - 1) * _fontHeight + _conf->margins().top();
  int w = _fontWidth;
  int h = _fontHeight;
  p.setPen(_conf->selection());
  p.setBrush(_conf->selection());
  p.drawRect(x, y, w, h);
  p.restore();
}

void TermRender::_drawCoords(QPainter &p, const QVector<Coord> &coords) {
  const TextBuffer &buffer = _renderData->GetTextBuffer();
  bool blinkFlag = false;
  for (Coord coord : coords) {
    Coord rpos{coord.x, coord.y + _flagY};
    if (buffer.IsWithInPos(rpos)) {
      const Row &row = buffer.GetRow(rpos.y);
      const CharRowCell &charCell = row.GetCharRow().at(rpos.x);
      const AttrRowCell &attrCell = row.GetAttrRow().at(rpos.x);

      if (_IsInSelection(rpos)) {
        _drawSelection(p, coord);
      }

      if (buffer.isCursor(rpos)) {
        _drawCursor(p, coord, charCell.width());
      }

      if (attrCell.isBlink() && !blinkFlag) {
        //        qDebug() << coord.x << coord.y;
        blinkFlag = true;
      }

      _drawContent(p, coord, charCell, attrCell);
    }
  }
  //  qDebug() << "blinkflag: " << blinkFlag;
  if (!blinkFlag) {
    _timer->stop();
  }
}

bool TermRender::_IsInSelection(const Coord &coord) {
  if (((coord.y > _start.y) || (coord.y == _start.y && coord.x >= _start.x)) &&
      ((_end.y > coord.y) || (_end.y == coord.y && _end.x >= coord.x))) {
    return true;
  } else {
    return false;
  }
}

QColor TermRender::_tag2color(const TextColor &textColor) {
  switch (textColor.tag()) {
    case TextColor::WindowText:
      return _conf->windowText();
    case TextColor::Window:
      return _conf->window();
    case TextColor::Black:
      return _conf->black();
    case TextColor::Red:
      return _conf->red();
    case TextColor::Green:
      return _conf->green();
    case TextColor::Yellow:
      return _conf->yellow();
    case TextColor::Blue:
      return _conf->blue();
    case TextColor::Magenta:
      return _conf->magenta();
    case TextColor::Cyan:
      return _conf->cyan();
    case TextColor::White:
      return _conf->white();
    case TextColor::Bright_Black:
      return _conf->brightBlack();
    case TextColor::Bright_Red:
      return _conf->brightRed();
    case TextColor::Bright_Green:
      return _conf->brightGreen();
    case TextColor::Bright_Yellow:
      return _conf->brightYellow();
    case TextColor::Bright_Blue:
      return _conf->brightBlue();
    case TextColor::Bright_Magenta:
      return _conf->brightMagenta();
    case TextColor::Bright_Cyan:
      return _conf->brightCyan();
    case TextColor::Bright_White:
      return _conf->brightWhite();
    case TextColor::Custom_Rgb:
      return _conf->windowText();
    default:
      break;
  }
}

void TermRender::TriggerRedraw(const Rect &rect) {
  this->TriggerRedraw(rect.x1, rect.y1, rect.x2, rect.y2);
}

void TermRender::TriggerRedraw(int x1, int y1, int x2, int y2) {
  //  QVector<Coord> coords = _calcCoords(x1, y1, x2, y2);
  //  this->update(_coords2region(coords));
  this->update();
}

void TermRender::TriggerRedrawCursor(const Coord *const pcoord) {
  this->update();
}

void TermRender::TriggerRedrawAll() { emit this->update(); }

void TermRender::TriggerTeardown() {}

void TermRender::TriggerSelection() {}

void TermRender::TriggerScroll() {}

void TermRender::TriggerScroll(const Coord *const pcoordDelta) {}

void TermRender::TriggerCircling() {}

void TermRender::TriggerTitleChange() {}

void TermRender::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);

  QPainter p(this);
  p.setFont(_conf->font());
  p.setRenderHint(QPainter::Antialiasing);

  const QRegion &region = event->region();
  for (const QRect &rect : region) {
    QVector<Coord> coords = _rect2coords(rect);
    _drawCoords(p, coords);
  }
}

QSize TermRender::sizeHint() const {
  int w = _fontWidth * _conf->column() + _conf->margins().left() +
          _conf->margins().right();
  int h = _fontHeight * _conf->row() + _conf->margins().top() +
          _conf->margins().bottom();
  return QSize(w, h);
}
