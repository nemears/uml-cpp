#pragma once

#include "package.h"

namespace UML{
    class Model : public Package {

        friend struct ElementInfo<Model>;

        protected:
            std::string m_viewpoint;
        public:
            Model(std::size_t elementType, AbstractManager& manager);
            std::string getViewpoint() const;
            void setViewpoint(const std::string& viewpoint);
            typedef TypeInfo<std::tuple<Package>, Model> Info;
    };

    template <>
    struct ElementInfo<Model> : public DefaultInfo {
        static const bool abstract = false;
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "Model"; }
        static SetList sets(__attribute__((unused)) Model& el) {
            return SetList{};
        }
    };
}
