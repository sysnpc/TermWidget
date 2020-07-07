#pragma once

#include <QtWidgets>
#include "NexTermProfile.h"
#include "NexTermRender.h"
#include "NexTermViewport.h"
#include "terminal/ITerminalControl.h"
#include "terminal/ITerminalInput.h"
#include "terminal/Terminal.h"

using namespace PresudoMicrosoft::Terminal::Core;

class NexTermWidget : public QAbstractScrollArea,
                      public ITerminalInput,
                      public ITerminalControl {
  Q_OBJECT
 public:
  explicit NexTermWidget(const NexTermProfile &profile,
                         QWidget *parent = nullptr);

  void write(const QByteArray &text);

  inline const NexTermProfile &profile() const { return _profile; }
  void updateProfile(const NexTermProfile &profile);

  QString GetSelectionText() const;

 public slots:
  void onWrite(const QByteArray &text);

 signals:
  void SendSignal(int signal) override;
  void SendString(const QString &str) override;
  void BracketedPasteMode(bool enabled) override;
  void SetWindowTitle(const QString &title) override;

  void PtyResize(int row, int column);

 private slots:
  void _OnTimeOut();

 private:
  std::unique_ptr<ITerminalInput> _terminalInput;

  std::unique_ptr<Terminal> _terminal;

  std::unique_ptr<NexTermViewport> _viewport;
  std::unique_ptr<NexTermRender> _render;
  std::unique_ptr<QScrollBar> _vsb;
  std::unique_ptr<QScrollBar> _hsb;
  QMargins _margin;

  NexTermProfile _profile;

  int _flagY;

  QFontMetrics _fm;
  int _fontWidth;
  int _fontHeight;

  bool _traceScroll;

  void _initTerminal();

  QEvent::Type _lastMouseClickType;
  QDateTime _lastMouseClickTime;
  QPoint _lastMouseClickPos;
  QPoint _Coord2Point(const Coord &coord) const;
  Coord _Point2Coord(const QPoint &point) const;

  struct SelectionAnchors {
    Coord start;
    Coord end;
    Coord pivot;
  };
  std::optional<SelectionAnchors> _selection;
  QPoint _singleClickPos;

  std::unique_ptr<QTimer> _timer;

  // QWidget interface
 public:
  QSize sizeHint() const override;
  QVariant inputMethodQuery(Qt::InputMethodQuery query) const override;
  bool event(QEvent *event) override;

 protected:
  void resizeEvent(QResizeEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
  void inputMethodEvent(QInputMethodEvent *event) override;
  void scrollContentsBy(int dx, int dy) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;
  void mouseTripleClickEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event) override;

  // ITerminalControl interface
 public:
  void NotifyHightChange(int height) override;
  void WarningBell() override;
  void SendDeviceAttributes(const QString &str) override;
  void RenderInit() override;

  // ITerminalInput interface
 public:
  int CalcWidth(const QString &str) override;
};
