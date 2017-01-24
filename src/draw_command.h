#ifndef DRAWCOMMAND_HPP
#define DRAWCOMMAND_HPP

#include "command.h"
#include "bio_crt.h"

class DrawCommand : Command
{
private:
  unsigned int drawMode;
  void selectDrawMode();
public:
  DrawCommand(WWIVDrawMain* win) : Command(win) {}
  void run();
};

class DrawLineCommand : Command
{
public:
  DrawLineCommand(WWIVDrawMain* win) : Command(win) {}
  void run();
};


#endif
