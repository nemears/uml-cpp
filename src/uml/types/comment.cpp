#include "uml/types/comment.h"

using namespace UML;

void Comment::referenceErased(ID id) {
    Element::referenceErased(id);
    eraseFromSet(id, m_annotatedElements);
}

Comment::Comment() : Element(ElementType::COMMENT) {
    
}

Comment::~Comment() {
    
}

std::string Comment::getBody() const {
    return m_body;
}

void Comment::setBody(const std::string& body) {
    m_body = body;
}

Set<Element, Comment>& Comment::getAnnotatedElements() {
    return m_annotatedElements;
}

bool Comment::is(ElementType eType) const {
    bool ret = Element::is(eType);

    if (!ret) {
        ret = eType == ElementType::COMMENT;
    }

    return ret;
}
