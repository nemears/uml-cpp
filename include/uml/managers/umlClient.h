#ifndef _UML_UML_CLIENT_H_
#define _UML_UML_CLIENT_H_

#include "managerPolicy.h"
#include "serverAccessPolicy.h"
#include "serverPersistencePolicy.h"

namespace UML {
    // typedef Manager<SimpleAccessPolicy, ServerPersistencePolicy> UmlClient;

    class UmlClient : public Manager<ServerAccessPolicy, ServerPersistencePolicy> {
        public:
            ElementPtr get(std::string qualifiedName) {
                return getElFromServer(qualifiedName, this);
            }
            ElementPtr get(ID id) {
                return Manager<ServerAccessPolicy, ServerPersistencePolicy>::get(id);
            }
            void setRoot(Element* root) override {
                setRootForServer(root);
                Manager<ServerAccessPolicy, ServerPersistencePolicy>::setRoot(root);
            }
    };

    // class UmlClient : public Manager<SimpleAccessPolicy, ServerPersistencePolicy> {
    //     protected:
    //         Element* get(Element* me, ID theID) override;
    //     public:
    //         ElementPtr get(ID id) override;
    //         ElementPtr get(std::string qualifiedName);
    //         void put(Element& el);
    //         void putAll();
    //         Element& post(ElementType eType);
    //         template<class T = Element> 
    //         UmlPtr<T> create() {
    //             return UmlPtr<T>(&post(T::elementType()).template as<T>());
    //         }
    //         void erase(Element& el) override;
    //         void release(Element& el) override;
    //         template <class ... Elements> void release(Element& el, Elements&... els) {
    //             release(el);
    //             release(els...);
    //         }
    //         void setRoot(Element* root) override;
    //         void shutdownServer(); // maybe we need to pass a key or something
    //         void save() override;
    //         void save(std::string path) override;
    // };
}

#endif