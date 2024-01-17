#pragma once

#include "uml/types/element.h"
#include "abstractManager.h"
#include "filePersistencePolicy.h"
#include "serialization/open_uml/openUmlSerializationPolicy.h"
#include "uml/umlPtr.h"
#include "uml/set/readOnlySet.h"
#include <shared_mutex>

namespace UML {

    struct ThreadSafeSetLock : public SetLock {
        std::unique_lock<std::mutex> m_lck;
        ThreadSafeSetLock(ManagerNode& node) : m_lck(node.m_mtx) {}
    };

    template <
            class SerializationPolicy = OpenUmlSerializationPolicy, 
            class PersistencePolicy = FilePersistencePolicy
        >
    class Manager : public AbstractManager, public SerializationPolicy, public PersistencePolicy {
        protected:

            ElementPtr m_root;
            std::unordered_map<ID, ManagerNode> m_graph;
            std::shared_timed_mutex m_graphMtx;

            void setPtr(ElementPtr& ptr, ID id, const ManagerNode* node) {
                ptr.m_id = id;
                ptr.m_manager = this;
                ptr.m_node = const_cast<ManagerNode*>(node);
                if (node->m_element) {
                    ptr.m_ptr = node->m_element;
                }
            }

            ElementPtr createPtr(ID id) override {
                std::shared_lock<std::shared_timed_mutex> lck(m_graphMtx);
                std::unordered_map<ID, ManagerNode>::const_iterator nodeIt = m_graph.find(id);
                const ManagerNode* node = 0;
                if (nodeIt == m_graph.end()) {
                    node = &m_graph[id];
                } else {
                    node = &nodeIt->second;
                }
                ElementPtr ret;
                setPtr(ret, id, node);
                assignPtr(ret);
                return ret;
            }
            void removePtr(AbstractUmlPtr& ptr) override {
                std::lock_guard<std::mutex> ptrsLck(ptr.m_node->m_ptrsMtx);
                ptr.m_node->m_ptrs.erase(&ptr);
            }
            void destroyNode(ID id, ManagerNode* node) {
                bool removeFromGraph = false;
                {
                    std::lock_guard<std::mutex> ptrsLck(node->m_ptrsMtx);
                    if (node->m_ptrs.empty() && !node->m_element) {

                        // remove trailing references that may have been left around from another element in memory
                        for (auto referencePair : node->m_references) {
                            if (referencePair.second.node->m_references.count(id)) {
                                referencePair.second.node->m_references.erase(id);
                            }
                        }

                        // remove from graph
                        removeFromGraph = true;
                    }
                }
                if (removeFromGraph) {
                    std::unique_lock<std::shared_timed_mutex> graphLck(m_graphMtx);
                    m_graph.erase(id);
                }
 
            }
            void destroyPtr(AbstractUmlPtr& ptr) override {
                destroyNode(ptr.m_id, ptr.m_node);
            }
            void assignPtr(AbstractUmlPtr& ptr) override {
                std::lock_guard<std::mutex> ptrsLck(ptr.m_node->m_ptrsMtx);
                ptr.m_node->m_ptrs.insert(&ptr);
            }
            void restorePtr(AbstractUmlPtr& ptr) override {
                std::lock_guard<std::mutex> ptrsLck(ptr.m_node->m_ptrsMtx);
                ptr.m_node->m_ptrs.insert(&ptr);
            }

            SetLock lockEl(Element& el) override {
                return ThreadSafeSetLock(*el.m_node);
            }
        public:
            virtual ~Manager() {
                for (auto& pair : m_graph) {
                    pair.second.erasePtrs();
                    delete pair.second.m_element;
                    pair.second.erase();
                }
                m_graph.clear();
            }
            template <class T = Element>
            UmlPtr<T> create() {
                T* newElement = new T;
                std::unique_lock<std::shared_timed_mutex> graphLock(m_graphMtx);
                ManagerNode& node = m_graph[newElement->getID()];
                node.m_element = newElement;
                newElement->m_node = &node;
                newElement->m_manager = this;
                UmlPtr<T> ret(newElement);
                return ret;
            }

