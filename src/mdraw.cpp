/*
 * Mystic Draw 1.6 : A nice ANSI Drawing tool
 * Copyright(C) 1996-2003 by Mike Krueger (mike@icsharpcode.net)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#include <memory>
#include <string>
#include <vector>
#include <cerrno>
#include "sauce.hpp"
#include "mdraw.h"

using namespace std;

unsigned char tabs[80];
unsigned char CursorPos[80], cpos=1;

unsigned char Attribute=7;
bool Undo=false, FontTyped=false;
unsigned char ActiveCharset = 1;
bool FullScreen = false;

char* CharSet[12]={
     "",
     "⁄ø¿Ÿƒ≥√¥¡¬",
     "…ª»ºÕ∫Ãπ À",
     "’∏‘æÕ≥∆µœ—",
     "≈Œÿ◊üÈõúôÔ",
     "∞±≤€ﬂ‹›ﬁ˛˙",
     "˜˘≠®ëíûÄá",
     "ìîï¢ßñÅó£ö",
     "ÆØÚÛ©™˝ˆ´¨",
     "„ÒÙıÍù‰¯˚¸",
     "‡·‚ÂÊÁÎÏÌÓ",
     "àâäÇêåãç°¶"
};

char EliteTable[256];
teffekt effect;


void CopyScreen(int p1, int p2)
{
  // !!! TODO !!!
  /*
  int x;
  for (x=0;x<MAX_LINES;x++)
  memcpy(Screen[p2][x],Screen[p1][x],160);*/
}

void SaveScreen() {
  // !!! TODO !!!
  /*
  CopyScreen(ActivePage,UNDOPage);
  memcpy(&Screen[UNDOPage][0][0],
  &Screen[ActivePage][0][0],MAX_LINES*160);*/
}

void about()
{
  SDL_Event event;
  DrawBox(29, 10, 61, 14);
  ansout << gotoxy(30, 11);
  CoolWrite("coded 1996 by Mike Krueger     ");
  ansout << gotoxy(30, 12);
  CoolWrite("ansis made by Col. Blair^TUSCON");
  ansout << gotoxy(30, 13);
  CoolWrite("Version 1.6 (GPL)              ");

  do {
    screenEngine.showScreen();
    SDL_Delay(50);

    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_KEYDOWN:
        return;
      }
    }
  } while (true);
}

static void ClearScreen(MysticDrawMain& m, ScreenBuffer* screen)
{
  int x;
  m.ClearMessageLine();
  ansout << gotoxy(0, LINES - 1);
  CoolWrite("Clear Screen :");
  x = chooser(15, 1, "Yes", "No", 0);
  Undo = false;
  if (x == 1) {
    screen->clear();
  }
  SaveScreen();
}

static void global(MysticDrawMain& m, ScreenBuffer* screen) {
  int ch;
  m.ClearMessageLine();
  ansout << gotoxy(0, LINES - 1);
  CoolWrite("Global :");
  int x = chooser(10, 1, "Fill", "Copy", "Text", "Abort", 0);
  switch (x) {
  case 1:
    m.ClearMessageLine();
    ansout << gotoxy(0, LINES - 1);
    CoolWrite("Fill :");
    x = chooser(8, 1, "Character", "aTribute", "Fore", "Back", "Abort", 0);
    switch (x) {
    case 1:
      ch = m.readCharacter();
      screen->fillCharacter(ch);
      break;
    case 2:
      screen->fillAttribute(Attribute);
      break;
    case 3:
      screen->fillForeColor(Attribute & 15);

      break;
    case 4:
      screen->fillBackColor(Attribute & 240);
      break;
    }
    break;
  case 2:
    m.ClearMessageLine();
    ansout << gotoxy(0, LINES - 1);
    CoolWrite("Copy to page :");
    switch (chooser(15, 1, "1", "2", "Abort", 0)) {
    case 1:
      // TODO !!!
      //CopyScreen(ActivePage,1);
      break;
    case 2:
      // TODO !!!
      //CopyScreen(ActivePage,2);
      break;
    };
    break;
  case 3:
    m.ClearMessageLine();
    ansout << gotoxy(0, LINES - 1);
    CoolWrite("Text :");
    switch (chooser(8, 1, "Left", "Right", "Center", "Elite", "eFfect", "Abort", 0)) {
    case 1:
      screen->leftTrim();
      break;
    case 2:
      screen->rightTrim();
      break;
    case 3:
      screen->center();
      break;
    case 4:
      screen->transformElite();
      break;
    case 5:
      screen->drawEffect(effect.Effekt, effect.getColorTable());
      break;
    }
  }
}

void SetPage(MysticDrawMain& m)
{
  m.ClearMessageLine();
  ansout << gotoxy(0, LINES - 1);
  CoolWrite("Set Page :");
  int i = chooser(12, m.getCurrentBufferNumber() + 1, "1", "2", 0);
  if (i >= 1 && i <= 2) {
    m.getCurrentBufferNumber() = i - 1;
  }
}

void UndoLast(MysticDrawMain& m) {
  if (Undo) {
    m.ClearMessageLine();
    ansout << gotoxy(0, LINES - 1);
    CoolWrite("Undo :");
    // TODO !!!
    //if (chooser(7, ActivePage, "Yes", "No", 0) == 1)  {
    //	CopyScreen(UNDOPage,ActivePage);
    //}
  }
}


unsigned char oldColor = 7;
char *AvatarColor(int col) {
  char *tmp;
  tmp = (char *)malloc(4);
  if (col == oldColor) return NULL;
  sprintf(tmp, "%c", col);
  oldColor = col;
  return tmp;
}
/*
char *strupr(char *str) {
  unsigned int x;
  for (x = 0; x <= strlen(str); x++)
    str[x] = toupper(str[x]);
  return str;
}
*/

char *PCBoardColor(int col) {
  char *a;
  if (col == oldColor) return NULL;
  a = (char *)malloc(4);
  sprintf(a, "@X%02x", col);
  a = strupr(a);
  oldColor = col;
  return a;
}

char *AnsiColor(unsigned char col) {
  char *a;
  if (col == oldColor) return NULL;
  a = (char *)malloc(30);
  *a = 0;
  if ((oldColor >= 128) && (col<128)) {
    sprintf(a, "[0m");
    oldColor = 7;
    if (col == 7) return a;
  }
  sprintf(a, "%s[", a);
  if (((col & 8) != 8)&((oldColor & 8) == 8)) {
    sprintf(a, "%s0;", a);
    oldColor = oldColor & 15;
  }
  if (((col & 8) == 8)&((oldColor & 8) != 8)) sprintf(a, "%s1;", a);

  if ((col & 128) == 128) {
    sprintf(a, "%s5", a);
    if ((col & 15) != (oldColor & 15) || (col & 112) != (oldColor & 112))
      sprintf(a, "%s;", a);
  }

  if ((col & 15) != (oldColor & 15))
    switch (col & 7) {
    case 0:
      sprintf(a, "%s30", a);
      break;
    case 1:
      sprintf(a, "%s34", a);
      break;
    case 2:
      sprintf(a, "%s32", a);
      break;
    case 3:
      sprintf(a, "%s36", a);
      break;
    case 4:
      sprintf(a, "%s31", a);
      break;
    case 5:
      sprintf(a, "%s35", a);
      break;
    case 6:
      sprintf(a, "%s33", a);
      break;
    case 7:
      sprintf(a, "%s37", a);
      break;
    }
  if (((col & 15)) != (oldColor & 15) && (col & 112) != (oldColor & 112))
    sprintf(a, "%s;", a);
  if ((col & 112) != (oldColor & 112))
    switch ((col & 112) >> 4) {
    case 0:
      sprintf(a, "%s40", a);
      break;
    case 1:
      sprintf(a, "%s44", a);
      break;
    case 2:
      sprintf(a, "%s42", a);
      break;
    case 3:
      sprintf(a, "%s46", a);
      break;
    case 4:
      sprintf(a, "%s41", a);
      break;
    case 5:
      sprintf(a, "%s45", a);
      break;
    case 6:
      sprintf(a, "%s43", a);
      break;
    case 7:
      sprintf(a, "%s47", a);
      break;
    }
  sprintf(a, "%sm", a);
  oldColor = col;
  return a;
}

