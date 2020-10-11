#pragma once

#include <Windows.h>
typedef DWORD OsInt;
typedef void *OsHandle;
typedef const char* cstr;
typedef unsigned int uint;

typedef double           f64;
typedef unsigned __int64 u64;

class Timer
{
public:
  void Start(void);
  f64 End(void);

  static f64 GetTime(void);

  static void InitTimer(void);

private:
  u64 mStartTime;
  static u64 sFrequency;
  static f64 sConversion;
};

#define CheckWin(expression, ...) expression
#define VerifyWin(expression, ...) expression
