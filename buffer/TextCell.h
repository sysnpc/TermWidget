#pragma once

#include "TextAttribute.h"
#include "UChar.h"

class TextCell {
 public:
  enum TextCellType : unsigned short {
    Null = 0,
    Extend = 1 << 1,
    Normal = 1 << 2,
  };

 public:
  TextCell();
  TextCell(const UChar& uch, const TextAttribute& attr);
  TextCell(TextCell* parent);
  const UChar& uch() const;
  TextCell* parent() const;
  const TextAttribute& attr() const;

  bool isNull() const;
  bool isExtend() const;
  bool isNormal() const;

  void SetNull();

 private:
  TextCellType _type;
  UChar _uch;
  TextAttribute _attr;

  TextCell* _parent = nullptr;
};
