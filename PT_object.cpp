#include <vcl\vcl.h>

#pragma hdrstop

#include "PT_object.h"

PT_object::PT_object(int px, int py , int pv , int pf , int pw, int ph , AnsiString resource_name = "")
: x(px) , y(py) , v(pv) , f(pf) , w(pw) , h(ph) , state(DEAD)
{
   if(resource_name != "")
   {
      art = new Graphics::TBitmap;
      art->LoadFromResourceName((int)HInstance , resource_name);
   }
}

PT_object::~PT_object()
{
   if(art) delete art;
}

bool PT_object::collision(PT_object *other)
{
   if(other->state != ALIVE || state != ALIVE) return false;

   return (y <= other.y + other.h && y + h >= other.y && x <= other.x + other.w && x + w >= other.x);
}

bool PT_object::collision(int x1 , int y1 , int x2 , int y2)
{
   if(state != ALIVE) return false;

   return (y <= y2 && y + h >= y1 && x <= x2 && x + w >= x1);
}
