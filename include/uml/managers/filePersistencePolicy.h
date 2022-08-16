#ifndef _UML_MANAGERS_FILE_PERSISTENCE_POLICY_H_
#define _UML_MANAGERS_FILE_PERSISTENCE_POLICY_H_

#include "abstractPersistencePolicy.h"
#include "uml/element.h"

namespace UML {
    class FilePersistencePolicy : public AbstractPersistencePolicy {
        private:
            std::filesystem::path m_mountBase;
        protected:
            ElementPtr aquire(ID id, AbstractManager* manager) {
                if (!std::filesystem::exists(m_mountBase / "mount" / (id.string() + ".yml"))) {
                    throw ID_doesNotExistException(id);
                }
                Parsers::ParserMetaData data;
                data.m_manager2 = manager;
                data.m_path = m_mountBase / "mount" / (id.string() + ".yml");
                data.m_strategy = Parsers::ParserStrategy::INDIVIDUAL;
                ElementPtr ret = Parsers::parse(data);
                return ret;
            }

            void write(Element& el, AbstractManager* me) {
                if (m_mountBase.empty()) {
                    throw ManagerPolicyStateException("manager not mounted");
                }
                Parsers::EmitterMetaData data = { m_mountBase / std::filesystem::path("mount"),
                                                    Parsers::EmitterStrategy::INDIVIDUAL,
                                                    el.getID().string() + ".yml",
                                                    0,
                                                    me };
                Parsers::emitToFile(el, data, data.m_path.string(), data.m_fileName);
            }
        public:
            void mount(std::filesystem::path mountPath) {
                m_mountBase = mountPath;
                std::filesystem::create_directories(m_mountBase / std::filesystem::path("mount"));
                // for (auto& pair : m_graph) {
                //     mountEl(*pair.second.m_managerElementMemory);
                // }
            }
    };
}

#endif