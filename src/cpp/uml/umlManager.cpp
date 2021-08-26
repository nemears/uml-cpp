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
    for (auto& e : m_graph) {
        if (e.second.m_managerElementMemory) {
            delete e.second.m_managerElementMemory;
        }
    }
    m_graph.clear();
    m_elements.clear();
}

UmlManager::UmlManager() {
    m_model = 0;
    m_root = 0;
}

UmlManager::~UmlManager() {
    clear();
    if (!m_mountBase.empty()) {
        filesystem::remove_all(m_mountBase / "mount");
    }
}

size_t UmlManager::count(ID id) {
    return m_elements.count(id);
}

void UmlManager::reindex(ID oldID, ID newID) {
    if (m_elements.count(newID)) {
        // Element with this ID already exists, overwrite it with new one
        // This logic should only be called when it is loading from disk
        // and will overwrite the existing element in memory with one from disk
        // during a UmlManager::open() or UmlManager::aquire(id) invoke
        
        ManagerNode* m_node = &m_graph[newID];
        delete m_node->m_managerElementMemory;
        m_node->m_managerElementMemory = m_graph[oldID].m_managerElementMemory;
        m_node->m_managerElementMemory->m_node = m_node;
        for (auto& countPair : m_node->m_referenceCount) {
            countPair.second = 0;
        }
        m_elements.erase(oldID);
        m_graph.erase(oldID);
    } else  {
        // reindex
        m_elements.erase(oldID);
        m_elements.insert(newID);
        ManagerNode disc = m_graph[oldID];
        m_graph.erase(oldID);
        m_graph[newID] = disc;
    }
}

void UmlManager::setElementAndChildrenMount(filesystem::path parentPath, Element& el) {
    m_graph[el.getID()].m_mountPath = parentPath / (el.getID().string() + ".yml");
    for (auto& child : el.getOwnedElements()) {
        setElementAndChildrenMount(parentPath, child);
    }
}

void UmlManager::mount(string path) {
    m_mountBase = path;
    if (m_root) {
        filesystem::create_directories(path / filesystem::path("mount"));
        setElementAndChildrenMount(path / filesystem::path("mount"), *m_root);
        Parsers::EmitterMetaData data = {path / filesystem::path("mount"), 
                                         Parsers::EmitterStrategy::COMPOSITE, 
                                         m_root->getID().string() + ".yml",
                                         this};
        Parsers::emit(data);
    } else {
        // TODO throw error
    }
}

void UmlManager::aquire(ID id) {
    if (!m_mountBase.empty()) {
        Parsers::ParserMetaData data(this);
        data.m_path = m_graph[id].m_mountPath;
        data.m_strategy = Parsers::ParserStrategy::INDIVIDUAL;
        m_graph[id].m_managerElementMemory = Parsers::parse(data);
        for (auto& refID : m_graph[id].m_referenceOrder) {
            if (m_graph[id].m_references[refID]->m_managerElementMemory) {
                m_graph[id].m_references[refID]->m_managerElementMemory->restoreReleased(id, m_graph[id].m_managerElementMemory);
            }
        }
    } else {
        throw ManagerNotMountedException();
    }
}

void UmlManager::release(ID id) {
    release(*m_graph[id].m_managerElementMemory);
}

void UmlManager::release(Element& el) {
    if (!m_mountBase.empty()) {
        Parsers::EmitterMetaData data = {filesystem::path(el.m_node->m_mountPath).parent_path(),
                                         Parsers::EmitterStrategy::INDIVIDUAL,
                                         filesystem::path(el.m_node->m_mountPath).filename(), this};
        Parsers::emitToFile(*el.m_node->m_managerElementMemory, data, data.m_path, data.m_fileName);
        ManagerNode* node = el.m_node;
        ID id = el.getID();
        if (node->m_managerElementMemory) {
            delete node->m_managerElementMemory;
        }
        for (auto& e : node->m_references) {
            if (e.second->m_managerElementMemory) {
                e.second->m_managerElementMemory->referencingReleased(id);
            }
        }
        node->m_managerElementMemory = 0;
    } else {
        throw ManagerNotMountedException();
    }
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
    clear();
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
    m_graph[elID].m_path = path;
}