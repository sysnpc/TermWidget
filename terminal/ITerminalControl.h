#pragma once

#include <QString>

class ITerminalControl {
 public:
  virtual void NotifyHightChange(int height) = 0;
  virtual void WarningBell() = 0;

  virtual void SendDeviceAttributes(const QString &str) = 0;

  virtual void RenderInit() = 0;

  // signal
  virtual void SetWindowTitle(const QString &title) = 0;
  virtual void BracketedPasteMode(bool enabled) = 0;
};
