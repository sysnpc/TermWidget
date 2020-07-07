#include "NexTermViewport.h"

#include "Config/Preferences.h"

NexTermViewport::NexTermViewport(QWidget *parent) : QWidget(parent)
{
  this->setAutoFillBackground(true);

  QPalette pal = this->palette();
  pal.setColor(QPalette::Window, appconf->profile()->background());
  this->setPalette(pal);
}
