#ifndef UML_MANAGER_H
#define UML_MANAGER_H

#include <unordered_map>
#include "id.h"
#include "element.h"
#include "parsers/parserMetaData.h"

namespace UML {

    struct DiscData {
        std::string m_path;
        int m_lineBegin;
        int m_lineEnd;
        bool m_sequence;
    };

    /**
     * UmlManager is the object that handles all of the instantiation and deletion of UML objects
     * from a model. It follows object pool semantics to be able to hold information about large
     * models without taking up too much memory
     **/
    class UmlManager {
        private:
            class AbstractFactory {};
            template <class T = Element> class Factory : public AbstractFactory {
                private:
                    std::vector<T*> m_created;
                    virtual ~Factory() {
                        for (auto const& el : m_created) {
                            delete el;
                        }
                    };
                public:
                    T& create() {
                        T* ret = new T;
                        m_created.push_back(ret);
                        return *ret;
                    };
            };
            Parsers::ParserMetaData m_parserData;
            std::unordered_map<ID, Element*> m_elements;
            std::unordered_map<ID, DiscData> m_disc;
            std::unordered_map<ElementType, AbstractFactory*> m_factories;
        public:
            UmlManager();
            ~UmlManager();
            void aquire(ID id);
            void release(ID id);
            template <class T = Element> T& get(ID id) {
                return *dynamic_cast<T*>(m_elements[id]);
            };
            template <class T = Element> T& create() {
                T& ret = static_cast<Factory<T>*>(m_factories[T::elementType()])->create();
                ret.m_manager = this;
                m_elements[ret.getID2()] = &ret;
                return ret;
            };
    };
}

#endif