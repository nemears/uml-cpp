#pragma once

#include "uml/umlPtr.h"

namespace UML {
    class SetNode;
}

size_t getNumberOfBlackToLeafNodes(UML::SetNode*);
void checkAllPathsAreEqual(UML::SetNode*);
void checkNode(UML::SetNode*);
void isValidRedBlackTree(UML::SetNode*);

namespace UML {
    class AbstractSet;

    /**
     * This class is a node of RedBlack balanced Binary Search Tree,
    */
    class SetNode {
        public:
            enum class RedOrBlack {
                RED,
                BLACK
            };
            RedOrBlack m_color = RedOrBlack::BLACK;
            SetNode* m_parent = 0;
            SetNode* m_left = 0;
            SetNode* m_right = 0;
            AbstractSet* set = 0;
            ElementPtr m_ptr;

            // helper functions
            void insertBST(SetNode* node);
            bool is_root(AbstractSet* set);
            void left_rotate(AbstractSet* set);
            void right_rotate(AbstractSet* set);
            void shiftNodes(SetNode* newNode, AbstractSet* set);
            void deleteCaseSix(SetNode* parent, SetNode* sibling, SetNode* distantCousin, AbstractSet* set, bool isLeft);

            // search and insert
            SetNode* search(ID key);
            void insert(SetNode* node);

            // rule of three
            SetNode() {};
            virtual ~SetNode(); // remove from tree by deleting
            SetNode(const SetNode&) = delete;
            SetNode& operator= (const SetNode&) = delete;
    };
}
