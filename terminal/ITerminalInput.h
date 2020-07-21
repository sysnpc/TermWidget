#pragma once

#include <QKeyEvent>

class ITerminalInput {
 public:
  ITerminalInput();

  // signal
  virtual void SendSignal(int signal) = 0;
  virtual void SendString(const QString &str) = 0;

  void SetCursorKeysMode(bool enabled);
  void SetKeypadMode(bool enabled);

 protected:
  bool HandleKeyEvent(QKeyEvent *event);

 private:
  bool _keypadApplicationMode = false;
  bool _cursorKeysMode = false;

  const QHash<Qt::Key, QString> &_GetKeyMapping(QKeyEvent *event);
  bool _isCursorKey(QKeyEvent *event);
};
