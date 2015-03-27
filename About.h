#ifndef AboutH
#define AboutH
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ExtCtrls.hpp>

class TFabout: public TForm
{
__published:
   TMemo *Text;
   TMemo *Memo1;
   TLabel *Label1;
   TLabel *Label2;
   TLabel *Label3;
   TLabel *Label4;
   TLabel *Label5;
   TImage *Image1;
   TImage *Image2;
   TImage *Image3;
   TGroupBox *Box;
   TButton *Button1;

public:
   virtual __fastcall TFabout(TComponent *AOwner);
};

extern TFabout *Fabout;
#endif