            Element* create(ElementType type) override {
                switch (type) {
                    case ElementType::ABSTRACTION : {
                        return create<Abstraction>().ptr();
                    }
                    case ElementType::ACTION_INPUT_PIN : {
                        return create<ActionInputPin>().ptr();
                    }
                    case ElementType::ACTIVITY : {
                        return create<Activity>().ptr();
                    }
                    case ElementType::ACTIVITY_FINAL_NODE : {
                        return create<ActivityFinalNode>().ptr();
                    }
                    case ElementType::ACTIVITY_PARAMETER_NODE : {
                        return create<ActivityParameterNode>().ptr();
                    }
                    case ElementType::ACTIVITY_PARTITION : {
                        return create<ActivityPartition>().ptr();
                    }
                    case ElementType::ARTIFACT : {
                        return create<Artifact>().ptr();
                    }
                    case ElementType::ASSOCIATION : {
                        return create<Association>().ptr();
                    }
                    case ElementType::CALL_BEHAVIOR_ACTION : {
                        return create<CallBehaviorAction>().ptr();
                    }
                    case ElementType::CENTRAL_BUFFER_NODE : {
                        return create<CentralBufferNode>().ptr();
                    }
                    case ElementType::CLASS : {
                        return create<Class>().ptr();
                    }
                    case ElementType::CLASSIFIER_TEMPLATE_PARAMETER : {
                        return create<ClassifierTemplateParameter>().ptr();
                    }
                    case ElementType::COMMENT : {
                        return create<Comment>().ptr();
                    }
                    case ElementType::CONNECTOR : {
                        return create<Connector>().ptr();
                    }
                    case ElementType::CONNECTOR_END : {
                        return create<ConnectorEnd>().ptr();
                    }
                    case ElementType::CONSTRAINT : {
                        return create<Constraint>().ptr();
                    }
                    case ElementType::CONTROL_FLOW : {
                        return create<ControlFlow>().ptr();
                    }
                    // case ElementType::CREATE_OBJECT_ACTION : {
                    //     return create<CreateObjectAction>().ptr();
                    // }
                    case ElementType::DATA_STORE_NODE : {
                        return create<DataStoreNode>().ptr();
                    }
                    case ElementType::DATA_TYPE : {
                        return create<DataType>().ptr();
                    }
                    case ElementType::DECISION_NODE : {
                        return create<DecisionNode>().ptr();
                    }
                    case ElementType::DEPENDENCY : {
                        return create<Dependency>().ptr();
                    }
                    case ElementType::DEPLOYMENT : {
                        return create<Deployment>().ptr();
                    }
                    case ElementType::ELEMENT_IMPORT : {
                        return create<ElementImport>().ptr();
                    }
                    case ElementType::ENUMERATION : {
                        return create<Enumeration>().ptr();
                    }
                    case ElementType::ENUMERATION_LITERAL : {
                        return create<EnumerationLiteral>().ptr();
                    }
                    case ElementType::EXCEPTION_HANDLER : {
                        return create<ExceptionHandler>().ptr();
                    }
                    case ElementType::EXPRESSION : {
                        return create<Expression>().ptr();
                    }
                    case ElementType::EXTENSION : {
                        return create<Extension>().ptr();
                    }
                    case ElementType::EXTENSION_END : {
                        return create<ExtensionEnd>().ptr();
                    }
                    case ElementType::FLOW_FINAL_NODE : {
                        return create<FlowFinalNode>().ptr();
                    }
                    case ElementType::FORK_NODE : {
                        return create<ForkNode>().ptr();
                    }
                    case ElementType::GENERALIZATION : {
                        return create<Generalization>().ptr();
                    }
                    case ElementType::GENERALIZATION_SET : {
                        return create<GeneralizationSet>().ptr();
                    }
                    case ElementType::INITIAL_NODE : {
                        return create<InitialNode>().ptr();
                    }
                    case ElementType::INPUT_PIN : {
                        return create<InputPin>().ptr();
                    }
                    case ElementType::INSTANCE_SPECIFICATION : {
                        return create<InstanceSpecification>().ptr();
                    }
                    case ElementType::INSTANCE_VALUE : {
                        return create<InstanceValue>().ptr();
                    }
                    case ElementType::INTERFACE_UML : {
                        return create<Interface>().ptr();
                    }
                    case ElementType::INTERFACE_REALIZATION : {
                        return create<InterfaceRealization>().ptr();
                    }
                    case ElementType::JOIN_NODE : {
                        return create<JoinNode>().ptr();
                    }
                    case ElementType::LITERAL_BOOL : {
                        return create<LiteralBool>().ptr();
                    }
                    case ElementType::LITERAL_INT : {
                        return create<LiteralInt>().ptr();
                    }
                    case ElementType::LITERAL_NULL : {
                        return create<LiteralNull>().ptr();
                    }
                    case ElementType::LITERAL_REAL : {
                        return create<LiteralReal>().ptr();
                    }
                    case ElementType::LITERAL_STRING : {
                        return create<LiteralString>().ptr();
                    }
                    case ElementType::LITERAL_UNLIMITED_NATURAL : {
                        return create<LiteralUnlimitedNatural>().ptr();
                    }
                    case ElementType::MANIFESTATION : {
                        return create<Manifestation>().ptr();
                    }
                    case ElementType::MERGE_NODE : {
                        return create<MergeNode>().ptr();
                    }
                    case ElementType::MODEL : {
                        return create<Model>().ptr();
                    }
                    case ElementType::OBJECT_FLOW : {
                        return create<ObjectFlow>().ptr();
                    }
                    case ElementType::OPAQUE_ACTION : {
                        return create<OpaqueAction>().ptr();
                    }
                    case ElementType::OPAQUE_BEHAVIOR : {
                        return create<OpaqueBehavior>().ptr();
                    }
                    case ElementType::OPERATION : {
                        return create<Operation>().ptr();
                    }
                    case ElementType::OUTPUT_PIN : {
                        return create<OutputPin>().ptr();
                    }
                    case ElementType::PACKAGE: {
                        return create<Package>().ptr();
                    }
                    case ElementType::PACKAGE_IMPORT : {
                        return create<PackageImport>().ptr();
                    }
                    case ElementType::PACKAGE_MERGE : {
                        return create<PackageMerge>().ptr();
                    }
                    case ElementType::PARAMETER : {
                        return create<Parameter>().ptr();
                    }
                    case ElementType::PARAMETER_SET : {
                        return create<ParameterSet>().ptr();
                    }
                    case ElementType::PORT : {
                        return create<Port>().ptr();
                    }
                    case ElementType::PRIMITIVE_TYPE : {
                        return create<PrimitiveType>().ptr();
                    }
                    case ElementType::PROFILE : {
                        return create<Profile>().ptr();
                    }
                    case ElementType::PROFILE_APPLICATION: {
                        return create<ProfileApplication>().ptr();
                    }
                    case ElementType::PROPERTY : {
                        return create<Property>().ptr();
                    }
                    case ElementType::REALIZATION : {
                        return create<Realization>().ptr();
                    }
                    case ElementType::RECEPTION  : {
                        return create<Reception>().ptr();
                    }
                    case ElementType::REDEFINABLE_TEMPLATE_SIGNATURE : {
                        return create<RedefinableTemplateSignature>().ptr();
                    }
                    case ElementType::SIGNAL : {
                        return create<Signal>().ptr();
                    }
                    case ElementType::SLOT : {
                        return create<Slot>().ptr();
                    }
                    case ElementType::STEREOTYPE : {
                        return create<Stereotype>().ptr();
                    }
                    case ElementType::TEMPLATE_BINDING : {
                        return create<TemplateBinding>().ptr();
                    }
                    case ElementType::TEMPLATE_PARAMETER : {
                        return create<TemplateParameter>().ptr();
                    }
                    case ElementType::TEMPLATE_PARAMETER_SUBSTITUTION :{
                        return create<TemplateParameterSubstitution>().ptr();
                    }
                    case ElementType::TEMPLATE_SIGNATURE : {
                        return create<TemplateSignature>().ptr();
                    }
                    case ElementType::USAGE : {
                        return create<Usage>().ptr();
                    }
                    case ElementType::VALUE_PIN : {
                        return create<ValuePin>().ptr();
                    }
                    default: {
                        throw ManagerStateException("could not do multimethod create for element type: " + Element::elementTypeToString(type));
                    }
                }
                return 0;
            }

