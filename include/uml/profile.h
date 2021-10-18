#ifndef PROFILE_H
#define PROFILE_H

#include "package.h"

namespace UML {
    class Profile : public Package {

        friend class Stereotype;
        friend class UmlManager;

        protected:
            class ProfileAddOwnedStereotypeFunctor : public TemplateAbstractSequenceFunctor<Stereotype, Profile> {
                public:
                    ProfileAddOwnedStereotypeFunctor(Profile* el) : TemplateAbstractSequenceFunctor(el) {};
                    void operator()(Stereotype& el) const override;
            };
            class ProfileRemoveOwnedStereotypeFunctor : public TemplateAbstractSequenceFunctor<Stereotype, Profile> {
                public:
                    ProfileRemoveOwnedStereotypeFunctor(Profile* el) : TemplateAbstractSequenceFunctor(el) {};
                    void operator()(Stereotype& el) const override;
            };
            Profile();
        public:
            Profile(const Profile& profile);
            virtual ~Profile();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PROFILE;
            };
    };
}

#endif