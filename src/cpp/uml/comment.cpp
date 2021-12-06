#include "uml/comment.h"
#include "uml/uml-stable.h"

using namespace UML;

Comment::Comment() : Element(ElementType::COMMENT) {
    
}

Comment::Comment(const Comment& rhs) : Element(rhs, ElementType::COMMENT) {
    Element::copy(rhs);
}

std::string Comment::getBody() {
    return m_body;
}

void Comment::setBody(std::string body) {
    m_body = body;
}

bool Comment::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::COMMENT;
    }

    return ret;
}