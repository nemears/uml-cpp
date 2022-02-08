#ifndef UML_MACROS_H
#define UML_MACROS_H

#define UML_IMPLEMENT_BASE_CLASS(signature, base) base::signature

#define UML_IMPLEMENT_BASE_CLASS(signature, base, ...) \
    base::signature; \
    UML_IMPLEMENT_BASE_CLASS(signature, base __VA_OPT__(,) __VA_ARGS__ )

#define IMPLEMENT_ELEMENT_VIRTUALS0(type, ...)    \
    void type::referencingReleased(ID id) {     \
        UML_IMPLEMENT_BASE_CLASS(referencingReleased(id), __VA_ARGS__)   \
    } \
    void type::referenceReindexed(ID oldID, ID newID)   \
        UML_IMPLEMENT_BASE_CLASS(referenceReindexed(oldID, newID), __VA_ARGS__)   \
    } \
    void type::referenceErased(ID id) { \
        UML_IMPLEMENT_BASE_CLASS(referenceErased(id), __VA_ARGS__)   \
    } 


#define IMPLEMENT_ELEMENT_VIRTUALS1(type, one, ...) \
    void type::referencingReleased(ID id) {     \
        UML_IMPLEMENT_BASE_CLASS(referencingReleased(id), __VA_ARGS__)   \
        one.release(id);    \
    } \
    void type::referenceReindexed(ID oldID, ID newID)   \
        UML_IMPLEMENT_BASE_CLASS(referenceReindexed(oldID, newID), __VA_ARGS__)   \
        one.reindex(oldID, newID);  \
    } \
    void type::referenceErased(ID id) { \
        UML_IMPLEMENT_BASE_CLASS(referenceErased(id), __VA_ARGS__)   \
        one.eraseElement(id);   \
    } 

#define IMPLEMENT_ELEMENT_VIRTUALS2(type, one, two, ...) \
    void type::referencingReleased(ID id) {     \
        UML_IMPLEMENT_BASE_CLASS(referencingReleased(id), __VA_ARGS__)   \
        one.release(id);    \
        two.release(id);    \
    } \
    void type::referenceReindexed(ID oldID, ID newID) {  \
        UML_IMPLEMENT_BASE_CLASS(referenceReindexed(oldID, newID), __VA_ARGS__)   \
        one.reindex(oldID, newID);  \
        two.reindex(oldID, newID);  \
    } \
    void type::referenceErased(ID id) { \
        UML_IMPLEMENT_BASE_CLASS(referenceErased(id), __VA_ARGS__)   \
        one.eraseElement(id);   \
        two.eraseElement(id);   \
    } 

#endif