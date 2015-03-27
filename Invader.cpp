#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <vcl\vcl.h>

#pragma hdrstop

#include "About.h"
#include "Invader.h"
#include "Options.h"

#pragma resource "*.dfm"
#pragma resource "rc.res"

TFinvade *Finvade;

__fastcall TFinvade::TFinvade(TComponent *Owner) : TForm(Owner){}

void __fastcall TFinvade::Setup(TObject *Sender)
{
   Gstate = STOP;

   Application->OnMinimize = Rep;
   Application->OnDeactivate = Rep;

   diff = 3;
   numbad = 16;
   numstars = 6;
   gamespeed = 1;
   music = false;
   sound = false;
                      // x            , y   , v  , f , w  , h
   pship = new TP_object(G_W / 2 - 15 , 375 , 10 , 0 , 30 , 34);

   for(int a = 0; a < G_SHOT_MAX; a++)
   {                        // x , y , v  , f , w , h
      pshot[a] = new TP_object(0 , 0 , 20 , 0 , 4 , 13);
   }

   for(int a = 0; a < G_BAD_MAX; a++)
   {
      badie[a] = new TP_object(0 , 0 , 20 , 0 , 4 , 13);
      bshot[a] = new TP_object(0 , 0 , 20 , 0 , 4 , 13);
   }

   buffer->Width = Gw;
   buffer->Height = Gh;

   pause->LoadFromResourceName((int)HInstance , "Pause");
   earth->LoadFromResourceName((int)HInstance , "Earth");
   background->LoadFromFile("bg.bmp");

   lookup[0]  = 0x006F6F6F;
   lookup[1]  = 0x008F8F8F;
   lookup[2]  = 0x00FFFFFF;
   lookup[3]  = 0x0000FF00;
   lookup[4]  = 0x000000FF;
   lookup[5]  = 0x00FF0000;
   lookup[6]  = 0x00AFFAFF;
   lookup[7]  = 0x00888888;
   lookup[8]  = 0x00FFFAAA;
   lookup[9]  = 0x00BBFFBB;
   lookup[10] = 0x00FF00FF;
   lookup[11] = 0x00FF1111;
   lookup[12] = 0x006D6D6D;
   lookup[13] = 0x008A8A8A;
   lookup[14] = 0x00FF55FF;

   Start();
}

void __fastcall TFinvade::Start()
{
   int x,y;

   mStop->Enabled=false;

   buffer->Canvas->Brush->Color = clBlack;
   Welcome->Canvas->Brush->Color = clBlack;

   buffer->Canvas->FillRect( Rect(0 , 0 , Gw , Gh) );
   Welcome->Canvas->FillRect( Rect(0 , 0 , Gw , Gh) );

   asteroid[0]->LoadFromResourceName((int)HInstance,"Rock1");
   asteroid[1]->LoadFromResourceName((int)HInstance,"Rock1");

   Welcome->Canvas->Draw(120,285,asteroid[0]);
   Welcome->Canvas->Draw(245,285,asteroid[1]);

   earth->LoadFromResourceName((int)HInstance,"Moon");
   Welcome->Canvas->Draw(5,5,earth);

   earth->LoadFromResourceName((int)HInstance,"Earth");
   Welcome->Canvas->Draw(250,10,earth);

   bad->Draw(Welcome->Canvas,25,120,1);
   bad->Draw(Welcome->Canvas,55,120,0);
   bad->Draw(Welcome->Canvas,85,120,1);
   bad->Draw(Welcome->Canvas,115,120,2);
   bad->Draw(Welcome->Canvas,145,120,2);
   bad->Draw(Welcome->Canvas,175,120,0);
   bad->Draw(Welcome->Canvas,205,120,2);
   bad->Draw(Welcome->Canvas,235,120,0);
   weap->Draw(Welcome->Canvas,33,145,2);
   weap->Draw(Welcome->Canvas,83,175,2);
   weap->Draw(Welcome->Canvas,153,230,2);
   weap->Draw(Welcome->Canvas,183,200,2);
   weap->Draw(Welcome->Canvas,40,165,0);
   ship->Draw(Welcome->Canvas,40,360,0);

   Welcome->Canvas->Font->Color=clWhite;
   Welcome->Canvas->Font->Size=30;
   Welcome->Canvas->Font->Name="Ultraworld";
   Welcome->Canvas->TextOut(110,355,"PROTECTOR");
   Welcome->Canvas->Font->Color=clGray;
   Welcome->Canvas->Font->Size=14;
   Welcome->Canvas->Font->Name="Copperplate Gothic Bold";
   Welcome->Canvas->TextOut(110,335,"Final Version 1.0");

   for(int b=0; b<400; b++)
   {
      x=rand()%400; y=rand()%403;
      if(Welcome->Canvas->Pixels[x][y]==clBlack) Welcome->Canvas->Pixels[x][y]=lookup[b%3];
   }

   Welcome->Visible=true;
}