static int SelectSaveMode(MysticDrawMain& m)
{
  m.ClearMessageLine();
  return chooser(16, 1, "Clearscreen", "Home", "None", 0);
}

static char *EnterName(MysticDrawMain& m, char *b) {
  char *a;
  char *ext;
  m.ClearMessageLine();
  ansout << gotoxy(0, LINES - 1);
  CoolWrite("Enter Filename :");
  a = "";
  a = inputfield(a, 60, 16, LINES - 1);
  if (strlen(a) == 0) a = strdup("NONAME");
  ext = strchr(a, '.');
  if (ext == NULL) a = strcat(a, b);
  return a;
}

static int CharCount(ScreenBuffer* screen, int d, int e, int a, int chr) {
  int c = 0;
  for (int b = d; b <= e; b++)
    if ((screen->getCharacter(a, b) == chr)&(screen->getAttribute(a, b) == screen->getAttribute(a, d))) c++; else break;
  return c;
}

static int Numberofchars(ScreenBuffer* screen, int a) {
  int c = 0;
  for (int b = 0; b <= 79; b++)
    if (((screen->getCharacter(a, b) != ' ') & (screen->getAttribute(a, b) != 0)
      ) |
      ((screen->getAttribute(a, b) & 112) != 0)) c = b;
  return c;
}

void save(MysticDrawMain& m, ScreenBuffer* screen) {
  char *Name, *s;
  FILE *fp;
  int x, y, z, chnum;
  oldColor = 0;
  ansout << gotoxy(0, LINES - 1);
  x = chooser(0, 1, "aNsi", "aVatar", "Pcboard", "aScii", "Binary", "C", "XBin", "Abort", 0);
  int lastLine = screen->getLastNonEmptyLine();
  switch (x) {
  case 1: /*ANSI Save*/
    Name = EnterName(m, ".ans");
    fp = fopen(Name, "wb");
    switch (SelectSaveMode(m)) {
    case 1:
      fprintf(fp, "[2J");
      break;
    case 2:
      fprintf(fp, "[1;1H");
      break;
    }
    for (y = 0; y <= lastLine; y++) {
      /*	if (y>0) if (Numberofchars(screen, y-1)>=79) fprintf(fp,"[A");*/
      chnum = Numberofchars(screen, y);
      for (x = 0; x <= chnum; x++) {
        z = CharCount(screen, x, chnum, y, ' ');
        if ((z>2)&((screen->getAttribute(y, x) & 112) == 0)) {
          fprintf(fp, "[%dC", z);
          x += z - 1;
        }
        else {
          if (screen->getCharacter(y, x) == 0) screen->getCharacter(y, x) = 32;
          s = AnsiColor(screen->getAttribute(y, x));
          if (s != NULL) fprintf(fp, "%s", s);
          fputc(screen->getCharacter(y, x), fp);
        }
      }
      fputc(13, fp);
      fputc(10, fp);
    }
    fprintf(fp, "[0m");
    if (screen->doSaveSauce()) {
      screen->getSauce().DataType = 1;
      screen->getSauce().FileType = 1;
      screen->getSauce().TInfo1 = 80;
      screen->getSauce().TInfo2 = lastLine;
      screen->getSauce().AppendSauce(fp);
    }
    fclose(fp);
    break;
  case 2: /*AVATAR Save*/
    Name = EnterName(m, ".avt");
    fp = fopen(Name, "wb");
    switch (SelectSaveMode(m)) {
    case 1:
      fputc('', fp);
      break;
    case 2:
      fprintf(fp, "");
      break;
    }
    for (y = 0; y <= lastLine; y++) {
      chnum = Numberofchars(screen, y);
      for (x = 0; x <= chnum; x++) {
        z = CharCount(screen, x, chnum, y, screen->getCharacter(y, x));
        s = AvatarColor(screen->getAttribute(y, x));
        if (s != NULL) fprintf(fp, "%s", s);
        if ((z>2)&((screen->getAttribute(y, x) & 112) == 0)) {
          fprintf(fp, "%c%c", screen->getCharacter(y, x), z);
          x += z - 1;
        }
        else {
          if (screen->getAttribute(y, x) == 0) fputc(0, fp);
          if (screen->getCharacter(y, x) == 0) screen->getCharacter(y, x) = 32;
          fputc(screen->getCharacter(y, x), fp);
        }
      }
      fputc(13, fp);
      fputc(10, fp);
    }
    fprintf(fp, "");
    if (screen->doSaveSauce()) {
      screen->getSauce().DataType = 1;
      screen->getSauce().FileType = 5;
      screen->getSauce().AppendSauce(fp);
    }
    fclose(fp);
    break;
  case 3:
    Name = EnterName(m, ".pcb");
    fp = fopen(Name, "wb");
    switch (SelectSaveMode(m)) {
    case 1:
      fprintf(fp, "@CLS@");
      break;
    case 2:
      fprintf(fp, "@HOME@");
      break;
    }
    for (y = 0; y <= lastLine; y++) {
      for (x = 0; x <= Numberofchars(screen, y); x++) {
        if (screen->getCharacter(y, x) == 0) screen->getCharacter(y, x) = 32;
        s = PCBoardColor(screen->getAttribute(y, x));
        if (s != NULL) fprintf(fp, "%s", s);
        fputc(screen->getCharacter(y, x), fp);
      }
      fputc(13, fp);
      fputc(10, fp);
    }
    if (screen->doSaveSauce()) {
      screen->getSauce().DataType = 1;
      screen->getSauce().FileType = 4;
      screen->getSauce().TInfo1 = 80;
      screen->getSauce().TInfo2 = lastLine;
      screen->getSauce().AppendSauce(fp);
    }
    fclose(fp);
    break;
  case 4:
    Name = EnterName(m, ".asc");
    fp = fopen(Name, "wb");
    for (y = 0; y <= lastLine; y++) {
      for (x = 0; x <= Numberofchars(screen, y); x++) {
        if (screen->getCharacter(y, x) == 0) screen->getCharacter(y, x) = 32;
        fputc(screen->getCharacter(y, x), fp);
      }
      fputc(13, fp);
      fputc(10, fp);
    }
    if (screen->doSaveSauce()) {
      screen->getSauce().DataType = 1;
      screen->getSauce().FileType = 0;
      screen->getSauce().TInfo1 = 80;
      screen->getSauce().TInfo2 = lastLine;
      screen->getSauce().AppendSauce(fp);
    }
    fclose(fp);
    break;
  case 5:
    Name = EnterName(m, ".bin");
    fp = fopen(Name, "wb");
    for (y = 0; y <= lastLine; y++) {
      for (x = 0; x<screen->getWidth(); x++) {
        fputc(screen->getCharacter(y, x), fp);
        fputc(screen->getAttribute(y, x), fp);
      }
    }
    fclose(fp);
    break;
  case 6:
    Name = EnterName(m, " ");
    fp = fopen(Name, "wb");
    fprintf(fp, "unsigned char %s[%d]={\n", Name, (lastLine + 1) * 160);
    for (y = 0; y <= lastLine; y++) {
      for (x = 0; x<screen->getWidth(); x++) {
        fprintf(fp, "%d,%d", screen->getCharacter(y, x),
          screen->getAttribute(y, x));
        if (x + 1 <screen->getWidth()) fputc(',', fp);
      }
      if (y<lastLine) fputc(',', fp); else fprintf(fp, "};");
      fputc(13, fp);
      fputc(10, fp);
    }
    fclose(fp);
    break;
  case 7:
    Name = EnterName(m, " ");
    screen->save(Name, XBinary);
    break;
  }
}

