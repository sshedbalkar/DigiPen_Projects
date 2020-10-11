
#include "Thread.hpp"


///Used to set the thread name in Visual Studio. This raises an exception that 
//Visual Studio catches and then sets the thread name.
typedef struct tagTHREADNAME_INFO
{
  DWORD dwType;     // must be 0x1000
  LPCSTR szName;    // pointer to name (in user addr space)
  DWORD dwThreadID; // thread ID (-1=caller thread)
  DWORD dwFlags;    // reserved for future use, must be zero
} THREADNAME_INFO;

inline void SetThreadDebugName(DWORD dwThreadID, LPCSTR szThreadName)
{
  THREADNAME_INFO info;
  info.dwType = 0x1000;
  info.szName = szThreadName;
  info.dwThreadID = dwThreadID;
  info.dwFlags = 0;

  //Yes this looks strange but it works for VS
  __try
  {
    RaiseException(0x406D1388, 0, sizeof(info) / sizeof(DWORD), (CONST ULONG_PTR*)&info);
  }
  __except(EXCEPTION_CONTINUE_EXECUTION)
  {
  }
}

Thread::Thread()
{
  mHandle = NULL;
  mThreadId = 0;
}

Thread::~Thread()
{
  Close();
}

bool Thread::Initialize(EntryFunction entry, void* instance, cstr threadName)
{
  strcpy(mThreadName, threadName);

  const int cStackSize = 65536;
  mHandle = ::CreateThread(
    NULL, //No Security
    cStackSize,
    (LPTHREAD_START_ROUTINE)entry,
    (LPVOID)instance, 
    CREATE_SUSPENDED,
    &mThreadId);

  CheckWin(mHandle != INVALID_HANDLE_VALUE, "Failed to create thread named %s", 
    threadName);

  if(mHandle != INVALID_HANDLE_VALUE)
  {
    SetThreadDebugName(mThreadId, threadName);
    return true;
  }
  else
  {
    mHandle = NULL;
    return false;
  }
}

bool Thread::IsValid()
{
  return mHandle != NULL;
}

void Thread::Resume()
{
  if(IsValid())
    VerifyWin(ResumeThread(mHandle), "Failed to resume thread. Thread name: %", 
    mThreadName);
}

void Thread::Suspend()
{
  if(IsValid())
    VerifyWin(SuspendThread(mHandle),
    "Failed to suspend thread. Thread name: %", mThreadName);
}

//Close the thread handle. 
void Thread::Close()
{
  if(IsValid())
    VerifyWin(CloseHandle(mHandle),
    "Failed to close thread handle. Thread name: %", mThreadName);
  mHandle = NULL;
}

OsHandle Thread::Detach()
{
  OsHandle handle = mHandle;
  mHandle = NULL;
  return handle;
}

OsInt Thread::WaitForCompletion()
{
  if(!IsValid())
    return -1;

  DWORD result = WaitForSingleObject(mHandle, INFINITE);
  if(result != WAIT_OBJECT_0)
  {
    //DebugPrint("Failed to wait on on thread. Thread name: %s", mThreadName);
    return -1;
  }
  else
  {
    OsInt returnCode;
    GetExitCodeThread(mHandle, &returnCode);
    return returnCode;
  }
}


ThreadLock::ThreadLock()
{
  ::InitializeCriticalSection((CRITICAL_SECTION*)&mPrivateData);
}

ThreadLock::~ThreadLock()
{
  ::DeleteCriticalSection((CRITICAL_SECTION*)&mPrivateData);
}

void ThreadLock::Lock()
{
  ::EnterCriticalSection((CRITICAL_SECTION*)&mPrivateData);
}

void ThreadLock::Unlock()
{
  ::LeaveCriticalSection((CRITICAL_SECTION*)&mPrivateData);
}

StackThreadLock::StackThreadLock(ThreadLock* lock)
  :mLock(lock)
{
  mLock->Lock();
}

StackThreadLock::~StackThreadLock()
{
  mLock->Unlock();
}


OsEvent::OsEvent()
  :mHandle(NULL)
{
}

OsEvent::~OsEvent()
{
  Close();
}

OsEvent::OsEvent(bool manualReset, bool startSignaled)
{
  Initialize(manualReset, startSignaled);
}

void OsEvent::Initialize(bool manualReset, bool startSignaled)
{
  mHandle = CreateEvent(NULL, manualReset, startSignaled, NULL);
  CheckWin(mHandle!=INVALID_HANDLE_VALUE, "Failed to create event.");
}

void OsEvent::Close()
{
  if(mHandle!=NULL)
  {
    VerifyWin(CloseHandle(mHandle), "Failed to close event.");
  }
}

void OsEvent::Signal()
{
  if(mHandle!=NULL)
    VerifyWin(SetEvent(mHandle), "Failed to Signal event.");
}

void OsEvent::Reset()
{
  if(mHandle!=NULL)
    VerifyWin(ResetEvent(mHandle), "Failed to Reset event.");
}

Semaphore::Semaphore()
{
  mHandle = CreateSemaphore(NULL, 0, MaxSemaphoreCount, NULL);
}

Semaphore::~Semaphore()
{
  VerifyWin(CloseHandle(mHandle),"Failed to close Semaphore handle");
}

void Semaphore::Increment()
{
  VerifyWin(ReleaseSemaphore(mHandle, 1, NULL), "Failed to increment semaphore");
}

void Semaphore::Decrement()
{
  WaitForSingleObject(mHandle, 0);
}

void Semaphore::Reset()
{
  VerifyWin(CloseHandle(mHandle),"Failed to close Semaphore handle");
  mHandle = CreateSemaphore(NULL, 0, MaxSemaphoreCount, NULL);
}

void Semaphore::WaitAndDecrement()
{
  OsInt result = WaitForSingleObject(mHandle, INFINITE);
  if(result != WAIT_OBJECT_0)
  {
  }
}
