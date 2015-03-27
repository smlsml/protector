// I N C L U D E S /////////////////////////////////////////////////////////////
#ifndef InvaderH
#define InvaderH

#include <vcl\Forms.hpp>
#include <vcl\Menus.hpp>
#include <vcl\MPlayer.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\ExtCtrls.hpp>

// C O N S T
const int G_H = 420;
const int G_W = 400;
const int G_BAD_MAX = 48;
const int G_SHOT_MAX = 3;
const int G_STAR_MAX = 96;

enum PT_state {STOP , PLAY , PAUSE , ANI};
enum PT_life {ALIVE , DEAD , DYING , OFF};

struct PT_point
{
   int x , y;
};

// C L A S S ///////////////////////////////////////////////////////////////////
class TFinvade: public TForm
{
__published:
   TTimer *Time;
   TImage *Welcome;
   TMediaPlayer *Midi;
        TMediaPlayer *Wav;
   TImageList *bad;
   TImageList *ship;
   TImageList *shoot;
   TImageList *flame;
   TImageList *boom;
   TImageList *weap;

        TMainMenu *mMenu;
        TMenuItem *mFile;
        TMenuItem *mHelp;
        TMenuItem *mAbout;
        TMenuItem *mHowto;
        TMenuItem *mL3;
        TMenuItem *mExit;
        TMenuItem *mL2;
        TMenuItem *mNew;
        TMenuItem *mPause;
        TMenuItem *mOptions;
        TMenuItem *mStop;
        TMenuItem *mScores;
        TMenuItem *mL1;
   void __fastcall Rep(TObject *Sender);
   void __fastcall Unp(TObject *Sender);
   void __fastcall Exit(TObject *Sender);
   void __fastcall Leak(TObject *Sender);
   void __fastcall Setup(TObject *Sender);
   void __fastcall WinLoop(TObject *Sender);
   void __fastcall LoseLoop(TObject *Sender);
   void __fastcall NewGame(TObject *Sender);
   void __fastcall StopGame(TObject *Sender);
   void __fastcall ShowAbout(TObject *Sender);
   void __fastcall NextFrame(TObject *Sender);
   void __fastcall ShowOptions(TObject *Sender);
   void __fastcall ReadyClose(TObject *Sender, bool &CanClose);
   void __fastcall UserHitKey(TObject *Sender, WORD &Key, TShiftState Shift);
   void __fastcall UserLetKey(TObject *Sender, WORD &Key, TShiftState Shift);
        
private:

   PT_state Gstate;

   TColor lookup[15];

   PT_point stars[G_STAR_MAX];

   PT_object *pship;
   PT_object *pshot[G_SHOT_MAX];

   PT_object *badie[G_BAD_MAX];
   PT_object *bshot[G_BAD_MAX];

   PT_object *bg;
   PT_object *pause;
   PT_object *planet;
   PT_object *blocks;

   Graphics::TBitmap *buffer;

   bool drop,music,sound,speedflag,dropflag,PG_bg;
   int dir,speed,shipart,counts,loopwait,numbad,numstars,gamespeed,diff,starflag,moveflag,backboom,dropcount;

   bool pShooting;
   bool pShootlock;
   bool pCanshoot;
   bool pBursting;
   int pLastshot , pNextshot;

   int GGX,GGY;

    void __fastcall Start();
    void __fastcall Score();
    void __fastcall Over(bool);

   public:

    __fastcall TFinvade(TComponent *Owner);

    friend class TFoptions;
};

extern TFinvade *Finvade;

#endif
