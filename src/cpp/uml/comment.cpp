#include "uml/comment.h"

using namespace std;
using namespace UML;

string Comment::getBody() {
    return m_body;
}

void Comment::setBody(string body) {
    m_body = body;
}

ElementType Comment::getElementType() const {
    return ElementType::COMMENT;
}

bool Comment::isSubClassOf(ElementType eType) {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::COMMENT;
    }

    return ret;
}