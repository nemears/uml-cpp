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

UmlManager::~UmlManager() {
    for (auto& e : m_loaded) {
        delete e.second;
    }
}

void UmlManager::reindex(ID oldID, ID newID) {
    m_elements.erase(oldID);
    m_elements.insert(newID);
    Element* el = m_loaded[oldID];
    m_loaded.erase(oldID);
    m_loaded[newID] = el;
    DiscData disc = m_disc[oldID];
    m_disc.erase(oldID);
    m_disc[newID] = disc;
}