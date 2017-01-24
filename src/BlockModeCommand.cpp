#include "BlockModeCommand.hpp"
#include "mdraw.h"

void BlockModeCommand::CopyBlock(char Mode)
{
  bool under = false;
  unsigned char c, d;
  int x, y, ymax = 22;
  if (FullScreen) ymax++;
  ymax += (LINES - 25);
  //SaveScreen();
  Undo = true;
  draw_main().ClearMessageLine();
  ansout << gotoxy(0, LINES - 1);
  CodeWrite("[S]tamp [P]age [U]nder [X]/[Y] Flip [RETURN] [ESC]");

  ScreenBlock screenBlock = screen_buffer().getBlock(X1, Y1, X2, Y2);

  if (Mode == 2) {
    screen_buffer().clear(X1, Y1, X2, Y2);
  }

  SDL_Event event;
  bool done = false;
  do {
    if (FullScreen)  ansout << gotoxy(0, 0); else  ansout << gotoxy(0, 1);
    for (y = 0; y <= ymax; y++) {
      for (x = 0; x <= 79; x++) {
        ansout << textattr(screen_buffer().getAttribute(y + caret().getUpperLeftCornerLine(), x + caret().getUpperLeftCornerRow()));
        c = screen_buffer().getCharacter(y + caret().getUpperLeftCornerLine(), x + caret().getUpperLeftCornerRow());
        if (c >= ' ') {
          ansout << c;
        }
        else {
          ansout << ' ';
        }
      }
      if (COLS > 80) {
        ansout << endl;
      }
    }

    for (y = Y1; y <= Y2; y++) {
      if (caret().getY() + (y - Y1) < ymax + 1) {
        if (FullScreen) {
          ansout << gotoxy(caret().getX(), caret().getY() + (y - Y1));
        }
        else {
          ansout << gotoxy(caret().getX(), caret().getY() + (y - Y1) + 1);
        }
        for (x = X1; x <= X2; x++) {
          if (caret().getX() + (x - X1) < 80)
          {
            ansout << textattr(screenBlock.getAttribute(y - Y1, x - X1));
            c = screenBlock.getCharacter(y - Y1, x - X1);
            if (under) {
              d = screen_buffer().getCharacter(caret().getLogicalY() + (y - Y1), caret().getLogicalX() + (x - X1));
              if ((d == 32) | (d == 0)) {
                if (c >= ' ') {
                  ansout << c;
                }
                else {
                  ansout << ' ';
                }
              }
              else {
                ansout << gotoxy(caret().getX() + (x - X1) + 1, caret().getY() + (y - Y1) + 1);
              }
            }
            else {
              if (c >= ' ') {
                ansout << c;
              }
              else {
                ansout << ' ';
              }
            }
          }
        }
      }
      if (COLS > 80) {
        ansout << gotoxy(0, caret().getY() + (y - Y1));
      }
    }

    draw_main().drawStatusLine();
    draw_main().updateColorStatus(Attribute);
    if (FullScreen) {
      ansout << gotoxy(caret().getX(), caret().getY());
    }
    else {
      ansout << gotoxy(caret().getX(), caret().getY() + 1);
    }
#ifdef HAS_GPM
    if (MouseSupport) {
      if (mouse_getbutton() == MOUSE_LEFTBUTTON) ch = 13;
      if (mouse_getbutton() == MOUSE_RIGHTBUTTON) ch = 's';
    }
#endif

    screenEngine.showScreen();
    SDL_Delay(10);
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
          done = true;
          break;
        case SDLK_RETURN:
          screen_buffer().stampBlock(screenBlock, caret().getLogicalX(), caret().getLogicalY(), under);
          done = true;
          break;
        default:
          caret().handleKeyStroke(&event, screen_buffer());
          switch (event.key.keysym.unicode) {
          case 'y':
            screenBlock.flipY();
            break;
          case 'x':
            screenBlock.flipX();
            break;
          case 'u':
            under = !under;
            break;
          case 'p':
            // TOOD: SET ACTIVE PAGE
            //if (++ActivePage>2) ActivePage=1;
            break;
          case 's':
            screen_buffer().stampBlock(screenBlock, caret().getLogicalX(), caret().getLogicalY(), under);
            break;
          }
          break;
        }

        break;
      }
    }

    if (caret().getY() > ymax) {
      caret().getY() = ymax;
      caret().getUpperLeftCornerLine()++;
    }
    caret().checkCaretPosition(screen_buffer());
  } while (!done);

#ifdef HAS_GPM
  if (MouseSupport) {
    do {
      mouse_update();
    } while (mouse_getbutton() == MOUSE_LEFTBUTTON ||
      mouse_getbutton() == MOUSE_RIGHTBUTTON);
  }
#endif
}

