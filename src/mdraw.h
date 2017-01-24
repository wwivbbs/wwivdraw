#ifndef MYSTICDRAWMAIN__HPP
#define MYSTICDRAWMAIN__HPP

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <iomanip>

#include "bio_crt.h"
#include "caret.h"

#include "ScreenBuffer.hpp"
#include "command.hpp"
#include "HelpCommand.hpp"
#include "ASCIITableCommand.hpp"
#include "TabulatorCommand.hpp"
#include "SelectFontCommand.hpp"
#include "SelectSauceCommand.hpp"
#include "SelectEffectModeCommand.hpp"
#include "SelectColorCommand.hpp"
#include "FontEditorCommand.hpp"
#include "PaletteEditorCommand.hpp"
#include "BlockModeCommand.hpp"
#include "DrawCommand.hpp"
#include "ViewModeCommand.hpp"
#include "sauce.hpp"
#include "fonts.h"

#define UNDOPage 0
#define COPYPage 3

extern char* CharSet[12];
extern unsigned char tabs[80];
extern unsigned char CursorPos[80];
extern unsigned char cpos;
extern unsigned char Attribute;
extern bool Undo, FontTyped;
extern unsigned char ActiveCharset;
extern bool FullScreen;


struct teffekt {
  int Effekt;
  unsigned char colorTable[5][10];
  unsigned char* getColorTable()
  {
    return colorTable[Effekt];
  }
};

extern teffekt effect;

using namespace std;
class MysticDrawMain
{
private:
  int    currentBuffer;

  bool   done;
  ScreenBuffer** screen;

  Caret caret;

  const string getConfigurationFileName()
  {
#if WIN32
    return "mdraw.config";
#else
    return string(getenv("HOME")) + "/.mdraw/mdraw.config";
#endif		
  }

  void loadconfig();
  void saveconfig();

  void renderFontCharacter(char c);
public:
  char** args;

  static MysticDrawMain& getInstance();

  MysticDrawMain()
  {
    const int maxScreens = 4;
    screen = new ScreenBuffer*[maxScreens];
    for (int i = 0; i < maxScreens; ++i) {
      screen[i] = new ScreenBuffer();
    }

    currentBuffer = 0;
  }

  ScreenBuffer* getCurrentBuffer()
  {
    return screen[currentBuffer];
  }

  ScreenBuffer* getUndoBuffer()
  {
    return screen[3];
  }

  int& getCurrentBufferNumber()
  {
    return currentBuffer;
  }

  Caret& getCaret()
  {
    return caret;
  }

  void startMysticDraw(int argnum, char* args[]);
  void drawStatusLine();
  void updateColorStatus(unsigned char color);

  void ClearMessageLine();
  void drawScreen(int startLine, int endLine);
  void typeCharacter(unsigned char ch);
  char readCharacter();

  void exitMysticDraw();

  ~MysticDrawMain()
  {
    delete[] screen;
  }
};


#endif