void __fastcall TFinvade::NewGame(TObject *Sender)
{
   mStop->Enabled=true;
   mPause->Enabled=true;
   Welcome->Visible=false;
   mOptions->Enabled=false;
   Time->OnTimer=NextFrame;
   Time->Interval=gamespeed;
   buffer->Canvas->CopyMode=cmSrcPaint;
   buffer->Canvas->Brush->Color=clBlack;
   buffer->Canvas->FillRect(Rect(0,0,Gw,Gh));

   time_t now=time(0);
   srand(now);

   TSprite rock1(false,20,285,1,0);
   TSprite rock2(false,135,285,1,0);
   TSprite planet(false,-200,-200,2,0);

   home=planet;
   blocks[0]=rock1;
   blocks[1]=rock2;

   asteroid[1]->Canvas->Pen->Color=clBlack;
   asteroid[0]->Canvas->Pen->Color=clBlack;
   asteroid[0]->Canvas->Brush->Color=clBlack;
   asteroid[1]->Canvas->Brush->Color=clBlack;
   earth->LoadFromResourceName((int)HInstance,"Earth");
   asteroid[0]->LoadFromResourceName((int)HInstance,"Rock1");
   asteroid[1]->LoadFromResourceName((int)HInstance,"Rock1");

   dir=1;                     //direction var for baddies 1=right -1=left
   shipart=0;                 //ship artframe
   canshoot=true;
   drop=false;                //badie drop var
   backboom=0;                //explosion adjustment var
   dropcount=0;
   loopwait=18;               //delay after win/lose flags
   dropflag=false;            //badie drop flag
   speedflag=false;           //speed up flag
   speed=4-(numbad/8);        //find starting speed
   if(numbad==8) starflag=2;  //fixes star loop
   else starflag=1;

   for(int a=0; a<numbad; a++)
   {
      TSprite temp(false,1+((a+8)%8)*40,(25*((a+8)/8))-20,5,a%3);
      badie[a]=temp;
   }

   for(int a=0; a<numstars*16; a++)
   {
      stars[a].x = rand() % (Gw + 1);
      stars[a].y = rand() % (Gh + 1);
   }

   if(music)
   {
      Midi->Stop();
      Midi->Rewind();
      Midi->Play();
   }

   Score();
   Gstate = PLAY;
   Time->Enabled=true;
   GGX=0; GGY=0;
   PG_bg = true;
}

