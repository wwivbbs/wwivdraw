#ifndef BLOCKMODECOMMAND__HHP
#define BLOCKMODECOMMAND__HHP

#include <algorithm>
#include "command.hpp"

class MysticDrawMain;
class ScreenBuffer;
class Caret;

class BlockModeCommand : public Command
{
public:
  BlockModeCommand(MysticDrawMain* win, ScreenBuffer* screen_buffer, Caret* caret)
    : Command(win) {}
  void run();

private:
  int X1, Y1, X2, Y2;
  void CopyBlock(char Mode);

  ScreenBuffer* screen_buffer_ = nullptr;
  Caret* caret_ = nullptr;
};
#endif
