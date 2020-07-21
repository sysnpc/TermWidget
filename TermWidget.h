#pragma once

#include <QtWidgets>
#include "TermRender.h"
#include "TermViewport.h"
#include "config/TermConfig.h"
//#include "terminal/ITerminalControl.h"
#include "terminal/ITerminalInput.h"
#include "terminal/Terminal.h"

using namespace PresudoMicrosoft::Console::VirtualTerminal;

class TermWidget final : public QAbstractScrollArea, public ITerminalInput {
  Q_OBJECT
 public:
  explicit TermWidget(TermConfig *conf = nullptr, QWidget *parent = nullptr);

  QString GetSelectionText() const;

 public slots:
  void WriteText(const QByteArray &text);

 signals:
  void SendSignal(int signal) override;
  void SendString(const QString &str) override;
  //  void BracketedPasteMode(bool enabled) override;
  //  void SetWindowTitle(const QString &title) override;

  void TermResize(int row, int column);

 private slots:
  void _OnResizeTimeOut();

 private:
  //  std::unique_ptr<ITerminalInput> _terminalInput;

  std::unique_ptr<Terminal> _terminal;

  std::unique_ptr<TermViewport> _viewport;
  std::unique_ptr<TermRender> _render;
  std::unique_ptr<QScrollBar> _vsb;
  std::unique_ptr<QScrollBar> _hsb;

  TermConfig *_conf = nullptr;

  int _flagY;

  int _fontWidth;
  int _fontHeight;

  bool _traceScroll = true;

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

  std::unique_ptr<QTimer> _resizeTimer;

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
  //  void NotifyHightChange(int height) override;
  //  void WarningBell() override;
  //  void SendDeviceAttributes(const QString &str) override;
  //  void RenderInit() override;
};