unsigned char LoadAnsi[4096] = {
  177,42,223,2,32,2,220,8,223,8,223,8,223,8,223,8,223,8,223,8,220,
  8,32,8,32,8,32,40,220,42,32,42,32,42,32,42,219,2,223,2,32,
  2,220,8,223,8,223,8,223,8,223,8,223,8,254,8,220,8,223,8,223,
  8,223,8,223,8,223,8,223,8,223,8,223,8,223,8,223,8,223,8,223,
  8,223,8,223,8,254,8,223,8,223,8,223,8,223,8,223,8,223,8,223,
  8,223,8,220,8,32,8,32,8,32,8,223,8,220,8,220,8,223,8,223,
  8,223,8,223,8,223,8,223,8,223,8,223,8,223,8,223,8,223,8,223,
  8,223,8,223,8,223,8,220,8,32,8,32,40,176,42,177,42,254,42,176,
  42,32,8,219,8,32,8,220,2,219,2,219,2,219,2,219,2,220,2,32,
  2,219,8,32,8,32,8,32,40,176,42,177,42,32,42,223,2,32,2,219,
  8,32,8,220,2,219,2,219,2,219,2,220,2,32,2,254,8,220,2,219,
  2,219,2,219,2,219,2,219,2,219,2,219,2,219,2,219,2,219,2,219,
  2,219,2,220,2,32,2,219,2,219,2,219,2,219,2,219,2,219,2,219,
  2,220,2,32,2,219,8,32,8,32,8,32,8,222,8,219,8,32,8,68,
  15,105,7,83,8,80,8,76,8,65,8,89,8,255,8,70,15,105,7,76,
  8,69,8,83,8,32,8,219,8,221,8,32,8,32,40,178,42,177,42,176,
  42,32,8,219,8,32,8,219,2,219,2,219,2,176,42,177,42,219,2,220,
  2,32,2,219,8,32,8,32,40,177,42,176,42,223,2,32,2,219,8,32,
  8,220,2,219,2,176,42,177,42,254,2,219,2,220,2,222,2,219,2,219,
  2,219,2,219,114,176,42,177,42,176,42,219,114,176,42,177,42,219,114,254,
  2,219,114,219,2,221,2,219,2,219,2,219,2,176,42,177,42,176,42,219,
  114,219,2,220,2,32,2,219,8,32,8,32,8,222,8,219,7,32,7,87,
  15,105,7,84,8,72,8,255,8,84,15,72,8,105,7,83,8,255,8,32,
  8,32,8,32,8,222,8,219,7,221,8,32,8,32,40,176,42,178,42,176,
  42,32,8,223,8,220,8,222,2,219,2,219,2,177,42,219,114,176,42,219,
  2,32,2,219,8,32,8,32,40,176,42,223,2,32,2,219,8,32,8,220,
  2,219,2,176,42,177,42,219,2,177,42,219,114,219,2,220,2,223,2,219,
  2,219,114,176,42,177,42,219,114,223,2,223,2,219,114,219,114,176,42,219,
  114,219,2,223,2,220,2,219,2,176,42,177,42,219,2,177,42,219,114,177,
  42,219,114,219,2,220,2,32,2,254,8,32,8,32,8,254,8,32,8,69,
  15,88,8,84,8,69,8,78,8,83,8,105,7,79,8,78,8,83,8,32,
  8,32,8,32,8,32,8,254,8,32,8,32,8,32,40,176,42,178,42,176,
  42,32,42,32,8,254,8,32,8,219,2,219,2,177,42,177,42,254,2,219,
  2,32,2,219,8,32,8,32,8,223,2,32,2,219,8,32,8,220,2,219,
  2,254,2,219,114,219,2,223,2,219,2,177,42,176,42,219,2,220,2,222,
  2,219,2,176,42,219,114,219,114,32,8,32,8,219,114,219,2,177,42,219,
  2,221,2,32,2,219,2,219,2,219,114,177,42,219,2,222,2,176,42,177,
  42,176,42,219,2,219,2,32,2,219,8,32,8,32,8,254,8,32,8,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,
  8,32,8,32,8,32,8,219,8,32,8,32,8,32,40,177,42,177,42,176,
  42,32,8,220,8,223,8,222,2,219,2,176,42,177,42,219,114,176,42,219,
  2,32,2,254,8,223,8,223,8,223,8,223,8,254,8,32,8,219,2,219,
  2,219,114,177,42,219,2,220,2,219,2,177,42,176,42,219,114,219,2,222,
  2,219,2,176,42,177,42,219,114,220,2,220,2,219,2,177,42,219,114,219,
  2,221,2,219,2,219,2,219,2,219,2,177,42,219,2,32,2,219,2,177,
  42,219,114,219,114,219,2,221,2,222,8,221,8,32,8,254,8,32,8,91,
  8,120,15,93,8,32,8,42,7,46,7,65,15,78,7,83,8,32,8,32,
  8,32,8,32,8,254,8,254,8,32,8,32,8,32,40,176,42,177,42,32,
  8,220,8,223,8,32,8,219,2,219,2,177,42,219,114,177,42,219,2,220,
  2,219,2,219,2,219,2,219,2,219,2,219,2,220,2,32,2,223,2,219,
  2,219,114,176,42,177,42,219,2,177,42,219,114,176,42,219,2,219,2,222,
  2,219,2,219,114,177,42,219,114,219,2,219,2,219,114,178,42,177,42,219,
  2,221,2,223,2,219,2,219,2,219,114,178,42,219,2,32,2,32,2,177,
  42,176,42,219,114,219,2,221,2,222,8,221,8,32,8,250,8,32,8,91,
  8,120,15,93,8,32,8,42,7,46,7,65,15,86,7,84,8,32,8,32,
  8,32,8,254,8,254,8,254,8,32,8,32,40,254,42,176,42,178,42,220,
  8,223,8,32,8,219,2,219,2,176,42,219,114,176,42,176,42,177,42,177,
  42,176,42,176,42,177,42,177,42,176,42,219,2,219,114,219,2,220,2,223,
  2,219,2,219,114,176,42,177,42,219,114,254,2,219,2,219,2,32,2,222,
  2,219,2,176,42,177,42,219,2,32,2,32,2,219,2,177,42,219,114,219,
  2,221,2,32,2,219,2,219,2,176,42,178,42,177,42,220,2,219,114,176,
  42,254,2,219,2,219,2,32,2,219,8,32,8,32,8,254,8,32,8,91,
  8,120,15,93,8,32,8,42,7,46,7,66,15,73,7,78,8,32,8,32,
  8,254,8,254,8,254,8,32,8,32,40,176,42,176,42,177,42,178,42,219,
  8,32,8,219,2,219,2,254,2,176,42,177,42,219,114,219,114,219,2,219,
  2,219,2,219,2,219,2,219,114,32,34,177,42,254,2,219,114,219,2,32,
  2,219,2,219,114,219,114,176,42,219,114,219,114,219,2,32,2,32,2,219,
  114,219,114,254,2,219,2,219,2,32,2,32,2,219,2,219,2,254,2,176,
  42,219,2,32,2,223,2,219,2,219,2,176,42,219,114,254,2,177,42,219,
  114,219,2,219,2,32,2,220,8,223,8,32,8,32,8,254,8,32,8,91,
  8,120,15,93,8,32,8,42,7,46,7,80,15,67,7,66,8,32,8,254,
  8,254,8,223,8,32,8,32,40,177,42,178,42,177,42,178,42,178,42,219,
  8,32,8,223,2,219,2,219,2,219,2,219,2,223,2,32,2,220,8,220,
  8,220,8,220,8,220,8,32,8,223,2,219,2,219,2,219,2,223,2,254,
  8,32,8,223,2,219,2,219,2,219,2,223,2,32,2,254,8,223,2,219,
  2,219,114,219,114,219,2,223,2,254,8,223,2,219,2,219,2,219,2,219,
  114,219,114,223,2,32,2,219,2,219,2,219,2,219,2,219,2,219,2,219,
  2,223,2,32,2,220,8,223,8,32,8,32,8,32,8,219,8,32,8,91,
  8,32,8,93,8,32,8,42,7,46,7,88,15,88,7,88,8,32,8,254,
  8,32,8,32,8,32,40,177,42,176,42,177,42,219,42,178,42,219,42,220,
  2,223,8,220,8,220,8,220,8,220,8,220,8,220,8,223,8,32,8,32,
  8,32,8,32,8,32,8,223,8,220,8,220,8,220,8,220,8,220,8,223,
  8,254,8,220,8,220,8,220,8,220,8,220,8,254,8,223,8,220,8,220,
  8,220,8,220,8,220,8,220,8,223,8,220,8,220,8,220,8,220,8,220,
  8,220,8,254,8,254,8,220,8,220,8,220,8,220,8,220,8,220,8,220,
  8,220,8,223,8,32,8,32,8,220,8,220,8,223,8,32,8,223,8,220,
  8,220,8,220,8,220,8,220,8,220,8,220,8,220,8,220,8,220,8,223,
  8,32,8,32,40,176,42,254,42,178,42,219,42,178,42,219,42,254,42,254,
  8,223,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,220,
  8,254,8,223,8,223,8,223,8,223,8,223,8,223,8,223,8,223,8,223,
  8,223,8,223,8,223,8,254,8,220,8,254,8,223,8,223,8,223,8,223,
  8,223,8,223,8,223,8,223,8,223,8,223,8,223,8,223,8,223,8,223,
  8,223,8,223,8,223,8,223,8,91,8,84,7,105,7,84,7,76,7,69,
  7,93,8,223,8,254,8,220,8,254,8,223,8,223,8,223,8,223,8,223,
  8,223,8,91,8,80,7,65,7,105,7,78,7,84,7,69,7,82,7,93,
  8,223,8,254,8,220,8,254,8,223,8,223,8,223,8,223,8,223,8,91,
  8,71,7,82,7,79,7,85,7,80,7,93,8,223,8,254,8,254,8,219,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,219,8,219,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,219,8,219,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,219,8,219,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,219,8,219,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,219,8,219,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,219,8,219,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,219,8,219,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,219,8,219,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,7,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,219,8,223,
  8,254,8,220,8,220,8,220,8,220,8,220,8,220,8,220,8,220,8,220,
  8,220,8,220,8,220,8,254,8,223,8,254,8,220,8,220,8,220,8,220,
  8,220,8,220,8,220,8,220,8,220,8,220,8,220,8,220,8,254,8,220,
  8,220,8,220,8,220,8,220,8,220,8,220,8,220,8,220,8,220,8,220,
  8,220,8,220,8,254,8,223,8,254,8,220,8,220,8,220,8,220,8,220,
  8,220,8,220,8,220,8,220,8,220,8,220,8,220,8,220,8,220,8,220,
  8,220,8,254,8,223,8,254,8,220,8,220,8,220,8,220,8,220,8,220,
  8,220,8,220,8,220,8,220,8,220,8,220,8,220,8,254,8,254,8,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,
  8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,8,32,
  7,32,7,32,7,0,7,0,7,0,7,0,7,0,7,0,7,223,8,32,
  8,223,8,32,8,223,8,32,8,32,8,32,8,223,8,223,8,0,7,0,
  7,0,7,0,7,0,7,0,7,0,7,0,7,0,7,0,7,0,7,0,
  7,0,7,0,7,0,7,0,7,0,7,0,7,0,7,0,7,0,7,0,
  7,0,7,0,7,0,7,0,7,0,7,0,7,0,7 };

