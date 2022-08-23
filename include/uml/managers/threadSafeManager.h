#ifndef _UML_MANAGERS_THREAD_SAFE_MANAGER_H_
#define _UML_MANAGERS_THREAD_SAFE_MANAGER_H_

#include "threadSafeAccessPolicy.h"
#include "filePersistencePolicy.h"
#include "managerPolicy.h"

namespace UML {
    typedef Manager<ThreadSafeAccessPolicy, FilePersistencePolicy> ThreadSafeManager;
}

#endif