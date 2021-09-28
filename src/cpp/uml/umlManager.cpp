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

bool UmlManager::loaded(ID id) {
    if (count(id)) {
        return m_graph[id].m_managerElementMemory;
    } else {
        return false;
    }
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
        ManagerNode& discRef = m_graph[oldID];
        m_graph[newID] = discRef;
        ManagerNode* newDisc = &m_graph[newID];
        newDisc->m_managerElementMemory = discRef.m_managerElementMemory;
        for (auto& ref : newDisc->m_references) {
            if (ref.second->m_references.count(oldID)) {
                ref.second->m_references.erase(oldID);
                ref.second->m_references[newID] = newDisc;
                (*find(ref.second->m_referenceOrder.begin(), ref.second->m_referenceOrder.end(), oldID)) = newID;
                size_t count = ref.second->m_referenceCount[oldID];
                ref.second->m_referenceCount[newID] = count;
                ref.second->m_referenceCount.erase(oldID);
            }
            if (!ref.second->m_managerElementMemory) {
                aquire(ref.first);
            }
            ref.second->m_managerElementMemory->referenceReindexed(oldID, newID);
        }
        newDisc->m_managerElementMemory->m_node = newDisc;
        for (auto& copy : newDisc->m_copies) {
            copy->m_node = newDisc;
        }
        m_graph.erase(oldID);
        if (!m_mountBase.empty()) {
            filesystem::remove(m_mountBase / "mount" / (oldID.string() + ".yml"));
            /** Should we write to new file now?**/
        }
    }
}

void UmlManager::addToMount(Element& el) {
    if (!el.m_node->m_mountedFlag) {
        Parsers::EmitterMetaData data = { m_mountBase / filesystem::path("mount"),
                                         Parsers::EmitterStrategy::INDIVIDUAL,
                                         el.getID().string() + ".yml",
                                         this };
        el.m_node->m_mountedFlag = true;
        Parsers::emitToFile(el, data, data.m_path.string(), data.m_fileName);
        for (auto& child : el.getOwnedElements()) {
            addToMount(child);
        }
    }
}

void UmlManager::mount(string path) {
    m_mountBase = path;
    if (m_root) {
        filesystem::create_directories(path / filesystem::path("mount"));
        addToMount(*m_root);
    } else {
        // TODO throw error
    }
}

Element* UmlManager::aquire(ID id) {
    if (!m_mountBase.empty()) {
        Parsers::ParserMetaData data(this);
        data.m_path = m_mountBase / "mount" / (id.string() + ".yml");
        data.m_strategy = Parsers::ParserStrategy::INDIVIDUAL;
        Element* ret = Parsers::parse(data);
        m_graph[id].m_managerElementMemory = ret;
        ret->restoreReferences();
        return ret;
    } else {
        throw ManagerNotMountedException();
    }
}

void UmlManager::release(ID id) {
    release(*m_graph[id].m_managerElementMemory);
}

void UmlManager::release(Element& el) {
    if (!m_mountBase.empty()) {
        Parsers::EmitterMetaData data = {m_mountBase / "mount",
                                         Parsers::EmitterStrategy::INDIVIDUAL,
                                         el.getID().string() + ".yml", this};
         Parsers::emitToFile(*el.m_node->m_managerElementMemory, data, data.m_path.string(), data.m_fileName);
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
        for (auto& copy : node->m_copies) {
            // TODO show warning, bad practice to release without destroying all copies
            // effecctively dereferences the copies from the manager
            copy->setManager(0);
        }
        m_graph.erase(id);
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
    data.m_fileName = m_path.filename().string();
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