#pragma once

#include "TextCell.h"

class TextRow
{
 public:
  TextRow(int column);

  void ReplaceCell(int index, const UChar &uch, const TextAttribute &attr);
  void DeleteCell(int index);
  const TextCell &GetCell(int index) const;
  TextCell &GetCell(int index);
  void SetWraped(bool enabled);
  bool isWraped() const;

  bool isEmptyRow() const;

  int GetEffectLength() const;

 private:
  int _FindParentIndex(TextCell *parent);

 private:
  std::vector<TextCell> _cells;
  bool _wraped = false;
};

