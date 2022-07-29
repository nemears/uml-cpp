#include "uml/umlManager.h"
#include "uml/set.h"
#include <fstream>
#include "uml/parsers/parser.h"
#include "uml/parsers/emitterMetaData.h"
#include "uml/umlPtr.h"
#include "uml/uml-stable.h"
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
    std::unordered_map<ID, ManagerNode>::const_iterator result = m_graph.find(id);
    return result != m_graph.end() && (*result).second.m_managerElementMemory;
}

Element& UmlManager::create(ElementType eType) {
    switch (eType) {
        case ElementType::ABSTRACTION : {
            return static_cast<Element&>(*create<Abstraction>());
        }
        case ElementType::ACTION_INPUT_PIN : {
            return static_cast<Element&>(*create<ActionInputPin>());
        }
        case ElementType::ACTIVITY : {
            return static_cast<Element&>(*create<Activity>());
        }
        case ElementType::ACTIVITY_FINAL_NODE : {
            return static_cast<Element&>(*create<ActivityFinalNode>());
        }
        case ElementType::ACTIVITY_PARAMETER_NODE : {
            return static_cast<Element&>(*create<ActivityParameterNode>());
        }
        case ElementType::ACTIVITY_PARTITION : {
            return static_cast<Element&>(*create<ActivityPartition>());
        }
        case ElementType::ARTIFACT : {
            return static_cast<Element&>(*create<Artifact>());
        }
        case ElementType::ASSOCIATION : {
            return static_cast<Element&>(*create<Association>());
        }
        case ElementType::CALL_BEHAVIOR_ACTION : {
            return static_cast<Element&>(*create<CallBehaviorAction>());
        }
        case ElementType::CENTRAL_BUFFER_NODE : {
            return static_cast<Element&>(*create<CentralBufferNode>());
        }
        case ElementType::CLASS : {
            return static_cast<Element&>(*create<Class>());
        }
        case ElementType::CLASSIFIER_TEMPLATE_PARAMETER : {
            return static_cast<Element&>(*create<ClassifierTemplateParameter>());
        }
        case ElementType::COMMENT : {
            return static_cast<Element&>(*create<Comment>());
        }
        case ElementType::CONNECTOR : {
            return static_cast<Element&>(*create<Connector>());
        }
        case ElementType::CONNECTOR_END : {
            return static_cast<Element&>(*create<ConnectorEnd>());
        }
        case ElementType::CONSTRAINT : {
            return static_cast<Element&>(*create<Constraint>());
        }
        case ElementType::CONTROL_FLOW : {
            return static_cast<Element&>(*create<ControlFlow>());
        }
        case ElementType::DATA_STORE_NODE : {
            return static_cast<Element&>(*create<DataStoreNode>());
        }
        case ElementType::DATA_TYPE : {
            return static_cast<Element&>(*create<DataType>());
        }
        case ElementType::DECISION_NODE : {
            return static_cast<Element&>(*create<DecisionNode>());
        }
        case ElementType::DEPENDENCY : {
            return static_cast<Element&>(*create<Dependency>());
        }
        case ElementType::DEPLOYMENT : {
            return static_cast<Element&>(*create<Deployment>());
        }
        case ElementType::ELEMENT_IMPORT : {
            return static_cast<ElementImport&>(*create<ElementImport>());
        }
        case ElementType::ENUMERATION : {
            return static_cast<Element&>(*create<Enumeration>());
        }
        case ElementType::ENUMERATION_LITERAL : {
            return static_cast<Element&>(*create<EnumerationLiteral>());
        }
        case ElementType::EXCEPTION_HANDLER : {
            return static_cast<Element&>(*create<ExceptionHandler>());
        }
        case ElementType::EXPRESSION : {
            return static_cast<Element&>(*create<Expression>());
        }
        case ElementType::EXTENSION : {
            return static_cast<Element&>(*create<Extension>());
        }
        case ElementType::EXTENSION_END : {
            return static_cast<Element&>(*create<ExtensionEnd>());
        }
        case ElementType::FLOW_FINAL_NODE : {
            return static_cast<Element&>(*create<FlowFinalNode>());
        }
        case ElementType::FORK_NODE : {
            return static_cast<Element&>(*create<ForkNode>());
        }
        case ElementType::GENERALIZATION : {
            return static_cast<Element&>(*create<Generalization>());
        }
        case ElementType::GENERALIZATION_SET : {
            return static_cast<Element&>(*create<GeneralizationSet>());
        }
        case ElementType::INSTANCE_SPECIFICATION : {
            return static_cast<Element&>(*create<InstanceSpecification>());
        }
        case ElementType::INITIAL_NODE : {
            return static_cast<Element&>(*create<InitialNode>());
        }
        case ElementType::INPUT_PIN : {
            return static_cast<Element&>(*create<InputPin>());
        }
        case ElementType::INSTANCE_VALUE : {
            return static_cast<Element&>(*create<InstanceValue>());
        }
        case ElementType::INTERFACE_UML : {
            return static_cast<Element&>(*create<Interface>());
        }
        case ElementType::INTERRUPTIBLE_ACTIVITY_REGION : {
            return static_cast<Element&>(*create<InterruptibleActivityRegion>());
        }
        case ElementType::INTERFACE_REALIZATION : {
            return static_cast<Element&>(*create<InterfaceRealization>());
        }
        case ElementType::JOIN_NODE : {
            return static_cast<Element&>(*create<JoinNode>());
        }
        case ElementType::LITERAL_BOOL : {
            return static_cast<Element&>(*create<LiteralBool>());
        }
        case ElementType::LITERAL_INT : {
            return static_cast<Element&>(*create<LiteralInt>());
        }
        case ElementType::LITERAL_NULL : {
            return static_cast<Element&>(*create<LiteralNull>());
        }
        case ElementType::LITERAL_REAL : {
            return static_cast<Element&>(*create<LiteralReal>());
        }
        case ElementType::LITERAL_STRING : {
            return static_cast<Element&>(*create<LiteralString>());
        }
        case ElementType::LITERAL_UNLIMITED_NATURAL : {
            return static_cast<Element&>(*create<LiteralUnlimitedNatural>());
        }
        case ElementType::MANIFESTATION : {
            return static_cast<Element&>(*create<Manifestation>());
        }
        case ElementType::MERGE_NODE : {
            return static_cast<Element&>(*create<MergeNode>());
        }
        case ElementType::MODEL : {
            return static_cast<Element&>(*create<Model>());
        }
        case ElementType::OBJECT_FLOW : {
            return static_cast<Element&>(*create<ObjectFlow>());
        }
        case ElementType::OPAQUE_ACTION : {
            return static_cast<OpaqueAction&>(*create<OpaqueAction>());
        }
        case ElementType::OPAQUE_BEHAVIOR : {
            return static_cast<Element&>(*create<OpaqueBehavior>());
        }
        case ElementType::OPERATION : {
            return static_cast<Element&>(*create<Operation>());
        }
        case ElementType::OUTPUT_PIN : {
            return static_cast<Element&>(*create<OutputPin>());
        }
        case ElementType::PACKAGE : {
            return static_cast<Element&>(*create<Package>());
        }
        case ElementType::PACKAGE_IMPORT : {
            return static_cast<Element&>(*create<PackageImport>());
        }
        case ElementType::PACKAGE_MERGE : {
            return static_cast<Element&>(*create<PackageMerge>());
        }
        case ElementType::PARAMETER : {
            return static_cast<Element&>(*create<Parameter>());
        }
        case ElementType::PARAMETER_SET : {
            return static_cast<Element&>(*create<ParameterSet>());
        }
        case ElementType::PORT : {
            return static_cast<Element&>(*create<Port>());
        }
        case ElementType::PRIMITIVE_TYPE : {
            return static_cast<Element&>(*create<PrimitiveType>());
        }
        case ElementType::PROFILE : {
            return static_cast<Element&>(*create<Profile>());
        }
        case ElementType::PROFILE_APPLICATION : {
            return static_cast<Element&>(*create<ProfileApplication>());
        }
        case ElementType::PROPERTY : {
            return static_cast<Element&>(*create<Property>());
        }
        case ElementType::REALIZATION : {
            return static_cast<Element&>(*create<Realization>());
        }
        case ElementType::RECEPTION : {
            return static_cast<Element&>(*create<Reception>());
        }
        case ElementType::REDEFINABLE_TEMPLATE_SIGNATURE : {
            return static_cast<Element&>(*create<RedefinableTemplateSignature>());
        }
        case ElementType::SIGNAL : {
            return static_cast<Element&>(*create<Signal>());
        }
        case ElementType::SLOT : {
            return static_cast<Element&>(*create<Slot>());
        }
        case ElementType::STEREOTYPE : {
            return static_cast<Element&>(*create<Stereotype>());
        }
        case ElementType::TEMPLATE_BINDING : {
            return static_cast<Element&>(*create<TemplateBinding>());
        }
        case ElementType::TEMPLATE_PARAMETER : {
            return static_cast<Element&>(*create<TemplateParameter>());
        }
        case ElementType::TEMPLATE_PARAMETER_SUBSTITUTION : {
            return static_cast<Element&>(*create<TemplateParameterSubstitution>());
        }
        case ElementType::TEMPLATE_SIGNATURE : {
            return static_cast<Element&>(*create<TemplateSignature>());
        }
        case ElementType::USAGE : {
            return static_cast<Element&>(*create<Usage>());
        }
        case ElementType::VALUE_PIN : {
            return static_cast<Element&>(*create<ValuePin>());
        }
        default : {
            throw ManagerStateException("Could not create element with unmapped element type: " + Element::elementTypeToString(eType));
        }
    }
}

