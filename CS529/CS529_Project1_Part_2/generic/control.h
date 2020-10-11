#if !defined CONTROL_H
#define CONTROL_H
//------------------------------------
//  control.h
//  Controller
//  (c) Reliable Software, 1997, 98
//------------------------------------
#include "view.h"
#include "model.h"
#include <windows.h>

class Controller
{
public:
    Controller(HWND hwnd, CREATESTRUCT * pCreate);
    ~Controller ();
    void    Size (int x, int y);
    void    Paint ();
    void    Command (int cmd);

private:

    HWND        _hwnd;

    Model       _model;
    View        _view;

};

#endif

