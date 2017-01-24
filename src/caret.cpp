#include "caret.h"
#include "mdraw.h"

#include "bio_crt.h"
#include "ScreenBuffer.hpp"

// From mdraw.h

extern char* CharSet[12];
extern unsigned char tabs[80];
extern unsigned char CursorPos[80];
extern unsigned char cpos;
extern unsigned char Attribute;
extern bool Undo, FontTyped;
extern unsigned char ActiveCharset;
extern bool FullScreen;

bool Caret::handleKeyStroke(SDL_Event* event, ScreenBuffer& buffer)
{
#ifdef HAS_GPM
  if (MouseSupport) {
    mouse_update();
    caretX += mouse_deltax;
    caretY += mouse_deltay;
    mouse_deltax = 0;
    mouse_deltay = 0;
  }
#endif
  switch (event->key.keysym.sym) {
  case SDLK_DOWN:
    caretY++;
    checkCaretPosition(buffer);
    return true;
  case SDLK_UP:
    caretY--;
    checkCaretPosition(buffer);
    return true;
  case SDLK_LEFT:
    caretX--;
    checkCaretPosition(buffer);
    return true;
  case SDLK_RIGHT:
    caretX++;
    checkCaretPosition(buffer);
    return true;
  case SDLK_PAGEDOWN:
    upperLeftCornerLine += LINES - 1;
    if (FullScreen) {
      caretY = LINES - 1;
    }
    else {
      caretY = LINES - 2;
    }
    checkCaretPosition(buffer);
    return true;
  case SDLK_PAGEUP:
    upperLeftCornerLine -= LINES - 1;
    caretY = 0;
    checkCaretPosition(buffer);
    return true;
  case SDLK_HOME:
    caretX = 0;
    checkCaretPosition(buffer);
    return true;
  case SDLK_END:
    caretX = 79;
    checkCaretPosition(buffer);
    return true;
  default:
    break;
  }
  if (event->key.keysym.mod & KMOD_SHIFT) {
    switch (event->key.keysym.sym) {
    case SDLK_F1:
      ActiveCharset = 1;
      return true;
    case SDLK_F2:
      ActiveCharset = 2;
      return true;
    case SDLK_F3:
      ActiveCharset = 3;
      return true;
    case SDLK_F4:
      ActiveCharset = 4;
      return true;
    case SDLK_F5:
      ActiveCharset = 5;
      return true;
    case SDLK_F6:
      ActiveCharset = 6;
      return true;
    case SDLK_F7:
      ActiveCharset = 7;
      return true;
    case SDLK_F8:
      ActiveCharset = 8;
      return true;
    case SDLK_F9:
      ActiveCharset = 9;
      return true;
    case SDLK_F10:
      ActiveCharset = 10;
      return true;
    default:
      break;
    }
  }
  return false;
}

void Caret::checkCaretPosition(ScreenBuffer& buffer) {
  if (caretY < 0) {
    caretY = 0;
    --upperLeftCornerLine;
  }
  if (caretX < 0) {
    caretX = 0;
    --upperLeftCornerRow;
  }

  if (FullScreen) {
    if (caretY > LINES - 1) {
      caretY = LINES - 1;
      ++upperLeftCornerLine;
    }
    if (upperLeftCornerLine + LINES > buffer.getHeight()) {
      upperLeftCornerLine = buffer.getHeight() - LINES;
    }
  }
  else {
    if (caretY > LINES - 2) {
      caretY = LINES - 2;
      ++upperLeftCornerLine;
    }
    if (upperLeftCornerLine + LINES - 1 > buffer.getHeight()) {
      upperLeftCornerLine = buffer.getHeight() - LINES + 1;
    }
  }

  if (caretX >= COLS) {
    upperLeftCornerRow += caretX - COLS + 1;
    caretX = COLS - 1;
  }

  if (upperLeftCornerRow + 80 > buffer.getWidth()) {
    upperLeftCornerRow = buffer.getWidth() - 80;
  }

  upperLeftCornerLine = max(upperLeftCornerLine, 0);
  upperLeftCornerRow = max(upperLeftCornerRow, 0);

  caretX = max(caretX, 0);
  caretX = min(caretX, 79);
}


