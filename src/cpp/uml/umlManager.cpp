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

void UmlManager::setReference(ID referencing, ID referenced, Element* ptr) {
    if (m_disc[referenced].m_references.count(referencing)) {
        m_disc[referenced].m_referenceCount[referencing]++;
    } else {
        m_disc[referenced].m_references[referencing] = ptr;
        m_disc[referenced].m_referenceCount[referencing] = 1;
    }
}

void UmlManager::removeReference(ID referencing, ID referenced) {
    if (m_disc[referenced].m_referenceCount[referencing] > 1) {
        m_disc[referenced].m_referenceCount[referencing]--;
    } else {
        m_disc[referenced].m_references.erase(referencing);
        m_disc[referenced].m_referenceCount.erase(referencing);
    }
}

void UmlManager::clear() {
    for (auto& e : m_disc) {
        if (e.second.m_managerElementMemory) {
            delete e.second.m_managerElementMemory;
        }
    }
    m_disc.clear();
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

        delete m_disc[newID].m_managerElementMemory;
        m_disc[newID].m_managerElementMemory = m_disc[oldID].m_managerElementMemory;
        m_elements.erase(oldID);
        m_disc.erase(oldID);
    } else  {
        // reindex
        m_elements.erase(oldID);
        m_elements.insert(newID);
        DiscData disc = m_disc[oldID];
        m_disc.erase(oldID);
        m_disc[newID] = disc;
    }
}

void UmlManager::setElementAndChildrenMount(filesystem::path parentPath, Element& el) {
    filesystem::create_directories(parentPath / el.getID().string());
    m_disc[el.getID()].m_mountPath = parentPath / el.getID().string() / (el.getID().string() + ".yml");
    for (auto& child : el.getOwnedElements()) {
        setElementAndChildrenMount(parentPath / el.getID().string(), child);
    }
}

void UmlManager::mount(string path) {
    m_mountBase = path;
    if (m_root) {
        filesystem::create_directories(path / filesystem::path("mount") / m_root->getID().string());
        setElementAndChildrenMount(path / filesystem::path("mount"), *m_root);
        Parsers::EmitterMetaData data = {path / filesystem::path("mount") / m_root->getID().string(), 
                                         Parsers::EmitterStrategy::COMPOSITE, 
                                         m_root->getID().string() + ".yml", this};
        Parsers::emit(data);
    } else {
        // TODO throw error
    }
}

void UmlManager::aquire(ID id) {
    if (!m_mountBase.empty()) {
        Parsers::ParserMetaData data(this);
        data.m_path = m_disc[id].m_mountPath;
        data.m_strategy = Parsers::ParserStrategy::INDIVIDUAL;
        m_disc[id].m_managerElementMemory = Parsers::parse(data);
        for (auto& ref : m_disc[id].m_references) {
            if (ref.second) {
                ref.second->restoreReleased(id, m_disc[id].m_managerElementMemory);
            }
        }
    } else {
        // TODO throw error
    }
}

void UmlManager::release(ID id) {
    if (!m_mountBase.empty()) {
        Parsers::EmitterMetaData data = {filesystem::path(m_disc[id].m_mountPath).parent_path(),
                                         Parsers::EmitterStrategy::INDIVIDUAL,
                                         filesystem::path(m_disc[id].m_mountPath).filename(), this};
        Parsers::emitToFile(*m_disc[id].m_managerElementMemory, data, data.m_path, data.m_fileName);
        if (m_disc[id].m_managerElementMemory) {
            delete m_disc[id].m_managerElementMemory;
        }
        for (auto& e : m_disc[id].m_references) {
            e.second->referencingReleased(id);
        }
        m_disc[id].m_managerElementMemory = 0;
    } else {
        // TODO throw error
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
    m_disc[elID].m_path = path;
}