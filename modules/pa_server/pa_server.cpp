/*************************************************************************/
/*  pa_server.cpp                                                        */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2020 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2020 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "pa_server.h"
#include "core/dictionary.h"
#include "core/list.h"
#include "core/os/os.h"
#include "core/variant.h"

void PAServer::thread_func(void *pUserData)
{
	PAServer *pServer = static_cast<PAServer*>(pUserData);
	uint64_t msDelay = 20;

	while (!pServer->exit_thread)
	{
		pServer->lock();
		// Do Update i guess.
		pServer->unlock();
		OS::get_singleton()->delay_usec(msDelay * 1000);
	}
}

PAServer *PAServer::singleton = nullptr;

PAServer::PAServer()
	: thread_exited(false)
	, exit_thread(false)
	, thread(nullptr)
	, mutex(nullptr)
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

PAServer *PAServer::get_singleton()
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
