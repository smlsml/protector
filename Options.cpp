#include <vcl\vcl.h>

#pragma hdrstop

#include "Options.h"
#include "Invader.h"

#pragma resource "*.dfm"

TFoptions *Foptions;

__fastcall TFoptions::TFoptions(TComponent *OOwner):TForm(OOwner)
{}

void __fastcall TFoptions::Preset(TObject *Sender)
{
   Mus->Checked = Finvade->music;
   Snd->Checked = Finvade->sound;
   Hard->Position = Finvade->diff;
   Bads->Position = Finvade->numbad / 8;
   Stars->Position = Finvade->numstars;
   Speed->Position = Finvade->gamespeed;
}

void __fastcall TFoptions::okbutClick(TObject *Sender)
{
   Finvade->music = Mus->Checked;
   Finvade->sound = Snd->Checked;
   Finvade->diff = Hard->Position;
   Finvade->numbad = 8 * Bads->Position;
   Finvade->numstars = Stars->Position;
   Finvade->gamespeed = Speed->Position;

   if(Finvade->diff < 0 || Finvade->diff > 5) Finvade->diff = 0;
   if(Finvade->numstars < 0 || Finvade->numstars > 6) Finvade->numstars = 3;
   if(Finvade->gamespeed < 1 || Finvade->gamespeed > 500) Finvade->gamespeed = 1;
   if(Finvade->numbad != 8 && Finvade->numbad != 16 && Finvade->numbad != 24) Finvade->numbad = 16;
}


