#pragma once

#include <vector>
#include "TextCursor.h"
#include "TextRow.h"

class TextBuffer final {
 public:
  enum TextBufferType { Main, Alternate };

  TextBuffer(int row = 24, int column = 80, TextBufferType type = Main);
  TextBuffer(const TextBufferType type, int row, int column);
  TextBuffer(const TextBuffer& buffer) = delete;

  void SetCursor(TextCursor* cursor);
  TextCursor* GetCursor();

  inline bool isMain() const { return _type == TextBufferType::Main; }
  inline bool isAlternate() const { return _type == TextBufferType::Alternate; }

  bool isDrawCell(int x, int y) const;
  const TextRow& GetRow(int index) const;
  TextRow& GetRow(int index);
  const TextCell& GetCell(int x, int y) const;

  void Append(const TextCell& cell);

  int GetHeight() const;

  int GetEffectiveRowHeight() const;

  void SetRowWraped(bool enabled);

  void InsertRow(int index);
  void DeleteRow(int index);
  void DeleteRow(int st, int ed);

  void AppendRow(int n);
  void PopBackRow(int n);

  void InsertUChar(const UChar& uchar);
  void ReplaceUChar(const UChar& uchar);
  //  void FillScreen(const UAttrCharCell& cell);
  //  void InsertEmptyLine();
  //  void ExtendLine();

  //  void InsertLine(int n);
  //  void DeleteLine(int n);

  //  void CR();
  //  void LF(bool withReturn);
  //  void ReverseLF();

  //  void Erase(Coord st, Coord ed);
  //  void Erase(int st, int count);

  //  Coord GetWordStart(Coord target, const QString& wordDelimiters);
  //  Coord GetWordEnd(Coord target, const QString& wordDelimiters);

  //  QString GetText(Coord start, Coord end);

  //  Row& GetRowByOffset(const int index);
  //  const Row& GetRow(const int index) const;

  //  int OffsetY() const;

  //  bool IsWithInPos(Coord pos) const;

  //  TextBuffer* ResizeColumn(int column);
  //  int ResizeRow(int row);

 private:
  //  int GetNotEmptyLineCount();

  //  void ResizeSaveCursor();
  //  void ResizeRestoreCursor();

 private:
  int _row;
  int _column;
  TextBufferType _type;
  std::vector<TextRow> _rows;
  std::unique_ptr<TextCursor> _cursor;

  //  struct ResizeSaveState {
  //    Cursor cursor;
  //    int flagY;
  //  };
  //  ResizeSaveState _resizeState;

  //  IRenderTarget* _renderTarget = nullptr;
  //  ITerminalControl* _control = nullptr;
};
