#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  _layout = std::make_unique<QVBoxLayout>(ui->centralwidget);
  _layout->setMargin(0);

  _term = std::make_unique<TermWidget>();
  _layout->addWidget(_term.get());
}

MainWindow::~MainWindow()
{
  delete ui;
}
