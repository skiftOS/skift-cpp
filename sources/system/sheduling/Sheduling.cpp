#include <libruntime/LinkedList.h>
#include <libsystem/Logger.h>

#include "arch/Arch.h"
#include "system/sheduling/Sheduling.h"

using namespace libruntime;
using namespace system::tasking;

namespace system::sheduling
{

static RefPtr<Thread> _running_thread;
static LinkedList<RefPtr<Thread>> *_threads;

void initialize()
{
    logger_info("Initializing sheduling");

    _threads = new LinkedList<RefPtr<Thread>>();

    logger_trace("Creating the kernel thread");

    logger_info("The kernel thread has been created");
}

void register_thread(RefPtr<Thread> thread)
{
    _threads->push_back(thread);

    if (_running_thread.necked() == NULL)
    {
        logger_info("Using {} has running thread", thread.necked());
        _running_thread = thread;
    }

    logger_trace("Registering {}", thread.necked());
}

void unregister_thread(RefPtr<Thread> thread)
{
    _threads->remove_all(thread);

    logger_trace("Unregistering {}", thread.necked());
}

uintptr_t shedule(uintptr_t stack_pointer)
{
    if (_running_thread.necked() == NULL)
    {
        return stack_pointer;
    }

    _running_thread->stack().set_pointer(stack_pointer);

    _running_thread = _threads->peek_and_pushback();

    logger_info("Running thread is now {}", _running_thread.necked());

    return _running_thread->stack().get_pointer();
}

} // namespace system::sheduling