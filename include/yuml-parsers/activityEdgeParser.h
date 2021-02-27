#ifndef ACTIVITYEDGEPARSER_H
#define ACTIVITYEDGEPARSER_H

#include "namedElementParser.h"
#include "uml/activityEdge.h"
#include "uml/activityNode.h"
#include "uml/instanceValue.h"
#include "uml/objectNode.h"
#include "uml/primitiveType.h"
#include "uml/literalBool.h"
#include "uml/literalInt.h"
#include "uml/literalReal.h"
#include "uml/literalString.h"

class ActivityEdgeParser : public NamedElementParser {
    protected:
        bool parseFeatures(YAML::Node node, Element* el) override;
    public:
        bool emit(YAML::Emitter& emitter, Element* el) override;
        ActivityEdgeParser(map<boost::uuids::uuid, Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            NamedElementParser(elements, postParsers) {};
        static void setGuardLater(Element* edge, Element* instGuard) {
            InstanceValue* iv = new InstanceValue;
            iv->setInstance(dynamic_cast<InstanceSpecification*>(instGuard));
            dynamic_cast<ActivityEdge*>(edge)->setGuard(dynamic_cast<ValueSpecification*>(iv));
        };
};

#endif