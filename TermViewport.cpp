#include "TermViewport.h"

TermViewport::TermViewport(QWidget *parent) : QWidget(parent) {
  this->setAutoFillBackground(true);

  QPalette pal = this->palette();
  pal.setColor(QPalette::Window, Qt::white);
  this->setPalette(pal);
}
