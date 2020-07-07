#include "NexTermWidget.h"

#include <QGuiApplication>
#include "Render/RenderProfile.h"

using namespace NexTerm::Render;

NexTermWidget::NexTermWidget(const NexTermProfile &profile, QWidget *parent)
    : QAbstractScrollArea(parent),
      _profile(profile),
      _flagY{0},
      _fm(QFontMetrics(_profile.font)),
      _traceScroll{true},
      _lastMouseClickType(QEvent::MouseButtonPress),
      _lastMouseClickTime(QDateTime::currentDateTime()),
      _lastMouseClickPos(QPoint(0, 0)) {
  this->setAttribute(Qt::WA_InputMethodEnabled);

  this->setFrameShape(QFrame::NoFrame);

  _viewport = std::make_unique<NexTermViewport>();
  this->setViewport(_viewport.get());

  _margin = QMargins(16, 6, 16, 4);

  _terminal = std::make_unique<Terminal>(profile.row, profile.column);

  RenderProfile rf{profile.font, profile.row, profile.column, 0, _margin};
  _render = std::make_unique<NexTermRender>(rf, _terminal.get());
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
  _vsb->setPageStep(profile.row);
  _vsb->setValue(0);

  this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  _timer = std::make_unique<QTimer>(this);
  _timer->setSingleShot(true);
  QObject::connect(_timer.get(), &QTimer::timeout, this,
                   &NexTermWidget::_OnTimeOut);
}

void NexTermWidget::write(const QByteArray &text) { _terminal->Write(text); }

void NexTermWidget::updateProfile(const NexTermProfile &profile) {
  _profile = profile;
}

QString NexTermWidget::GetSelectionText() const {
  return _terminal->GetText(_selection->start, _selection->end);
}

void NexTermWidget::onWrite(const QByteArray &text) { write(text); }

void NexTermWidget::_OnTimeOut() {
  emit this->PtyResize(_profile.row, _profile.column);
}

void NexTermWidget::_initTerminal() {
  _fontWidth = _fm.maxWidth();
  _fontHeight = _fm.height();
}

QPoint NexTermWidget::_Coord2Point(const Coord &coord) const {
  int x = _margin.left() + _fontWidth * (coord.x - 1);
  int y = _margin.top() + _fontHeight * (coord.y - 1);
  return QPoint(x, y);
}

Coord NexTermWidget::_Point2Coord(const QPoint &point) const {
  int x = qCeil((point.x() - _margin.left()) / (_fontWidth * 1.0));
  int y = qCeil((point.y() - _margin.top()) / (_fontHeight * 1.0));
  x = std::clamp(x, 0, _profile.column + 1);
  y = std::clamp(y, 1, _profile.row);
  return Coord{x, y};
}

QSize NexTermWidget::sizeHint() const {
  int w = _fontWidth * _profile.column + _margin.left() + _margin.right();
  int h = _fontHeight * _profile.row + _margin.top() + _margin.bottom();
  return QSize(w, h);
}

void NexTermWidget::resizeEvent(QResizeEvent *event) {
  _timer->start(68);
  QSize size = event->size();
  int h = size.height() - _margin.top() - _margin.bottom();
  int w = size.width() - _margin.left() - _margin.right();
  if (_vsb->isVisible()) {
    w += _vsb->size().width();
  }
  if (_hsb->isVisible()) {
    h += _hsb->size().height();
  }
  int row_old = _profile.row, column_old = _profile.column;
  int row_new = h / _fontHeight, column_new = w / _fontWidth;
  _profile.row = row_new;
  _profile.column = column_new;
  //  qDebug() << row << column;

  if (column_old != column_new) {
    _terminal->ResizeColumn(column_new);
    _render->UpdateColumn(column_new);
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
    _render->UpdateRow(row_new);
  }
  emit this->NotifyHightChange(_terminal->GetTextBuffer().LineCount());
  //  emit this->PtyResize(row_new, column_new);
}

void NexTermWidget::keyPressEvent(QKeyEvent *event) {
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

void NexTermWidget::inputMethodEvent(QInputMethodEvent *event) {
  //  qDebug() << event;
  //  QAbstractScrollArea::inputMethodEvent(event);
  QString str = event->commitString();
  if (!str.isEmpty()) {
    emit this->SendString(str);
  }
}

QVariant NexTermWidget::inputMethodQuery(Qt::InputMethodQuery query) const {
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

void NexTermWidget::NotifyHightChange(int height) {
  if (height >= _profile.row && height != _vsb->maximum()) {
    _vsb->setMaximum(height - _profile.row);
    if (_traceScroll) {
      _vsb->setValue(_vsb->maximum());
    }
  }
}

void NexTermWidget::scrollContentsBy(int dx, int dy) {
  _flagY -= dy;
  _render->UpdateFlagY(_flagY);
  if (_vsb->value() == _vsb->maximum()) {
    _traceScroll = true;
  } else {
    _traceScroll = false;
  }
}

void NexTermWidget::WarningBell() { qApp->beep(); }

void NexTermWidget::SendDeviceAttributes(const QString &str) {
  emit this->SendString(str);
}

void NexTermWidget::RenderInit() { _render->UpdateFlagY(0); }

void NexTermWidget::mousePressEvent(QMouseEvent *event) {
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

void NexTermWidget::mouseDoubleClickEvent(QMouseEvent *event) {
  _lastMouseClickType = event->type();
  _lastMouseClickTime = QDateTime::currentDateTime();
  _lastMouseClickPos = event->pos();
}

void NexTermWidget::mouseTripleClickEvent(QMouseEvent *event) {
  qDebug() << "triple click";
}

void NexTermWidget::mouseMoveEvent(QMouseEvent *event) {
  if (event->buttons().testFlag(Qt::LeftButton)) {
    QPoint pos = event->pos();
    if (pos.x() > _margin.left() && pos.y() > _margin.top()) {
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

bool NexTermWidget::event(QEvent *event) {
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

int NexTermWidget::CalcWidth(const QString &str) {
  // TODO: use Map
  return qCeil(_fm.horizontalAdvance(str) / (_fontWidth * 1.0));
}
