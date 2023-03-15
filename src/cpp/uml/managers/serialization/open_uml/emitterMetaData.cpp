#include "uml/managers/serialization/open_uml/emitterMetaData.h"
#include "uml/managers/abstractManager.h"

namespace UML {
namespace Parsers {

std::filesystem::path EmitterMetaData::getPath(ID id) {
    return m_manager->getLocation(id);
}

}
}