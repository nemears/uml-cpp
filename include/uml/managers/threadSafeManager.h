#ifndef _UML_MANAGERS_THREAD_SAFE_MANAGER_H_
#define _UML_MANAGERS_THREAD_SAFE_MANAGER_H_

#include "threadSafeAccessPolicy.h"
#include "serialization/open_uml/openUmlSerializationPolicy.h"
#include "manager.h"

namespace UML {
    typedef Manager<ThreadSafeAccessPolicy, OpenUmlSerializationPolicy> ThreadSafeManager;
}

#endif