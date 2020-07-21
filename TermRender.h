#pragma once

#include <QVector>
#include <QtWidgets>
#include "common/precomp.h"
#include "config/TermConfig.h"
#include "terminal/Terminal.h"

using namespace PresudoMicrosoft::Console::VirtualTerminal;

class TermRender : public QWidget {
  Q_OBJECT
 public:
  explicit TermRender(TermConfig *conf, QWidget *parent = nullptr);

  void SetTerminal(Terminal *term);

  void UpdateFlagY(int flagY);
  //  void UpdateSelection(Coord start, Coord end);
  //  void ClearSelection();
  void UpdateSize();

 signals:

 private slots:
  void _BlinkTimeOut();

 private:
  TermConfig *_conf;

  Terminal *_term = nullptr;
  //  IRenderData *_renderData;

  int _flagY = 0;

  int _fontWidth;
  int _fontHeight;

  // selection
  Coord _start;
  Coord _end;

  std::unique_ptr<QTimer> _blinkTimer;
  bool _blinkShow = true;

  QVector<Coord> _calcCoords(int x1, int y1, int x2, int y2);
  QVector<Coord> _rect2coords(const QRect &rect);

  //  QRect _coord2rect(const Coord &coord);
  //  QRegion _coords2region(const QVector<Coord> &coords);

  //  void _setPen(QPainter &p, const AttrRowCell &attrcell);
  //  void _drawCursor(QPainter &p, const Coord &coord, int width);
  void _drawContent(QPainter &p, const Coord &coord, const TextCell &cell);
  //  void _drawSelection(QPainter &p, const Coord &coord);
  void _drawCoords(QPainter &p, const QVector<Coord> &coords);

  //  bool _IsInSelection(const Coord &coord);

  QColor _tag2color(const TextColor &textColor);

 public:
  // IRenderTarget interface
  //  void TriggerRedraw(const Rect &rect) override;
  //  void TriggerRedraw(int x1, int y1, int x2, int y2) override;
  //  void TriggerRedrawCursor(const Coord *const pcoord) override;
  //  void TriggerRedrawAll() override;
  //  void TriggerTeardown() override;
  //  void TriggerSelection() override;
  //  void TriggerScroll() override;
  //  void TriggerScroll(const Coord *const pcoordDelta) override;
  //  void TriggerCircling() override;
  //  void TriggerTitleChange() override;

  // QWidget interface
 public:
  QSize sizeHint() const override;

 protected:
  void paintEvent(QPaintEvent *event) override;
};
