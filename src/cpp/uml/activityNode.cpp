#include "uml/activityNode.h"
#include "uml/activity.h"

using namespace UML;

ElementType ActivityNode::getElementType() {
    return ElementType::ACTIVITY_NODE;
}

Activity* ActivityNode::getActivity() {
    return activity;
}

void ActivityNode::setActivity(Activity* activity) {
    this->activity = activity;
}