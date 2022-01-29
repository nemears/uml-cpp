#include "uml/umlManager.h"
#include <fstream>
#include "uml/parsers/parser.h"
#include "uml/parsers/emitterMetaData.h"
#include "uml/model.h"
#include <algorithm>
#include "uml/uml-stable.h"

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

void UmlManager::createNode(Element* el) {
    m_graph[el->getID()] = {el};
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
    if (m_graph.count(id)) {
        return true;
    } else {
        return false;
    }
}

Element& UmlManager::get(ID id) {
    return get<>(id);
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
    filesystem::create_directories(path / filesystem::path("mount"));
    if (m_root) {
        addToMount(*m_root);
    }
}

Element* UmlManager::aquire(ID id) {
    if (!m_mountBase.empty()) {
        if (!loaded(id)) {
            if (filesystem::exists(m_mountBase / "mount" / (id.string() + ".yml"))) {
                Parsers::ParserMetaData data(this);
                data.m_path = m_mountBase / "mount" / (id.string() + ".yml");
                data.m_strategy = Parsers::ParserStrategy::INDIVIDUAL;
                Element* ret = Parsers::parse(data);
                if (ret) {
                    ret->m_node->m_managerElementMemory = ret;
                    size_t numEls = ret->m_node->m_referenceOrder.size();
                    for (size_t i = 0; i < numEls; i++) {
                        ID refID = ret->m_node->m_referenceOrder[i];
                        ManagerNode* node = ret->m_node->m_references.at(refID);
                        Element* el = 0;
                        if (!node && loaded(refID)) {
                            try {
                                el = &get<>(refID); // TODO make this faster somehow this line is a real limiter of speed
                                ret->m_node->m_references[refID] = el->m_node;
                            } catch (std::exception e) {
                                // nothing
                            }
                        } else if (node && loaded(refID)) {
                            el =  node->m_managerElementMemory;
                        }
                        if (el) {
                            el->restoreReference(ret);
                            ret->restoreReference(el);
                        }
                    }
                    ret->restoreReferences();
                } else {
                    throw ManagerStateException();
                }
                return ret;
            } else {
                throw ID_doesNotExistException(id);
            }
        } else {
            return &get<>(id);
        }
    } else {
        throw ManagerNotMountedException();
    }
}

void UmlManager::release(ID id) {
    release(*m_graph[id].m_managerElementMemory);
}

void UmlManager::releaseNode(Element& el) {
    ManagerNode* node = el.m_node;
    ID id = el.getID();
    if (node->m_managerElementMemory) {
        delete node->m_managerElementMemory;
    }
    for (auto& e : node->m_references) {
        if (!e.second && loaded(e.first)) {
            e.second = get(e.first).m_node; // slow :(
        }
        if (e.second) {
            e.second->m_managerElementMemory->referencingReleased(id);
        }
    }
    for (auto& copy : node->m_copies) {
        // TODO show warning, bad practice to release without destroying all copies
        // effecctively dereferences the copies from the manager
        copy->m_node = 0;
        copy->m_manager = 0;
    }
    m_graph.erase(id);
}

void UmlManager::release(Element& el) {
    if (!m_mountBase.empty()) {
        Parsers::EmitterMetaData data = {m_mountBase / "mount",
                                         Parsers::EmitterStrategy::INDIVIDUAL,
                                         el.getID().string() + ".yml", this};
        Parsers::emitToFile(*el.m_node->m_managerElementMemory, data, data.m_path.string(), data.m_fileName);
        releaseNode(el);
    } else {
        throw ManagerNotMountedException();
    }
}

void UmlManager::eraseNode(ManagerNode* node, ID id) {
    for (size_t i = 0; i < node->m_referenceOrder.size(); i++) {
        if (!node->m_references[node->m_referenceOrder[i]]->m_managerElementMemory) {
            aquire(node->m_referenceOrder[i]);
        }
        node->m_references[node->m_referenceOrder[i]]->m_managerElementMemory->referenceErased(id);
    }
    if (node->m_managerElementMemory) {
        delete node->m_managerElementMemory;
    }
    if (node->m_copies.size() > 0) {
        // TODO warning
        for (auto& copy : node->m_copies) {
            copy->m_node = 0;
        }
    }
    m_graph.erase(id);
    m_elements.erase(std::find(m_elements.begin(), m_elements.end(), id));
    if (!m_mountBase.empty()) {
        filesystem::remove_all(m_mountBase / (id.string() + ".yml"));
    }
}

void UmlManager::erase(ID id) {
    eraseNode(&m_graph[id], id);
}

void UmlManager::erase(Element& el) {
    eraseNode(el.m_node, el.getID());
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
    if (!getRoot()) {
       setRoot(el);
    }
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
        // m_model = dynamic_cast<Model*>(el);
    }
}

void UmlManager::setRoot(Element& el) {
    setRoot(&el);
}

Element* UmlManager::getRoot() {
    return m_root;
}

void UmlManager::setPath(ID elID, string path) {
    m_graph[elID].m_path = path;
}