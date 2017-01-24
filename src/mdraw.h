#ifndef MYSTICDRAWMAIN__HPP
#define MYSTICDRAWMAIN__HPP

#include <cctype>
#include <cstring>
#include <cstdlib>
#include <sys/types.h> 
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <iomanip>

#include "bio_crt.h"
#include "caret.h"

#include "screen_buffer.h"
#include "command.h"
#include "help_command.h"
#include "ascii_table_command.h"
#include "tabulator_command.h"
#include "select_font_command.h"
#include "select_sauce_command.h"
#include "select_effect_mode_command.h"
#include "select_color_command.h"
#include "font_editor_command.h"
#include "palette_editor_command.h"
#include "block_mode_command.h"
#include "draw_command.h"
#include "view_mode_command.h"
#include "sauce.h"
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
  unsigned char* getColorTable() {
    return colorTable[Effekt];
  }
};

extern teffekt effect;

class WWIVDrawMain {
private:
  int    currentBuffer;

  bool   done;
  ScreenBuffer** screen;

  Caret caret;

  const std::string getConfigurationFileName()
  {
#if WIN32
    return "mdraw.config";
#else
    return std::string(getenv("HOME")) + "/.mdraw/mdraw.config";
#endif		
  }

  void loadconfig();
  void saveconfig();

  void renderFontCharacter(char c);
public:
  char** args;

  WWIVDrawMain() {
    constexpr int maxScreens = 4;
    screen = new ScreenBuffer*[maxScreens];
    for (auto i = 0; i < maxScreens; ++i) {
      screen[i] = new ScreenBuffer();
    }
    currentBuffer = 0;
  }

  ScreenBuffer* getCurrentBuffer() { return screen[currentBuffer]; }
  ScreenBuffer* getUndoBuffer() { return screen[3]; }
  int& getCurrentBufferNumber() { return currentBuffer; }
  Caret& getCaret() { return caret; }

  void start(int argnum, char* args[]);
  void drawStatusLine();
  void updateColorStatus(unsigned char color);

  void ClearMessageLine();
  void drawScreen(int startLine, int endLine);
  void typeCharacter(unsigned char ch);
  char readCharacter();

  void exit();

  ~WWIVDrawMain() {
    delete[] screen;
  }
};


#endif
