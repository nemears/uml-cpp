#pragma once

#include "uml/umlPtr.h"

namespace UML {
    class AbstractSet;
    class SetAllocationPolicy;
    template <
                class T, 
                class U, 
                class AdditionPolicy,
                class RemovalPolicy,
                class AllocationPolicy
            >
    class PrivateSet;

    /**
     * This class is a node of RedBlack balanced Binary Search Tree,
    */
    class SetNode {

        friend SetAllocationPolicy;
        template <
                    class T, 
                    class U, 
                    class AdditionPolicy,
                    class RemovalPolicy,
                    class AllocationPolicy
                >
        friend class PrivateSet;

        enum class RedOrBlack {
            RED,
            BLACK
        };

        private:
            RedOrBlack m_color = RedOrBlack::BLACK;
            ElementPtr m_ptr;
            SetNode* m_parent = 0;
            SetNode* m_left = 0;
            SetNode* m_right = 0;
            AbstractSet* set = 0;

            // helper functions
            void insertBST(SetNode* node);
            bool is_root(AbstractSet* set);
            void left_rotate(AbstractSet* set);
            void right_rotate(AbstractSet* set);
            void shiftNodes(SetNode* newNode, AbstractSet* set);
            void deleteCaseSix(SetNode* parent, SetNode* sibling, SetNode* distantCousin, AbstractSet* set, bool isLeft);
        public:

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