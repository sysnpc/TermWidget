#pragma once

#include <vector>
#include "CharRowCell.h"

// the characters of one row of screen buffer
// we keep the following values so that we don't write
// more pixels to the screen than we have to:
// left is initialized to screenbuffer width.  right is
// initialized to zero.
//
//      [     foo.bar    12-12-61                       ]
//       ^    ^                  ^                     ^
//       |    |                  |                     |
//     Chars Left               Right                end of Chars buffer
class CharRow final {
 public:
  CharRow(int rowWidth);

  const CharRowCell& replace(int x, const CharRowCell& charCell);

  void ClearCell(int x);

  bool IsEmpty() const;

  inline const CharRowCell& at(int index) const {
    return _data.at(index - 1);
  }

  int GetLastNotNullIndex() const;

 private:
  std::vector<CharRowCell> _data;

  int _FindParentIndex(const CharRowCell* parent);
};
