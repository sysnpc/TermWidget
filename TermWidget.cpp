#include "TermWidget.h"

#include <QGuiApplication>

using namespace NexTerm::Render;

TermWidget::TermWidget(TermConfig *conf, QWidget *parent)
    : QAbstractScrollArea(parent),
      _flagY{0},
      _traceScroll{true},
      _lastMouseClickType(QEvent::MouseButtonPress),
      _lastMouseClickTime(QDateTime::currentDateTime()),
      _lastMouseClickPos(QPoint(0, 0)) {
  this->setAttribute(Qt::WA_InputMethodEnabled);
  this->setFrameShape(QFrame::NoFrame);

  if (conf == nullptr) {
    _conf = new TermConfig;
  } else {
    _conf = conf;
  }

  _viewport = std::make_unique<TermViewport>();
  this->setViewport(_viewport.get());

  _terminal = std::make_unique<Terminal>(_conf->row(), _conf->column());

  _render = std::make_unique<TermRender>(_conf, _terminal.get());
  _render->setParent(_viewport.get());

  _terminal->SetRenderTarget(_render.get());
  _terminal->SetTerminalInput(this);
  _terminal->SetTerminalControl(this);

  _initTerminal();

  _vsb = std::make_unique<QScrollBar>();
  _hsb = std::make_unique<QScrollBar>();
  this->setVerticalScrollBar(_vsb.get());
  this->setHorizontalScrollBar(_hsb.get());
  _vsb->setRange(0, 0);
  _vsb->setSingleStep(1);
  _vsb->setPageStep(_conf->row());
  _vsb->setValue(0);

  this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  _timer = std::make_unique<QTimer>(this);
  _timer->setSingleShot(true);
  QObject::connect(_timer.get(), &QTimer::timeout, this,
                   &TermWidget::_OnTimeOut);
}

void TermWidget::write(const QByteArray &text) { _terminal->Write(text); }

QString TermWidget::GetSelectionText() const {
  return _terminal->GetText(_selection->start, _selection->end);
}

void TermWidget::onWrite(const QByteArray &text) { write(text); }

void TermWidget::_OnTimeOut() {
  emit this->PtyResize(_conf->row(), _conf->column());
}

void TermWidget::_initTerminal() {
  QFontMetrics fm(_conf->font());
  _fontWidth = fm.maxWidth();
  _fontHeight = fm.height();
}

QPoint TermWidget::_Coord2Point(const Coord &coord) const {
  int x = _conf->margins().left() + _fontWidth * (coord.x - 1);
  int y = _conf->margins().top() + _fontHeight * (coord.y - 1);
  return QPoint(x, y);
}

Coord TermWidget::_Point2Coord(const QPoint &point) const {
  int x = qCeil((point.x() - _conf->margins().left()) / (_fontWidth * 1.0));
  int y = qCeil((point.y() - _conf->margins().top()) / (_fontHeight * 1.0));
  x = std::clamp(x, 0, _conf->column() + 1);
  y = std::clamp(y, 1, _conf->row());
  return Coord{x, y};
}

QSize TermWidget::sizeHint() const {
  int w = _fontWidth * _conf->column() + _conf->margins().left() +
          _conf->margins().right();
  int h = _fontHeight * _conf->row() + _conf->margins().top() +
          _conf->margins().bottom();
  return QSize(w, h);
}

void TermWidget::resizeEvent(QResizeEvent *event) {
  _timer->start(68);
  QSize size = event->size();
  int h = size.height() - _conf->margins().top() - _conf->margins().bottom();
  int w = size.width() - _conf->margins().left() - _conf->margins().right();
  if (_vsb->isVisible()) {
    w += _vsb->size().width();
  }
  if (_hsb->isVisible()) {
    h += _hsb->size().height();
  }
  int row_old = _conf->row(), column_old = _conf->column();
  int row_new = h / _fontHeight, column_new = w / _fontWidth;
  _conf->setRow(row_new);
  _conf->setColumn(column_new);
  //  qDebug() << row << column;

  if (column_old != column_new) {
    _terminal->ResizeColumn(column_new);
    _render->UpdateSize();
  }

  if (row_old != row_new) {
    int df = _terminal->ResizeRow(row_new);
    if (df > 0) {
      TextBuffer &buffer = _terminal->GetTextBuffer();
      if (row_new > row_old) {
        buffer.AdjustFlagY(-df);
      } else {
        buffer.AdjustFlagY(df);
      }
    }
    _render->UpdateSize();
  }
  emit this->NotifyHightChange(_terminal->GetTextBuffer().LineCount());
  //  emit this->PtyResize(row_new, column_new);
}

