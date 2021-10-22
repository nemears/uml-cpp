#ifndef UML_MANAGERS_DISTRIBUTED_MANAGER_H
#define UML_MANAGERS_DISTRIBUTED_MANAGER_H

#include "uml/umlManager.h"

namespace UML {

    class DistributedManager : public UmlManager {
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