            void restoreNode(ManagerNode* restoredNode) {
                restoredNode->restoreReferences();
            }

            void restoreNode(ElementPtr ptr) {
                restoreNode(ptr->m_node);
            }

            ElementPtr get(ID id) override {
                ElementPtr ret;
                {
                    std::shared_lock<std::shared_timed_mutex> graphLock(m_graphMtx);
                    std::unordered_map<ID, ManagerNode>::iterator result = m_graph.find(id);
                    if (result != m_graph.end() && (*result).second.m_element) {
                        // lock node for creation
                        ret = ElementPtr(result->second.m_element);
                    }
                }

                if (ret) {
                    return ret;
                }

                // not loaded or does not exist
                ret = SerializationPolicy::parseIndividual(PersistencePolicy::loadElementData(id), *this);

                if (ret) {
                    restoreNode(ret->m_node);
                }

                return ret;
            };

            bool loaded(ID id) override {
                std::shared_lock<std::shared_timed_mutex> graphLock(m_graphMtx);
                std::unordered_map<ID, ManagerNode>::const_iterator result = m_graph.find(id);
                return result != m_graph.end() && (*result).second.m_element;
            }

            void release(Element& el) override {
                PersistencePolicy::saveElementData(SerializationPolicy::emitIndividual(el, *this), el.getID());
                ManagerNode* node = el.m_node;
                ID id = node->m_element->getID();
                node->releasePtrs();
                delete node->m_element;
                node->m_element = 0;
                destroyNode(id, node);
            }

