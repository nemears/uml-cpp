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

    template <class T> class Set;

    class AbstractContainer {
        template <class T> friend class Set;
        protected:
            size_t m_size = 0;
            struct ContainerNode {
                virtual ~ContainerNode() {};
                ID m_id;
                ContainerNode* m_parent = 0;
                ContainerNode* m_left = 0;
                ContainerNode* m_right = 0;
                size_t m_guard = 0;
            };
            ContainerNode* m_root = 0;
            size_t m_guard = 0;
            virtual void place(ContainerNode* node, ContainerNode* parent) = 0;
    };

    template <class T> class SetIterator;

    /**
     * This container is based around a weighted binary search tree
     **/
    template <class T = Element> class Set : public AbstractContainer {

        template <class U> friend class Set;
        template <class U> friend class SetIterator;

        protected:
            struct SetNode : public ContainerNode {
                SetNode(T& el) : m_el(&el) { m_id = el.getID(); };
                SetNode(){}; // blank constructor
                virtual ~SetNode() {};
                T* m_el = 0;
            };
            std::vector<AbstractContainer*> m_subsetOf;
            std::vector<AbstractContainer*> m_subsettedContainers;

            void place(ContainerNode* node, ContainerNode* parent) override {
                if (parent->m_left) {
                    // prefer placement to left
                    if (parent->m_right) {
                        // both children are populated, determine where to place
                        if (node->m_id > parent->m_right->m_id && parent->m_left->m_guard <= m_guard) {
                            // place to left if greater than right
                            place(node, parent->m_left);
                        } else if (parent->m_right->m_guard <= m_guard) {
                            // place to right if less than right
                            place(node, parent->m_right);
                        } else {
                            if (node->m_id > parent->m_right->m_id) {
                                ContainerNode* temp = parent->m_left;
                                parent->m_left = node;
                                place(parent->m_right, temp);
                                parent->m_right = temp;
                                node->m_parent = parent;
                            }
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
            ContainerNode* search(ID id, ContainerNode* node) {
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
                            return 0;
                        }
                    }
                }
            }
        public:
            virtual ~Set() { 
                ContainerNode* curr = m_root;
                while (curr) {
                    if (!curr->m_right && !curr->m_left) {
                        ContainerNode* temp = curr->m_parent;
                        delete curr;
                        if (curr->m_parent) {
                            if (curr->m_parent->m_guard < m_guard) {
                                break;
                            }
                        }
                        curr = temp;
                    } else {
                        if (curr->m_right) {
                            if (curr->m_right->m_guard <= m_guard) {
                                ContainerNode* temp = curr->m_right;
                                curr->m_right = 0;
                                curr = temp;
                                continue;
                            }
                            curr->m_right = 0;
                        } if (curr->m_left) {
                            if (curr->m_left->m_guard <= m_guard) {
                                ContainerNode* temp = curr->m_left;
                                curr->m_left = 0;
                                curr = temp;
                            } else {
                                curr->m_left = 0;
                            }
                        }
                    }
                }
            };
            template <class U = Element> void subsets(Set<U>& subsetOf) {
                if (!m_root && !subsetOf.m_root) {
                    m_subsetOf.push_back(&subsetOf);
                    subsetOf.m_subsettedContainers.push_back(this);
                    if (subsetOf.m_guard <= m_guard) {
                        m_guard = subsetOf.m_guard + 1;
                    }
                } else {
                    // TODO error
                    std::cerr << "WARNING subset set after set was used, must make sure subsetting is done during configuration, before use!" << std::endl;
                }
            };
            void add(T& el) {
                SetNode* node = new SetNode(el);
                node->m_guard = m_guard;
                if (!m_root) {
                    m_root = node;
                    for (auto& subsetOf : m_subsetOf) {
                        if (subsetOf->m_root) {
                            subsetOf->place(node, subsetOf->m_root);
                        } else {
                            subsetOf->m_root = node;
                        }
                    }
                } else {
                    if (m_root->m_guard > m_guard) {
                        // if the root is a subsetted sequence push it under this one
                        ContainerNode* temp = m_root;
                        m_root = node;
                        place(temp, node);
                    } else {
                        place(node, m_root);
                    }
                }
                m_size++;
                for (auto& subsetOf : m_subsetOf) {
                    subsetOf->m_size++;
                }
            };
            void remove(T& el) {
                remove(el.getID());
            };
            void remove(ID id) {
                if (m_root) {
                    ContainerNode* temp = search(id, m_root);
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
                        // we are removing base root of tree
                        if (temp->m_left) {
                            m_root = temp->m_left;
                            if (temp->m_right) {
                                place(temp->m_right, m_root);
                            }
                            m_root->m_parent = 0;
                        }
                    }
                    if (temp->m_id == m_root->m_id) {
                        // if we removed the root of the sequence
                        if (temp->m_left) {
                            place(m_root->m_right, m_root->m_left);
                            m_root->m_left->m_parent = m_root->m_parent;
                            m_root = temp->m_left;
                        } else {
                            m_root = 0;
                        }
                    }
                    for (auto subset : m_subsettedContainers) {
                        if (temp->m_guard >= subset->m_guard) {
                            subset->m_size--;
                            if (subset->m_root->m_id == id) {
                                if (subset->m_root->m_left) {
                                    place(subset->m_root->m_right, subset->m_root->m_left);
                                    subset->m_root->m_left->m_parent = subset->m_root->m_parent;
                                    subset->m_root = subset->m_root->m_left;
                                } else {
                                    subset->m_root = 0;
                                }
                            }
                        }
                    }
                    for (auto subsetOf : m_subsetOf) {
                        if (subsetOf->m_root->m_id == id) {
                            if (subsetOf->m_root->m_left) {
                                place(subsetOf->m_root->m_right, subsetOf->m_root->m_left);
                                m_root->m_left->m_parent = m_root->m_parent;
                                m_root = subsetOf->m_root->m_left;
                            } else {
                                subsetOf->m_root = 0;
                            }
                        }
                        subsetOf->m_size--;
                    }
                    delete temp;
                    m_size--;
                } else {
                    throw ID_doesNotExistException2(id);
                }
            };
            T& get(ID id) {
                if (m_root) {
                    T* ret = dynamic_cast<SetNode*>(search(id, m_root))->m_el;
                    if (ret) {
                        return *ret;
                    }
                }
                throw ID_doesNotExistException2(id);
            };
            bool contains(ID id) {
                bool ret = false;
                if (m_root) {
                    ContainerNode* t = search(id, m_root);
                    ret = t > 0;
                } 
                return ret;
            }
            size_t size() const { return m_size; };
            SetIterator<T> begin() {
                SetIterator<T> it;
                it.m_node = dynamic_cast<SetNode*>(m_root);
                return it;
            };
            SetIterator<T> end() {
                SetIterator<T> it;
                it.m_node = it.m_endNode;
                return it;
            };
    };

    template <class T = Element> struct SetIterator {

        friend class Set<T>;

        using NodeType = typename Set<T>::SetNode;
        private:
            NodeType* m_node;
            NodeType* m_endNode;
        public:
            SetIterator() {
                m_endNode = new NodeType();
            };
            ~SetIterator() {
                // if (m_node->m_id == ID::nullID()) {
                //     delete m_endNode;
                // }
            };
            T& operator*() { return *m_node->m_el; };
            T* operator->() { return m_node->m_el; };

            // This is dfs
            SetIterator operator++() {
                if (m_node->m_left) {
                    // always go left
                    m_node = dynamic_cast<NodeType*>(m_node->m_left);
                } else {
                    // we hit bottom, choose next right
                    NodeType* temp;
                    NodeType* last = dynamic_cast<NodeType*>(m_node);
                    bool found = false;
                    do {
                        temp = dynamic_cast<NodeType*>(last->m_parent);
                        if (temp->m_right) {
                            if (temp->m_right->m_id != last->m_id) {
                                found = true;
                                break;
                            }
                        }
                        last = temp;
                    } while (temp->m_parent);
                    if (!found) {
                        m_node = m_endNode;
                    } else {
                        m_node = dynamic_cast<NodeType*>(temp->m_right);
                    }
                }
                return *this;
            };
            SetIterator operator++(int) {
               if (m_node->m_left) {
                    // always go left
                    m_node = dynamic_cast<NodeType*>(m_node->m_left);
                } else {
                    // we hit bottom, choose next right
                    NodeType* temp;
                    NodeType* last = dynamic_cast<NodeType*>(m_node);
                    bool found = false;
                    do {
                        temp = dynamic_cast<NodeType*>(last->m_parent);
                        if (temp->m_right) {
                            if (temp->m_right->m_id != last->m_id) {
                                found = true;
                                break;
                            }
                        }
                        last = temp;
                    } while (temp->m_parent);
                    if (!found) {
                        m_node = m_endNode;
                    } else {
                        m_node = dynamic_cast<NodeType*>(temp->m_right);
                    }
                }
                SetIterator ret = *this;
                return ret;
            };
            friend bool operator== (const SetIterator& a, const SetIterator& b) { return a.m_node->m_id == b.m_node->m_id; };
            friend bool operator!= (const SetIterator& a, const SetIterator& b) { return a.m_node->m_id != b.m_node->m_id; };
    };
}

#endif