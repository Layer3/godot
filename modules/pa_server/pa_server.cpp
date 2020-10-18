#include "pa_server.h"
#include "core/dictionary.h"
#include "core/list.h"
#include "core/os/os.h"
#include "core/variant.h"

void PAServer::thread_func(void* pUserData)
{
    PAServer* pServer = static_cast<PAServer*>(pUserData);
    uint64_t msDelay = 20;

    while (!pServer->exit_thread)
    {
        pServer->lock();
        // Do Update i guess.
        pServer->unlock();
        OS::get_singleton()->delay_usec(msDelay * 1000);
    }
}

PAServer* PAServer::singleton = nullptr;

PAServer::PAServer()
{
    singleton = this;
}

Error PAServer::init()
{
    thread_exited = false;
    mutex = Mutex::create();
    thread = Thread::create(PAServer::thread_func, this);
    return OK;
}

PAServer* PAServer::get_singleton()
{
    return singleton;
}

void PAServer::unlock()
{
    if (!thread || !mutex)
    {
        return;
    }
    
   mutex->unlock();
}

void PAServer::lock()
{
    if (!thread || !mutex)
    {
        return;
    }

    mutex->lock();
}

void PAServer::finish()
{
    if (!thread)
    {
        return;
    }
    
    exit_thread = true;
    Thread::wait_to_finish(thread);
    memdelete(thread);
    
    if (mutex)
    {
        memdelete(mutex);
    }

    thread = nullptr;
}