            template <class ... Elements> 
            void release(Element& el, Elements&... els) {
                release(el);
                release(els...);
            }

            void erase(Element& el) override {
                PersistencePolicy::eraseEl(el.getID());

                ManagerNode* node = el.m_node;
                ID id = node->m_element->getID();
                {
                    std::vector<ManagerNode*> nodesToErase(node->m_references.size());
                    {
                        // TODO lock node
                        std::vector<ID> idsToAquire(node->m_references.size());
                        size_t i = 0;
                        for (auto& pair : node->m_references) {
                            if (!pair.second.node || !pair.second.node->m_element) {
                                // element has been released, aquire later
                                nodesToErase[i] = 0;
                                idsToAquire[i] = pair.first;
                            } else {
                                nodesToErase[i] = pair.second.node;
                            }
                            i++;
                        }
                        i = 0;
                        for (auto& refNode : nodesToErase) {
                            if (!refNode) {
                                refNode = get(idsToAquire[i]).m_node;
                            }
                            i++;
                        }
                    }
                    for (auto& refNode : nodesToErase) {
                        if (!refNode) {
                            continue;
                        }
                        refNode->referenceErased(id); // TODO automate instead of dispatch
                        if (refNode->m_references.count(id)) {
                            refNode->m_references.erase(id);
                        }
                    }
                    node->erase();
                    delete node->m_element;
                }
                // lock graph for deletion
                std::unique_lock<std::shared_timed_mutex> graphLock(m_graphMtx);
                m_graph.erase(id);
            };

