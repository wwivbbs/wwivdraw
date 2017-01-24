#ifndef BLOCKMODECOMMAND__HHP
#define BLOCKMODECOMMAND__HHP

#include <algorithm>
#include "command.h"

class WWIVDrawMain;
class ScreenBuffer;
class Caret;

class BlockModeCommand : public Command
{
public:
  BlockModeCommand(WWIVDrawMain* win, ScreenBuffer* screen_buffer, Caret* caret)
    : Command(win) {}
  void run();

private:
  int X1, Y1, X2, Y2;
  void CopyBlock(char Mode);

  ScreenBuffer* screen_buffer_ = nullptr;
  Caret* caret_ = nullptr;
};
#endif
