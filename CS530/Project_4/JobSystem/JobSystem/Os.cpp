#include "Os.hpp"

u64 Timer::sFrequency = 0;
f64 Timer::sConversion = f64(0.0);

void Timer::InitTimer(void)
{
  ::QueryPerformanceFrequency((LARGE_INTEGER*)&sFrequency);

  sConversion = f64(1.0) / f64(sFrequency);
}

void Timer::Start(void)
{
  mStartTime = 0;
  QueryPerformanceCounter((LARGE_INTEGER*)&mStartTime);
}

f64 Timer::End(void)
{
  u64 count;
  QueryPerformanceCounter((LARGE_INTEGER*)&count);
  return static_cast<f64>((count - mStartTime) * sConversion);
}

double Timer::GetTime(void)
{
  u64 count;
  QueryPerformanceCounter((LARGE_INTEGER*)&count);
  return static_cast<f64>(count * sFrequency);
}