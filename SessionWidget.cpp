#include "SessionWidget.h"

SessionWidget::SessionWidget(QWidget *parent) : QWidget(parent) {
  _layout = std::make_unique<QVBoxLayout>(this);
  _layout->setMargin(0);

  _pty = std::make_unique<PtyProcess>(24, 80);

  NexTermProfile profile{QFont("SF Mono", 12), 24, 80};
  _term = std::make_unique<NexTermWidget>(profile);
  this->setFocusProxy(_term.get());

  _layout->addWidget(_term.get());

  QObject::connect(_pty.get(), &PtyProcess::receiveData, _term.get(),
                   &NexTermWidget::onWrite);
  QObject::connect(_term.get(), &NexTermWidget::SendString, this,
                   &SessionWidget::OnSendString);
  QObject::connect(_term.get(), &NexTermWidget::SendSignal, this,
                   &SessionWidget::OnSendSignal);
  QObject::connect(_term.get(), &NexTermWidget::BracketedPasteMode, this,
                   &SessionWidget::OnBracketedPasteMode);

  QObject::connect(_term.get(), &NexTermWidget::SetWindowTitle, this,
                   &SessionWidget::OnSetWindowTitle);

  QObject::connect(_term.get(), &NexTermWidget::PtyResize, this,
                   &SessionWidget::OnPtyResize);

  this->StartPty();
}

SessionWidget::~SessionWidget() { _pty->close(); }

void SessionWidget::PasteString(const QString &str) {
  QString data = str;
  if (_bracketedPasteMode) {
    data = "\x1b[200~" + str + "\x1b[201~";
  }
  _pty->sendText(data.toLocal8Bit());
}

QString SessionWidget::CopyString() { return _term->GetSelectionText(); }

void SessionWidget::OnSendString(const QString &str) {
  _pty->sendText(str.toLocal8Bit());
}

void SessionWidget::OnSendSignal(int signal) { _pty->sendSignal(signal); }

void SessionWidget::OnSetWindowTitle(const QString &title) {
  //  emit this->ChangeTitle(title);
}

void SessionWidget::OnBracketedPasteMode(bool enabled) {
  _bracketedPasteMode = enabled;
}

void SessionWidget::OnPtyResize(int row, int column) {
  _pty->setWinSize(row, column);
}

void SessionWidget::StartPty() {
  QStringList args;
  args.append("-fp");
  args.append("luxinxin");

  QStringList envs;
  envs.append("TERM=xterm-256color");
  envs.append("LC_CTYPE=UTF-8");
  _pty->start("login", args, envs);
}

QSize SessionWidget::sizeHint() const { return _term->sizeHint(); }
