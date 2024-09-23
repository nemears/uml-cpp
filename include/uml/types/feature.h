#pragma once

#include "redefinableElement.h"
#include "uml/set/singleton.h"
#include "uml/types/element.h"

namespace UML {

    class Classifier;
    typedef UmlPtr<Classifier> ClassifierPtr;

    class Feature : virtual public RedefinableElement {

        friend class Classifier;
        friend struct ElementInfo<Feature>;

        protected:
            ReadOnlySingleton<Classifier, Feature> m_featuringClassifier = ReadOnlySingleton<Classifier, Feature>(this);
            bool m_static = false;
            ReadOnlySingleton<Classifier, Feature>& getFeaturingClassifierSingleton();
            Feature(std::size_t elementType,AbstractManager& manager);
        public:
            ClassifierPtr getFeaturingClassifier() const;
            bool isStatic();
            void setStatic(bool isStatic);
            typedef TypeInfo<std::tuple<RedefinableElement>, Feature> Info;
    };

    template <>
    struct ElementInfo<Feature> : public DefaultInfo {
        static constexpr std::string_view name {"Feature"};
        static SetList sets(Feature& el) {
            return SetList {
                makeSetPair("featuringClassifier", el.m_featuringClassifier)
            };
        }
        static const bool extraData = true;
        struct FeatureStaticPolicy : public AbstractDataPolicy {
            Feature& el;
            FeatureStaticPolicy(Feature& el) : el(el) {}
            std::string getData() override {
                if (el.isStatic()) {
                    return "true";
                }
                return "";
            }
            void setData(std::string data) override {
                if (data == "true") {
                    el.setStatic(true);
                }
            }
        };
        static DataList data(Feature& el) {
            return DataList {
                createDataPair("static", new FeatureStaticPolicy(el))
            };
        }
    };
}
