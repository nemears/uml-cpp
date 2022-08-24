#ifndef _UML_MANAGERS_FILE_PERSISTENCE_POLICY_H_
#define _UML_MANAGERS_FILE_PERSISTENCE_POLICY_H_

#include "uml/element.h"
#include "uml/parsers/parser.h"

namespace UML {
    class FilePersistencePolicy {
        private:
            std::filesystem::path m_mountBase;
            std::filesystem::path m_persistenPath;
            std::unordered_map<ID, std::filesystem::path> m_elementPaths;
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

            void write(AbstractManager* me) {
                if (m_persistenPath.empty()) {
                    throw ManagerStateException("bad path when saving!");
                }
                Parsers::EmitterMetaData data = { m_persistenPath.parent_path(),
                                                    Parsers::EmitterStrategy::WHOLE,
                                                    m_persistenPath.filename().string(),
                                                    me };
                Parsers::emitToFile(*me->getRoot(), data, data.m_path.string(), data.m_fileName);
            }

            void write(std::filesystem::path path, AbstractManager* me) {
                m_persistenPath = path;
                write(me);
            }

            ElementPtr parse(AbstractManager* me) {
                if (m_persistenPath.empty()) {
                    throw ManagerStateException("bad path when opening!");
                }
                Parsers::ParserMetaData data;
                data.m_manager = me;
                data.m_path = m_persistenPath;
                data.m_strategy = Parsers::ParserStrategy::WHOLE;
                return Parsers::parse(data);
            }

            ElementPtr parse(std::filesystem::path path, AbstractManager* me) {
                m_persistenPath = path;
                return parse(me);
            }

            std::string getLocation(ID id) {
                if (m_elementPaths.count(id)) {
                    return m_elementPaths.at(id).string();
                }
                return m_persistenPath.string();
            }

            void setLocation(ID id, std::filesystem::path location) {
                m_elementPaths[id] = location;
            }

        public:
            void mount(std::filesystem::path mountPath) {
                m_mountBase = mountPath;
                std::filesystem::create_directories(m_mountBase / std::filesystem::path("mount"));
            }

            virtual ~FilePersistencePolicy() {
                // delete mount
                if (!m_mountBase.empty()) {
                    std::filesystem::remove_all(m_mountBase / "mount");
                }
            }
    };
}

#endif