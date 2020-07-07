#pragma once

#include <QString>

class UChar final {
 public:
  UChar();
  UChar(const QString& uch, int width = 1);

  inline const QString& udata() const { return _uch; }
  inline int width() const { return _width; }

 private:
  QString _uch;
  int _width;
};
