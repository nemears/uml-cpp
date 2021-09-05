#ifndef UML_SINGLETON_H
#define UML_SINGLETON_H

#include "element.h"
#include "umlManager.h"

namespace UML {

    template <class T, class U> class TemplateAbstractSequenceFunctor;

    template <class T = Element> class SingletonProcedure {
        public:
            virtual void operator()(ID id, T* el) const = 0;
    };

    template <class T = Element, class U = Element> class AbstractSingletonProcedure : public SingletonProcedure<T> {
        protected:
            U* m_me;
            AbstractSingletonProcedure<T,U>(U* me) {
                m_me = me;
            };
    };

    template <class T = Element, class U = Element> class Singleton {

        friend class UmlManager;
        template <class T, class U> friend class TemplateAbstractSequenceFunctor;

        public:
            Singleton<T,U> U::*m_signature;
            U* m_me;
        private:
            ID m_id;
            T* m_ptr;
            bool isSameOrNull(ID id, Element* el) {
                if (id.isNull()) {
                    return true;
                } else {
                    if (el) {
                        return id == el->getID();
                    }
                    return false;
                }
            };
            void updateCopies() {
                if (m_me->m_node) {
                    if (m_me->m_node->m_managerElementMemory != m_me) {
                        if ((m_me->m_node->m_managerElementMemory->template as<U>().*m_signature).m_id != m_id) {
                            (m_me->m_node->m_managerElementMemory->template as<U>().*m_signature).m_id = m_id;
                            (m_me->m_node->m_managerElementMemory->template as<U>().*m_signature).m_ptr = 0;
                        }
                    }
                    for (auto& copy : m_me->m_node->m_copies) {
                        if (copy != m_me) {
                            if ((copy->template as<U>().*m_signature).m_id != m_id) {
                                (copy->template as<U>().*m_signature).m_id = m_id;
                                (copy->template as<U>().*m_signature).m_ptr = 0;
                            }
                        }
                    }
                }
            };
        public:
            std::vector<SingletonProcedure<T>*> m_removeProcedures;
            std::vector<SingletonProcedure<T>*> m_addProcedures;
            Singleton<T,U>(U* me) {
                m_me = me;
                m_ptr = 0;
            };
            ~Singleton<T,U>() {
                for (auto const& proc: m_removeProcedures) {
                    delete proc;
                }
                for (auto const& proc : m_addProcedures) {
                    delete proc;
                };
            };
            void set(T* val) {
                if (!isSameOrNull(m_id, val)) {
                    if (!m_ptr) {
                        m_ptr = m_me->m_manager->get<T>(m_me, m_id);
                    }
                    if (m_me->m_manager) {
                        m_me->removeReference(m_id);
                    }
                    for (auto const& proc : m_removeProcedures) {
                        (*proc)(m_id, m_ptr);
                    }
                    m_id = ID::nullID();
                    m_ptr = 0;
                }
                
                if (val) {
                    m_id = val->getID();
                }

                if (!m_me->m_manager) {
                    m_ptr = val;
                }

                if (val) {
                    if (m_me->m_manager) {
                        m_me->setReference(val);
                    }
                    for (auto const& proc : m_addProcedures) {
                        (*proc)(m_id, val);
                    }
                }

                if (m_me->m_node) {
                    updateCopies();
                }
            };
            void set(T& val) {
                set(&val);
            };
            T* get() {
                if (!m_ptr) {
                    if (m_me->m_manager) {
                        m_ptr = m_me->m_manager->get<T>(m_me, m_id);
                    }
                }
                return m_ptr;
            };
            bool has() {
                return !m_id.isNull();
            };
    };
}

#endif