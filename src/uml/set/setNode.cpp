#include "uml/set/setNode.h"
#include "uml/set/abstractSet.h"

using namespace UML;

SetNode* SetNode::search(ID key) {
    if (m_ptr.id() == key) {
        return this;
    }
    if (key < m_ptr.id()) {
        if (m_left) {
            return m_left->search(key);
        }
    } else {
        if (m_right) {
            return m_right->search(key);
        }
    }
    return 0;
}

void SetNode::insertBST(SetNode* node) {
    if (node->m_ptr.id() == m_ptr.id()) {
        throw SetStateException("Duplicate node added to set!");
    }
    if (m_ptr.id() > node->m_ptr.id()) {
        SetNode* nextParent = m_left;
        if (!nextParent) {
            m_left = node;
            node->m_parent = this;
        } else {
            nextParent->insertBST(node);
        }
    } else {
        SetNode* nextParent = m_right;
        if (!nextParent) {
            m_right = node;
            node->m_parent = this;
        } else {
            nextParent->insertBST(node);
        }
    }
}

// determines if the parent of the node is a valid part of this set
// basically a boolean to check if it is a valid root of the set
bool SetNode::is_root(AbstractSet* set) {
    return !m_parent || set->getRoot() == this;
}

void SetNode::left_rotate(AbstractSet* set) {
    SetNode* rightNode = m_right;
    bool nodeIsLeft = m_parent && m_parent->m_left == this;
    m_right = rightNode->m_left;
    rightNode->m_left = this;
    rightNode->m_parent = m_parent;
    m_parent = rightNode;
    if (m_right) {
        m_right->m_parent = this;
    }
    if (!is_root(set)) {
        if (nodeIsLeft) {
            rightNode->m_parent->m_left = rightNode;
        } else {
            rightNode->m_parent->m_right = rightNode;
        }
    } else {
        set->setRoot(rightNode);
    }
}

void SetNode::right_rotate(AbstractSet* set) {
    SetNode* leftNode = m_left;
    bool nodeIsLeft = m_parent && m_parent->m_left == this;
    m_left = leftNode->m_right;
    leftNode->m_right = this;
    leftNode->m_parent = m_parent;
    m_parent = leftNode;
    if (m_left) {
        m_left->m_parent = this;
    }
    if (!is_root(set)) {
        if (nodeIsLeft) {
            leftNode->m_parent->m_left = leftNode;
        } else {
            leftNode->m_parent->m_right = leftNode;
        }
    } else {
        set->setRoot(leftNode);
    }
}

void SetNode::insert(SetNode* node) {
    AbstractSet* set = node->set;
    insertBST(node);
    node->m_color = RedOrBlack::RED;
    while (!node->is_root(set) && node->m_parent->m_color == RedOrBlack::RED) {
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
                    node->right_rotate(set);
                }
            } else {
                if (node->m_parent->m_right == node) {
                    node = node->m_parent;
                    node->left_rotate(set);
                }
            }
            node->m_parent->m_color = RedOrBlack::BLACK;
            node->m_parent->m_parent->m_color = RedOrBlack::RED;
            if (leftUncle) {
                node->m_parent->m_parent->left_rotate(set);
            } else {
                node->m_parent->m_parent->right_rotate(set);
            }
        }
    }
    set->getRoot()->m_color = RedOrBlack::BLACK;
}

void SetNode::shiftNodes(SetNode* newNode, AbstractSet* set) {
    if (set->getRoot() == this) {
        set->setRoot(newNode);
    } else if (m_parent->m_left == this) {
        m_parent->m_left = newNode;
    } else {
        m_parent->m_right = newNode;
    }
    if (set->getRoot() != newNode && newNode) {
        newNode->m_parent = m_parent;
    }
}

void SetNode::deleteCaseSix(SetNode* parent, SetNode* sibling, SetNode* distantCousin, AbstractSet* set, bool isLeft) {
    if (isLeft) {
        parent->left_rotate(set);
    } else {
        parent->right_rotate(set);
    }
    sibling->m_color = parent->m_color;
    parent->m_color = RedOrBlack::BLACK;
    distantCousin->m_color = RedOrBlack::BLACK;
}