void __fastcall TFinvade::NextFrame(TObject *Sender)
{
   if(Gstate != PLAY) return;

   int max=-1,m,starturd;

   moveflag = 0;
   counts = (numstars * 16) - 1;

   if(!PG_bg) buffer->Canvas->FillRect( Rect(0 , 0 , Gw , Gh) );
   else
   {
      buffer->Canvas->Draw(GGX , GGY , background);
      buffer->Canvas->Draw(GGX , GGY - Gh , background);

      if(GGX >= 0)
      {
         buffer->Canvas->Draw( GGX - Gw , GGY , background);
         buffer->Canvas->Draw( GGX - Gw , GGY - Gh , background);
      }
      else
      {
         buffer->Canvas->Draw(Gw + GGX , GGY , background);
         buffer->Canvas->Draw(Gw + GGX, GGY - Gh , background);
      }

      if(GGY >= Gh) GGY = 1;
      if(GGX <= Gw * -1 + 1 || GGX >= Gw - 1) GGX = 0;
      GGY++;
   }
   home.y++; home.x+=2;
   blocks[0].x++;
   blocks[1].x++;

   if(one->ship->GetFrame == 4 || one->ship->GetFrame == 6)
   {
      home.x+=1;
      moveflag=-1;
      GGX--;
   }
   else if(one->ship->GetFrame == 5 || one->ship->GetFrame == 7)
   {
      home.x-=1;
      moveflag=1;
      GGX++;
   }

   if(home.x > Gw)
   {
      home.x=-200;
      home.y=-150;
      earth->LoadFromResourceName((int)HInstance,"Moon");
   }

   buffer->Canvas->Draw(home.x,home.y,earth);
   buffer->Canvas->Draw(blocks[0].x,blocks[0].y,asteroid[0]);
   buffer->Canvas->Draw(blocks[1].x,blocks[1].y,asteroid[1]);

   for(int a = numbad - 1; a >= 0; a--)
   {
      if(a < 16)
      {
         for(int b = counts; b > counts - (numstars * starflag); b--)
         {
            starturd = b % 3;
            stars[b].y += 1 + starturd + (starturd * starturd);
            stars[b].x += (2 + ( ((b % 3) * 5) * (b % 3)) ) * moveflag;
            if(stars[b].y > Gh) stars[b].y -= Gh;
            if(stars[b].x < 0) stars[b].x += Gw;
            if(stars[b].x > Gw) stars[b].x -= Gw;
            buffer->Canvas->Pixels[ stars[b].x ][ stars[b].y ] = lookup[b % 3];
         }
         counts -= numstars;
      }

      if(badie[a].shot.alive && badie[a].shot.y<=335 && badie[a].shot.y>=273)
      {
         int which=-1,j=0;

         if(badie[a].shot.x<=(blocks[0].x+75) && badie[a].shot.x>=blocks[0].x) which=0;
         else if(badie[a].shot.x<=(blocks[1].x+75) &&  badie[a].shot.x>=blocks[1].x) which=1;

         if(which>=0)
         {
            while((asteroid[which]->Canvas->Pixels[badie[a].shot.x-blocks[which].x][j]==clBlack || asteroid[which]->Canvas->Pixels[badie[a].shot.x-blocks[which].x][j]==-1) && (asteroid[which]->Canvas->Pixels[badie[a].shot.x-blocks[which].x+3][j]==clBlack || asteroid[which]->Canvas->Pixels[badie[a].shot.x-blocks[which].x+3][j]==-1) && j<50) j++;

            if(j!=50)
            {
               badie[a].shot.alive=false;
               boom->Draw(buffer->Canvas,badie[a].shot.x-28,285+j-23,3*(rand()%2));
               asteroid[which]->Canvas->Ellipse(badie[a].shot.x-blocks[which].x-1,-5,badie[a].shot.x-blocks[which].x+5,j+15);
            }
         }
      }

      if(badie[a].shot.alive && badie[a].shot.y>=428 && badie[a].shot.x<=one.x+30 && badie[a].shot.x+4>=one.x)
      {
         badie[a].shot.alive=false;
         boom->Draw(buffer->Canvas,badie[a].shot.x-13,badie[a].shot.y-7,a%2);
         boom->Draw(buffer->Canvas,one.x-10,one.y-8,(a%2)+2);
         one.health-=50-(abs((one.x+15)-(badie[a].shot.x+2)));
         Score();
      }

      for(int g = 2; g >= 0; g--)
      {
         if(one.shot[g].alive && badie[a].shot.alive && one.shot[g].x>=badie[a].shot.x-5 && one.shot[g].x<=badie[a].shot.x+5 && one.shot[g].y<=badie[a].shot.y+21 && one.shot[g].y>=badie[a].shot.y-13)
         {
            one.shot[g].alive=false;
            badie[a].shot.alive=false;
            boom->Draw(buffer->Canvas,one.shot[g].x-28,one.shot[g].y-23,3*(a%2));
            boom->Draw(buffer->Canvas,badie[a].shot.x-28,badie[a].shot.y-23,3*(a%2));
         }
         if(one.shot[g].alive && !badie[a].dead &&badie[a].art<5&& one.shot[g].y<=badie[a].y+23 && one.shot[g].y>=badie[a].y-2 && one.shot[g].x>=badie[a].x-5 && one.shot[g].x<=badie[a].x+21)
         {
            speedflag=true;
            badie[a].art=5;
            one.shot[g].alive=false;
            boom->Draw(buffer->Canvas,one.shot[g].x-28,one.shot[g].y-23,3*(a%2));
            if(abs((one.shot[g].x+2)-(badie[a].x+10))<=0) one.points+=1000;
            else one.points+=1000/(abs((one.shot[g].x+2)-(badie[a].x+10)));
            if(speed-(5-(numbad/8))==numbad) one.points+=2000-(badie[a].y*2);
            Score();
         }
         else if(one.shot[g].alive && (one.shot[g].y-one.shot[g].vel)<=335 && (one.shot[g].y-one.shot[g].vel)>=273)
         {
            int which=-1,k=335-one.shot[g].y,h=335-(one.shot[g].y-one.shot[g].vel);

            if(one.shot[g].x<=(blocks[0].x+75) && one.shot[g].x>=blocks[0].x) which=0;
            else if(one.shot[g].x<=(blocks[1].x+75) &&  one.shot[g].x>=blocks[1].x) which=1;

            if(which>=0)
            {
               while((asteroid[which]->Canvas->Pixels[one.shot[g].x-blocks[which].x][50-k]==clBlack || asteroid[which]->Canvas->Pixels[one.shot[g].x-blocks[which].x][50-k]==-1) && (asteroid[which]->Canvas->Pixels[one.shot[g].x-blocks[which].x+3][50-k]==clBlack || asteroid[which]->Canvas->Pixels[one.shot[g].x-blocks[which].x+3][50-k]==-1) && k<h) k++;

               if(k!=h)
               {
                  one.shot[g].alive=false;
                  boom->Draw(buffer->Canvas,one.shot[g].x-28,285-k+23,3*(rand()%2));
                  asteroid[which]->Canvas->Ellipse(one.shot[g].x-blocks[which].x-1,50-k-5,one.shot[g].x-blocks[which].x+5,55);
               }
            }
         }
      }
      if(badie[a].art>=5) goto fix;

      if(!badie[a].shot.alive && !drop && !badie[a].dead && (a+8>=numbad || badie[a+8].dead))
      {
         if( ( (abs((badie[a].x+10)-(one.x+15)) < (30+(diff*10))) && (rand()%(20-(diff+5))<=diff)) || ((rand()%1000-(diff*10))<=diff))
         {
            badie[a].shot.alive=true;
            badie[a].shot.x=badie[a].x+8;

            if(drop)
            {
               badie[a].shot.y=badie[a].y+20;
               badie[a].shot.x=badie[a].x+8;
            }
            else
            {
               badie[a].shot.y=badie[a].y;
               badie[a].shot.x=badie[a].x+(((speed/(numbad/8))+1)*dir)+8;
            }
            badie[a].art=4;
         }
      }

      fix:
      if(badie[a].shot.alive)
      {
         if(badie[a].shot.y>=500) badie[a].shot.alive=false;
         badie[a].shot.y+=badie[a].shot.vel;
         weap->Draw(buffer->Canvas,badie[a].shot.x,badie[a].shot.y,2);
      }
      if(badie[a].art>=5) goto cool;
      if(badie[a].dead) continue;
      if(a>max) max=a;

      if(drop)
      {
         badie[a].y+=4;
         if(badie[a].art<=3) badie[a].art=3;
      }
      else badie[a].x+=((speed/(numbad/8))+1)*dir;

      if(badie[a].x <= 0 || badie[a].x >= Gw - 20) dropflag = true;

      cool:
      if(badie[a].art>=10) continue;
      if(badie[a].art<=5) bad->Draw(buffer->Canvas,badie[a].x,badie[a].y,badie[a].art);
      if(badie[a].art>=5) boom->Draw(buffer->Canvas,badie[a].x-20,badie[a].y-20,((badie[a].art-backboom)-5)+(3*(a%2)));

      switch(badie[a].art)
      {
         case 0: case 1: badie[a].art++; break;
         case 2: badie[a].art=0; break;
         case 3: badie[a].art=a%3; break;
         case 4: badie[a].vel--; break;
         case 5: badie[a].art++; PlotShrap(badie[a].x,badie[a].y,6); break;
         case 6: badie[a].art++; PlotShrap(badie[a].x,badie[a].y,9); break;
         case 7: badie[a].art++; PlotShrap(badie[a].x,badie[a].y,12); backboom=2; break;
         case 8: badie[a].art++; PlotShrap(badie[a].x,badie[a].y,15); backboom+=2; break;
         case 9: badie[a].dead=true; badie[a].art++; backboom=0; PlotShrap(badie[a].x,badie[a].y,18); break;
      }
      if(badie[a].vel==0)
      {
         badie[a].art=a%3;
         badie[a].vel=5;
      }
   }

   if(drop)
   {
      if(dropcount>=5)
      {
         drop=false;
         dropflag=false;
         dir*=-1;
         dropcount=0;
      }
      else dropcount++;
   }

   if(dropflag)
   {
      drop=true;
      dropflag=false;
   }

   if(speedflag)
   {
      speed++;
      speedflag=false;
   }

   if(one.art == 4 || one.art == 6)
   {
      one.x += one.vel;
      if(one.x >= Gw) one.x = Gw;
      flame->Draw(buffer->Canvas , one.x - 12 , one.y + 24 , one.art - 4);
   }
   else if(one.art == 5 || one.art == 7)
   {
      one.x -= one.vel;
      if(one.x <= -10) one.x =- 10;
      flame->Draw(buffer->Canvas , one.x + 30 , one.y + 24 , one.art - 4);
   }

   if(shipart == 1)
   {
      one.shot[one.cs].alive = true;
      one.shot[one.cs].x = one.x + 13;
      one.shot[one.cs].y = one.y;
      shoot->Draw(buffer->Canvas , one.x + 3 , one.y - 28 , 0);
   }

   ship->Draw(buffer->Canvas , one.x , one.y , shipart);
   shipart = 0;

   for(int g=0; g<3; g++)
   {
      if(one.shot[g].alive==2)
      {
         boom->Draw(buffer->Canvas,one.shot[g].x-28,one.shot[g].y-23,3*(rand()%2));
         one.shot[g].alive=false;
      }
      else if(one.shot[g].alive)
      {
         if(one.shot[g].y<=-13)
         {
            one.shot[g].alive=false;
         }
         one.shot[g].y-=one.shot[g].vel;
         weap->Draw(buffer->Canvas,one.shot[g].x,one.shot[g].y,0);
         PlotShrap(one.shot[g].x-8,one.shot[g].y+8,-12);
      }
   }

   if(one.health<=0) one.dead=true;
   else if(max>=0 && badie[max].y>=335) one.dead=true;

   if(max==-1 || one.dead)
   {
      if(one.dead)
      {
         PlotShrap(one.x,one.y,1+(loopwait%7)*4);
         boom->Draw(buffer->Canvas,one.x-15,one.y-15,rand()%6);
      }
      loopwait--;
   }

   if(loopwait<0) Over(!one.dead);

   Score();
   Canvas->Draw(0,0,buffer);
}

