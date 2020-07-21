#pragma once

#include <QMainWindow>
#include "TermWidget.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  Ui::MainWindow *ui;
  std::unique_ptr<QBoxLayout> _layout;
  std::unique_ptr<TermWidget> _term;
};