SetNode::~SetNode() {
    bool notSwapped = true;
    do {
        notSwapped = true;
        if (is_root(set) && !m_left && !m_right) {
            // node is root and has no children
            if (set->getRoot() == this) {
                set->setRoot(0);
            } 
        } else if (m_color == RedOrBlack::RED && !m_left && !m_right) {
            // node is red and has no children
            // simply just remove it
            if (m_parent->m_left == this) {
                m_parent->m_left = 0;
            } else {
                m_parent->m_right = 0;
            }
        } else if (m_left && m_right) {
            //find successor
            SetNode* succesor = 0;
            if (m_right) {
                succesor = m_right;
                while (succesor->m_left) {
                    succesor = succesor->m_left;
                }
            } else {
                succesor = m_parent;
                SetNode* currNode = 0;
                while (succesor && set->getRoot() != succesor && currNode == succesor->m_right) {
                    currNode = succesor;
                    succesor = succesor->m_parent;
                }
            }

            // swap with successor
            SetNode* parent = m_parent;
            SetNode* left = m_left;
            SetNode* right = m_right;
            RedOrBlack color = m_color;
            bool isRoot = is_root(set);
            m_color = succesor->m_color;
            m_left = succesor->m_left;
            if (m_left) {
                m_left->m_parent = this;
            }
            m_right = succesor->m_right;
            if (m_right) {
                m_right->m_parent = this;
            }
            if (succesor->m_parent == this) {
                m_parent = succesor;
                succesor->m_right = this;
                succesor->m_left = left;
                left->m_parent = succesor;
            } else {
                m_parent = succesor->m_parent;
                if (m_parent->m_left == succesor) {
                    m_parent->m_left = this;
                } else {
                    m_parent->m_right = this;
                }
                succesor->m_left = left;
                left->m_parent = succesor;
                succesor->m_right = right;
                right->m_parent = succesor;
            }
            succesor->m_parent = parent;
            if (parent) {
                if (parent->m_left == this) {
                    parent->m_left = succesor;
                } else {
                    parent->m_right = succesor;
                }
            }
            succesor->m_color = color;
            if (isRoot) {
                set->setRoot(succesor);
            }

            if (succesor->set != set) {
                throw SetStateException("deleting node and swaping values, nodes do not have same set! contact dev!");
            }
            notSwapped = false;
        } else if (m_left) {
            if (m_left->m_color == RedOrBlack::RED) {
                // replace it with it's child and paint it black
                m_left->m_color = RedOrBlack::BLACK;
                if (set->getRoot() == this) {
                    // set root
                    set->setRoot(m_left);
                    m_left->m_parent = m_parent;
                } else {
                    if (m_parent->m_left == this) {
                        m_parent->m_left = m_left;
                    } else {
                        m_parent->m_right = m_left;
                    }
                    m_left->m_parent = m_parent;
                }
            } else {
                // this should be impossible to get to
                throw SetStateException("deleteFromTree node->m_left->m_color == BLACK");
            }
        } else if (m_right) {
            if (m_right->m_color == RedOrBlack::RED) {
                // replace it with it's child and paint it black
                m_right->m_color = RedOrBlack::BLACK;
                if (set->getRoot() == this) {
                    // set root
                    set->setRoot(m_right);
                    m_right->m_parent = m_parent;
                } else {
                    if (m_parent->m_left == this) {
                        m_parent->m_left = m_right;
                    } else {
                        m_parent->m_right = m_right;
                    }
                    m_right->m_parent = m_parent;
                }
            } else {
                // this should be impossible to get to
                throw SetStateException("deleteFromTree node->m_right->m_color == BLACK");
            }
        } else if (m_color == RedOrBlack::BLACK && !m_left && !m_right) {
            // black and no children 
            SetNode* node = this;
            bool isLeft = node->m_parent->m_left == node;
            SetNode* parent = node->m_parent;
            SetNode* sibling = isLeft ? parent->m_right : parent->m_left;
            SetNode* closeCousin = isLeft ? sibling->m_left : sibling->m_right;
            SetNode* distantCousin = isLeft ? sibling->m_right : sibling->m_left;

            // remove from tree
            if (parent) {
                if (isLeft) {
                    parent->m_left = 0;
                } else {
                    parent->m_right = 0;
                }
            }
            node = 0;

            // balance
            // case d1 covered by this loop condition
            while (set->getRoot() != node) {
                if (node) {
                    isLeft = true;
                    if (parent->m_left == node) {
                        sibling = parent->m_right;
                        if (!sibling) {
                            throw SetStateException("node deletion special case does not have sibling! Contact dev!");
                        }
                        closeCousin = sibling->m_left;
                        distantCousin = sibling->m_right;
                    } else {
                        sibling = parent->m_left;
                        if (!sibling) {
                            throw SetStateException("node deletion special case does not have sibling! Contact dev!");
                        }
                        closeCousin = sibling->m_right;
                        distantCousin = sibling->m_left;
                        isLeft = false;
                    }
                }
                if (sibling->m_color == RedOrBlack::RED) {
                    // case d3
                    if (isLeft) {
                        parent->left_rotate(set);
                    } else {
                        parent->right_rotate(set);
                    }
                    parent->m_color = RedOrBlack::RED;
                    sibling->m_color = RedOrBlack::BLACK;
                    sibling = closeCousin;
                    if (isLeft) {
                        distantCousin = sibling->m_right;
                    } else {
                        distantCousin = sibling->m_left;
                    }
                    if (distantCousin && distantCousin->m_color == RedOrBlack::RED) {
                        // case d6
                        deleteCaseSix(parent, sibling, distantCousin, set, isLeft);
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
                            sibling->right_rotate(set);
                        } else {
                            sibling->left_rotate(set);
                        }
                        sibling->m_color = RedOrBlack::RED;
                        closeCousin->m_color = RedOrBlack::BLACK;
                        distantCousin = sibling;
                        sibling = closeCousin;
                        deleteCaseSix(parent, sibling, distantCousin, set, isLeft);
                        break;
                    }
                }
                if (distantCousin && distantCousin->m_color == RedOrBlack::RED) {
                    // case d6
                    deleteCaseSix(parent, sibling, distantCousin, set, isLeft);
                    break;
                } 
                if (closeCousin && closeCousin->m_color == RedOrBlack::RED) {
                    // case d5
                    if (isLeft) {
                        sibling->right_rotate(set);
                    } else {
                        sibling->left_rotate(set);
                    }
                    sibling->m_color = RedOrBlack::RED;
                    closeCousin->m_color = RedOrBlack::BLACK;
                    distantCousin = sibling;
                    sibling = closeCousin;
                    deleteCaseSix(parent, sibling, distantCousin, set, isLeft);
                    break;
                } 
                if (parent->m_color == RedOrBlack::RED) {
                    // case d4
                    sibling->m_color = RedOrBlack::RED;
                    parent->m_color = RedOrBlack::BLACK;
                    break;
                }
                // case d2
                sibling->m_color = RedOrBlack::RED;
                node = parent;
                parent = node->m_parent;
            }
        } else {
            // error, should be impossible
            throw SetStateException("Unhandled edge case in Red Black Tree deleteFromTree, contact developer!");
        }
    } while (!notSwapped);
}

void SetNode::copyData(SetNode* node) {}