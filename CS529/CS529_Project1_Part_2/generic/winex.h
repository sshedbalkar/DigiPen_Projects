#if !defined WINEX_H
#define WINEX_H
//------------------------------------
//  winex.h
//  Windows Exception
//  (c) Reliable Software, 1997, 98
//------------------------------------
#include <windows.h>

// out of memory handler that throws WinException
int NewHandler (size_t size);

class WinException
{
public:
    WinException (char* msg)
    : _err (GetLastError()), _msg(msg)
    {}
    DWORD GetError() const { return _err; }
    char const * GetMessage () const { return _msg; }
private:
    DWORD  _err;
    char * _msg;
};

#endif