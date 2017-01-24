#include "command.hpp"
#include "mdraw.h"

Command::Command(MysticDrawMain* main)
  : main_(main),
  screen_buffer_(main->getCurrentBuffer()),
  caret_(&main->getCaret()) {}