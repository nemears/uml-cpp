#include "uml/umlManager.h"
#include <fstream>
#include "uml/parsers/parser.h"
#include "uml/parsers/emitterMetaData.h"
#include "uml/callBehaviorAction.h"
#include "uml/controlFlow.h"
#include "uml/createObjectAction.h"
#include "uml/decisionNode.h"
#include "uml/finalNode.h"
#include "uml/forkNode.h"
#include "uml/initialNode.h"
#include "uml/joinNode.h"
#include "uml/mergeNode.h"
#include "uml/parameterNode.h"
#include "uml/model.h"

using namespace std;
using namespace UML;

void UmlManager::clear() {
    for (auto& e : m_loaded) {
        delete e.second;
    }
}

UmlManager::UmlManager() {
    m_model = 0;
    m_root = 0;
}

UmlManager::~UmlManager() {
    clear();
}

size_t UmlManager::count(ID id) {
    return m_elements.count(id);
}

void UmlManager::reindex(ID oldID, ID newID) {
    if (m_elements.count(newID)) {
        // Element with this ID already exists, overwrite it with new one
        // This logic should only be called when it is loading from disk
        // and will overwrite the existing element in memory with one from disk
        // during a UmlManager::save() invoke

        delete m_loaded[newID];
        m_loaded[newID] = m_loaded[oldID];
        m_elements.erase(oldID);
        m_loaded.erase(oldID);
        m_disc.erase(oldID);
    } else  {
        // reindex
        m_elements.erase(oldID);
        m_elements.insert(newID);
        Element* el = m_loaded[oldID];
        m_loaded.erase(oldID);
        m_loaded[newID] = el;
        DiscData disc = m_disc[oldID];
        m_disc.erase(oldID);
        m_disc[newID] = disc;
    }
}

void UmlManager::mount(string path) {
    m_mountBase = path;
    if (m_root) {
        Parsers::EmitterMetaData data = {path, Parsers::EmitterStrategy::COMPOSITE, m_mountBase.filename(), this};
        Parsers::emit(data);
    } else {
        // TODO throw error
    }
}

void UmlManager::aquire(ID id) {
    // TODO get path
    Parsers::ParserMetaData data(this);
    data.m_path = m_disc[id].m_path;
    m_loaded[id] = Parsers::parse(data);
}

void UmlManager::release(ID id) {
    // Parsers::emit(m_loaded[id])
}

void UmlManager::save() {
    if (m_path.empty() || !m_root) {
        // TODO throw error
        return;
    }
    Parsers::EmitterMetaData data;
    data.m_manager =  this;
    data.m_strategy = Parsers::EmitterStrategy::WHOLE;
    data.m_path = m_path.parent_path();
    data.m_fileName = m_path.filename();
    Parsers::emit(data);
}

void UmlManager::save(string path) {
    m_path = path;
    save();
}

void UmlManager::open() {
    if (m_path.empty()) {
        // todo thow error
        return;
    }
    //clear();
    Parsers::ParserMetaData data(this);
    m_root = Parsers::parse(data);
    if (m_root->isSubClassOf(ElementType::MODEL)) {
        m_model = dynamic_cast<Model*>(m_root);
    }
}

void UmlManager::open(string path) {
    m_path = path;
    open();
}

Element* UmlManager::parse(string path) {
    m_path = path;
    Parsers::ParserMetaData data(this);
    Element* el = Parsers::parse(data);
    return el;
}

void UmlManager::setModel(Model* model) {
    m_model = model;
    m_root = model;
}

Model* UmlManager::getModel() {
    return m_model;
}

void UmlManager::setRoot(Element* el) {
    m_root = el;
    if (m_root->isSubClassOf(ElementType::MODEL)) {
        m_model = dynamic_cast<Model*>(el);
    }
}

Element* UmlManager::getRoot() {
    return m_root;
}

void UmlManager::setPath(ID elID, string path) {
    m_disc[elID].m_path = path;
}