#include "uml/managers/abstractManager.h"
#include "uml/umlPtr.h"

namespace UML {

Element* AbstractManager::get(Element* me, ID theID) {
    if (!theID.isNull()) {
        if (me->m_node) {
            if (me->m_node->m_references.count(theID)) {
                if (!me->m_node->m_references[theID].node || !me->m_node->m_references[theID].node->m_element) {
                    ElementPtr aquired = get(theID);
                    me->m_node->m_references[theID].node = aquired->m_node;
                }
                return me->m_node->m_references[theID].node->m_element;
            } else {
                throw ManagerStateException("could not find reference " + theID.string());
            }
        } else {
            get(theID);
            return  me->m_node->m_references[theID].node->m_element;
        }
    }
    return 0;
}

}