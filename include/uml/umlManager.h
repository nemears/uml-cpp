#ifndef UML_MANAGER_H
#define UML_MANAGER_H

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
    class Property;
    class Feature;
    class Package;

    template <class T, class U> class Singleton;
    class Model;

    class SetOwnerFunctor;
    class RemoveOwnerFunctor;

    class ManagerStateException : public std::exception {
        std::string m_msg;
        const char* what() const throw() override {
            return m_msg.c_str();
        };
        public:
            ManagerStateException(){};
            ManagerStateException(std::string msg) : m_msg("UmlManager bad state! " + msg) {};
    };

    class UnknownID_Exception : public std::exception {
        private:
            std::string m_msg;
            const char* what() const throw() override {
                return m_msg.c_str();
            };
        public:
            UnknownID_Exception(ID id) : m_msg("unknown ID given when trying to get an element from the manager, ID: "  + id.string()) {};
    };

    template <class T = Element>
    class CreateValue : public T {

        friend class UmlManager;

        CreateValue() : Element(T::elementType()) {
            Element::m_createVal = true;
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
        friend class Property;
        friend class Feature;
        friend class Package;
        friend class AddToMountFunctor;
        template <class T, class U> friend class Singleton;
        template <class T, class U> friend class Set;
        template <class T> friend class SetIterator;

        protected:
            std::unordered_set<ID> m_elements;
            std::unordered_map<ID, ManagerNode> m_graph;
            std::filesystem::path m_path;
            std::filesystem::path m_mountBase;
            Model* m_model;
            Element* m_root;
            void clear();
            /** Using this get is faster than the get<T>(ID id) method (usually) because it will base it's
             *  search on a particular element, only for internal api use where trying to set and return a ptr**/
            template <class T = Element> T* get(Element* me, ID theID) {
                if (!theID.isNull()) {
                    if (me->m_node) {
                        if (me->m_node->m_references.count(theID)) {
                            if (!me->m_node->m_references[theID]) {
                                if (loaded(theID)) {
                                    me->restoreReference(&get<T>(theID));
                                } else {
                                    Element* aquired = aquire(theID);
                                    me->m_node->m_references[theID] = aquired->m_node;
                                }
                            }
                            return dynamic_cast<T*>(me->m_node->m_references[theID]->m_managerElementMemory);
                        } else {
                            throw ManagerStateException();
                        }
                    } else {
                        aquire(theID);
                        return  dynamic_cast<T*>(me->m_node->m_references[theID]->m_managerElementMemory);
                    }
                }
                return 0;
            }
            void addToMount(Element& el);
            virtual void createNode(Element* el);
            void eraseNode(ManagerNode* node, ID id);
            void releaseNode(Element& el);
        public:
            UmlManager();
            virtual ~UmlManager();
            /**
              * get<T>(id) is used to get an element from the manager through just it's 
              * @param id, the id of the element you wish to get from the manager
              * @return the element you wish to get from the manager
             **/
            template <class T = Element> T& get(ID id) {
                if (m_elements.count(id)) {
                    if (!m_graph.count(id)) {
                        aquire(id);
                    }
                    return m_graph[id].m_managerElementMemory->template as<T>();
                } else {
                    throw UnknownID_Exception(id);
                }
            };
            virtual Element& get(ID id);
            size_t count(ID id);
            virtual bool loaded(ID id);
            template <class T = Element> CreateValue<T>& createVal() {
                CreateValue<T>* ret = new CreateValue<T>;
                ret->m_manager = this;
                m_elements.insert(ret->getID());
                createNode(ret);
                ret->m_node = &m_graph[ret->getID()];
                return *ret;
            };
            template <class T = Element> T& create() {
                T* ret = new T;
                ret->m_manager = this;
                m_elements.insert(ret->getID());
                createNode(ret);
                ret->m_node = &m_graph[ret->getID()];
                return *ret;
            };
            Element& create(ElementType eType);
            void reindex(ID oldID, ID newID);
            // Sets up composite directory of model for quick aquire and release
            void mount(std::string path);
            void mountEl(Element& el);
            /**
             * aquire(id) will aquire an element from disc if it is not already loaded
             * and return a pointer to the element if it was succesfully aquired
             * @param id, the id of the element you wish to aquire
             * @return a pointer to the element you wish to aquire, will never be null
             **/
            virtual Element* aquire(ID id);
            virtual void release(ID id);
            /**
             * release(el) will effectively delete the element object and write it's contents
             * to a file so that it can be aquired later if needed. Caveat: make sure all references
             * to this element are being used because this function does delete the pointer to the
             * element's memory.
             * @param el, the element being released from memory
             **/
            virtual void release(Element& el);
            template <class ... Elements> void release(Element& el, Elements&... els) {
                release(el);
                release(els...);
            };
            void erase(ID id);
            /**
             * erase(el) effectively deletes the element object from memory, as well as removes all
             * references to this element from within the manager. This function is used to dispose 
             * of an element completely and not keep track of it in persistent/mounted storage
             * @param el, the element to erase
             **/
            virtual void erase(Element& el);
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
            void setRoot(Element& el);
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