#include <vcl\vcl.h>
#pragma hdrstop

USERES("ProjInvad.res");
USEFORM("About.cpp", Fabout);
USEFORM("Invader.cpp", Finvade);
USEFORM("Options.cpp", Foptions);
USEUNIT("PT_object.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE , HINSTANCE , LPSTR , int)
{
   try
   {
      Application->Initialize();
      Application->Title = "Protector!";
      Application->CreateForm(__classid(TFinvade), &Finvade);
                Application->CreateForm(__classid(TFabout), &Fabout);
                Application->CreateForm(__classid(TFoptions), &Foptions);
                Application->Run();
   }
   catch(Exception &exception)
   {
      Application->ShowException(&exception);
   }
   return 0;
}

