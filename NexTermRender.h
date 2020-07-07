#pragma once

#include <QVector>
#include <QtWidgets>
#include "buffer/IRenderTarget.h"
#include "render/IRenderData.h"
#include "render/RenderProfile.h"

using namespace PresudoMicrosoft::Console::Render;
using namespace NexTerm::Render;

class CharRowCell;
class AttrRowCell;

class NexTermRender : public QWidget, public IRenderTarget {
  Q_OBJECT
 public:
  explicit NexTermRender(const RenderProfile &profile, IRenderData *renderData,
                         QWidget *parent = nullptr);

  void UpdateFlagY(int flagY);
  void UpdateSelection(Coord start, Coord end);
  void ClearSelection();
  void UpdateColumn(int column);
  void UpdateRow(int row);

 signals:

 private slots:
  void _BlinkUpdate();

 private:
  RenderProfile _profile;
  IRenderData *_renderData;

  int _fontWidth;
  int _fontHeight;

  // selection
  Coord _start;
  Coord _end;

  std::unique_ptr<QTimer> _timer;
  bool _isBlinkShow = false;

  QVector<Coord> _calcCoords(int x1, int y1, int x2, int y2);

  QVector<Coord> _rect2coords(const QRect &rect);
  QRect _coord2rect(const Coord &coord);
  QRegion _coords2region(const QVector<Coord> &coords);

  void _setPen(QPainter &p, const AttrRowCell &attrcell);
  void _drawCursor(QPainter &p, const Coord &coord, int width);
  void _drawContent(QPainter &p, const Coord &coord,
                    const CharRowCell &charCell, const AttrRowCell &attrCell);
  void _drawSelection(QPainter &p, const Coord &coord);
  void _drawCoords(QPainter &p, const QVector<Coord> &coords);

  bool _IsInSelection(const Coord &coord);

 public:
  // IRenderTarget interface
  void TriggerRedraw(const Rect &rect) override;
  void TriggerRedraw(int x1, int y1, int x2, int y2) override;
  void TriggerRedrawCursor(const Coord *const pcoord) override;
  void TriggerRedrawAll() override;
  void TriggerTeardown() override;
  void TriggerSelection() override;
  void TriggerScroll() override;
  void TriggerScroll(const Coord *const pcoordDelta) override;
  void TriggerCircling() override;
  void TriggerTitleChange() override;

  // QWidget interface
 public:
  QSize sizeHint() const override;

 protected:
  void paintEvent(QPaintEvent *event) override;
};
