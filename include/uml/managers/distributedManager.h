#ifndef UML_MANAGERS_DISTRIBUTED_MANAGER_H
#define UML_MANAGERS_DISTRIBUTED_MANAGER_H

#define PORT_NUMBER "8652" // roughly translates to umlc

#include "uml/umlManager.h"

namespace UML {

    /**
     * Regular UmlManager will HAVE to own all of the elements in the model, not good when
     * you still want to share resources between threads and applications. This manager
     * solves that problem by binding itself to a port, and passing information between 
     * multiple managers using sockets
     * TODO: IMPLEMENT
     **/
    class DistributedManager : public UmlManager {
        protected:
            int m_serverSocketDescriptor;
        public:
            DistributedManager();
            virtual ~DistributedManager();
            /**
             * This function lets the manager access elements already mounted by another manager
             * this allows for distributed computing with multiple managers interacting with eachother
             * asyncronously
             * @param path, the path of the mounted elements
             **/
            void loadFromMount(std::string path);
    };

}

#endif