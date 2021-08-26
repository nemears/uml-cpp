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
        struct EmitterMetaData;
    }

    class InstanceSpecification;
    class PackageableElement;
    class NamedElement;
    class TypedElement;

    template <class T = Element> class Sequence;
    class Model;

    class SetOwnerFunctor;
    class RemoveOwnerFunctor;

    class ManagerStateException : public std::exception {
        const char* what() const throw() override {
            return "UmlManager bad state!";
        };
    };

    /**
     * UmlManager is the object that handles all of the instantiation and deletion of UML objects
     * from a model. It follows object pool semantics to be able to hold information about large
     * models without taking up too much memory
     * 
     * Important notes,
     *  1) create<T>() is the proper way to instantiate any type of uml element into a model
     *  2) After creating the manager make sure to tell it where to store it's swapped memory
     *     with the mount(path) method and where to keep it's data persistently with save(path)
     *  3) To keep memory use down but disc access only when we need to the api uses object pool
     *     semantics for control of when memory needs to be used and this object controls it.
     *     a) To enable an object pool state the manager must be mounted somewhere on the disc to
     *        write the elements to a database, this can be done with UmlManager::mount(path)
     *     b) Once mounted Elements that have been created or parsed in may be written to disc 
     *        with the UmlManager::release(ID id) method, and brought back into memory with the
     *        UmlManager::aquire(ID id) method.
     *  4) Copying Elements is fine, they are automatically tracked upon copying, infact it is
     *     encouraged to copy from the Manager just for the scope you need, e.g:
     *          Package copiedJustForMyScope = manager.create<Package>();
     *      instead of:
     *          Package& referenceToManagerMemory = manager.create<Package>();
     *      The main benefit is because a client using a manager can easily enforce not releasing
     *      an element that has copies not yet deleted
     *  5) The Manager can be used as a top level filter of all of the model elements through the
     *         get<T>(ID id) function
     **/

    struct ManagerNode {
        Element* m_managerElementMemory;
        std::string m_path;
        std::string m_mountPath;
        std::unordered_map<ID, ManagerNode*> m_references;
        std::unordered_map<ID, size_t> m_referenceCount;
        std::vector<ID> m_referenceOrder;
        std::unordered_set<Element*> m_copies;
    };

    class UmlManager {
        friend class Parsers::ParserMetaData;
        friend struct Parsers::EmitterMetaData;
        friend class Element;
        friend class InstanceSpecification;
        friend class PackageableElement;
        friend class NamedElement;
        friend class TypedElement;
        friend class SetOwnerFunctor;
        friend class RemoveOwnerFunctor;
        friend class Comment;
        friend class PackageMerge;
        template<typename> friend class Sequence;
        private:
            std::unordered_set<ID> m_elements;
            std::unordered_map<ID, ManagerNode> m_graph;
            std::filesystem::path m_path;
            std::filesystem::path m_mountBase;
            Model* m_model;
            Element* m_root;
            void clear();
            void setElementAndChildrenMount(std::filesystem::path parentPath, Element& el);
            template <class T = Element, class U = Element> void updateCopiesSingleton(T* me, ID id, ID T::*memberID, U* T::*memberPtr) {
                if (me->m_node) {
                    if (me->m_node->m_managerElementMemory != me) {
                        if ((me->m_node->m_managerElementMemory->template as<T>().*memberID) != id) {
                            (me->m_node->m_managerElementMemory->template as<T>().*memberID) = id;
                            (me->m_node->m_managerElementMemory->template as<T>().*memberPtr) = 0;
                        }
                    }
                    for (auto& copy : me->m_node->m_copies) {
                        if (copy != me) {
                            if ((copy->template as<T>().*memberID) != id) {
                                (copy->template as<T>().*memberID) = id;
                                (copy->template as<T>().*memberPtr) = 0;
                            }
                        }
                    }
                } else {
                    if (m_graph[me->getID()].m_managerElementMemory != me) {
                        if ((m_graph[me->getID()].m_managerElementMemory->template as<T>().*memberID) != id) {
                            (m_graph[me->getID()].m_managerElementMemory->template as<T>().*memberID) = id;
                            (m_graph[me->getID()].m_managerElementMemory->template as<T>().*memberPtr) = 0;
                        }
                    }
                    for (auto& copy : m_graph[me->getID()].m_copies) {
                        if (copy != me) {
                            if ((copy->template as<T>().*memberID) != id) {
                                (copy->template as<T>().*memberID) = id;
                                (copy->template as<T>().*memberPtr) = 0;
                            }
                        }
                    }
                }
            };
            /** Using this get is faster than the get<T>(ID id) method (usually) because it will base it's
             *  search on a particular element, only for internal api use where trying to set and return a ptr**/
            template <class T = Element, class U = Element> T* get(U* me, ID theID, T* U::*thePtr) {
                if (!theID.isNull()) {
                    if (!(me->*thePtr)) {
                        if (me->m_node) {
                            if (me->m_node->m_references.count(theID)) {
                                if (!me->m_node->m_references[theID]->m_managerElementMemory) {
                                    aquire(theID);
                                }
                                (me->*thePtr) = dynamic_cast<T*>(me->m_node->m_references[theID]->m_managerElementMemory);
                            } else {
                                throw ManagerStateException();
                            }
                        } else {
                            throw ManagerStateException();
                        }
                    }
                    return (me->*thePtr);
                }
                return 0;
            };
            template <class T = Element> T* get(Element* me, ID theID) {
                if (!theID.isNull()) {
                    if (me->m_node) {
                        if (me->m_node->m_references.count(theID)) {
                            if (!me->m_node->m_references[theID]->m_managerElementMemory) {
                                aquire(theID);
                            }
                            return dynamic_cast<T*>(me->m_node->m_references[theID]->m_managerElementMemory);
                        }
                    } else {
                        throw ManagerStateException();
                    }
                }
                return 0;
            }
        public:
            UmlManager();
            ~UmlManager();
            template <class T = Element> T& get(ID id) {
                if (!m_graph[id].m_managerElementMemory) {
                    aquire(id);
                }
                return *dynamic_cast<T*>(m_graph[id].m_managerElementMemory);
            };
            size_t count(ID id);
            template <class T = Element> T& create() {
                T* ret = new T;
                ret->setManager(this);
                m_elements.insert(ret->getID());
                // ManagerNode discData = {ret};
                m_graph[ret->getID()] = {ret};
                ret->m_node = &m_graph[ret->getID()];
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

            // Sets up composite directory of model for quick aquire and release
            void mount(std::string path);
            // WARN: unfinished
            // NOTE: I want to go away from key pair access, and efficiently traverse the model tree to find the id
            // I think performance can be improved a lot by going in that direction 
            void aquire(ID id);
            void release(ID id);
            
            /**
             * Saves the manager's model to the manager's path as a uml configuration file
             * WARN: if the model is saved, pointers to elements in model will have to be reaccessed 
             * from the manager if the model is open'd again because they have been deleted
             **/
            void save();
            /**
             * Saves the specified model to the specified path as a uml configuration file
             * WARN: if the model is saved, pointers to elements in model will have to be reaccessed 
             * from the manager if the model is open'd again because they have been deleted
             **/
            void save(std::string path);
            /**
             * Loads the file specified in the manager's path into memory referenceable by the manager
             * WARN: if the manager already has members in the path, it will dereference them when they are reloaded
             **/
            void open();
            /**
             * Loads the file specified by the path parameter into memory referenceable by the manager and sets root
             * to top level element
             * WARN: if the manager already has members in the path, it will dereference them when they are reloaded
             **/
            void open(std::string path);

            /**
             * Parses the file into memory, but does not set root
             **/
            Element* parse(std::string path);

            void setModel(Model* model);
            void setRoot(Element* el);
            Model* getModel();
            Element* getRoot();
            void setPath(ID elID, std::string path);
    };

    class ManagerNotMountedException : public std::exception {
        const char* what() const throw() override {
            return "Tried to aquire or release when manager has not been mounted yet!";
        };
    };
}

#endif