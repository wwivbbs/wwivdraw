#ifndef SELECTSAUCECOMMAND__HPP
#define SELECTSAUCECOMMAND__HPP

#include "bio_crt.h"
#include "command.h"
#include "sauce.h"

class WWIVDrawMain;
class ScreenBuffer;

class SelectSauceCommand : Command
{
public:
  SelectSauceCommand(WWIVDrawMain* win);
  void run();
private:
  ScreenBuffer* screen_buffer_ = nullptr;
};

#endif
