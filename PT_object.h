#ifndef PT_objectH
#define PT_objectH

class PT_object
{
   public:

   PT_object(int px, int py , int pv , int pf , int pw, int ph , AnsiString resource_name);
   ~PT_object();

   bool collision(PT_object *other);
   bool collision(int x1 , int y1 , int x2 , int y2);

   void AddVel(int pv) {v += pv; }
   void AddFrame(int pf) {f += pf; }
   void AddMove(int px , int py) {x += px; y += py; }

   int w;  // width
   int h;  // height
   int v;  // velocity
   int x;  // x
   int y;  // y
   int f;  // current animation frame

   PT_life state;

   Graphics::TBitmap *art;
};

#endif
