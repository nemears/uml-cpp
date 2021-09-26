#include "uml/parsers/emitterMetaData.h"
#include "uml/umlManager.h"

namespace UML {
namespace Parsers {

std::filesystem::path EmitterMetaData::getMountPath(ID id) {
    return m_manager->m_mountBase / "mount" / (id.string() + ".yml");
    //return m_manager->m_graph[id].m_mountPath;
}

std::filesystem::path EmitterMetaData::getPath(ID id) {
    return m_manager->m_graph[id].m_path;
}

}
}