void __fastcall TFinvade::WinLoop(TObject *Sender)
{
   int x,y;

   buffer->Canvas->Brush->Color=clBlack;
   Welcome->Canvas->Brush->Color=clBlack;
   buffer->Canvas->FillRect(Rect(0,0,Gw,Gh));
   Welcome->Canvas->FillRect(Rect(0,0,Gw,Gh));

   earth->LoadFromResourceName((int)HInstance,"Moon");  Welcome->Canvas->Draw(5,5,earth);
   earth->LoadFromResourceName((int)HInstance,"Earth"); Welcome->Canvas->Draw(250,10,earth);

   boom->Draw(Welcome->Canvas,20,275,2);
   boom->Draw(Welcome->Canvas,290,225,5);
   boom->Draw(Welcome->Canvas,130,250,2);
   boom->Draw(Welcome->Canvas,165,180,5);
   ship->Draw(Welcome->Canvas,50,360,0);

   Welcome->Canvas->Font->Color=clWhite;
   Welcome->Canvas->Font->Size=30;
   Welcome->Canvas->Font->Name="Ultraworld";
   Welcome->Canvas->TextOut(110,355,"YOU WIN");

   for(int b=0; b<400; b++)
   {
      x=rand()%400,y=rand()%403;
      if(Welcome->Canvas->Pixels[x][y]==clBlack) Welcome->Canvas->Pixels[x][y]=lookup[b%3];
   }

   Welcome->Visible=true;
   Time->Interval=5000;
   Time->OnTimer=StopGame;
}