struct FileDescriptor
{
  string name;
  bool   isDirectory;

  // sauce information
  string title;
  string artist;
  string group;
};

static void load() {
  // TODO(rushfan): Fix this *** BROKEN ***
#if 0
  vector<FileDescriptor> files;
  int readnew = 1;
  FILE *fp;
  struct dirent *direntp;
  Sauce sauce;
  DIR *dirp;

  ansout << gotoxy(0, 0);
  for (int x = 0; x <= 1950; x++) {
    if (LoadAnsi[x << 1] == 0) {
      LoadAnsi[x << 1] = ' ';
    }
    ansout << textattr(LoadAnsi[(x << 1) + 1]) << LoadAnsi[x << 1];
    if (COLS>80 && (x + 1) % 80 == 0) {
      ansout << endl;
    }
  }
  ansout << setfill(' ');
  SDL_Event event;
  bool done = false;
  unsigned int z = 0;
  unsigned int y = 0;
  do {
    if (readnew == 1) {
      z = y = 0;
      readnew = 0;
      files.clear();
      dirp = opendir(".");
      direntp = readdir(dirp);
      if (dirp != NULL) {
        for (;;) {
          direntp = readdir(dirp);
          if (direntp == NULL) break;
          FileDescriptor newDescriptor;
          newDescriptor.name = string(direntp->d_name);
          errno = 0;

          fp = fopen(direntp->d_name, "rb");
          bool sauceRead = sauce.ReadSauce(fp);
          if (errno == EISDIR || fp == NULL) {
            newDescriptor.isDirectory = true;
            files.push_back(newDescriptor);
          }
          else {
            newDescriptor.isDirectory = false;
            if (sauceRead) {
              newDescriptor.title = string((const char*)sauce.Title);
              newDescriptor.artist = string((const char*)sauce.Author);
              newDescriptor.group = string((const char*)sauce.Group);
            }
            fclose(fp);
            files.push_back(newDescriptor);
          }
        }
      }
      closedir(dirp);
    }
    for (unsigned int x = 0; x <= 8; x++) {
      if (x + z < files.size()) {
        ansout << gotoxy(2, 13 + x);
        ansout << textattr(15 + (y == x ? 16 : 0));
        if (files[x + z].isDirectory) {
          ansout << textattr(7 + (y == x ? 16 : 0));
        }
        ansout << setw(12) << files[x + z].name;
        ansout << gotoxy(14, 13 + x) << ' ';
        if (!files[x + z].isDirectory) {
          ansout << textattr(7 + (y == x ? 16 : 0));
          ansout << (char)250;
          ansout << textattr(15 + (y == x ? 16 : 0));
          ansout << " " << setw(27) << files[x + z].title;
          ansout << textattr(7 + (y == x ? 16 : 0));
          ansout << (char)250;
          ansout << textattr(14 + (y == x ? 16 : 0));
          ansout << " " << setw(17) << files[x + z].artist;
          ansout << textattr(7 + (y == x ? 16 : 0));
          ansout << (char)250;
          ansout << textattr(15 + (y == x ? 16 : 0));
          ansout << setw(15) << files[x + z].group;
        }
        else {
          ansout << textattr(8 + (y == x ? 16 : 0));
          ansout << " <DiRECTORY>                                                    ";
        }
      }
      else {
        ansout << gotoxy(2, 13 + x) << textattr(7) << "                                                                             ";
      }
    }
    screenEngine.showScreen();
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
          done = true;
          break;
        case SDLK_RETURN:
          if (files[y + z].isDirectory) {
            chdir(files[y + z].name.c_str());
            readnew = 1;
          }
          else {
            screen->load((char*)files[y + z].name.c_str());
            done = true;
          }
          break;
        case SDLK_PAGEUP:
          y = 0;
          if (z < 8) {
            z = 0;
          }
          else {
            z -= 8;
          }
          break;
        case SDLK_PAGEDOWN:
          y = 8;
          z += 8;
          if (z + 9 > files.size()) z = files.size() - 9;
          if (y + 1 > files.size()) {
            y = files.size() - 1;
            z = 0;
          }
          break;
        case SDLK_UP:
          if (y >= 1) {
            y--;
          }
          else if (z > 0) {
            z--;
          }
          break;
        case SDLK_DOWN:
          y++;
          if (y>8) {
            y = 8;
            if (z + 9 < files.size()) z++;
          }
          if (y + 1 > files.size())  y = files.size() - 1;
          break;
        default:
          break;
        }
      }
    }
  } while (!done);
#endif
}


unsigned char ActiveMenue = 1, MaxItem;
char * MenueItem[20], Length;
int MouseX, MouseY;

