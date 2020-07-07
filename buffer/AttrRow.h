#pragma once

#include "AttrRowCell.h"

class AttrRow final {
 public:
  AttrRow(int rowWidth);

  void replace(int x, const AttrRowCell& attrCell);

  inline const AttrRowCell& at(int index) const { return _data.at(index - 1); }

  void ClearCell(int x);

 protected:
  std::vector<AttrRowCell> _data;
};
