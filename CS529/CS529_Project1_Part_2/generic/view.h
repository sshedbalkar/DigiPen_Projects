#if !defined VIEW_H
#define VIEW_H
//------------------------------------
//  view.h
//  View
//  (c) Reliable Software, 1997, 98
//------------------------------------
#include <windows.h>

class Canvas;
class Model;

class View
{
public:

    void SetSize (int cxNew, int cyNew)
    {
        _cx = cxNew;
        _cy = cyNew;
    }

    void Paint (Canvas & canvas, Model &model);

protected:

   int _cx;
   int _cy;
};

#endif