static int Menues(Caret& caret, int x, int y) {
  int a, b, c, d;
  DrawBox(x, y, x + Length + 1, y + MaxItem + 1);
  b = 1;
  c = 255;
  SDL_Event event;
  bool done = false;
  do {
    if (c != b) {
      for (a = 1; a <= MaxItem; a++) {
        ansout << gotoxy(x + 1, y + a);
        if (b == a) ansout << textattr(32 + 15); else ansout << textattr(10);
        for (d = 1; d <= 2; d++) {
          ansout << MenueItem[a][d];
        }
        if (b == a) ansout << textattr(32 + 15); else ansout << textattr(2);
        for (d = 3; d<Length - 6; d++) {
          ansout << MenueItem[a][d];
        }
        if (b == a) ansout << textattr(32 + 8); else ansout << textattr(7);
        for (d = Length - 6; d <= Length; d++) {
          ansout << MenueItem[a][d];
        }
      }
    }
    c = b;
#ifdef HAS_GPM
    if (MouseSupport == TRUE)
      Gpm_DrawPointer(mouse_x + 1, mouse_y + 1, 1);
#endif
    if (FullScreen) {
      ansout << gotoxy(caret.getX(), caret.getY() + 0);
    }
    else {
      ansout << gotoxy(caret.getX(), caret.getY() + 1);
    }

#ifdef HAS_GPM
    if (MouseSupport == TRUE) {
      mouse_update();
      MouseX = mouse_getx();
      MouseY = mouse_gety();
      if (MouseX >= x&&MouseX <= x + Length + 1 && MouseY>y&&MouseY<y + MaxItem + 1) {
        b = MouseY - y;
        if (mouse_getbutton() == MOUSE_LEFTBUTTON) ch = 13;
      }
    }
#endif
    screenEngine.showScreen();
    SDL_Delay(50);
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        exit(0);
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
          done = true;
          break;
        case SDLK_RETURN:
          return b;
        case SDLK_UP:
          b--;
          break;
        case SDLK_DOWN:
          b++;
          break;
        case SDLK_LEFT:
          return 253;
        case SDLK_RIGHT:
          return 254;
        default:
          break;
        }
      }
    }
    if (b<1) b = 1;
    if (b>MaxItem) b = MaxItem;

#ifdef HAS_GPM
    if (MouseSupport == TRUE) {
      if (MouseX>79)
        MouseX = 79;
      if (MouseY>25)
        MouseY = 25;
      if (mouse_getbutton() == MOUSE_RIGHTBUTTON) ch = 27;
      if (mouse_getbutton() == MOUSE_LEFTBUTTON&&MouseY == 1) {
        if (MouseX >= 2 && MouseX <= 6) {
          ActiveMenue = 1;
          return 252;
        }
        if (MouseX >= 13 && MouseX <= 17) {
          ActiveMenue = 2;
          return 252;
        }
        if (MouseX >= 24 && MouseX <= 30) {
          ActiveMenue = 3;
          return 252;
        }
        if (MouseX >= 37 && MouseX <= 42) {
          ActiveMenue = 4;
          return 252;
        }
        if (MouseX >= 49 && MouseX <= 53) {
          ActiveMenue = 5;
          return 252;
        }
        if (MouseX >= 62 && MouseX <= 68) {
          ActiveMenue = 6;
          return 252;
        }
        if (MouseX >= 73 && MouseX <= 76) {
          ActiveMenue = 7;
          return 252;
        }
      }
    }
#endif
  } while (!done);
#ifdef HAS_GPM
  if (MouseSupport == TRUE)
    do {
      mouse_update();
    } while (mouse_getbutton() == MOUSE_RIGHTBUTTON);
#endif
    return 255;
}

int menue(MysticDrawMain& m, Caret& caret)
{
  int x, a, b;
  b = 0;
  ActiveMenue = 1;
#ifdef HAS_GPM
  if (MouseSupport == TRUE)
    do {
      mouse_update();
    } while (mouse_getbutton() == MOUSE_RIGHTBUTTON);
#endif
    do {
      a = 0;
      if (ActiveMenue != b) {
        if (FullScreen) {
          m.drawScreen(1, 25);
        }
        else {
          m.drawScreen(1, 24);
        }
        ansout << gotoxy(0, 0) << textattr(8);
        ansout << (char)223;
        ansout << textattr(7);
        ansout << ' ';
        ansout << textattr(8);
        for (x = 1; x<79; x++) {
          ansout << (char)220;
        }
        ansout << textattr(7);
        ansout << gotoxy(79, 0);
        ansout << (char)223;
        ansout << gotoxy(0, 1);
        ansout << textattr(2);
        ansout << (char)223;
        if (ActiveMenue == 1) ansout << textattr(8 + 32); else ansout << textattr(15 + 32);
        ansout << " FILES      ";
        if (ActiveMenue == 2) ansout << textattr(8 + 32); else ansout << textattr(15 + 32);
        ansout << "FONTS      ";
        if (ActiveMenue == 3) ansout << textattr(8 + 32); else ansout << textattr(15 + 32);
        ansout << "OPTIONS      ";
        if (ActiveMenue == 4) ansout << textattr(8 + 32); else ansout << textattr(15 + 32);
        ansout << "SCREEN      ";
        if (ActiveMenue == 5) ansout << textattr(8 + 32); else ansout << textattr(15 + 32);
        ansout << "MISC.        ";
        if (ActiveMenue == 6) ansout << textattr(8 + 32); else ansout << textattr(15 + 32);
        ansout << "TOGGLES    ";
        if (ActiveMenue == 7) ansout << textattr(8 + 32); else ansout << textattr(15 + 32);
        ansout << "HELP";
        ansout << textattr(2);
        ansout << (char)219 << (char)220 << (char)223;
      }
      b = ActiveMenue;
      switch (ActiveMenue) {
      case 1:
        MenueItem[1] = " LOAD       ALT+L  ";
        MenueItem[2] = " SAVE       ALT+S  ";
        MenueItem[3] = " QUIT       ALT+X  ";
        Length = 18;
        MaxItem = 3;
        a = Menues(caret, 0, 2);
        break;
      case 2:
        MenueItem[1] = " SELECT FONT    ALT+F  ";
        MenueItem[2] = " FONT MODE      ALT+N  ";
        MenueItem[3] = " OUTLINE TYPE   ALT+W  ";
        Length = 22;
        MaxItem = 3;
        a = Menues(caret, 12, 2);
        break;
      case 3:
        MenueItem[1] = " SAUCE SETUP   CTRL+S ";
        MenueItem[2] = " SET PAGE      ALT+P  ";
        MenueItem[3] = " TAB SETUP     ALT+T  ";
        MenueItem[4] = " GLOBAL        ALT+G  ";
        MenueItem[5] = " SET EFFECT    ALT+M  ";
        Length = 20;
        MaxItem = 5;
        a = Menues(caret, 25, 2);
        break;
      case 4:
        MenueItem[1] = " CLEAR PAGE    ALT+C  ";
        MenueItem[2] = " INSERT LINE   ALT+I  ";
        MenueItem[3] = " DELTE LINE    ALT+Y  ";
        MenueItem[4] = " INSERT COLUMN ALT+1  ";
        MenueItem[5] = " DELTE COLUMN  ALT+2  ";
        MenueItem[6] = " UNDO/RESTORE  ALT+R  ";
        Length = 21;
        MaxItem = 6;
        a = Menues(caret, 38, 2);
        break;
      case 5:
        MenueItem[1] = " SET COLORS    ALT+A  ";
        MenueItem[2] = " PICK UP COLOR ALT+U  ";
        MenueItem[3] = " ASCII TABLE   ALT+K  ";
        Length = 21;
        MaxItem = 3;
        a = Menues(caret, 50, 2);
        break;
      case 6:
        MenueItem[1] = " LINE DRAW        ALT+D  ";
        MenueItem[2] = " DRAW MODE        ALT+-  ";
        MenueItem[3] = " INSERT MODE      INS    ";
        MenueItem[4] = " VIEW IN 320x200  ALT+V  ";
        MenueItem[5] = " ELITE MODE       ALT+E  ";
        Length = 24;
        MaxItem = 5;
        a = Menues(caret, 53, 2);
        break;
      case 7:
        MenueItem[1] = " HELP         ALT+H  ";
        MenueItem[2] = " ABOUT               ";
        Length = 20;
        MaxItem = 2;
        a = Menues(caret, 56, 2);
        break;
      };
      switch (a) {
      case 253:
        ActiveMenue--;
        break;
      case 254:
        ActiveMenue++;
        break;
      case 255:
        return 0;
        break;
      };
      if (ActiveMenue<1) ActiveMenue = 7;
      if (ActiveMenue>7) ActiveMenue = 1;
    } while (a>200);
#ifdef HAS_GPM
    if (MouseSupport == TRUE)
      do {
        mouse_update();
      } while (mouse_getbutton() == MOUSE_RIGHTBUTTON);
#endif
      return a + (ActiveMenue << 8);
}

