#if !defined MODEL_H
#define MODEL_H
//------------------------------------
//  model.h
//  The Model
//  (c) Reliable Software, 1997, 98
//------------------------------------
#include <string.h>

class Model
{
    enum { TEXT_SIZE = 20 };
public:
    Model (char const * str)
    {
        SetText (str);
        _text [TEXT_SIZE] = '\0';
    }

    void SetText (char const * str)
    {
        strncpy (_text, str, TEXT_SIZE);
    }

    char const * GetText () const { return _text; }
    int GetLen () const { return strlen (_text); }

private:

    char _text [TEXT_SIZE + 1];
};

#endif
