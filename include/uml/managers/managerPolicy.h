#ifndef _UML_MANAGERS_MANAGER_POLICY_H_
#define _UML_MANAGERS_MANAGER_POLICY_H_

#include "uml/element.h"
#include "abstractManager.h"
#include "abstractAccessPolicy.h"
#include "uml/umlPtr.h"

namespace UML {

    /**
     * This is the policy class for a manager of UML elements, it requires two policies to be defined
     * in order to use it:
     *  AccessPolicy: The policy handling accessing the managers current memory of uml elements
     *  PersistencePolicy: The policy handling the persistent storage of uml elements as well as the
     *      swap space. // TODO think about seperating persistence and swap policies
     * 
     * These policies have methods that need to be implemented:
     *      AccessPolicy:
     *          - ManagerNode& assignNode(Element* newElement) : assigns the element a ManagerNode* object or sub object
     *          - ElementPtr get(ID id) : get an element from memory
     *          - bool loaded(ID id) : the element with the specified id is loaded in memory
     *          - void restoreNode(ManagerNode* restoredNode) : restore a node from memory
     *          - void eraseNode(ManagerNode* node, AbstractManager* me) : erase a node from existence
     *          - void releaseNode(ManagerNode* node) : release a node from memory
     *          - void reindex(ID oldID, ID newID) : reindex a node within the node graph
     *          - void removeNode(ID id) : remove a node with the specified id from the graph
     *          - void assignPtr(AbstractUmlPtr& ptr) : assign a pointer to the list
     *          - void removePtr(AbstractUmlPtr& ptr) : remove a pointer from the list
     *          - void restorePtr(AbstractUmlPtr& ptr) : restores a ptr in the list (TODO reimplement)
     *      PersistencePolicy:
     *          - ElementPtr aquire(ID id, AbstractManager* manager) : aquire an element with the specified id from disk
     *          - void write(Element& el, AbstractManager* me) : write an element to disk
     *          - void write(AbstractManager* me) : write everything to disk whole
     *          - void write(std::string key, AbstractManager* me) : write to disk under key
     *          - ElementPtr parse(AbstractManager* me) : read from disk
     *          - ElementPtr parse(std::string path, AbstractManager* me) : read from specified signifier
     *          - std::string getLocation(ID id) : get ID of signifier
     *          - void setLocation(ID id, std::string location) : set a signifier ID pair
     **/
    template <typename AccessPolicy, typename PersistencePolicy>
    class Manager : public AbstractManager , public AccessPolicy, public PersistencePolicy {
        protected:
            ElementPtr m_root;
            void removePtr(AbstractUmlPtr& ptr) override {
                AccessPolicy::removePtr(ptr);
            }
            void destroyPtr(AbstractUmlPtr& ptr) override {
                AccessPolicy::destroyPtr(ptr);
            }
            void assignPtr(AbstractUmlPtr& ptr) override {
                AccessPolicy::assignPtr(ptr);
            }
            void restorePtr(AbstractUmlPtr& ptr) override {
                AccessPolicy::restorePtr(ptr);
            }
        public:
            virtual ~Manager() {
                AccessPolicy::clear();
            }
            template <class T = Element>
            UmlPtr<T> create() {
                T* newElement = new T;
                ManagerNode& node = AccessPolicy::assignNode(newElement);
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

            ElementPtr get(ID id) override {
                ElementPtr ret = AccessPolicy::get(id);
                if (ret) {
                    return ret;
                }

                // not loaded or does not exist
                ret = PersistencePolicy::aquire(id, this);
                
                if (!ret) {
                    // Manager has no reference to an element of this id
                    // throw ManagerPolicyStateException("unknown id!");
                } else {
                    AccessPolicy::restoreNode(ret.m_node);
                }

                return ret;
            };

            bool loaded(ID id) override {
                return AccessPolicy::loaded(id);
            }

            void release(Element& el) override {
                PersistencePolicy::write(el, this);
                AccessPolicy::releaseNode(el.m_node);
            }

            template <class ... Elements> 
            void release(Element& el, Elements&... els) {
                release(el);
                release(els...);
            }

            void erase(Element& el) override {
                AccessPolicy::eraseNode(el.m_node, this);
            };

            void reindex(ID oldID, ID newID) override {
                if (oldID == newID) {
                    return;
                }
                if (AccessPolicy::exists(newID)) {
                    AccessPolicy::overwrite(oldID, newID);
                } else {
                    AccessPolicy::reindex(oldID, newID);
                }
                // TODO persistence policy reindex?
            }

            void removeNode(ID id) override {
                AccessPolicy::removeNode(id);
            }

            ElementPtr open(std::string path) override {
                setRoot(PersistencePolicy::parse(path, this).ptr());
                return m_root;
            }

            ElementPtr open() override {
                setRoot(PersistencePolicy::parse(this).ptr());
                return m_root;
            }

            void save(std::string location) override {
                PersistencePolicy::write(location, this);
            }

            void save() override {
                PersistencePolicy::write(this);
            }

            void setRoot(Element* root) override {
                m_root = root;
            }

            ElementPtr getRoot() const override {
                return m_root;
            }

            std::string getLocation(ID id) override {
                return PersistencePolicy::getLocation(id);
            }

            std::string getLocation() override {
                return PersistencePolicy::getLocation();
            }

            void setLocation(ID id, std::string location) override {
                PersistencePolicy::setLocation(id, location);
            }
    };
}

#endif