void TermWidget::keyPressEvent(QKeyEvent *event) {
  //  qDebug() << event;
  if (this->HandleKeyEvent(event)) {
    event->accept();
    _render->ClearSelection();
    if (_vsb->value() != _vsb->maximum()) {
      _vsb->setValue(_vsb->maximum());
    }
  } else {
    event->ignore();
  }
}

void TermWidget::inputMethodEvent(QInputMethodEvent *event) {
  //  qDebug() << event;
  //  QAbstractScrollArea::inputMethodEvent(event);
  QString str = event->commitString();
  if (!str.isEmpty()) {
    emit this->SendString(str);
  }
}

QVariant TermWidget::inputMethodQuery(Qt::InputMethodQuery query) const {
  const Cursor &cursor = _terminal->GetTextBuffer().GetCursor();
  Coord coord = cursor.GetPosition();
  switch (query) {
    case Qt::ImEnabled:
      return true;
      break;
    case Qt::ImCursorRectangle:
      return QRect(_Coord2Point(coord), QSize(_fontWidth, _fontHeight));
      break;
    default:
      return QAbstractScrollArea::inputMethodQuery(query);
      break;
  }
}

void TermWidget::NotifyHightChange(int height) {
  if (height >= _conf->row() && height != _vsb->maximum()) {
    _vsb->setMaximum(height - _conf->row());
    if (_traceScroll) {
      _vsb->setValue(_vsb->maximum());
    }
  }
}

void TermWidget::scrollContentsBy(int dx, int dy) {
  _flagY -= dy;
  _render->UpdateFlagY(_flagY);
  if (_vsb->value() == _vsb->maximum()) {
    _traceScroll = true;
  } else {
    _traceScroll = false;
  }
}

void TermWidget::WarningBell() { qApp->beep(); }

void TermWidget::SendDeviceAttributes(const QString &str) {
  emit this->SendString(str);
}

void TermWidget::RenderInit() { _render->UpdateFlagY(0); }

void TermWidget::mousePressEvent(QMouseEvent *event) {
  if (_lastMouseClickType == QEvent::MouseButtonDblClick &&
      _lastMouseClickPos == event->pos() &&
      qAbs(QDateTime::currentDateTime().msecsTo(_lastMouseClickTime)) < 300) {
    mouseTripleClickEvent(event);
    return;
  }

  bool shiftEnabled = event->modifiers().testFlag(Qt::ShiftModifier);
  if (shiftEnabled) {
  } else {
    _render->ClearSelection();
    Coord start = _Point2Coord(event->pos());
    _selection->pivot = Coord{start.x, start.y + _flagY};
  }

  _lastMouseClickType = event->type();
  _lastMouseClickTime = QDateTime::currentDateTime();
  _lastMouseClickPos = event->pos();
}

void TermWidget::mouseDoubleClickEvent(QMouseEvent *event) {
  _lastMouseClickType = event->type();
  _lastMouseClickTime = QDateTime::currentDateTime();
  _lastMouseClickPos = event->pos();
}

void TermWidget::mouseTripleClickEvent(QMouseEvent *event) {
  qDebug() << "triple click";
}

void TermWidget::mouseMoveEvent(QMouseEvent *event) {
  if (event->buttons().testFlag(Qt::LeftButton)) {
    QPoint pos = event->pos();
    if (pos.x() > _conf->margins().left() && pos.y() > _conf->margins().top()) {
      Coord coord = _Point2Coord(pos);
      Coord end{coord.x, coord.y + _flagY};
      Coord start = _selection->pivot;

      if ((end.y > start.y) || (end.y == start.y && end.x >= start.x)) {
        _render->UpdateSelection(start, end);
        _selection->start = start;
        _selection->end = end;
      } else {
        _render->UpdateSelection(end, start);
        _selection->start = end;
        _selection->end = start;
      }
    }
  }
}

bool TermWidget::event(QEvent *event) {
  //  qDebug() << event;
  // FIX: tab change focus
  if (event->type() == QEvent::KeyPress) {
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    if (keyEvent->key() == Qt::Key_Tab) {
      this->keyPressEvent(keyEvent);
      return true;
    }
  }
  return QAbstractScrollArea::event(event);
}

int TermWidget::CalcWidth(const QString &str) {
  // TODO: use Map
  QFontMetrics fm(_conf->font());
  return qCeil(fm.horizontalAdvance(str) / (_fontWidth * 1.0));
}
