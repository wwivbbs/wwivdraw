#ifndef EFFECTMODECOMMAND__HPP
#define EFFECTMODECOMMAND__HPP

#include "command.h"
#include "mdraw.h"

class WWIVDrawMain;

class SelectEffectModeCommand : public Command
{
private:
  void draweffekt(int effekt, char *blabla, int highlite);
  void changecolor(int Effekt);
public:
  SelectEffectModeCommand(WWIVDrawMain* win) : Command(win) {}
  void run();
};

#endif
