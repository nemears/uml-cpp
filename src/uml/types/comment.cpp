#include "uml/uml-stable.h"

using namespace UML;

Comment::Comment(std::size_t elementType, AbstractManager& manager) : Element(elementType, manager) {
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
