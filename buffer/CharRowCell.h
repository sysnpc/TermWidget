#pragma once

#include "UChar.h"

class CharRowCell final {
 public:
  enum CharRowCellType : unsigned char {
    CT_Null = 0x01,
    CT_Extend = 0x02,
    CT_Normal = 0x04,
  };

 public:
  CharRowCell();
  //  CharRowCell(const QString& uch) noexcept;
  CharRowCell(const UChar& uch);
  CharRowCell(const CharRowCell* parent);

  const CharRowCell* parent() const { return _parent; }

  const UChar& uch() const { return _uch; }
  int width() const;
  inline bool isNull() const { return _type == CT_Null; }
  inline bool isExtend() const { return _type == CT_Extend; }
  inline bool isNormal() const { return _type == CT_Normal; }

  inline void operator=(const CharRowCellType& type) { _type = type; }

 private:
  CharRowCellType _type;
  UChar _uch;

  const CharRowCell* _parent;
};
