#pragma once

#include "NexTermWidget.h"
#include "Pty/PtyProcess.h"

class SessionWidget : public QWidget {
  Q_OBJECT
 public:
  explicit SessionWidget(QWidget *parent = nullptr);
  ~SessionWidget();

  inline const NexTermWidget *term() const { return _term.get(); }

  void PasteString(const QString &str);
  QString CopyString();

 public slots:
  void OnSendString(const QString &str);
  void OnSendSignal(int signal);
  void OnSetWindowTitle(const QString &title);
  void OnBracketedPasteMode(bool enabled);

  void OnPtyResize(int row, int column);

 private:
  std::unique_ptr<QVBoxLayout> _layout;

  std::unique_ptr<PtyProcess> _pty;
  std::unique_ptr<NexTermWidget> _term;

  bool _bracketedPasteMode = false;

 private:
  void StartPty();

  // QWidget interface
 public:
  QSize sizeHint() const override;
};
