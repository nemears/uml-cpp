#ifndef MODELH
#define MODELH

#include "package.h"

namespace UML{
    class Model : public Package {

        friend class UmlManager;

        protected:
            Model();
            std::string m_viewpoint;
        public:
            Model(const Model& rhs);
            std::string getViewpoint() const;
            void setViewpoint(std::string viewpoint);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::MODEL;
            };
    };
}
#endif