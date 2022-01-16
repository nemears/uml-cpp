#include "uml/comment.h"
#include "uml/uml-stable.h"

using namespace UML;

void Comment::referencingReleased(ID id) {
    Element::referencingReleased(id);
    m_annotatedElements.release(id);
}

void Comment::referenceReindexed(ID oldID, ID newID) {
    Element::referenceReindexed(oldID, newID);
    m_annotatedElements.reindex(oldID, newID);
}

void Comment::reindexName(std::string oldName, std::string newName) {
    Element::reindexName(oldName, newName);
    m_annotatedElements.reindexName(oldName, newName);
}

void Comment::referenceErased(ID id) {
    Element::referenceErased(id);
    m_annotatedElements.eraseElement(id);
}

void Comment::init() {
    m_annotatedElements.m_signature = &Comment::getAnnotatedElements;
}

void Comment::copy(const Comment& rhs) {
    m_annotatedElements = rhs.m_annotatedElements;
}

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