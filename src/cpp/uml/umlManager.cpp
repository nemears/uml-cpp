#include "uml/umlManager.h"
#include <fstream>
#include "uml/parsers/parser.h"

// all of these need to be added to parser eventually
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

using namespace std;
using namespace UML;

UmlManager::UmlManager() {
    // I left in all abstract element types, may want to remove them for user in future
    m_factories[ElementType::ACTION] = new Factory<Action>;
    m_factories[ElementType::ACTIVITY] = new Factory<Activity>;
    m_factories[ElementType::ACTIVITY_EDGE] = new Factory<ActivityEdge>;
    m_factories[ElementType::ACTIVITY_NODE] = new Factory<ActivityNode>;
    m_factories[ElementType::ASSOCIATION] = new Factory<Association>;
    m_factories[ElementType::BEHAVIOR] = new Factory<Behavior>;
    m_factories[ElementType::BEHAVIORAL_FEATURE] = new Factory<BehavioralFeature>;
    m_factories[ElementType::CALL_BEHAVIOR_ACTION] =  new Factory<CallBehaviorAction>;
    m_factories[ElementType::CLASS] = new Factory<Class>;
    m_factories[ElementType::CLASSIFIER] = new Factory<Classifier>;
    m_factories[ElementType::COMMENT] = new Factory<Comment>;
    m_factories[ElementType::CONNECTABLE_ELEMENT] = new Factory<ConnectableElement>;
    m_factories[ElementType::CONTROL_FLOW] = new Factory<ControlFlow>;
    m_factories[ElementType::CREATE_OBJECT_ACTION] = new Factory<CreateObjectAction>;
    m_factories[ElementType::DATA_TYPE] = new Factory<DataType>;
    m_factories[ElementType::DECISION_NODE] = new Factory<DecisionNode>;
    m_factories[ElementType::DIRECTED_RELATIONSHIP] = new Factory<DirectedRelationship>;
    m_factories[ElementType::ELEMENT] = new Factory<>;
    m_factories[ElementType::ENUMERATION] = new Factory<Enumeration>;
    m_factories[ElementType::ENUMERATION_LITERAL] = new Factory<EnumerationLiteral>;
    m_factories[ElementType::EXPRESSION] = new Factory<Expression>;
    m_factories[ElementType::FEATURE] = new Factory<Feature>;
    m_factories[ElementType::FINAL_NODE] = new Factory<FinalNode>;
    m_factories[ElementType::FORK_NODE] = new Factory<ForkNode>;
    m_factories[ElementType::GENERALIZATION] = new Factory<Generalization>;
    m_factories[ElementType::INITIAL_NODE] = new Factory<InitialNode>;
    m_factories[ElementType::INPUT_PIN] = new Factory<InputPin>;
    m_factories[ElementType::INSTANCE_SPECIFICATION] = new Factory<InstanceSpecification>;
    m_factories[ElementType::INSTANCE_VALUE] = new Factory<InstanceValue>;
    m_factories[ElementType::JOIN_NODE] = new Factory<JoinNode>;
    m_factories[ElementType::LITERAL_BOOL] = new Factory<LiteralBool>;
    m_factories[ElementType::LITERAL_INT] = new Factory<LiteralInt>;
    m_factories[ElementType::LITERAL_REAL] = new Factory<LiteralReal>;
    m_factories[ElementType::LITERAL_SPECIFICATION] = new Factory<LiteralSpecification>;
    m_factories[ElementType::LITERAL_STRING] = new Factory<LiteralString>;
    m_factories[ElementType::MERGE_NODE] = new Factory<MergeNode>;
    m_factories[ElementType::MODEL] = new Factory<Model>;
    m_factories[ElementType::MULTIPLICITY_ELEMENT] = new Factory<MultiplicityElement>;
    m_factories[ElementType::NAMED_ELEMENT] =  new Factory<NamedElement>;
    m_factories[ElementType::NAMESPACE] = new Factory<Namespace>;
    m_factories[ElementType::OBJECT_FLOW] = new Factory<ObjectFlow>;
    m_factories[ElementType::OBJECT_NODE] = new Factory<ObjectNode>;
    m_factories[ElementType::OPAQUE_BEHAVIOR] = new Factory<OpaqueBehavior>;
    m_factories[ElementType::OPERATION] = new Factory<Operation>;
    m_factories[ElementType::OUTPUT_PIN] = new Factory<OutputPin>;
    m_factories[ElementType::PACKAGE] = new Factory<Package>;
    m_factories[ElementType::PACKAGE_MERGE] = new Factory<PackageMerge>;
    m_factories[ElementType::PACKAGEABLE_ELEMENT] = new Factory<PackageableElement>;
    m_factories[ElementType::PARAMETER] = new Factory<Parameter>;
    m_factories[ElementType::PARAMETER_NODE] = new Factory<ParameterNode>;
    m_factories[ElementType::PIN] = new Factory<Pin>;
    m_factories[ElementType::PRIMITIVE_TYPE] = new Factory<PrimitiveType>;
    m_factories[ElementType::PROPERTY] = new Factory<Property>;
    m_factories[ElementType::REDEFINABLE_ELEMENT] = new Factory<RedefinableElement>;
    m_factories[ElementType::RELATIONSHIP] = new Factory<Relationship>;
    m_factories[ElementType::SLOT] = new Factory<Slot>;
    m_factories[ElementType::STRUCTURAL_FEATURE] = new Factory<StructuralFeature>;
    m_factories[ElementType::STRUCTURED_CLASSIFIER] = new Factory<StructuredClassifier>;
    m_factories[ElementType::TYPE] = new Factory<Type>;
    m_factories[ElementType::TYPED_ELEMENT] = new Factory<TypedElement>;
    m_factories[ElementType::VALUE_SPECIFICATION] = new Factory<ValueSpecification>;
}

UmlManager::~UmlManager() {
    for (auto& factory : m_factories) {
        delete factory.second;
    }
}

void UmlManager::aquire(ID id) {
    if (!m_elements[id]) {
        ifstream fin;
        DiscData disc = m_disc[id];
        fin.open(disc.m_path);
        for (size_t i = 0; i < disc.m_lineBegin; i++) {
            fin.ignore(numeric_limits<streamsize>::max(),'\n');
        }
        string buffer;
        for (size_t i = 0; i < disc.m_lineEnd - disc.m_lineBegin; i ++) {
            getline(fin, buffer);
            buffer += '\n';
        }
        fin.close();
        YAML::Node node = YAML::Load(buffer);
        m_elements[id] = Parsers::parseNode(node, m_parserData);
    }
}

void UmlManager::release(ID id) {
    // TODO, need to implement aquire in a different way
}