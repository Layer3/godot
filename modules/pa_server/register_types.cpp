#include "register_types.h"

#include "core/class_db.h"
#include "core/engine.h"

#include "pa_server.h"

static PAServer* paServer = nullptr;

void register_pa_server_types()
{
    paServer = memnew(PAServer);
    paServer->init();
    ClassDB::register_class<PAServer>();
    Engine::get_singleton()->add_singleton(Engine::Singleton("PAServer", PAServer::get_singleton()));
}

void unregister_pa_server_types()
{
    if (paServer)
    {
        paServer->finish();
        memdelete(paServer);
    }
}
