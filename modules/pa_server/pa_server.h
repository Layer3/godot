#pragma once

#include "core/list.h"
#include "core/object.h"
#include "core/os/thread.h"
#include "core/os/mutex.h"
#include "core/rid.h"
#include "core/set.h"
#include "core/variant.h"

class PAServer : public Object {
    GDCLASS(PAServer, Object);

public:
    PAServer();

private:
    static PAServer* singleton;
    static void thread_func(void* pUserData);

    bool thread_exited;
    mutable bool exit_thread;
    Thread* thread;
    Mutex* mutex;

public:
    static PAServer* get_singleton();
    Error init();
    void  lock();
    void  unlock();
    void  finish();

protected:
    static void _bind_methods() {};
};
