#pragma once

#include "setNode.h"
#include "abstractSet.h"

namespace UML {

    void insertBST(SetNode* parent, SetNode* node) {
        if (!parent->m_left) {
            parent->m_left = node;
            node->m_parent = parent;
        } else if (!parent->m_right) {
            if (node->m_ptr.id() < parent->m_left->m_ptr.id()) {
                parent->m_right = parent->m_left;
                parent->m_left = node;
            } else {
                parent->m_right = node;
            }
            node->m_parent = parent;
        } else if (parent->m_left->m_ptr.id() > node->m_ptr.id()) {
            insertBST(parent->m_left, node);
        } else {
            insertBST(parent->m_right, node);
        }
    }

    // determines if the parent of the node is a valid part of this set
    // basically a boolean to check if it is a valid root of the set
    bool is_root(SetNode* node, AbstractSet* set) {
        return set->m_root == node;
    }

    void left_rotate(SetNode* node, AbstractSet* set) {
        SetNode* rightNode = node->m_right;
        node->m_right = rightNode->m_left;
        rightNode->m_left = node;
        rightNode->m_parent = node->m_parent;
        node->m_parent = rightNode;
        node->m_right->m_parent = node;
        if (set->m_root == node) {
            set->m_root = rightNode;
        }
    }

    void right_rotate(SetNode* node, AbstractSet* set) {
        SetNode* leftNode = node->m_left;
        node->m_left = leftNode->m_left;
        leftNode->m_right = node;
        leftNode->m_parent = node->m_parent;
        node->m_parent = leftNode;
        node->m_left->m_parent = node;
        if (set->m_root == node) {
            set->m_root = leftNode;
        }
    }

    void insert(SetNode* treeRoot, SetNode* node) {
        AbstractSet* set = node->set;
        insertBST(treeRoot, node);
        node->m_color = RedOrBlack::RED;
        while (!is_root(node, set) && node->m_parent->m_color == RedOrBlack::RED) {
            // parent is not black
            // uncle node
            SetNode* uncle = 0;
            bool leftUncle = true;
            if (node->m_parent->m_parent->m_left == node->m_parent) {
                uncle = node->m_parent->m_parent->m_right;
                leftUncle = false;
            } else {
                uncle = node->m_parent->m_parent->m_left;
                leftUncle = true;
            }
            if (uncle && uncle->m_color == RedOrBlack::RED) {
                uncle->m_color = RedOrBlack::BLACK;
                node->m_parent->m_color = RedOrBlack::BLACK;
                node->m_parent->m_parent->m_color = RedOrBlack::RED;
                node = node->m_parent->m_parent;
            } else {
                // rebalance
                if (leftUncle) {
                    if (node->m_parent->m_left == node) {
                        node = node->m_parent;
                        right_rotate(node, set);
                    }
                } else {
                    if (node->m_parent->m_right == node) {
                        node = node->m_parent;
                        left_rotate(node, set);
                    }
                }
                // if (node->m_parent->m_right == node) {
                //     node = node->m_parent;
                //     if (leftUncle) {
                //         right_rotate(node, set);
                //     } else {
                //         left_rotate(node, set);
                //     }
                // }
                node->m_parent->m_color = RedOrBlack::BLACK;
                node->m_parent->m_parent->m_color = RedOrBlack::RED;
                if (leftUncle) {
                    left_rotate(node->m_parent->m_parent, set);
                } else {
                    right_rotate(node->m_parent->m_parent, set);
                }
            }
        }
        set->m_root->m_color = RedOrBlack::BLACK;
    }
}