void __fastcall TFinvade::LoseLoop(TObject *Sender)
{
   int x,y;

   Welcome->Canvas->Pen->Color=clBlack;
   Welcome->Canvas->Brush->Color=clBlack;
   earth->LoadFromResourceName((int)HInstance,"Earth");

   Canvas->FillRect(Rect(0,0,Gw,Gh));
   Welcome->Canvas->FillRect(Rect(0,0,Gw,Gh));
   Welcome->Canvas->Draw(100,150,earth);

   bad->Draw(Welcome->Canvas,85,45,3);
   bad->Draw(Welcome->Canvas,115,45,0);
   weap->Draw(Welcome->Canvas,93,70,2);
   bad->Draw(Welcome->Canvas,145,45,2);
   weap->Draw(Welcome->Canvas,123,75,2);
   bad->Draw(Welcome->Canvas,175,45,0);
   weap->Draw(Welcome->Canvas,153,80,2);
   bad->Draw(Welcome->Canvas,205,45,1);
   weap->Draw(Welcome->Canvas,183,140,2);
   bad->Draw(Welcome->Canvas,235,45,0);
   weap->Draw(Welcome->Canvas,213,125,2);
   bad->Draw(Welcome->Canvas,265,45,1);
   weap->Draw(Welcome->Canvas,243,95,2);
   bad->Draw(Welcome->Canvas,295,45,2);
   bad->Draw(Welcome->Canvas,95,20,1);
   bad->Draw(Welcome->Canvas,125,20,1);
   weap->Draw(Welcome->Canvas,133,72,2);
   bad->Draw(Welcome->Canvas,155,20,0);
   weap->Draw(Welcome->Canvas,163,85,2);
   bad->Draw(Welcome->Canvas,185,20,1);
   weap->Draw(Welcome->Canvas,193,90,2);
   bad->Draw(Welcome->Canvas,215,20,2);
   weap->Draw(Welcome->Canvas,223,145,2);
   bad->Draw(Welcome->Canvas,245,20,2);
   weap->Draw(Welcome->Canvas,253,130,2);
   bad->Draw(Welcome->Canvas,275,20,0);
   weap->Draw(Welcome->Canvas,283,100,2);
   bad->Draw(Welcome->Canvas,305,20,0);

   boom->Draw(Welcome->Canvas,100,250,0);
   boom->Draw(Welcome->Canvas,180,200,0);
   boom->Draw(Welcome->Canvas,200,160,1);
   boom->Draw(Welcome->Canvas,220,155,1);
   boom->Draw(Welcome->Canvas,240,190,4);
   boom->Draw(Welcome->Canvas,210,150,2);
   boom->Draw(Welcome->Canvas,125,225,2);
   boom->Draw(Welcome->Canvas,115,185,3);
   boom->Draw(Welcome->Canvas,210,290,3);
   boom->Draw(Welcome->Canvas,160,285,4);
   boom->Draw(Welcome->Canvas,200,240,5);
   boom->Draw(Welcome->Canvas,160,130,5);

   Welcome->Canvas->Font->Color=clWhite;
   Welcome->Canvas->Font->Size=30;
   Welcome->Canvas->Font->Name="Ultraworld";
   Welcome->Canvas->TextOut(110,355,"YOU LOST");

   for(int b=0; b<400; b++)
   {
      x=rand()%400,y=rand()%403;
      if(Welcome->Canvas->Pixels[x][y]==clBlack) Welcome->Canvas->Pixels[x][y]=lookup[b%3];
   }

   Welcome->Visible=true;
   Time->Interval=5000;
   Time->OnTimer=StopGame;
}

