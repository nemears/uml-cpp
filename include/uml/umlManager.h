#ifndef UML_MANAGER_H
#define UML_MANAGER_H

#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include "id.h"
#include "element.h"

namespace UML {
    namespace Parsers {
        class ParserMetaData;
        class ManagerFriendFunctor;
    }

    struct DiscData {
        std::string m_path;
        int m_lineBegin;
        int m_lineEnd;
        bool m_sequence;
    };

    template <class T = Element> class Sequence;
    class Model;

    /**
     * UmlManager is the object that handles all of the instantiation and deletion of UML objects
     * from a model. It follows object pool semantics to be able to hold information about large
     * models without taking up too much memory
     **/
    class UmlManager {
        friend class Parsers::ParserMetaData;
        friend class Parsers::ManagerFriendFunctor;
        private:
            //Parsers::ParserMetaData m_parserData;
            std::unordered_map<ID, Element*> m_loaded;
            std::unordered_set<ID> m_elements;
            std::unordered_map<ID, DiscData> m_disc;
            std::filesystem::path m_path;
            Model* m_model;
            void clear();
        public:
            ~UmlManager();
            template <class T = Element> T& get(ID id) {
                if (!m_loaded.count(id)) {
                    // TODO load
                }
                return *dynamic_cast<T*>(m_loaded.at(id));
            };
            template <class T = Element> T& create() {
                T* ret = new T;
                ret->setManager(this);
                m_elements.insert(ret->getID());
                m_loaded[ret->getID()] = ret;
                return *ret;
            };
            void reindex(ID oldID, ID newID);
            
            /**
             * This function doesn't deal with memory, just sets the m_manager so the Sequence Value
             * can communicate to the manager for allocation.
             * WARN: Sequences should always be values
             **/
            template <class T = Element> Sequence<T> createSequence() {
                Sequence<T> ret;
                ret.m_manager = this;
                return ret;
            };

            void save();
            void save(std::string path, Model& model);
            void open();
            void open(std::string path);

            Element* parse(std::string path);

            Model* getModel();
    };
}

#endif