void BlockModeCommand::run()
{
  int x1, y1, ch;
  int x, y, maxy = 22;
  if (FullScreen) maxy++;
  maxy += LINES - 25;
  draw_main().ClearMessageLine();
  x1 = caret().getLogicalX();
  y1 = caret().getLogicalY();

  SDL_Event event;
  bool done = false;

  do {
    if (FullScreen) {
      ansout << gotoxy(0, 0);
    }
    else {
      ansout << gotoxy(0, 1);
    }

    if (caret().getLogicalY() > y1) {
      Y1 = y1;
      Y2 = caret().getLogicalY();
    }
    else {
      Y2 = y1;
      Y1 = caret().getLogicalY();
    }

    if (caret().getLogicalX() > x1) {
      X1 = x1;
      X2 = caret().getLogicalX();
    }
    else {
      X2 = x1;
      X1 = caret().getLogicalX();
    }

    for (y = 0; y <= maxy; y++) {
      for (x = 0; x <= 79; x++) {
        if ((x + caret().getUpperLeftCornerRow() >= X1)&(x + caret().getUpperLeftCornerRow() <= X2)&(y + caret().getUpperLeftCornerLine() >= Y1)&(y + caret().getUpperLeftCornerLine() <= Y2)) {
          ansout << textattr(112);
        }
        else {
          ansout << textattr(screen_buffer().getAttribute(y + caret().getUpperLeftCornerLine(), x));
        }

        unsigned char c = screen_buffer().getCharacter(y + caret().getUpperLeftCornerLine(), x + caret().getUpperLeftCornerRow());
        if (c >= ' ') {
          ansout << c;
        }
        else {
          ansout << ' ';
        }
      }
      if (COLS > 80) {
        ansout << endl;
      }
    }

    draw_main().drawStatusLine();
    draw_main().updateColorStatus(Attribute);
    ansout << gotoxy(0, LINES - 1);
    CodeWrite("[C]opy [M]ove [F]ill [E]rase [D]elete [O]utline [T]ext [ESC]");

    if (FullScreen) {
      ansout << gotoxy(caret().getX(), caret().getY());
    }
    else {
      ansout << gotoxy(caret().getX(), caret().getY() + 1);
    }
    screenEngine.showScreen();
    SDL_Delay(10);
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
          done = true;
          break;
        default:
          caret().handleKeyStroke(&event, screen_buffer());
          switch (event.key.keysym.unicode) {
          case 'm':
            CopyBlock(2);
            done = true;
            break;
          case 'c':
            CopyBlock(1);
            done = true;
            break;
          case 'e': // ERASE BLOCK 
            screen_buffer().clear(X1, Y1, X2, Y2);
            done = true;
            break;
          case 'd': // DELETE BLOCK 
            screen_buffer().clear(X1, Y1, X2, Y2);
            for (y = Y1; y <= Y2; y++) {
              for (x = X2; x < 80; x++) {
                screen_buffer().getCharacter(y, x - (X2 - X1)) = screen_buffer().getCharacter(y, x);
                screen_buffer().getAttribute(y, x - (X2 - X1)) = screen_buffer().getAttribute(y, x);
              }
              for (x = 79 - (X2 - X1); x <= 79; ++x) {
                screen_buffer().getCharacter(y, x) = ' ';
                screen_buffer().getAttribute(y, x) = 7;
              }
            }
            done = true;
            break;
          case 'o': // OUTLINE
            for (y = Y1; y <= Y2; ++y) {
              screen_buffer().getCharacter(y, X1) = CharSet[ActiveCharset][5];
              screen_buffer().getAttribute(y, X1) = Attribute;
              screen_buffer().getCharacter(y, X2) = CharSet[ActiveCharset][5];
              screen_buffer().getAttribute(y, X2) = Attribute;
            }
            for (x = X1; x <= X2; x++) {
              screen_buffer().getCharacter(Y1, x) = CharSet[ActiveCharset][4];
              screen_buffer().getAttribute(Y1, x) = Attribute;
              screen_buffer().getCharacter(Y2, x) = CharSet[ActiveCharset][4];
              screen_buffer().getAttribute(Y2, x) = Attribute;
            }
            screen_buffer().getCharacter(Y1, X1) = CharSet[ActiveCharset][0];
            screen_buffer().getCharacter(Y1, X2) = CharSet[ActiveCharset][1];
            screen_buffer().getCharacter(Y2, X1) = CharSet[ActiveCharset][2];
            screen_buffer().getCharacter(Y2, X2) = CharSet[ActiveCharset][3];
            done = true;
            break;
          case 't': // TEXT
            draw_main().ClearMessageLine();
            ansout << gotoxy(0, LINES - 1);
            switch (chooser(0, 1, "Left", "Center", "Right", "Elite", "eFfect", "Abort", 0)) {
            case 1:
              screen_buffer().leftTrim(X1, Y1, X2, Y2);
              done = true;
              break;
            case 2:
              screen_buffer().center(X1, Y1, X2, Y2);
              done = true;
              break;
            case 3:
              screen_buffer().rightTrim(X1, Y1, X2, Y2);
              done = true;
              break;
            case 4:
              screen_buffer().transformElite(X1, Y1, X2, Y2);
              done = true;
              break;
            case 5:
              screen_buffer().drawEffect(X1, Y1, X2, Y2, effect.Effekt, effect.getColorTable());
              done = true;
              break;
            }
            break;
          case 'f': // FiLL 
            draw_main().ClearMessageLine();
            ansout << gotoxy(0, LINES - 1);
            switch (chooser(0, 1, "Character", "Attribute", "Fore", "Back", "Abort", 0)) {
            case 1:
              ch = draw_main().readCharacter();
              screen_buffer().fillCharacter(ch, X1, Y1, X2, Y2);
              done = true;
              break;
            case 2:
              screen_buffer().fillAttribute(Attribute, X1, Y1, X2, Y2);
              done = true;
              break;
            case 3:
              screen_buffer().fillForeColor(Attribute & 15, X1, Y1, X2, Y2);
              done = true;
              break;
            case 4:
              screen_buffer().fillBackColor(Attribute & 240, X1, Y1, X2, Y2);
              done = true;
              break;
            }
            break;
          }
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
  } while (!done);
}
