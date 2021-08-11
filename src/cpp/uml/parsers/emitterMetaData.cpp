#include "uml/parsers/emitterMetaData.h"
#include "uml/umlManager.h"

namespace UML {
namespace Parsers {

std::filesystem::path EmitterMetaData::getMountPath(ID id) {
    return m_manager->m_disc[id].m_mountPath;
}

std::filesystem::path EmitterMetaData::getPath(ID id) {
    return m_manager->m_disc[id].m_path;
}

}
}