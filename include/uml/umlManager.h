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

    template <class T = Element> class Sequence;
    class Model;

    /**
     * UmlManager is the object that handles all of the instantiation and deletion of UML objects
     * from a model. It follows object pool semantics to be able to hold information about large
     * models without taking up too much memory
     **/
    class UmlManager {
        friend class Parsers::ParserMetaData;
        friend struct Parsers::EmitterMetaData;
        friend class Element;
        friend class InstanceSpecification;
        friend class PackageableElement;
        template<typename> friend class Sequence;
        protected:
            struct DiscData {
                Element* m_managerElementMemory;
                std::string m_path;
                std::string m_mountPath;
                std::unordered_map<ID, Element*> m_references;
                std::unordered_map<ID, size_t> m_referenceCount;
            };
            void setReference(ID referencing, ID referenced, Element* ptr);
            void removeReference(ID referencing, ID referenced);
        private:
            std::unordered_set<ID> m_elements;
            std::unordered_map<ID, DiscData> m_disc;
            std::filesystem::path m_path;
            std::filesystem::path m_mountBase;
            Model* m_model;
            Element* m_root;
            void clear();
            void setElementAndChildrenMount(std::filesystem::path parentPath, Element& el);
        public:
            UmlManager();
            ~UmlManager();
            template <class T = Element> T& get(ID id) {
                if (!m_disc[id].m_managerElementMemory) {
                    aquire(id);
                }
                return *dynamic_cast<T*>(m_disc[id].m_managerElementMemory);
            };
            size_t count(ID id);
            template <class T = Element> T& create() {
                T* ret = new T;
                ret->setManager(this);
                m_elements.insert(ret->getID());
                DiscData discData = {ret};
                m_disc[ret->getID()] = discData;
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
}

#endif