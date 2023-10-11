#pragma once

#include "uml/umlPtr.h"

namespace UML {
    class AbstractSet;

    enum class RedOrBlack {
        RED,
        BLACK
    };

    struct SetNode {
        RedOrBlack m_color = RedOrBlack::BLACK;
        ElementPtr m_ptr;
        SetNode* m_parent = 0;
        SetNode* m_left = 0;
        SetNode* m_right = 0;
        AbstractSet* set = 0;
	    virtual ~SetNode() {}
    };
}