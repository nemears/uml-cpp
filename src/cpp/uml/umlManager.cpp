#include "uml/umlManager.h"
#include "uml/set.h"
#include <fstream>
#include "uml/parsers/parser.h"
#include "uml/parsers/emitterMetaData.h"
#include "uml/umlPtr.h"
// #include "uml/model.h"
#include <algorithm>

using namespace UML;

void UmlManager::clear() {
    for (const ID id : m_elements) {
        delete m_graph[id].m_managerElementMemory;
    }
    m_graph.clear();
    m_elements.clear();
}

void UmlManager::createNode(Element* el) {
    m_graph[el->getID()] = {el};
}

UmlManager::UmlManager() {

}

UmlManager::~UmlManager() {
    clear();
    if (!m_mountBase.empty()) {
        std::filesystem::remove_all(m_mountBase / "mount");
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

// Element& UmlManager::create(ElementType eType) {
//     switch (eType) {
//         case ElementType::ABSTRACTION : {
//             return static_cast<Element&>(create<Abstraction>());
//         }
//         case ElementType::ARTIFACT : {
//             return static_cast<Element&>(create<Artifact>());
//         }
//         case ElementType::ASSOCIATION : {
//             return static_cast<Element&>(create<Association>());
//         }
//         case ElementType::CLASS : {
//             return static_cast<Element&>(create<Class>());
//         }
//         case ElementType::COMMENT : {
//             return static_cast<Element&>(create<Comment>());
//         }
//         case ElementType::CONNECTOR : {
//             return static_cast<Element&>(create<Connector>());
//         }
//         case ElementType::CONNECTOR_END : {
//             return static_cast<Element&>(create<ConnectorEnd>());
//         }
//         case ElementType::DATA_TYPE : {
//             return static_cast<Element&>(create<DataType>());
//         }
//         case ElementType::DEPENDENCY : {
//             return static_cast<Element&>(create<Dependency>());
//         }
//         case ElementType::DEPLOYMENT : {
//             return static_cast<Element&>(create<Deployment>());
//         }
//         case ElementType::ENUMERATION : {
//             return static_cast<Element&>(create<Enumeration>());
//         }
//         case ElementType::ENUMERATION_LITERAL : {
//             return static_cast<Element&>(create<EnumerationLiteral>());
//         }
//         case ElementType::EXPRESSION : {
//             return static_cast<Element&>(create<Expression>());
//         }
//         case ElementType::EXTENSION : {
//             return static_cast<Element&>(create<Extension>());
//         }
//         case ElementType::EXTENSION_END : {
//             return static_cast<Element&>(create<ExtensionEnd>());
//         }
//         case ElementType::GENERALIZATION : {
//             return static_cast<Element&>(create<Generalization>());
//         }
//         case ElementType::GENERALIZATION_SET : {
//             return static_cast<Element&>(create<GeneralizationSet>());
//         }
//         case ElementType::INSTANCE_SPECIFICATION : {
//             return static_cast<Element&>(create<InstanceSpecification>());
//         }
//         case ElementType::INSTANCE_VALUE : {
//             return static_cast<Element&>(create<InstanceValue>());
//         }
//         case ElementType::INTERFACE : {
//             return static_cast<Element&>(create<Interface>());
//         }
//         case ElementType::INTERFACE_REALIZATION : {
//             return static_cast<Element&>(create<InterfaceRealization>());
//         }
//         case ElementType::LITERAL_BOOL : {
//             return static_cast<Element&>(create<LiteralBool>());
//         }
//         case ElementType::LITERAL_INT : {
//             return static_cast<Element&>(create<LiteralInt>());
//         }
//         case ElementType::LITERAL_NULL : {
//             return static_cast<Element&>(create<LiteralNull>());
//         }
//         case ElementType::LITERAL_REAL : {
//             return static_cast<Element&>(create<LiteralReal>());
//         }
//         case ElementType::LITERAL_STRING : {
//             return static_cast<Element&>(create<LiteralString>());
//         }
//         case ElementType::LITERAL_UNLIMITED_NATURAL : {
//             return static_cast<Element&>(create<LiteralUnlimitedNatural>());
//         }
//         case ElementType::MANIFESTATION : {
//             return static_cast<Element&>(create<Manifestation>());
//         }
//         case ElementType::MODEL : {
//             return static_cast<Element&>(create<Model>());
//         }
//         case ElementType::OPAQUE_BEHAVIOR : {
//             return static_cast<Element&>(create<OpaqueBehavior>());
//         }
//         case ElementType::OPERATION : {
//             return static_cast<Element&>(create<Operation>());
//         }
//         case ElementType::PACKAGE : {
//             return static_cast<Element&>(create<Package>());
//         }
//         case ElementType::PACKAGE_MERGE : {
//             return static_cast<Element&>(create<PackageMerge>());
//         }
//         case ElementType::PARAMETER : {
//             return static_cast<Element&>(create<Parameter>());
//         }
//         case ElementType::PORT : {
//             return static_cast<Element&>(create<Port>());
//         }
//         case ElementType::PRIMITIVE_TYPE : {
//             return static_cast<Element&>(create<PrimitiveType>());
//         }
//         case ElementType::PROFILE : {
//             return static_cast<Element&>(create<Profile>());
//         }
//         case ElementType::PROFILE_APPLICATION : {
//             return static_cast<Element&>(create<ProfileApplication>());
//         }
//         case ElementType::PROPERTY : {
//             return static_cast<Element&>(create<Property>());
//         }
//         case ElementType::REALIZATION : {
//             return static_cast<Element&>(create<Realization>());
//         }
//         case ElementType::RECEPTION : {
//             return static_cast<Element&>(create<Reception>());
//         }
//         case ElementType::SIGNAL : {
//             return static_cast<Element&>(create<Signal>());
//         }
//         case ElementType::SLOT : {
//             return static_cast<Element&>(create<Slot>());
//         }
//         case ElementType::STEREOTYPE : {
//             return static_cast<Element&>(create<Stereotype>());
//         }
//         case ElementType::TEMPLATE_BINDING : {
//             return static_cast<Element&>(create<TemplateBinding>());
//         }
//         case ElementType::TEMPLATE_PARAMETER : {
//             return static_cast<Element&>(create<TemplateParameter>());
//         }
//         case ElementType::TEMPLATE_PARAMETER_SUBSTITUTION : {
//             return static_cast<Element&>(create<TemplateParameterSubstitution>());
//         }
//         case ElementType::TEMPLATE_SIGNATURE : {
//             return static_cast<Element&>(create<TemplateSignature>());
//         }
//         case ElementType::USAGE : {
//             return static_cast<Element&>(create<Usage>());
//         }
//         default : {
//             throw ManagerStateException("Could not POST element with unmapped element type: " + Element::elementTypeToString(eType));
//         }
//     }
// }

Element& UmlManager::get(ID id) {
    if (m_elements.count(id)) {
        if (!m_graph.count(id)) {
            aquire(id);
        }
        return *m_graph[id].m_managerElementMemory;
    } else {
        throw UnknownID_Exception(id);
    }
}

void UmlManager::lossless(bool lossless) {
    m_lossless = lossless;
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
        for (auto& ptr : m_graph[oldID].m_ptrs) {
            static_cast<AbstractUmlPtr*>(ptr)->reindex(newID, m_node->m_managerElementMemory);
            m_node->m_ptrs.push_back(ptr);
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
        m_graph.erase(oldID);
        if (!m_mountBase.empty()) {
            std::filesystem::remove(m_mountBase / "mount" / (oldID.string() + ".yml"));
            /** Should we write to new file now?**/
        }
    }
}

void UmlManager::addToMount(Element& el) {
    if (!el.m_node->m_mountedFlag) {
        mountEl(el);
        for (auto& child : el.getOwnedElements()) {
            addToMount(child);
        }
    }
}

void UmlManager::mountEl(Element& el) {
    if (!m_mountBase.empty()) {
        Parsers::EmitterMetaData data = { m_mountBase / std::filesystem::path("mount"),
                                            Parsers::EmitterStrategy::INDIVIDUAL,
                                            el.getID().string() + ".yml",
                                            this };
        el.m_node->m_mountedFlag = true;
        Parsers::emitToFile(el, data, data.m_path.string(), data.m_fileName);
    }
}

void UmlManager::mount(std::string path) {
    m_mountBase = path;
    std::filesystem::create_directories(path / std::filesystem::path("mount"));
    if (m_root) {
        addToMount(*m_root);
    }
}

ElementPtr UmlManager::aquire(ID id) {
    if (!m_mountBase.empty()) {
        if (!loaded(id)) {
            if (std::filesystem::exists(m_mountBase / "mount" / (id.string() + ".yml"))) {
                Parsers::ParserMetaData data(this);
                data.m_path = m_mountBase / "mount" / (id.string() + ".yml");
                data.m_strategy = Parsers::ParserStrategy::INDIVIDUAL;
                ElementPtr ret = Parsers::parse(data);
                if (ret) {
                    ret->m_node->m_managerElementMemory = &*ret;
                    size_t numEls = ret->m_node->m_referenceOrder.size();
                    for (size_t i = 0; i < numEls; i++) {
                        ID refID = ret->m_node->m_referenceOrder[i];
                        ManagerNode* node = ret->m_node->m_references.at(refID);
                        Element* el = 0;
                        if (!node && loaded(refID)) {
                            try {
                                el = &get(refID); // TODO make this faster somehow this line is a real limiter of speed
                                ret->m_node->m_references[refID] = el->m_node;
                            } catch (std::exception e) {
                                // nothing
                            }
                        } else if (node && loaded(refID)) {
                            el =  node->m_managerElementMemory;
                        }
                        if (el) {
                            el->restoreReference(ret.ptr());
                            ret->restoreReference(el);
                        }
                    }
                    for (auto& ptr : ret->m_node->m_ptrs) {
                        static_cast<AbstractUmlPtr*>(ptr)->reindex(id, ret.ptr());
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
            return ElementPtr(&get(id));
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
    for (auto& e : node->m_references) {
        if (!e.second && loaded(e.first)) {
            e.second = &m_graph[e.first]; // slow :(
        }
        if (e.second) {
            e.second->m_managerElementMemory->referencingReleased(id);
        }
    }
    for (auto& ptr : node->m_ptrs) {
        static_cast<AbstractUmlPtr*>(ptr)->releasePtr();
    }
}

void UmlManager::release(Element& el) {
    if (!m_mountBase.empty()) {
        ID elID = el.getID();
        mountEl(el);
        releaseNode(el);
        delete &el;
        m_graph.erase(elID);
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
    delete node->m_managerElementMemory;
    m_graph.erase(id);
    m_elements.erase(std::find(m_elements.begin(), m_elements.end(), id));
    if (!m_mountBase.empty()) {
        std::filesystem::remove_all(m_mountBase / (id.string() + ".yml"));
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

void UmlManager::save(std::string path) {
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
    m_root = &*Parsers::parse(data);
    // if (m_root->isSubClassOf(ElementType::MODEL)) {
    //     m_model = dynamic_cast<Model*>(m_root);
    // }
}

void UmlManager::open(std::string path) {
    m_path = path;
    open();
}

ElementPtr UmlManager::parse(std::string path) {
    m_path = path;
    Parsers::ParserMetaData data(this);
    ElementPtr el = Parsers::parse(data);
    if (!getRoot()) {
       setRoot(*el);
    }
    return el;
}

// void UmlManager::setModel(Model* model) {
//     m_model = model;
//     // m_root = model;
// }

// Model* UmlManager::getModel() {
//     return m_model;
// }

void UmlManager::setRoot(Element* el) {
    m_root = el;
    if (m_root->isSubClassOf(ElementType::MODEL)) {
        // m_model = dynamic_cast<Model*>(el);
    }
}

void UmlManager::setRoot(Element& el) {
    setRoot(&el);
}

ElementPtr UmlManager::getRoot() {
    return m_root;
}

void UmlManager::setPath(ID elID, std::string path) {
    m_graph[elID].m_path = path;
}

Element* UmlManager::get(Element* me, ID theID) {
    if (!theID.isNull()) {
        if (me->m_node) {
            if (me->m_node->m_references.count(theID)) {
                if (!me->m_node->m_references[theID]) {
                    if (loaded(theID)) {
                        me->restoreReference(&get(theID));
                    } else {
                        ElementPtr aquired = aquire(theID);
                        me->m_node->m_references[theID] = aquired->m_node;
                    }
                }
                return me->m_node->m_references[theID]->m_managerElementMemory;
            } else {
                throw ManagerStateException();
            }
        } else {
            aquire(theID);
            return  me->m_node->m_references[theID]->m_managerElementMemory;
        }
    }
    return 0;
}

void UmlManager::lazy(bool lazy) {
    m_lazy = lazy;
}