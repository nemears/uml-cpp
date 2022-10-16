#ifndef _UML_MANAGERS_FILE_SWAP_POLICY_H_
#define _UML_MANAGERS_FILE_SWAP_POLICY_H_

namespace UML {
    class FileSwapPolicy {
        protected:
            ElementPtr aquire(ID id, AbstractManager* manager) {
                if (!std::filesystem::exists(m_mountBase / "mount" / (id.string() + ".yml"))) {
                    throw ID_doesNotExistException(id);
                }
                Parsers::ParserMetaData data;
                data.m_manager = manager;
                data.m_path = m_mountBase / "mount" / (id.string() + ".yml");
                data.m_strategy = Parsers::ParserStrategy::INDIVIDUAL;
                ElementPtr ret = Parsers::parse(data);
                return ret;
            }

            void write(Element& el, AbstractManager* me) {
                if (m_mountBase.empty()) {
                    throw ManagerStateException("manager not mounted");
                }
                Parsers::EmitterMetaData data = { m_mountBase / std::filesystem::path("mount"),
                                                    Parsers::EmitterStrategy::INDIVIDUAL,
                                                    el.getID().string() + ".yml",
                                                    me };
                Parsers::emitToFile(el, data, data.m_path.string(), data.m_fileName);
            }
    };
}

#endif