void menuemode(MysticDrawMain& m) {
  auto screen = m.getCurrentBuffer();
  auto caret = m.getCaret();
  HelpCommand           helpCommand(&m);
  ASCIITableCommand     asciiTableCommand(&m);
  TabulatorSetupCommand tabulatorSetupCommand(&m);
  SelectFontCommand     selectFontCommand(&m);
  SelectOutlineCommand  selectOutlineCommand(&m);
  SelectSauceCommand    selectSauceCommand(&m);
  DrawCommand           drawCommand(&m);
  DrawLineCommand       drawLineCommand(&m);
  SelectEffectModeCommand   selectEffectCommand(&m);
  SelectColorCommand selectColorCommand(&m);

  unsigned int a = menue(m, caret);

  switch ((a & 0xFF00) >> 8) {
  case 1:
    switch (a & 0xFF) {
    case 1:
      load();
      break;
    case 2:
      save(m, screen);
      break;
    case 3:
      m.exitMysticDraw();
      break;
    }
    break;
  case 2:
    switch (a & 0xFF) {
    case 1:
      selectFontCommand.run();
      break;
    case 2:
      caret.fontMode() = !caret.fontMode();
      Undo = false;
      SaveScreen();
      break;
    case 3:
      selectOutlineCommand.run();
      break;
    }
    break;
  case 3:
    switch (a & 0xFF) {
    case 1:
      selectSauceCommand.run();
      break;
    case 2:
      SetPage(m);
      break;
    case 3:
      tabulatorSetupCommand.run();
      break;
    case 4:
      global(m, screen);
      break;
    case 5:
      selectEffectCommand.run();
      break;
    }
    break;
  case 4:
    switch (a & 0xFF) {
    case 1:
      ClearScreen(m, screen);
      break;
    case 2:
      screen->insertLine(caret.getLogicalY());
      break;
    case 3:
      screen->removeLine(caret.getLogicalY());
      break;
    case 4:
      screen->insertColumn(caret.getX());
      break;
    case 5:
      screen->removeColumn(caret.getX());
      break;
    case 6:
      UndoLast(m);
      break;
    }
    break;
  case 5:
    switch (a & 0xFF) {
    case 1:
      selectColorCommand.run();
      break;
    case 2:
      Attribute = screen->getAttribute(caret.getLogicalY(), caret.getX());
      break;
    case 3:
      asciiTableCommand.run();
      break;
    }
    break;
  case 6:
    switch (a & 0xFF) {
    case 1:
      drawLineCommand.run();
      break;
    case 2:
      drawCommand.run();
      break;
    case 3:
      caret.insertMode() = !caret.insertMode();
      break;
#ifdef HAS_SVGALIB
    case 4:
      viewmode();
      break;
#endif
    case 5:
      caret.eliteMode() = !caret.eliteMode();
      break;
    }
  case 7:
    switch (a & 0xFF) {
    case 1:
      helpCommand.run();
      break;
    case 2:
      about();
      break;
    }
    break;
  }
}

void MysticDrawMain::drawStatusLine()
{
	if (FullScreen) {
		return;
	}
	ansout << textattr(2) << gotoxy(0, 0);
	for (int i = 0; i < 80; ++i) {
		ansout << ' ';
	}
	ansout << gotoxy(0, 0);
	ansout << '(' << setw(3) << setfill('0') << caret.getLogicalX() + 1;
	ansout << ", "<< setw(4) << setfill('0') << caret.getLogicalY() + 1 << ')';
	
	ansout << gotoxy(18, 0) << textattr(8);
	if (caret.eliteMode()) { 
		ansout << 'e';
	} else {
		ansout << ' ';
	}
	ansout << gotoxy(20, 0) << textattr(14);
	if (caret.insertMode()) {
		ansout << "Ins";
	} else {
		ansout << "   ";
	}
	ansout << gotoxy(24, 0) << textattr(2);
	ansout << "Page:" << currentBuffer << textattr(9);
	ansout << gotoxy(32, 0);
	ansout << "Set: " << (int)ActiveCharset;
	ansout << gotoxy(39, 0);
	for (int i = 1; i <= 10; ++i) {
		unsigned char b = CharSet[ActiveCharset][i - 1];
		ansout << textattr(64) << ' ' << i << "=" << textattr(64 + 15) << b;
	}
	ansout << textattr(7) << ' ';
}

void MysticDrawMain::updateColorStatus(unsigned char col)
{
	if (FullScreen) {
		return;
	}
	ansout << gotoxy(11, 0) << textattr(col) << " Color ";
}


/*
 * Version        4 Byte
 * Color          1 Byte
 * TABPos        80 Byte
 * Outline        1 Byte
 * EffectStruct   1 teffekt
*/
void MysticDrawMain::loadconfig()
{
	FILE* fp = fopen(getConfigurationFileName().c_str(), "rb");
	if (fp != NULL) {      
		int ver = 0;
		fread(&ver,4,1,fp);
		if (ver >= 1) { 	 
			fread(&Attribute,1,1,fp);
			for (int i = 1; i <= 80; ++i) {
				fread(&tabs[i],1,1,fp);
			}
			fread(&FontLibrary::getInstance().currentOutline,1,1,fp);
			fread(&effect,sizeof(effect),1,fp);
		}
		if (ver == 2) {
			fread(&caret.insertMode(),1,1,fp);
			fread(&caret.eliteMode(),1,1,fp);
			fread(&FullScreen,1,1,fp);
		}
		fclose(fp);
  } else {
		cout << "Error opening configuration file check that " << getConfigurationFileName() << " exists" << endl;
	}
}

void MysticDrawMain::saveconfig()
{
	int ver=2;
	FILE* fp = fopen(getConfigurationFileName().c_str(), "wb");
	if (fp != NULL) {
		fwrite(&ver,4,1,fp);
		fwrite(&Attribute,1,1,fp);
		for (int i=1;i<=80;i++) {
			fwrite(&tabs[i],1,1,fp);
		}
		fwrite(&FontLibrary::getInstance().currentOutline,1,1,fp);
		fwrite(&effect,sizeof(effect),1,fp);
		fwrite(&caret.insertMode(),1,1,fp);
		fwrite(&caret.eliteMode(),1,1,fp);
		fwrite(&FullScreen,1,1,fp);
		fclose(fp);
	} else {
		cout << "Error while writing to the configuration file " << getConfigurationFileName() << endl;
		cout << "Check that the path exists and that you've write permssion to it" << endl;
	}
}