void __fastcall TFinvade::Over(bool won)
{
   Score();
   Gstate = STOP;
   mPause->Enabled = false;

   if(won) Time->OnTimer = WinLoop;
   else Time->OnTimer = LoseLoop;
}

void __fastcall TFinvade::StopGame(TObject *Sender)
{
   Gstate = STOP;

   Time->Enabled = false;

   mPause->Caption = "&Pause";
   mPause->OnClick = Rep;
   mPause->Enabled = false;
   mOptions->Enabled = true;

   if(music) Midi->Stop();

   Score();
   Start();
}

void __fastcall TFinvade::Score()
{
   char out[15];
   if(one.health<0) one.health=0;
   sprintf(out,"Damage: %d%",one.health);
   buffer->Canvas->Font->Color=clGray;
   buffer->Canvas->Font->Name="Copperplate Gothic Bold";
   buffer->Canvas->Font->Size=24;
   buffer->Canvas->Brush->Color=NULL;
   buffer->Canvas->TextOut(0,450,AnsiString(one.health));
   buffer->Canvas->Brush->Color=clBlack;
}

void __fastcall TFinvade::Leak(TObject *Sender)
{
   Midi->Close();
   Wav->Close();

   bad->Free();
   boom->Free();
   weap->Free();
   ship->Free();
   flame->Free();
   shoot->Free();

   delete bad;
   delete ship;
   delete weap;
   delete boom;
   delete flame;
   delete shoot;
   delete pause;
   delete earth;
   delete buffer;
   delete asteroid[0];
   delete asteroid[1];
   delete background;
   delete one;
 }

