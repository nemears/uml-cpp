#include "uml/comment.h"

using namespace UML;

void Comment::referencingReleased(ID id) {
    Element::referencingReleased(id);
    m_annotatedElements.release(id);
}

void Comment::referenceReindexed(ID oldID, ID newID) {
    Element::referenceReindexed(oldID, newID);
    m_annotatedElements.reindex(oldID, newID);
}

void Comment::reindexName(ID id, std::string newName) {
    Element::reindexName(id, newName);
    m_annotatedElements.reindexName(id, newName);
}

void Comment::referenceErased(ID id) {
    Element::referenceErased(id);
    m_annotatedElements.eraseElement(id);
}

void Comment::init() {
    m_annotatedElements.m_signature = &Comment::getAnnotatedElements;
}

Comment::Comment() : Element(ElementType::COMMENT) {
    
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