void MysticDrawMain::startMysticDraw(int argnum, char* args[])
{
	this->args = args;
	int a,b;
	loadconfig();

#if WIN32
	char* homeDir = "";
	char* relFontDir  = "allfont.fnt";
#else
	char* homeDir = getenv("HOME");
	char* relFontDir  = "/.mdraw/allfont.fnt";
#endif	

	char* fontLibrary = (char*)malloc(strlen(homeDir) + strlen(relFontDir) + 1);
	sprintf(fontLibrary, "%s%s", homeDir, relFontDir);
	cout << fontLibrary << endl;
	if (!FontLibrary::getInstance().readFontLibrary(fontLibrary)) {
		cout << "Error loading font library see INSTALL" << endl; 
	}
	free(fontLibrary);
	
	if (argnum > 1) {
		cout << "Loading file " << args[1] << endl;
		getCurrentBuffer()->load(args[1]);
	}
	init_bio_crt();
	SDL_EnableUNICODE(1);
	
#ifdef HAS_GPM
	if (mouse_init()!=0) { 
		MouseSupport = false;
	}
#endif
	screenEngine.LoadFont(args);
	
  auto buf = this->getCurrentBuffer();
  auto& c = this->getCaret();
  HelpCommand           helpCommand(this);
  ASCIITableCommand     asciiTableCommand(this);
  TabulatorSetupCommand tabulatorSetupCommand(this);
  SelectFontCommand     selectFontCommand(this);
  SelectOutlineCommand  selectOutlineCommand(this);
  SelectSauceCommand    selectSauceCommand(this);
  DrawCommand           drawCommand(this);
  DrawLineCommand       drawLineCommand(this);
  SelectEffectModeCommand   selectEffectCommand(this);
  SelectColorCommand selectColorCommand(this);
  BlockModeCommand     blockModeCommand(this, buf, &c);
  FontEditorCommand    fontEditorCommand(this);
  PaletteEditorCommand paletteEditorCommand(this);
  ViewModeCommand      viewModeCommand(this);
  SDL_Event event;
	
	done = false;
	do {
		drawStatusLine();
		if (FullScreen) {
			drawScreen(1, 25);
		} else {
			drawScreen(1, 24);
		}
		updateColorStatus(Attribute);
		if (FullScreen) {
			ansout << gotoxy(caret.getX(), caret.getY() + 0);
			screenEngine.getCaretY() = caret.getY();
		} else {
			ansout << gotoxy(caret.getX(), caret.getY() + 1);
			screenEngine.getCaretY() = caret.getY() + 1;
		}
		/* *** */
#ifdef HAS_GPM
		if (MouseSupport)  {
			if (mouse_getbutton()==MOUSE_RIGHTBUTTON) a=27;
			if (mouse_getbutton()==MOUSE_LEFTBUTTON) blockModeCommand.run();
		}
#endif
		screenEngine.getCaretX() = caret.getX();
		
		screenEngine.showScreen();
		SDL_Delay(10);
		while (SDL_PollEvent(&event)){
			switch (event.type){
				case SDL_QUIT:
					done = true;
					break;
				case SDL_KEYDOWN:
					caret.handleKeyStroke(&event, *buf);
					if (event.key.keysym.mod & KMOD_CTRL) {
						switch (event.key.keysym.sym) {
							case 's':
								selectSauceCommand.run();
								break;
							case 'f':
								fontEditorCommand.run();
								break;
							case 'p':
								paletteEditorCommand.run();
								break;
							default:
								break;
						}
					
					} else if (event.key.keysym.mod & KMOD_ALT) {
						switch (event.key.keysym.sym) {
							case 'z':
								Attribute ^= 128;
								break;
							case 'k':
								asciiTableCommand.run();
								break;
							case '1':
								getCurrentBuffer()->removeColumn(caret.getLogicalX());
								break;
							case '2':
								getCurrentBuffer()->insertColumn(caret.getLogicalX());
								break;
							case 'v':
								viewModeCommand.run();
								break;
							case 'r': /* ALT+R UNDO*/
								UndoLast(*this);
								break;
							case 'g':
								global(*this, buf);
								break;
							case 'u': /* ALT+U PiCKUP COLOR*/
								Attribute = getCurrentBuffer()->getAttribute(caret.getLogicalY(), caret.getLogicalX());
								break;
							case 'w':
								selectOutlineCommand.run();
								break;
							case 'i':/* ALT+I InsLine*/
								getCurrentBuffer()->insertLine(caret.getLogicalY());
								break;
							case 'y': /* ALT+Y - DelLine */
								getCurrentBuffer()->removeLine(caret.getLogicalY());
								break;
							case 'e': /* ALT+E - Elite*/
								caret.eliteMode() = !caret.eliteMode();
								break;
							case 'x': /* ALT+X - Exit */
								exitMysticDraw();
								break;
							case 'a': /* ALT+A - Color */
								selectColorCommand.run();
								break;
							case 'n': /* ALT+N - Font Mode */
								caret.fontMode() = !caret.fontMode();
								Undo=0;
								SaveScreen();
								break;
							case 's':
								save(*this, buf);
								break;
							case 'c': /* ALT+C - ClearScreen*/
								ClearScreen(*this, buf);
								break;
							case 'l':
								load();
								break;
							case 'p': /* ALT+P - SetPage */
								SetPage(*this);
								break;
							case 't':
								tabulatorSetupCommand.run();
								break;
							case 'h': /* alt+h help */
								helpCommand.run();
								break;
							case '-': /* alt+- draw mode */
								drawCommand.run();
								break;
							case 9: /* ALT+TAB */
								// TODO
								//caret.getLogicalX() = getPrevTab(caret.getLogicalX());
								break;
							case 'b':
								blockModeCommand.run();
								break;
							case 'd': /* ALT+D Draw Line Mode */
								drawLineCommand.run();
								break;
							case 'm':
								selectEffectCommand.run();
								break; 
							case 'f': /* ALT+F - Select Font */
								selectFontCommand.run();
								break;
							default:
								break;
						}
					} else {
						switch (event.key.keysym.sym) {
							case SDLK_F1:
								typeCharacter(CharSet[ActiveCharset][0]);
								break;
							case SDLK_F2:
								typeCharacter(CharSet[ActiveCharset][1]);
								break;
							case SDLK_F3:
								typeCharacter(CharSet[ActiveCharset][2]);
								break;
							case SDLK_F4:
								typeCharacter(CharSet[ActiveCharset][3]);
								break;
							case SDLK_F5:
								typeCharacter(CharSet[ActiveCharset][4]);
								break;
							case SDLK_F6:
								typeCharacter(CharSet[ActiveCharset][5]);
								break;
							case SDLK_F7:
								typeCharacter(CharSet[ActiveCharset][6]);
								break;
							case SDLK_F8:
								typeCharacter(CharSet[ActiveCharset][7]);
								break;
							case SDLK_F9:
								typeCharacter(CharSet[ActiveCharset][8]);
								break;
							case SDLK_F10:
								typeCharacter(CharSet[ActiveCharset][9]);
								break;
							case SDLK_PRINT:
								FullScreen = !FullScreen;
								break;
							case SDLK_ESCAPE:
								menuemode(*this);
								break;
							case SDLK_TAB:
								// TODO:
								//caret.getLogicalX() = getNextTab(caret.getLogicalX();
								break;
							case SDLK_RETURN:
								caret.getX()                  = 0;
								caret.getUpperLeftCornerRow() = 0;
								
								if (caret.fontMode() && FontTyped)  {
									caret.getY() += FontLibrary::getInstance().maxY;
									Undo=false;
									SaveScreen();
									FontTyped=false;
								} else {
									caret.getY()++;
								}
								cpos=0;
								break;
							case SDLK_DELETE:
								for (int i = caret.getLogicalX(); i < getCurrentBuffer()->getWidth(); ++i) {
									getCurrentBuffer()->getCharacter(caret.getLogicalY(), i) = getCurrentBuffer()->getCharacter(caret.getLogicalY(), i + 1);
									getCurrentBuffer()->getAttribute(caret.getLogicalY(), i) = getCurrentBuffer()->getAttribute(caret.getLogicalY(), i + 1);
								}
								getCurrentBuffer()->getCharacter(caret.getLogicalY(), getCurrentBuffer()->getWidth() - 1) = ' ';
								getCurrentBuffer()->getAttribute(caret.getLogicalY(), getCurrentBuffer()->getWidth() - 1) = 7;
								break;
							case SDLK_INSERT:
								caret.insertMode() = !caret.insertMode();
								break;
							case SDLK_BACKSPACE:
								if (caret.getLogicalX()>0){
									if (caret.fontMode() && FontTyped && cpos > 0)  {
										caret.getX() -= CursorPos[cpos] - 1;
										for (a=0;a<=CursorPos[cpos];a++)
										for (b=0;b<=FontLibrary::getInstance().maxY;b++) {
											getCurrentBuffer()->getCharacter(caret.getLogicalY() + b, caret.getLogicalX()+a) = getUndoBuffer()->getCharacter(caret.getLogicalY() + b, caret.getLogicalX()+a);
											getCurrentBuffer()->getAttribute(caret.getLogicalY() + b, caret.getLogicalX()+a) = getUndoBuffer()->getAttribute(caret.getLogicalY() + b, caret.getLogicalX()+a);
										}
										cpos--;
									} else {	
										cpos=0;
										caret.getX()--;
										if (caret.insertMode()) {
											for (int i = caret.getLogicalX(); i < getCurrentBuffer()->getWidth(); ++i) {
												getCurrentBuffer()->getCharacter(caret.getLogicalY(), i) = getCurrentBuffer()->getCharacter(caret.getLogicalY(), i + 1);
												getCurrentBuffer()->getAttribute(caret.getLogicalY(), i) = getCurrentBuffer()->getAttribute(caret.getLogicalY(), i + 1);
											}
											getCurrentBuffer()->getCharacter(caret.getLogicalY(), getCurrentBuffer()->getWidth() - 1) = ' ';
											getCurrentBuffer()->getAttribute(caret.getLogicalY(), getCurrentBuffer()->getWidth() - 1) = 7;
										} else  {
											getCurrentBuffer()->getCharacter(caret.getLogicalY(), getCurrentBuffer()->getWidth() - 1) = ' ';
											getCurrentBuffer()->getAttribute(caret.getLogicalY(), getCurrentBuffer()->getWidth() - 1) = 7;
										} 
									}
								}
								break;
							default:
								a = event.key.keysym.unicode;
								if (caret.fontMode() && a > 32 && a < 127) {
									renderFontCharacter(a);
								} else  {
									if (caret.fontMode() && FontTyped) {
										cpos++;
										CursorPos[cpos]=2;
									}
									if (caret.eliteMode()) {
										typeCharacter(translate(a)); 
									} else {
										typeCharacter(a);
									}
								}
						}
					
					}
			}
		}
		caret.checkCaretPosition(*buf);
	} while (!done);
}