Element& UmlManager::get(ID id) {
    if (m_elements.count(id)) {
        std::unordered_map<ID, ManagerNode>::iterator it = m_graph.find(id);
        if (it == m_graph.end() || !(*it).second.m_managerElementMemory) {
            aquire(id);
        }
        return *m_graph[id].m_managerElementMemory;
    } else {
        throw UnknownID_Exception(id);
    }
}

void UmlManager::reindex(ID oldID, ID newID) {
    if (oldID != newID) {
        if (m_elements.count(newID)) {
            // Element with this ID already exists, overwrite it with new one
            // This logic should only be called when it is loading from disk
            // and will overwrite the existing element in memory with one from disk
            // during a UmlManager::open() or UmlManager::aquire(id) invoke
            
            ManagerNode* m_node = &m_graph[newID];
            delete m_node->m_managerElementMemory;
            ManagerNode& oldNode = m_graph[oldID];
            m_node->m_managerElementMemory = oldNode.m_managerElementMemory;
            m_node->m_managerElementMemory->m_node = m_node;
            for (auto& pair : m_node->m_references) {
                if (!pair.second.node || !pair.second.node->m_managerElementMemory) {
                    continue;
                }

                // Not sure why I am doing these two lines below here TODO investigate
                pair.second.node->m_managerElementMemory->referencingReleased(newID);
                pair.second.node->m_references[newID].node = 0;
            }
            for (auto& ptr : m_node->m_ptrs) {
                ptr->reindex(newID, m_node->m_managerElementMemory);
            }
            for (auto& ptr : oldNode.m_ptrs) {
                ptr->reindex(newID, m_node->m_managerElementMemory);
                ptr->m_ptrId = 0;
                if (m_node->m_ptrs.size() > 0) {
                    ptr->m_ptrId = m_node->m_ptrs.back()->m_ptrId + 1;
                }
                m_node->m_ptrs.push_back(ptr);
            }
            m_elements.erase(oldID);
            m_graph.erase(oldID);
        } else  {
            // reindex
            m_elements.erase(oldID);
            m_elements.insert(newID);
            ManagerNode& discRef = m_graph[oldID];
            m_graph[newID] = std::move(discRef);
            ManagerNode* newDisc = &m_graph[newID];
            // newDisc->m_managerElementMemory = discRef.m_managerElementMemory;
            for (auto& ref : newDisc->m_references) {
                if (!ref.second.node) {
                    // reference is relased currently with no ptrs
                    throw ManagerStateException("Bad state in reindex, reference released! TODO maybe aquire released el");
                } else if (ref.second.node->m_references.count(oldID)) {
                    size_t numRefs = ref.second.node->m_references[oldID].numRefs;
                    ref.second.node->m_references.erase(oldID);
                    ref.second.node->m_references[newID] = ManagerNode::NodeReference{newDisc, numRefs};
                }
                if (!ref.second.node->m_managerElementMemory) {
                    aquire(ref.first);
                }
                ref.second.node->m_managerElementMemory->referenceReindexed(oldID, newID);
            }
            newDisc->m_managerElementMemory->m_node = newDisc;
            for (auto& ptr : newDisc->m_ptrs) {
                ptr->reindex(newID, newDisc->m_managerElementMemory);
            }
            m_graph.erase(oldID);
            if (!m_mountBase.empty()) {
                std::filesystem::remove(m_mountBase / "mount" / (oldID.string() + ".yml"));
                /** Should we write to new file now?**/
            }
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
    for (auto& pair : m_graph) {
        mountEl(*pair.second.m_managerElementMemory);
    }
}

void UmlManager::restoreNode(Element& ret) {
    ret.m_node->m_managerElementMemory = &ret;
    for (auto& pair : ret.m_node->m_references) {
        ManagerNode* node = pair.second.node;
        if (!node || !node->m_managerElementMemory) {
            // element has been released, possibly there are no pointers
            continue;
        }
        node->m_managerElementMemory->restoreReference(&ret);
        ret.restoreReference(node->m_managerElementMemory);
    }
    for (auto& ptr : ret.m_node->m_ptrs) {
        ptr->reindex(ret.getID(), &ret);
    }
    ret.restoreReferences();
}

ElementPtr UmlManager::aquire(ID id) {
    if (m_mountBase.empty()) {
        throw ManagerNotMountedException();
    }
    if (UmlManager::loaded(id)) {
        return ElementPtr(&get(id));
    }
    if (std::filesystem::exists(m_mountBase / "mount" / (id.string() + ".yml"))) {
        Parsers::ParserMetaData data(this);
        data.m_path = m_mountBase / "mount" / (id.string() + ".yml");
        data.m_strategy = Parsers::ParserStrategy::INDIVIDUAL;
        ElementPtr ret = Parsers::parse(data);
        if (ret) {
            restoreNode(*ret);
        } else {
            throw ManagerStateException();
        }
        return ret;
    } else {
        throw ID_doesNotExistException(id);
    }
}

void UmlManager::release(ID id) {
    release(*m_graph[id].m_managerElementMemory);
}

void UmlManager::releaseNode(Element& el) {
    ManagerNode* node = el.m_node;
    ID id = el.getID();
    for (auto& e : node->m_references) {
        if (!e.second.node) {
            // el has been released there are no pointers
            continue;
        }
        if (e.second.node->m_managerElementMemory) {
            e.second.node->m_managerElementMemory->referencingReleased(id);
        }
    }
    for (auto& ptr : node->m_ptrs) {
        ptr->releasePtr();
    }
}

void UmlManager::release(Element& el) {
    if (!m_mountBase.empty()) {
        ID elID = el.getID();
        mountEl(el);
        releaseNode(el);
        ManagerNode* node = el.m_node;
        node->m_managerElementMemory = 0;
        delete &el;
        if (node->m_ptrs.empty()) {
            m_graph.erase(elID);
        }
    } else {
        throw ManagerNotMountedException();
    }
}

void UmlManager::eraseNode(ManagerNode* node, ID id) {
    std::vector<ManagerNode*> idsToErase(node->m_references.size());
    size_t i = 0;
    for (auto& pair : node->m_references) {
        if (!pair.second.node || !pair.second.node->m_managerElementMemory) {
            // element has been released, aquire
            try {
                idsToErase[i] = aquire(pair.first)->m_node;
            } catch (ID_doesNotExistException& idException) {
                idsToErase[i] = 0;
            }
        } else {
            idsToErase[i] = pair.second.node;
        }
        i++;
    }
    for (auto& refNode : idsToErase) {
        if (!refNode) {
            continue;
        }
        refNode->m_managerElementMemory->removeReference(id);
        refNode->m_managerElementMemory->referenceErased(id);
        if (refNode->m_references.count(id)) {
            refNode->m_references.erase(id);
        }
    }
    for (auto& ptr : node->m_ptrs) {
        ptr->erasePtr();
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
    if (m_path.empty() || m_root.isNull()) {
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
    setRoot(&*Parsers::parse(data));
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

void UmlManager::setRoot(Element* el) {
    m_root = el ? el->getID() : ID::nullID();
}

void UmlManager::setRoot(Element& el) {
    setRoot(&el);
}

ElementPtr UmlManager::getRoot() {\
    if (m_root.isNull()) {
        return ElementPtr(0);
    }
    return ElementPtr(&get(m_root));
}

void UmlManager::setPath(ID elID, std::string path) {
    m_graph[elID].m_path = path;
}

Element* UmlManager::get(Element* me, ID theID) {
    if (!theID.isNull()) {
        if (me->m_node) {
            if (me->m_node->m_references.count(theID)) {
                if (!me->m_node->m_references[theID].node || !me->m_node->m_references[theID].node->m_managerElementMemory) {
                    ElementPtr aquired = aquire(theID);
                    me->m_node->m_references[theID].node = aquired->m_node;
                }
                return me->m_node->m_references[theID].node->m_managerElementMemory;
            } else {
                throw ManagerStateException("could not find reference " + theID.string());
            }
        } else {
            aquire(theID);
            return  me->m_node->m_references[theID].node->m_managerElementMemory;
        }
    }
    return 0;
}

void UmlManager::lazy(bool lazy) {
    m_lazy = lazy;
}