/*----------------------------------------------------------------------------------------------------------------------\
| R E P ////////////////////////////////////////////////////////////////////////////////////////////////////////////////|
\----------------------------------------------------------------------------------------------------------------------*/

 void __fastcall TFinvade::Rep(TObject *Sender)
{                          // Pauses game
   if(Gstate == PLAY)
   {
      Gstate = PAUSE;
      Time->Enabled = false;

      if(music) Midi->Pause();

      mPause->Caption = "&Resume";
      mPause->OnClick = Unp;

      Welcome->Canvas->Draw(0 , 0 , buffer);
      Welcome->Canvas->Draw( (Gw / 2) - oPause->w / 2 , (Gh / 2) - oPause->h / 2 , oPause);
      Welcome->Visible=true;

      Score();
   }
}

/*----------------------------------------------------------------------------------------------------------------------\
| U N P ////////////////////////////////////////////////////////////////////////////////////////////////////////////////|
\----------------------------------------------------------------------------------------------------------------------*/

void __fastcall TFinvade::Unp(TObject *Sender)
{                         // Unpauses game
   if(Gstate == PAUSE)
   {
      Score();

      if(music) Midi->Resume();

      mPause->Caption = "&Pause";
      mPause->OnClick = Rep;

      Canvas->Draw(0 , 0 , buffer);
      Welcome->Visible = false;

      Time->Enabled = true;
      Gstate = PLAY;
   }
}

