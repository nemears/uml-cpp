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
    template <class T>
    class SetIterator;
    template <class T>
    class ID_Set;
    template <class T>
    class PtrSet;
    template <
                class T, 
                class U,
                class AdditionPolicy = DoNothingAdd<T, U>,
                class RemovalPolicy = DoNothingRemove<T, U>
            >
    class CustomReadOnlySet; 

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
        template <class T>
        friend class SetIterator; 
        template <class T>
        friend class ID_Set;
        template <class T>
        friend class PtrSet;
        template <
                class T, 
                class U,
                class AdditionPolicy,
                class RemovalPolicy
            >
        friend class CustomReadOnlySet; 

        enum class RedOrBlack {
            RED,
            BLACK
        };

        private:
            RedOrBlack m_color = RedOrBlack::BLACK;
            SetNode* m_parent = 0;
            SetNode* m_left = 0;
            SetNode* m_right = 0;
        protected:
            AbstractSet* set = 0;
            ElementPtr m_ptr;

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
            ElementPtr ptr() const;

            // rule of three
            SetNode() {};
            virtual ~SetNode(); // remove from tree by deleting
            SetNode(const SetNode&) = delete;
            SetNode& operator= (const SetNode&) = delete;
    };
}