            bool exists(ID id) override {
                // lock graph for access
                std::shared_lock<std::shared_timed_mutex> graphLock(m_graphMtx);
                return m_graph.count(id);
            }

            void reindex(ID oldID, ID newID) override {
                if (oldID == newID) {
                    return;
                }

                if (exists(newID)) {
                    // lock graph for edit
                    ManagerNode* nodeToBeOverwritten = 0;
                    ManagerNode* newNode = 0;
                    {
                        std::unique_lock<std::shared_timed_mutex> graphLock(m_graphMtx);

                        // overwrite
                        std::unordered_map<ID, ManagerNode>::iterator nodeToBeOverwrittenIterator = m_graph.find(newID);
                        nodeToBeOverwritten = &nodeToBeOverwrittenIterator->second;
                        newNode = &m_graph[oldID];
                    }
                    if (nodeToBeOverwritten->m_element) {
                        delete nodeToBeOverwritten->m_element;
                    }
                    nodeToBeOverwritten->m_element = newNode->m_element;
                    nodeToBeOverwritten->m_element->m_node = nodeToBeOverwritten;
                    nodeToBeOverwritten->reindexPtrs(newID);
                    for (auto& ptr : newNode->m_ptrs) {
                        ptr->reindex(newID, nodeToBeOverwritten->m_element);
                        assignPtr(*ptr);
                    }
                    std::unique_lock<std::shared_timed_mutex> graphLock(m_graphMtx);
                    m_graph.erase(oldID);
                } else {
                    // lock graph for edit
                    std::unique_lock<std::shared_timed_mutex> graphLock(m_graphMtx);

                    // reindex
                    ManagerNode& discRef = m_graph[oldID];

                    // TODO references
                    if (!discRef.m_references.empty()) {
                        throw ManagerStateException("Un-implemented, please set id before filling out element!");
                    }

                    ManagerNode& newDisc = m_graph[newID] = discRef;
                    newDisc.m_element->m_node = &newDisc;
                    for (auto& ptr : newDisc.m_ptrs) {
                        ptr->reindex(newID, newDisc.m_element);
                    }
                    m_graph.erase(oldID);
                }

                PersistencePolicy::reindex(oldID, newID);
            }

            ElementPtr open(std::string path) override {
                // doesn't clear, just adds to the current graph
                setRoot(SerializationPolicy::parseWhole(PersistencePolicy::getProjectData(path), *this).ptr());
                std::list<ElementPtr> queue;
                queue.push_back(m_root);
                while (!queue.empty()) {
                    ElementPtr front = queue.front();
                    queue.pop_front();
                    restoreNode(front.m_node);
                    for (auto& el : front->getOwnedElements()) {
                        queue.push_back(&el);
                    }
                }
                return m_root;
            }

            ElementPtr open() override {
                // {
                //     // clear all elements
                //     std::unique_lock<std::shared_timed_mutex> graphLock(m_graphMtx);
                //     m_root = ElementPtr();
                //     m_graph.clear();
                // }
                setRoot(SerializationPolicy::parseWhole(PersistencePolicy::getProjectData(), *this).ptr());
                std::list<ElementPtr> queue;
                queue.push_back(m_root);
                while (!queue.empty()) {
                    ElementPtr front = queue.front();
                    queue.pop_front();
                    restoreNode(front.m_node);
                    for (auto& el : front->getOwnedElements()) {
                        queue.push_back(&el);
                    }
                }
                return m_root;
            }

            void save(std::string location) override {
                PersistencePolicy::saveProjectData(SerializationPolicy::emitWhole(*getRoot(), *this), location);
            }

            void save() override {
                PersistencePolicy::saveProjectData(SerializationPolicy::emitWhole(*getRoot(), *this));
            }

            void setRoot(Element* root) override {
                m_root = root;
            }

            ElementPtr getRoot() const override {
                return m_root;
            }
    };
}