/*----------------------------------------------------------------------------------------------------------------------\
| U S E R  H I T  K E Y ////////////////////////////////////////////////////////////////////////////////////////////////|
\----------------------------------------------------------------------------------------------------------------------*/

 void __fastcall TFinvade::UserHitKey(TObject *Sender , WORD &Key , TShiftState Shift)
{
   if(Gstate != PLAY) return;

   switch(Key)
   {
      case 37: if(pBursting) pShip->f = 5; else pShip->f = 7; break;
      case 39: if(pBursting) pShip->f = 4; else pShip->f = 6; break;
      case 32: if(pBursting && pShip->state == ALIVE && !pKeylock)
               {
                  pNextshot = -1;

                  for(int a = 0; a < Gshotmax; a++)
                  {
                     if(pshot[a]->state == DEAD)
                     {
                        pNextShot = a;
                        break;
                     }
                  }

                  if(pNextshot >= 0)
                  {
                     Score();
                     pShooting = true;
                     pKeylock = true;
                     if(sound) Wav->Play();
                  }
               }
      break;
      case 38: if(pshot[plastshot]->state == ALIVE && pship->state == ALIVE) pshot[plastshot]->state = DYING; break;
      case 40: if(pShip->f == 4 || pShip->f == 5) pShip->f += 2; one.vel = 15; break;
      case 80:
      case 19: if(Gstate == PLAY) Rep(this);
   }
}

/*---------------------------------------------------------------------------------------------------------------------\
| U S E R  L E T  K E Y ///////////////////////////////////////////////////////////////////////////////////////////////|
\---------------------------------------------------------------------------------------------------------------------*/

void __fastcall TFinvade::UserLetKey(TObject *Sender , WORD &Key , TShiftState Shift)
{
   if(Gstate == PAUSE && (Key == 80 || Key == 19)) Unp(this);

   if(Gstate != PLAY) return;

   switch(Key)
   {
      case 32: pKeylock = false; break;
      case 37: if(pShip->f == 5 || pShip->f == 7) pShip->f = 0; break;
      case 39: if(pShip->f == 4 || pShip->f == 6) pShip->f = 0; break;
      case 40: if(pShip->f == 6 || pShip->f == 7) pShip->f -= 2; one.vel = 10; break;
   }
}

/*---------------------------------------------------------------------------------------------------------------------\
| E X I T /////////////////////////////////////////////////////////////////////////////////////////////////////////////|
\---------------------------------------------------------------------------------------------------------------------*/

void __fastcall TFinvade::Exit(TObject *Sender)
{
   if(music) Midi->Stop();

   Close();
}

/*---------------------------------------------------------------------------------------------------------------------\
| R E A D Y  C L O S E ////////////////////////////////////////////////////////////////////////////////////////////////|
\---------------------------------------------------------------------------------------------------------------------*/

void __fastcall TFinvade::ReadyClose(TObject *Sender, bool &CanClose)
{
   Rep(Sender);

   switch( MessageBox(Handle , "Do you want to quit?" , "Leaving already?" , (MB_YESNO + MB_ICONQUESTION)) )
   {
      case IDYES: CanClose = true; break;
      case IDNO:  CanClose = false; break;
   }
}

/*---------------------------------------------------------------------------------------------------------------------\
| S H O W   A B O U T  ////////////////////////////////////////////////////////////////////////////////////////////////|
\---------------------------------------------------------------------------------------------------------------------*/

void __fastcall TFinvade::ShowAbout(TObject *Sender)
{
    Rep(Sender);

    Fabout->ShowModal();
}

/*---------------------------------------------------------------------------------------------------------------------\
| S H O W  O P T I O N S //////////////////////////////////////////////////////////////////////////////////////////////|
\---------------------------------------------------------------------------------------------------------------------*/

void __fastcall TFinvade::ShowOptions(TObject *Sender)
{
   if(Gstate == STOP) Foptions->ShowModal();
}

