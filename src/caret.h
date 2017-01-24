#pragma once

// Just needed for SDL_Event
#include "bio_crt.h"

class ScreenBuffer;

class Caret
{
public:
  Caret() {}
  virtual ~Caret() {}

  int& getX()
  {
    return caretX;
  }

  int& getY()
  {
    return caretY;
  }

  int& getUpperLeftCornerLine()
  {
    return upperLeftCornerLine;
  }

  int& getUpperLeftCornerRow()
  {
    return upperLeftCornerRow;
  }

  int getLogicalY()
  {
    return caretY + upperLeftCornerLine;
  }

  int getLogicalX()
  {
    return caretX + upperLeftCornerRow;
  }

  bool& insertMode()
  {
    // guard against 'stange' insertMode values (like insertMode == 6 which switches between 6/7)
    if (isInInsertMode) {
      isInInsertMode = true;
    }

    return isInInsertMode;
  }

  bool& eliteMode()
  {
    // guard against 'stange' eliteMode values (like eliteMode == 6 which switches between 6/7)
    if (isInEliteMode) {
      isInEliteMode = true;
    }
    return isInEliteMode;
  }

  bool& fontMode()
  {
    // guard against 'stange' eliteMode values (like eliteMode == 6 which switches between 6/7)
    if (isInFontMode) {
      isInFontMode = true;
    }
    return isInFontMode;
  }

  void checkCaretPosition(ScreenBuffer& buffer);

  bool handleKeyStroke(SDL_Event* event, ScreenBuffer& buffer);

private:

  int caretX = 0;
  int caretY = 0;
  int upperLeftCornerLine = 0;
  int upperLeftCornerRow = 0;
  bool isInInsertMode = false;
  bool isInEliteMode = false;
  bool isInFontMode = false;

};

