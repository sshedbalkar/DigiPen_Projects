#include "Os.hpp"

const uint cDebugNameMax = 100;

/// Thread class manages Os threads.
class Thread
{
public:
  typedef OsInt (*EntryFunction)(void*);

  //Construct a thread object does not create the thread.
  //Initialize and then resume to run the thread.
  Thread();
  ~Thread();

  //Is this a valid thread or uninitialized.
  bool IsValid();

  //Initializes the thread but does not run it.
  bool Initialize(EntryFunction entry, void* instance, cstr threadName);

  //Resume the thread.
  void Resume();

  //Suspend the thread.
  void Suspend();

  //Close the thread handle. Thread should have been shut down.
  //before calling this function.
  void Close();

  //Block waiting for the thread to complete.
  //The thread should either complete in a reasonable way or
  //be signaled to be closed.
  OsInt WaitForCompletion();

  //Removes the thread from this object.
  OsHandle Detach();

  //Get the OsHandle to the thread.
  OsHandle GetThreadHandle(){return mHandle;}

  template<typename classType, OsInt (classType::*MemberFunction)()>
  static OsInt ObjectEntryCreator(void* objectInstance)
  {
    classType* object = (classType*)objectInstance;
    OsInt returnValue = (object->*MemberFunction)();
    return returnValue;
  }

private:
  //non copyable
  Thread(const Thread& right);
  Thread& operator = (const Thread& right);

  OsInt mThreadId;
  OsHandle mHandle;
  char mThreadName[cDebugNameMax];
};

class ThreadLock
{
public:
  ThreadLock();
  ~ThreadLock();
  void Lock();
  void Unlock();
  CRITICAL_SECTION mPrivateData;
};

class StackThreadLock
{
public:
  StackThreadLock(ThreadLock* lock);
  ~StackThreadLock();
  ThreadLock* mLock;
};

//Wrapper around an unnamed event.
class OsEvent
{
public:
  OsEvent();
  ~OsEvent();
  OsEvent(bool manualReset, bool startSignaled);
  void Initialize(bool manualReset = false, bool startSignaled = false);
  void Close();
  void Signal();
  void Reset();
  OsHandle GetHandle(){return mHandle;}
private:
  OsHandle mHandle;
};

const int MaxSemaphoreCount = 0x0FFFFFFF;

//Semaphore class. Multithreaded counter / gatekeeper.
class Semaphore
{
public:
  Semaphore();
  ~Semaphore();
  void Increment();
  void Decrement();
  void Reset();
  void WaitAndDecrement();
private:
  OsHandle mHandle;
};
