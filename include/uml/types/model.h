#ifndef _UML_MODEL_H_
#define _UML_MODEL_H_

#include "package.h"

namespace UML{
    class Model : public Package {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            Model();
            std::string m_viewpoint;
        public:
            virtual ~Model();
            std::string getViewpoint() const;
            void setViewpoint(const std::string& viewpoint);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::MODEL;
            };
    };
}
#endif