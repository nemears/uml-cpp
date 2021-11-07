#ifndef SPECIAL_SEQUENCE_H
#define SPECIAL_SEQUENCE_H

#include "element.h"

namespace UML {

    class ID_doesNotExistException2 : public std::exception {
        private:
            std::string m_msg;
        public:
            ID_doesNotExistException2(ID id) : m_msg(id.string() + " was not found within the sequence!"){};
            virtual const char* what() const throw() {
                return m_msg.c_str();
            }
    };

    /**
     * This container is based around a weighted binary search tree
     **/
    template <class T = Element> class SpecialSequence {
        private:
            struct SequenceNode {
                ID m_id;
                T* m_el = 0;
                SequenceNode* m_parent = 0;
                SequenceNode* m_left = 0;
                SequenceNode* m_right = 0;
            };

            SequenceNode* m_root = 0;
            size_t m_size = 0;

            void place(SequenceNode* node, SequenceNode* parent) {
                if (parent->m_left) {
                    // prefer placement to left
                    if (parent->m_right) {
                        // both children are populated, determine where to place
                        if (node->m_id > parent->m_right->m_id) {
                            // place to left if greater than right
                            place(node, parent->m_left);
                        } else {
                            // place to right if less than right
                            place(node, parent->m_right);
                        }
                    } else {
                        if (node->m_id > parent->m_left->m_id) {
                            // if node greater than left, swap left to right
                            parent->m_right = parent->m_left;
                            parent->m_left = node;
                        } else {
                            parent->m_right = node;
                        }
                        node->m_parent = parent;
                    }
                } else {
                    parent->m_left = node;
                    node->m_parent = parent;
                }
            };
            SequenceNode* search(ID id, SequenceNode* node) {
                if (node->m_id == id) {
                    // found match
                    return node;
                } else {
                    if (node->m_right) {
                        // if there is a right there is both children filled out
                        if (id > node->m_right->m_id) {
                            // if it is greater than the right it has to be within the left
                            return search(id, node->m_left);
                        } else {
                            // if not it must be in the right
                            return search(id, node->m_right);
                        }
                    } else {
                        if (node->m_left) {
                            // if left exist search it
                            return search(id, node->m_left);
                        } else {
                            // both are null, our search is unfruitful :(
                            throw ID_doesNotExistException2(id);
                        }
                    }
                }
            }
        public:
            virtual ~SpecialSequence() {
                SequenceNode* curr = m_root;
                while (curr) {
                    if (!curr->m_right && !curr->m_left) {
                        SequenceNode* temp = curr->m_parent;
                        delete curr;
                        curr = temp;
                    } else {
                        if (curr->m_right) {
                            SequenceNode* temp = curr->m_right;
                            curr->m_right = 0;
                            curr = temp;
                        } else if (curr->m_left) {
                            SequenceNode* temp = curr->m_left;
                            curr->m_left = 0;
                            curr = temp;
                        }
                    }
                }
            };
            void add(T& el) {
                SequenceNode* node = new SequenceNode{el.getID(), &el};
                if (!m_root) {
                    m_root = node;
                } else {
                    place(node, m_root);
                }
                m_size++;
            };
            void remove(T& el) {
                remove(el.getID());
            };
            void remove(ID id) {
                if (m_root) {
                    SequenceNode* temp = search(id, m_root);
                    if (temp->m_parent) {
                        // has parent
                        bool isLeft = temp->m_parent->m_left ? temp->m_parent->m_left->m_id == temp->m_id ? true : false : false;
                        if (isLeft) {
                            // The removed element is on the left side of its parent
                            if (!temp->m_parent->m_right) {
                                // If the parent has no right side just copy the removed nodes children to the parents children
                                temp->m_parent->m_left = temp->m_left;
                                temp->m_parent->m_right = temp->m_right;
                                if (temp->m_left) {
                                    temp->m_left->m_parent = temp->m_parent;
                                }
                                if (temp->m_right) {
                                    temp->m_right->m_parent = temp->m_parent;   
                                }
                            } else {
                                if (temp->m_left) {
                                    if (temp->m_parent->m_right->m_id > temp->m_left->m_id) {
                                        // If the removed node's left side is less than the right side put the right to the left and left to the right and place the removed nodes right on the left side
                                        temp->m_parent->m_left = temp->m_parent->m_right;
                                        temp->m_parent->m_right = temp->m_left;
                                        temp->m_left->m_parent = temp->m_parent;
                                        if (temp->m_right) {
                                            place(temp->m_right, temp->m_left);
                                        }
                                    } else {
                                        // put the removed node's left side to the parent's left and place the right wherever it belongs
                                        temp->m_parent->m_left = temp->m_left;
                                        temp->m_left->m_parent = temp->m_parent;
                                        if (temp->m_right) {
                                            place(temp->m_right, temp->m_parent);
                                        }
                                    }
                                } else {
                                    // removed node has no children
                                    temp->m_parent->m_left = temp->m_parent->m_right;
                                    temp->m_parent->m_right = 0;
                                    /** TODO: **/
                                    // may be of use to rebalance tree here?
                                }
                            }
                        } else {
                            if (!temp->m_left) {
                                // removed node has no children
                                temp->m_parent->m_right = 0;
                                /** TODO: **/
                                // may be of use to rebalance tree here?
                            } else {
                                if (temp->m_left->m_id > temp->m_parent->m_left->m_id) {
                                    temp->m_parent->m_right = temp->m_parent->m_left;
                                    temp->m_parent->m_left = temp->m_left;
                                    temp->m_left->m_parent = temp->m_parent;
                                    if (temp->m_right) {
                                        place(temp->m_right, temp->m_parent);
                                    }
                                } else {
                                    temp->m_parent->m_right = temp->m_left;
                                    temp->m_left->m_parent = temp->m_parent;
                                    if (temp->m_right) {
                                        place(temp->m_right, temp->m_parent);
                                    }
                                }
                            }
                        }
                    } else {
                        // we are removing root
                        m_root = temp->m_left;
                        place(temp->m_right, m_root);
                        m_root->m_parent = 0;
                    }
                    delete temp;
                    m_size--;
                } else {
                    throw ID_doesNotExistException2(id);
                }
            };
            T& get(ID id) {
                if (m_root) {
                    return *search(id, m_root)->m_el;
                } else {
                    throw ID_doesNotExistException2(id);
                }
            };
            bool contains(ID id) {
                if (m_root) {
                    return search(id, m_root);
                } else {
                    throw ID_doesNotExistException2(id);
                }
            }
            size_t size() const { return m_size; };
    };
}

#endif