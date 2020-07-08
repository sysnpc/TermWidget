#pragma once

#include <QColor>
#include <QFont>
#include <QMargins>

class TermConfig
{
 public:
  TermConfig();

  int row() { return _row; }
  void setRow(int row) { _row = row; }
  int column() { return _column; }
  void setColumn(int column) { _column = column; }
  const QFont& font() { return _font; }
  const QMargins margins() { return _margins; }

  const QColor& selection() const { return _selection; }
  // Color
  const QColor& windowText() const { return _window_text; }
  const QColor& window() const { return _window; }
  const QColor& black() const { return _black; }
  const QColor& red() const { return _red; }
  const QColor& green() const { return _green; }
  const QColor& yellow() const { return _yellow; }
  const QColor& blue() const { return _blue; }
  const QColor& magenta() const { return _magenta; }
  const QColor& cyan() const { return _cyan; }
  const QColor& white() const { return _white; }
  const QColor& brightBlack() const { return _bright_black; }
  const QColor& brightRed() const { return _bright_red; }
  const QColor& brightGreen() const { return _bright_green; }
  const QColor& brightYellow() const { return _bright_yellow; }
  const QColor& brightBlue() const { return _bright_blue; }
  const QColor& brightMagenta() const { return _bright_magenta; }
  const QColor& brightCyan() const { return _bright_cyan; }
  const QColor& brightWhite() const { return _bright_white; }

 private:
  int _row = 24;
  int _column = 80;
  QFont _font = QFont("SF Mono", 13);
  QMargins _margins = QMargins(16, 6, 16, 4);

  QColor _selection = QColor("#B3D7FF");

  QColor _window_text = QColor("#000000");
  QColor _window = QColor("#FFFFFF");
  QColor _black = QColor("#0C0C0C");
  QColor _red = QColor("#C50F1F");
  QColor _green = QColor("#13A10E");
  QColor _yellow = QColor("#C19C00");
  QColor _blue = QColor("#0037DA");
  QColor _magenta = QColor("#881798");
  QColor _cyan = QColor("#3A96DD");
  QColor _white = QColor("#CCCCCC");
  QColor _bright_black = QColor("#767676");
  QColor _bright_red = QColor("#E74856");
  QColor _bright_green = QColor("#16C60C");
  QColor _bright_yellow = QColor("#F9F1A5");
  QColor _bright_blue = QColor("#3B78FF");
  QColor _bright_magenta = QColor("#B4009E");
  QColor _bright_cyan = QColor("#61D6D6");
  QColor _bright_white = QColor("#F2F2F2");
};

