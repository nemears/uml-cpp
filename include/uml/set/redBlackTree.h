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
        bool nodeIsLeft = node->m_parent && node->m_parent->m_left == node;
        node->m_right = rightNode->m_left;
        rightNode->m_left = node;
        rightNode->m_parent = node->m_parent;
        node->m_parent = rightNode;
        node->m_right->m_parent = node;
        if (!is_root(node, set)) {
            if (nodeIsLeft) {
                rightNode->m_parent->m_left = rightNode;
            } else {
                rightNode->m_parent->m_right = rightNode;
            }
        } else {
            set->m_root = rightNode;
        }
    }

    void right_rotate(SetNode* node, AbstractSet* set) {
        SetNode* leftNode = node->m_left;
        bool nodeIsLeft = node->m_parent && node->m_parent->m_left == node;
        node->m_left = leftNode->m_right;
        leftNode->m_right = node;
        leftNode->m_parent = node->m_parent;
        node->m_parent = leftNode;
        node->m_left->m_parent = node;
        if (!is_root(node, set)) {
            if (nodeIsLeft) {
                leftNode->m_parent->m_left = leftNode;
            } else {
                leftNode->m_parent->m_right = leftNode;
            }
        } else {
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

    void shiftNodes(SetNode* shiftedNode, SetNode* newNode, AbstractSet* set) {
        if (set->getRoot() == shiftedNode) {
            set->setRoot(newNode);
        } else if (shiftedNode->m_parent->m_left == shiftedNode) {
            shiftedNode->m_parent->m_left = newNode;
        } else {
            shiftedNode->m_parent->m_right = newNode;
        }
        if (set->getRoot() != newNode && newNode) {
            newNode->m_parent = shiftedNode->m_parent;
        }
    }

    void deleteCaseSix(SetNode* parent, SetNode* sibling, SetNode* distantCousin, AbstractSet* set, bool isLeft) {
        if (isLeft) {
            left_rotate(parent, set);
        } else {
            right_rotate(parent, set);
        }
        sibling->m_color = parent->m_color;
        parent->m_color = RedOrBlack::BLACK;
        distantCousin->m_color = RedOrBlack::BLACK;
    }

    void deleteFromTree(SetNode* node) {
        AbstractSet* set = node->set;
        SetNode* nodeToDelete = node;        
        if (set->getRoot() == node && !node->m_left && !node->m_right) {
            // node is root and has no children
            set->setRoot(0);
        } if (node->m_color == RedOrBlack::RED && !node->m_left && !node->m_right) {
            // node is red and has no children
            // simply just remove it
            if (node->m_parent->m_left == node) {
                node->m_parent->m_left = 0;
            } else {
                node->m_parent->m_right = 0;
            }
        } else if (node->m_left && node->m_right) {
            // find successor
            SetNode* succesor = node->m_left;
            while (succesor->m_left) {
                succesor = succesor->m_left;
            }

            // replace with succesor
            if (succesor->m_parent != node) {
                shiftNodes(succesor, succesor->m_right, set);
                succesor->m_right = node->m_right;
                succesor->m_right->m_parent = succesor;
            }
            shiftNodes(node, succesor, set);
            succesor->m_left = node->m_left;
            succesor->m_left->m_parent = succesor;

            // repaint succesor to maintain balance of red and black
            succesor->m_color = node->m_color;
        } else if (node->m_left) {
            if (node->m_left->m_color == RedOrBlack::RED) {
                // replace it with it's child and paint it black
                node->m_left->m_color = RedOrBlack::BLACK;
                if (set->getRoot() == node) {
                    // set root
                    set->setRoot(node->m_left);
                } else if (node->m_parent->m_left == node) {
                    node->m_parent->m_left = node->m_left;
                } else {
                    node->m_parent->m_right = node->m_left;
                }
            } else {
                // this should be impossible to get to
                throw SetStateException("deleteFromTree node->m_left->m_color == BLACK");
            }
        } else if (node->m_right) {
            if (node->m_right->m_color == RedOrBlack::RED) {
                // replace it with it's child and paint it black
                node->m_right->m_color = RedOrBlack::BLACK;
                if (set->getRoot() == node) {
                    // set root
                    set->setRoot(node->m_right);
                } else if (node->m_parent->m_left == node) {
                    node->m_parent->m_left = node->m_right;
                } else {
                    node->m_parent->m_right = node->m_right;
                }
            } else {
                // this should be impossible to get to
                throw SetStateException("deleteFromTree node->m_right->m_color == BLACK");
            }
        } else if (node->m_color == RedOrBlack::BLACK && !node->m_left && !node->m_right) {
            // black and no children
            // balance
            // case d1 covered by this loop condition
            while (!is_root(node, set)) {
                SetNode* sibling = 0;
                SetNode* closeCousin = 0;
                SetNode* distantCousin = 0;
                bool isLeft = true;
                if (node->m_parent->m_left == node) {
                    sibling = node->m_parent->m_right;
                    closeCousin = sibling->m_left;
                    distantCousin = sibling->m_right;
                } else {
                    sibling = node->m_parent->m_left;
                    closeCousin = sibling->m_right;
                    distantCousin = sibling->m_left;
                    isLeft = false;
                }
                if (sibling->m_color == RedOrBlack::RED) {
                    // case d3
                    if (isLeft) {
                        left_rotate(node->m_parent, set);
                    } else {
                        right_rotate(node->m_parent, set);
                    }
                    node->m_parent->m_color = RedOrBlack::RED;
                    sibling->m_color = RedOrBlack::BLACK;
                    sibling = closeCousin;
                    if (isLeft) {
                        distantCousin = sibling->m_right;
                    } else {
                        distantCousin = sibling->m_left;
                    }
                    if (distantCousin && distantCousin->m_color == RedOrBlack::RED) {
                        // case d6
                        deleteCaseSix(node->m_parent, sibling, distantCousin, set, isLeft);
                        break;
                    }
                    if (isLeft) {
                        closeCousin = sibling->m_left;
                    } else {
                        closeCousin = sibling->m_right;
                    }
                    if (closeCousin && closeCousin->m_color == RedOrBlack::RED) {
                        // case d5
                        if (isLeft) {
                            right_rotate(sibling, set);
                        } else {
                            left_rotate(sibling, set);
                        }
                        sibling->m_color = RedOrBlack::RED;
                        closeCousin->m_color = RedOrBlack::BLACK;
                        distantCousin = sibling;
                        sibling = closeCousin;
                    }
                }
                if (distantCousin && distantCousin->m_color == RedOrBlack::RED) {
                    // case d6
                    deleteCaseSix(node->m_parent, sibling, distantCousin, set, isLeft);
                    break;
                }
                if (closeCousin && closeCousin->m_color == RedOrBlack::RED) {
                    // case d5
                    if (isLeft) {
                        right_rotate(sibling, set);
                    } else {
                        left_rotate(sibling, set);
                    }
                    sibling->m_color = RedOrBlack::RED;
                    closeCousin->m_color = RedOrBlack::BLACK;
                    distantCousin = sibling;
                    sibling = closeCousin;
                }
                if (node->m_parent->m_color == RedOrBlack::RED) {
                    // case d4
                    sibling->m_color = RedOrBlack::RED;
                    node->m_parent->m_color = RedOrBlack::BLACK;
                    break;
                }
                // case d2
                sibling->m_color = RedOrBlack::RED;
                node = node->m_parent;
            }

            // remove from tree
            if (nodeToDelete->m_parent) {
                if (nodeToDelete->m_parent->m_left == nodeToDelete) {
                    nodeToDelete->m_parent->m_left = 0;
                } else {
                    nodeToDelete->m_parent->m_right = 0;
                }
            }
        } else {
            // error, should be impossible
            throw SetStateException("Unhandled edge case in Red Black Tree deleteFromTree, contact developer!");
        }

        // delete the node from memory
        delete nodeToDelete;
    }
}