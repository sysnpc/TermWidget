#pragma once

#include "AttrRow.h"
#include "CharRow.h"

class Row final {
 public:
  Row(const int rowWidth);

  void replace(int x, const CharRowCell& charCell, const AttrRowCell& attrCell);
  void insert(int x, const CharRowCell& charCell, const AttrRowCell& attrCell);
  void fill(const CharRowCell& charCell, const AttrRowCell& attrCell);

  inline int size() const noexcept { return _rowWidth; }

  inline void setWraped(bool enabled) { _wraped = enabled; }
  inline bool isWraped() const { return _wraped; }

  inline const CharRow& GetCharRow() const noexcept { return _charRow; }
  inline CharRow& GetCharRow() noexcept { return _charRow; }

  inline const AttrRow& GetAttrRow() const noexcept { return _attrRow; }
  inline AttrRow& GetAttrRow() noexcept { return _attrRow; }

  void ClearCell(int x);

 private:
  int _rowWidth;
  CharRow _charRow;
  AttrRow _attrRow;

  bool _wraped;
};
