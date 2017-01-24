#include "draw_command.h"
#include "mdraw.h"

void DrawCommand::selectDrawMode()
{
  int a = 0;
  draw_main().ClearMessageLine();
  ansout << gotoxy(0, LINES - 1);
  CoolWrite("Select drawmode :");
  drawMode = 0xFF00;
  a = chooser(19, 1, "Character", "aTribute", "Fore", "Back", "Abort", 0);
  switch (a) {
  case 1:
    a = draw_main().readCharacter();
    drawMode = 0x0100 | (a & 255);
    break;
  case 2:
    drawMode = 0x0200 | Attribute;
    break;
  case 3:
    drawMode = 0x0300 | Attribute & 15;
    break;
  case 4:
    drawMode = 0x0400 | Attribute & 240;
    break;
  }
}

void DrawCommand::run()
{
  int maxy = 22;
  if (FullScreen) {
    maxy++;
  }
  maxy += LINES - 25;

  selectDrawMode();

  if (drawMode == 0xFF00) {
    return;
  }

  draw_main().ClearMessageLine();
  ansout << gotoxy(0, LINES - 1);
  CoolWrite("Drawmode, use cursorkeys, press <ESC> to quit");
  SDL_Event event;
  bool done = false;
  do {
    if (FullScreen) {
      ansout << gotoxy(caret().getX(), caret().getY());
    }
    else {
      ansout << gotoxy(caret().getX(), caret().getY() + 1);
    }

    screenEngine.showScreen();
    SDL_Delay(50);
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        done = true;
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
          done = true;
          break;
        default:
          caret().handleKeyStroke(&event, screen_buffer());
          break;
        }

        break;
      default:
        break;
      }
    }
    if (caret().getY() > maxy) {
      caret().getY() = maxy;
      caret().getUpperLeftCornerLine()++;
    }
    caret().checkCaretPosition(screen_buffer());
    int yPos = caret().getLogicalY();
    int xPos = caret().getLogicalX();
    switch (drawMode & 0xFF00) {
    case 0x0100:
      screen_buffer().getCharacter(yPos, xPos) = drawMode & 0xFF;
      break;
    case 0x0200:
      screen_buffer().getAttribute(yPos, xPos) = drawMode & 0xFF;
      break;
    case 0x0300:
      screen_buffer().getAttribute(yPos, xPos) = drawMode & 0xFF | screen_buffer().getAttribute(yPos, xPos) & 240;
      break;
    case 0x0400:
      screen_buffer().getAttribute(yPos, xPos) = drawMode & 0xFF | screen_buffer().getAttribute(yPos, xPos) & 15;
      break;
    }
    draw_main().drawStatusLine();
    if (FullScreen) {
      draw_main().drawScreen(1, 24);
    }
    else {
      draw_main().drawScreen(1, 23);
    }
    draw_main().updateColorStatus(Attribute);
  } while (!done);
}
int sgn(int a)
{
  if (a > 0) {
    return 1;
  }
  if (a < 0) {
    return -1;
  }
  return 0;
}

void DrawLineCommand::run()
{
  int a = 0, b = 0, c = 0, d = 0, maxy = 22;
  if (FullScreen) maxy++;
  maxy += LINES - 25;
  draw_main().ClearMessageLine();
  ansout << gotoxy(0, LINES - 1);
  CoolWrite("Draw line, use cursorkeys, press <ESC> to quit");
  SDL_Event event;
  bool done = false;
  int oldCaretX = caret().getLogicalX();
  int oldCaretY = caret().getLogicalY();
  do {
    if (FullScreen) {
      ansout << gotoxy(caret().getX(), caret().getY());
    }
    else {
      ansout << gotoxy(caret().getX(), caret().getY() + 1);
    }

    oldCaretX = caret().getLogicalX();
    oldCaretY = caret().getLogicalY();
    do {
      screenEngine.showScreen();
      SDL_Delay(50);
      while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
          done = true;
          break;
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
          case SDLK_ESCAPE:
            done = true;
            break;
          default:
            caret().handleKeyStroke(&event, screen_buffer());
            break;
          }

          break;
        default:
          break;
        }
      }
      b = sgn(caret().getLogicalX() - oldCaretX);
      a = sgn(caret().getLogicalY() - oldCaretY);
    } while (b == 0 && a == 0);
    if (caret().getY() > maxy) {
      caret().getY() = maxy;
      caret().getUpperLeftCornerLine()++;
    }

    caret().checkCaretPosition(screen_buffer());

    switch (a) {
    case 1:
    case -1:
      screen_buffer().getCharacter(caret().getLogicalY(), caret().getLogicalX()) = CharSet[ActiveCharset][5];
      screen_buffer().getAttribute(caret().getLogicalY(), caret().getLogicalX()) = Attribute;
      break;
    }
    switch (b) {
    case 1:
    case -1:
      screen_buffer().getCharacter(caret().getLogicalY(), caret().getLogicalX()) = CharSet[ActiveCharset][4];
      screen_buffer().getAttribute(caret().getLogicalY(), caret().getLogicalX()) = Attribute;
      break;
    }
    if ((c == 1)&(b == -1))
      screen_buffer().getCharacter(oldCaretY, oldCaretX) = CharSet[ActiveCharset][3];
    if ((c == 1)&(b == 1))
      screen_buffer().getCharacter(oldCaretY, oldCaretX) = CharSet[ActiveCharset][2];
    if ((c == -1)&(b == -1))
      screen_buffer().getCharacter(oldCaretY, oldCaretX) = CharSet[ActiveCharset][1];
    if ((c == -1)&(b == 1))
      screen_buffer().getCharacter(oldCaretY, oldCaretX) = CharSet[ActiveCharset][0];
    if ((a == 1)&(d == -1))
      screen_buffer().getCharacter(oldCaretY, oldCaretX) = CharSet[ActiveCharset][0];
    if ((a == 1)&(d == 1))
      screen_buffer().getCharacter(oldCaretY, oldCaretX) = CharSet[ActiveCharset][1];
    if ((a == -1)&(d == -1))
      screen_buffer().getCharacter(oldCaretY, oldCaretX) = CharSet[ActiveCharset][2];
    if ((a == -1)&(d == 1))
      screen_buffer().getCharacter(oldCaretY, oldCaretX) = CharSet[ActiveCharset][3];
    c = a;
    d = b;
    a = 0;
    b = 0;
    draw_main().drawStatusLine();
    if (FullScreen) {
      draw_main().drawScreen(1, 24);
    }
    else {
      draw_main().drawScreen(1, 23);
    }
    draw_main().updateColorStatus(Attribute);


  } while (!done);
}

