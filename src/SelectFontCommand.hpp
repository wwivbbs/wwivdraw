#ifndef SELECTFONTCOMMAND__HPP
#define SELECTFONTCOMMAND__HPP

#include <iostream>
#include <cmath>

#include "bio_crt.h"
#include "command.hpp"
#include "fonts.h"

class MysticDrawMain;

class SelectOutlineCommand : public Command
{
private:
  void setunderground(int Underg, int b);

public:
  SelectOutlineCommand(MysticDrawMain* win) : Command(win) {}
  void run();
};

class SelectFontCommand : public Command
{
private:
  void clearWindow();
  void showPreviewCharacter(int fontNumber);

public:
  SelectFontCommand(MysticDrawMain* win) : Command(win) {}
  void run();
};

#endif
