#pragma once

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
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::MODEL;
            };
    };
}
