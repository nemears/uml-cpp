#include "uml/activityEdge.h"
#include "uml/activityNode.h"

using namespace UML;

ActivityNode* ActivityEdge::getSource() {
    return source;
}

void ActivityEdge::setSource(ActivityNode* source) {
    this->source = source;
}

ActivityNode* ActivityEdge::getTarget() {
    return target;
}

void ActivityEdge::setTarget(ActivityNode* target) {
    this->target = target;
}