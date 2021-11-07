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
            };
            void remove(T& el) {

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
    };
}

#endif