void MysticDrawMain::ClearMessageLine()
{
	ansout << gotoxy(0, LINES - 1) << textattr(7);
	for (int i = 0; i < 80; ++i) {
		ansout << ' ';
	}
}

void MysticDrawMain::renderFontCharacter(char c)
{
	FontLibrary* fl= &FontLibrary::getInstance();
	
	if (!fl->GetFontChar(c)) {
		return;
	}
	
	if (getCaret().getLogicalX() + fl->maxX + fl->getCurrentFont()->spaces > getCurrentBuffer()->getWidth() 
    || fl->maxX == 0
    || fl->maxY + getCaret().getLogicalY() > getCurrentBuffer()->getHeight())  {
		return;
	}
	
	FontTyped= true;
	
	if (fl->maxY + getCaret().getY() > LINES - 1) {
		getCaret().getUpperLeftCornerLine() += fl->maxY + getCaret().getY() - LINES + 2;
		getCaret().getY()                   -= fl->maxY + getCaret().getY() - LINES + 2;
	}
	
	for (int y = 0; y < fl->maxY; ++y) {
		for (int x = 0; x < fl->maxX; ++x) {
			switch (fl->getCurrentFont()->fontType) {
				case 0:
				case 1:
					getCurrentBuffer()->getCharacter(getCaret().getLogicalY() + y, getCaret().getLogicalX() + x) = fl->charTable[y][x];
					getCurrentBuffer()->getAttribute(getCaret().getLogicalY() + y, getCaret().getLogicalX() + x) = Attribute;
					break;
				case 2:
					getCurrentBuffer()->getCharacter(getCaret().getLogicalY() + y, getCaret().getLogicalX() + x) = fl->charTable[y][x * 2];
					getCurrentBuffer()->getAttribute(getCaret().getLogicalY() + y, getCaret().getLogicalX() + x) = fl->charTable[y][x * 2 + 1];
					break;
			}
		}
	}
	
	cpos++;
	CursorPos[cpos]    = fl->maxX + fl->getCurrentFont()->spaces;
	getCaret().getX() += fl->maxX + fl->getCurrentFont()->spaces - 1;
}

void MysticDrawMain::drawScreen(int startLine, int endLine)
{
	unsigned char oldColor = 7;
	cout << gotoxy(0, startLine);
	if (FullScreen) {
		cout << gotoxy(0, 0);
	} else {
		cout << gotoxy(0, startLine);
	}
	
	cout << textattr(oldColor);
	
	screenEngine.getExtendedFontMode() = getCurrentBuffer()->getExtendedCharMode();
	screenEngine.getBlinkMode()        = getCurrentBuffer()->getBlinkMode();
	if (getCurrentBuffer()->getFont() != 0) {
		screenEngine.setFont(getCurrentBuffer()->getFont());
	} else {
		screenEngine.setFont(screenEngine.getDefaultFont());
	}
	
	if (getCurrentBuffer()->getPalette() != 0) {
		screenEngine.setPalette(getCurrentBuffer()->getPalette());
	} else {
		screenEngine.setPalette(screenEngine.getDefaultPalette());
	}
	
	for (int y = startLine - 1; y <= endLine - startLine + (LINES - 25); ++y) {
		for (int x = 0; x <= 79; ++x) {
			unsigned char newColor = getCurrentBuffer()->getAttribute(y + getCaret().getUpperLeftCornerLine(), x + getCaret().getUpperLeftCornerRow()); 
			if (newColor != oldColor) {
				cout << textattr(newColor);
				oldColor = newColor;
			}
			unsigned char ch = getCurrentBuffer()->getCharacter(y + getCaret().getUpperLeftCornerLine(), x + getCaret().getUpperLeftCornerRow());
			if (ch >= ' ') {
				ansout << ch;
			} else { 
				ansout << ' ';
			}
		}
		if (COLS > 80) {
			ansout << endl;
		}
	}
}

void MysticDrawMain::typeCharacter(unsigned char ch)
{
	if (ch < ' ') {
		return;
	}
	
	if (getCaret().insertMode()) {
		for (int x = getCurrentBuffer()->getWidth() - 1; x >= getCaret().getLogicalX(); x--) {
			getCurrentBuffer()->getCharacter(getCaret().getLogicalY(), x) = getCurrentBuffer()->getCharacter(getCaret().getLogicalY(), x - 1);
			getCurrentBuffer()->getAttribute(getCaret().getLogicalY(), x) = getCurrentBuffer()->getAttribute(getCaret().getLogicalY(), x - 1);
		}
	}
	
	getCurrentBuffer()->getCharacter(getCaret().getLogicalY(), getCaret().getLogicalX()) = ch;
	getCurrentBuffer()->getAttribute(getCaret().getLogicalY(), getCaret().getLogicalX()) = Attribute;
	
	++getCaret().getX();
}

char MysticDrawMain::readCharacter()
{
	int ch = 512;
	SDL_Event event;
	this->ClearMessageLine();
	cout << gotoxy(0, LINES - 1);
	
	CoolWrite("Enter Character :");
	do {
		screenEngine.showScreen();
		SDL_Delay(50);
		
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_KEYDOWN:
					ch = event.key.keysym.unicode;
					// TODO : Fx keys
					break;
			}
		}
	} while (ch > 255);
	return ch;
}


void MysticDrawMain::exitMysticDraw()
{
	this->ClearMessageLine();
	cout << gotoxy(0, LINES - 1);
	CoolWrite("Sure ? ");
	switch(chooser(8, 2, "Yes", "No", 0)){
		case 1:
			// saveconfig();
#ifdef HAS_GPM
			if (MouseSupport) {
				mouse_close();
			}
#endif
			done = true;
	}   
}

// precompiled SDL1.2 not happy with msvc 2015.
// http://stackoverflow.com/questions/30412951
FILE _iob[] = { *stdin, *stdout, *stderr };

extern "C" FILE * __cdecl __iob_func(void)
{
  return _iob;
}

int main(int argnum,char *args[]) 
{
  {
    auto md = std::make_unique<MysticDrawMain>();
    md->startMysticDraw(argnum, args);
  }
  
  cout << "Thank you for using Mystic Draw" << endl;

  // Calling exit(0) was cashing a crash. Something static
  // is crashing in the destructor and I can't get a stack
  // trace on windows.
  _exit(0);
  return 0;
}
