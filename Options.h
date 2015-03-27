#ifndef OptionsH
#define OptionsH
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ComCtrls.hpp>
#include <vcl\ExtCtrls.hpp>

class TFoptions: public TForm
{
__published:
   TButton *okbut;
   TButton *cancel;
   TCheckBox *Mus;
   TCheckBox *Snd;
   TLabel *Label1;
   TLabel *Label2;
   TLabel *Label3;
   TLabel *Label4;
   TLabel *Label5;
   TLabel *Label6;
   TLabel *Label7;
   TLabel *Label8;
   TLabel *Label9;
   TLabel *Label10;
   TLabel *Label11;
   TLabel *Label12;
   TLabel *Label13;
   TLabel *Label14;
   TLabel *Label15; 
   TBevel *Bevel1;
   TTrackBar *Bads;
   TTrackBar *Hard;
   TTrackBar *Speed;
   TTrackBar *Stars; 
   
   void __fastcall Preset(TObject *Sender);
   void __fastcall okbutClick(TObject *Sender);

public:
   __fastcall TFoptions(TComponent* Owner);
};

extern TFoptions *Foptions;
#endif