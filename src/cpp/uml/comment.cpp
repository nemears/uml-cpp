#include "uml/comment.h"

using namespace UML;

void Comment::referenceReindexed(ID newID) {
    Element::referenceReindexed(newID);
    m_annotatedElements.reindex(newID);
}

void Comment::referenceErased(ID id) {
    Element::referenceErased(id);
    m_annotatedElements.eraseElement(id);
}

void Comment::init() {
    
}

Comment::Comment() : Element(ElementType::COMMENT) {
    init();
}

Comment::~Comment() {
    mountAndRelease();
}

std::string Comment::getBody() {
    return m_body;
}

void Comment::setBody(std::string body) {
    m_body = body;
}

Set<Element, Comment>& Comment::getAnnotatedElements() {
    return m_annotatedElements;
}

bool Comment::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::COMMENT;
    }

    return ret;
}