#include "uml/association.h"

using namespace UML;

Association::Association() {
    m_memberEnds = new Sequence<Property>;
    m_ownedEnds = new Sequence<Property>;
    m_navigableOwnedEnds = new Sequence<Property>;
}

Association::~Association() {
    delete m_memberEnds;
    delete m_ownedEnds;
    delete m_navigableOwnedEnds;
}

Sequence<Property>& Association::getMemberEnds() {
    return *m_memberEnds;
}

Sequence<Property>& Association::getOwnedEnds() {
    return *m_ownedEnds;
}

Sequence<Property>& Association::getNavigableOwnedEnds() {
    return *m_navigableOwnedEnds;
}

ElementType Association::getElementType() {
    return ElementType::ASSOCIATION;
}

bool Association::isSubClassOf(ElementType eType) {
    bool ret = Relationship::isSubClassOf(eType);

    if (!ret) {
        ret = Classifier::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::ASSOCIATION;
    }

    return ret;
}