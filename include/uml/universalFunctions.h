#ifndef UNIVERSAL_FUNCTIONS_H
#define UNIVERSAL_FUNCTIONS_H

namespace UML {
    template <class T = Element> T* universalGet(ID theID, T* thePtr, UmlManager* theManager) {
        if (!theID.isNull()) {
            if (!thePtr) {
                if (theManager) {
                    thePtr = &theManager->get<T>(theID);
                }
            }
            return thePtr;
        }
        return 0;
    };
}

#endif