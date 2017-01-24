#include "command.h"
#include "mdraw.h"

Command::Command(WWIVDrawMain* main)
  : main_(main),
  screen_buffer_(main->getCurrentBuffer()),
  caret_(&main->getCaret()) {}