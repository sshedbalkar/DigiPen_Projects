//------------------------------------
//  view.cpp
//  (c) Reliable Software, 1997, 98
//------------------------------------
#include "view.h"
#include "canvas.h"
#include "model.h"

void View::Paint (Canvas & canvas, Model & model)
{
    canvas.Text (12, 1, model.GetText(), model.GetLen());
    canvas.Line (10, 0, 10, _cy);
}


