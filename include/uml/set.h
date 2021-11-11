#ifndef SPECIAL_SEQUENCE_H
#define SPECIAL_SEQUENCE_H

#include "element.h"
#include "umlManager.h"
#include <iostream>

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

    class DuplicateElementInSetException : public std::exception {
        public:
            virtual const char* what() const throw() {
                return "Duplicate element added to set!";
            };
    };

    template <class T, class U> class Set;
    template <class T, class U> class OrderedSet;

    class AbstractSet {
        template <class T, class U> friend class Set;
        template <class T, class U> friend class OrderedSet;
        protected:
            size_t m_size = 0;
            struct ContainerNode {
                ContainerNode(Element& el) : m_el(&el) { m_id = el.getID(); };
                ContainerNode(){};
                ID m_id;
                Element* m_el;
                std::string m_name;
                ContainerNode* m_parent = 0;
                ContainerNode* m_left = 0;
                ContainerNode* m_right = 0;
                size_t m_guard = 0;
            };
            ContainerNode* m_root = 0;
            size_t m_guard = 0;
            virtual void place(ContainerNode* node, ContainerNode* parent) = 0;
            void setName(ContainerNode* node);
    };

    template <class T> struct SetIterator;
    template <class T> struct ID_Set;

    class Namespace;
    class NamedElement;
    class Package;
    class PackageableElement;

    /**
     * This container is based around a weighted binary search tree
     **/
    template <class T = Element, class U = Element> class Set : public AbstractSet {
        
        template <class V, class W> friend class Set;
        friend class Set<U>;
        friend struct SetIterator<U>;
        friend struct ID_Set<T>;
        friend class Element;
        friend class Namespace;
        friend class NamedElement;
        friend class Package;
        friend class PackageableElement;

        protected:
            std::vector<AbstractSet*> m_subsetOf;
            std::vector<AbstractSet*> m_subsettedContainers;
            std::vector<AbstractSet*> m_redefines;
            bool m_rootRedefinedSet = true;
            Set<U, T>& (T::*m_oppositeSignature)() = 0;
            Element* m_el = 0;

            struct SetNode : public ContainerNode {
                SetNode(T& el) {
                    this->m_el = reinterpret_cast<Element*>(&el);
                    this->m_id = this->m_el->getID(); 
                };
                SetNode() {};
            };

            void place(ContainerNode* node, ContainerNode* parent) override {
                if (node->m_id == parent->m_id) {
                    delete node;
                    throw DuplicateElementInSetException();
                }
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
            ContainerNode* search(std::string name, ContainerNode* node) {
                if (node->m_name == name) {
                    return node;
                } else {
                    ContainerNode* temp;
                    if (node->m_right) {
                        if((temp = search(name, node->m_right)) != 0) {
                            return temp;
                        }
                    }
                    if (node->m_left) {
                        if ((temp = search(name, node->m_left)) != 0) {
                            return temp;
                        }
                    }
                }
                return 0;
            };
            void add(ContainerNode* node) {
                node->m_guard = m_guard;
                if (!m_root) {
                    m_root = node;
                    for (auto& redefined : m_redefines) {
                        if (redefined->m_root) {
                            if (redefined->m_root->m_id != m_root->m_id) {
                                m_root = redefined->m_root;
                                place(node, m_root);
                            }
                        } else {
                            redefined->m_root = node;
                        }
                    }
                    for (auto& subsetOf : m_subsetOf) {
                        if (subsetOf->m_root) {
                            subsetOf->place(node, subsetOf->m_root);
                        } else {
                            subsetOf->m_root = m_root;
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
                for (auto& redefined : m_redefines) {
                    redefined->m_size++;
                }
            };
            virtual ContainerNode* createNode(T& el) {
                return new SetNode(el);
            };
            virtual ContainerNode* createNode(ID id) {
                ContainerNode* ret = new SetNode();
                ret->m_id = id;
                return ret;
            };
            void release(ID id) {
                ContainerNode* searchResult = search(id, m_root);
                if (!searchResult) {
                    throw ID_doesNotExistException2(id);
                }
                if (searchResult->m_el) {
                    searchResult->m_el = 0;
                }
            };
            void reindex(ID oldID, ID newID) {
                ContainerNode* node = search(oldID, m_root);
                node->m_id = newID;
                if (node->m_parent->m_left->m_id == oldID) {
                    if (node->m_parent->m_right->m_id > newID) {
                        node->m_parent->m_left = node->m_parent->m_right;
                        node->m_parent->m_right = node;
                    }
                } else {
                    if (newID > node->m_parent->m_left->m_id) {
                        node->m_parent->m_right = node->m_parent->m_left;
                        node->m_parent->m_left = node;
                    }
                }
                // TODO redefined and subsetted sets
            };
            void eraseElement(ID id) {
                // only remove from root seq
                if (m_subsetOf.empty()) {
                    remove(id); // this will invoke opposite opposite (maybe make inner function to remove to not invoke opposite?)
                }
            };
        public:
            Set(Element* el) : m_el(el) {};
            Set() {};
            Set(const Set<T,U>& rhs) {
                if (rhs.m_root) {
                    m_root = new ContainerNode(*rhs.m_root->m_el);
                }
                m_size = rhs.m_size;
                m_rootRedefinedSet = rhs.m_rootRedefinedSet;
                ContainerNode* curr = rhs.m_root;
                ContainerNode* mine = m_root;
                m_guard = rhs.m_guard;
                while (curr) {
                    ContainerNode* temp = new ContainerNode();
                    temp->m_guard = m_guard;
                    temp->m_parent = mine;
                    if (curr->m_left) {
                        temp->m_id = curr->m_left->m_id;
                        temp->m_el = curr->m_left->m_el;
                        curr = curr->m_left;
                        mine = temp;
                    } else {
                        do {
                            curr = curr->m_parent;
                            mine = mine->m_parent;
                        } while (!curr->m_right && curr);
                        if (curr) {
                            temp->m_id = curr->m_right->m_id;
                            temp->m_el = curr->m_right->m_el;
                            curr = curr->m_right;
                            mine = temp;
                        }
                    }
                }
            };
            virtual ~Set() { 
                if (m_rootRedefinedSet) {
                    ContainerNode* curr = m_root;
                    while (curr) {
                        if (!curr->m_right && !curr->m_left) {
                            ContainerNode* temp = curr->m_parent;
                            bool deleteNode = true;
                            if (temp) {
                                if (m_root) {
                                    if (curr->m_id == m_root->m_id) {
                                        deleteNode = false;
                                    }
                                }
                                if (deleteNode) {
                                    if (temp->m_guard < m_guard) {
                                        if (temp->m_left->m_id == curr->m_id) {
                                            temp->m_left = 0;
                                        } else {
                                            temp->m_right = 0;
                                        }
                                    }
                                }
                            }
                            if (deleteNode) {
                                delete curr;
                            }
                            if (temp) {
                                if (temp->m_guard < m_guard) {
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
                                } else if (curr->m_right->m_parent->m_id == curr->m_id) {
                                    // delete lefover root
                                    delete curr->m_right;
                                }
                                curr->m_right = 0;
                            } if (curr->m_left) {
                                if (curr->m_left->m_guard <= m_guard) {
                                    ContainerNode* temp = curr->m_left;
                                    curr->m_left = 0;
                                    curr = temp;
                                } else {
                                    if (curr->m_left->m_parent->m_id == curr->m_id) {
                                        // delete leftover root
                                        delete curr->m_left;
                                    }
                                    curr->m_left = 0;
                                }
                            }
                        }
                    }
                }
            };
            /**
             * WARN: so for now ORDER MATTERS when subsetting a sequence
             * the first element subsetted should be the first element
             * instantiated, (e.g.):
             *  Set<> a;
             *  Set<> b;
             *  Set<> c;
             *  a.subsets(c); // BAD
             *  c.subsets(b); // BAD if subsetting a after
             *  c.subsets(a); // BAD
             *  c.subsets(a); // GOOD
             *  c.subsets(b); // GOOD because b instantiated after a
             **/
            template <class V = Element, class W = Element> void subsets(Set<V, W>& subsetOf) {
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
            void opposite(Set<U, T>& (T::*op)()) {
                /** TODO: static_assert that we have m_el for this instance **/
                m_oppositeSignature = op;
            };
            void redefines(Set<T>& redefined) {
                if (m_root) {
                    std::cerr << "WARNING redefines set after set was used, must make sure redefining is done during configuration, before use!" << std::endl;
                    return;
                }
                m_redefines.push_back(&redefined);
                redefined.m_redefines.push_back(this);
                m_rootRedefinedSet = false;
            };
            void add(T& el) {
                ContainerNode* node = createNode(el);
                setName(node);
                add(node);
                if (m_el) {
                    if (m_el->m_manager) {
                        m_el->setReference(&el);
                    }
                }
                if (m_oppositeSignature) {
                    if (!(el.*m_oppositeSignature)().contains(m_el->getID())) {
                        (el.*m_oppositeSignature)().add(*dynamic_cast<U*>(m_el));
                    }
                }
            };
            void add(ID id) {
                ContainerNode* node = createNode(id);
                add(node);
                if (m_el) {
                    if (m_el->m_manager) {
                        m_el->setReference(id);
                    }
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
                        for (auto& redefined : m_redefines) {
                            redefined->m_root = m_root;
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
                    for (auto& subsetOf : m_subsetOf) {
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
                    for (auto& redefined : m_redefines) {
                        redefined->m_size--;
                    }
                    delete temp;
                    m_size--;
                } else {
                    throw ID_doesNotExistException2(id);
                }
            };
            T& get(ID id) {
                if (m_root) {
                    ContainerNode* searchResult = search(id, m_root);
                    if (!searchResult) {
                        throw ID_doesNotExistException2(id);
                    }
                    if (!searchResult->m_el) {
                        searchResult->m_el = m_el->m_manager->get(m_el, id);
                    }
                    T* ret = dynamic_cast<T*>(searchResult->m_el);
                    if (ret) {
                        return *ret;
                    }
                }
                throw ID_doesNotExistException2(id);
            };
            T& get(std::string name) {
                if (m_root) {
                    ContainerNode* searchResult = search(name, m_root);
                    if (!searchResult) {
                        // TODO throw proper error
                        throw ManagerStateException("Improper name used in set!"); // TODO change
                    }
                    if (!searchResult->m_el) {
                        searchResult->m_el = m_el->m_manager->get(m_el, searchResult->m_id);
                    }
                    T* ret = dynamic_cast<T*>(searchResult->m_el);
                    if (ret) {
                        return *ret;
                    }
                } 
                throw ManagerStateException("Improper name used in set!"); // TODO change
            };
            T& get(int i) {
                int size = m_size;
                ContainerNode* node = m_root;  //this wont work
                while (i < size) {
                    // check size and determine which side to go down
                    size--;
                    size = size / 2;
                    if (i > size) {
                        node = node->m_left;
                    } else {
                        node = node->m_right;
                    }
                }
                if (!node->m_el) {
                    node->m_el = m_el->m_manager->get(m_el, node->m_id);
                }
                return *dynamic_cast<T*>(node->m_el);
            };
            T& front() {
                if (m_root) {
                    if (!m_root->m_el) {
                        m_root->m_el = m_el->m_manager->get(m_el, m_root->m_id);
                    }
                    return *dynamic_cast<T*>(m_root->m_el);
                }
                throw ManagerStateException("TODO front empty");
            };
            T& back() {
                if (m_root) {
                    ContainerNode* temp = m_root;
                    while (temp->m_right) {
                        temp = temp->m_right;
                    }
                    if (temp->m_left) {
                        temp = temp->m_left;
                    }
                    if (!temp->m_el) {
                        temp->m_el = m_el->m_manager->get(m_el, temp->m_id);
                    }
                    return *dynamic_cast<T*>(temp->m_el);
                }
                throw ManagerStateException("TODO back empty");
            };
            bool contains(ID id) {
                bool ret = false;
                if (m_root) {
                    ContainerNode* t = search(id, m_root);
                    ret = t > 0;
                } 
                return ret;
            };
            int count(ID id) {
                if (contains(id)) {
                    return 1;
                } else {
                    return 0;
                }
            };
            bool contains(std::string name) {
                bool ret = false;
                if (m_root) {
                    ContainerNode* t = search(name, m_root);
                    ret = t > 0;
                }
                return ret;
            };
            int count(std::string name) {
                if (contains(name)) {
                    return 1;
                } else {
                    return 0;
                }
            };
            bool empty() const {
                return m_root == 0;
            };
            size_t size() const { return m_size; };
            SetIterator<T> begin() {
                SetIterator<T> it;
                it.m_node = m_root;
                it.m_el = m_el;
                return it;
            };
            SetIterator<T> end() {
                SetIterator<T> it;
                it.m_node = &it.m_endNode;
                it.m_el = m_el;
                return it;
            };
            ID_Set<T> ids() {
                ID_Set<T> set;
                set.m_root = m_root;
                return set;
            };
    };

    template <class T = Element> struct SetIterator {

        template <class V, class W> friend class Set;
        friend class AbstractSet;
        template < class V> friend class ID_Set;

        protected:
            Element* m_el;
            AbstractSet::ContainerNode* m_node;
            AbstractSet::ContainerNode m_endNode;
        public:
            T& operator*() {
                if (!m_node->m_el) {
                    m_node->m_el = m_el->m_manager->get(m_el, m_node->m_id);
                }
                return dynamic_cast<T&>(*m_node->m_el);
            };
            T* operator->() {
                if (!m_node->m_el) {
                    m_node->m_el = m_el->m_manager->get(m_el, m_node->m_id);
                }
                return dynamic_cast<T*>(m_node->m_el);
            };

            // This is dfs
            SetIterator operator++() {
                if (m_node->m_left) {
                    // always go left
                    m_node = m_node->m_left;
                } else {
                    // we hit bottom, choose next right
                    AbstractSet::ContainerNode* temp;
                    AbstractSet::ContainerNode* last = dynamic_cast<AbstractSet::ContainerNode*>(m_node);
                    bool found = false;
                    do {
                        temp = dynamic_cast<AbstractSet::ContainerNode*>(last->m_parent);
                        if (temp->m_right) {
                            if (temp->m_right->m_id != last->m_id) {
                                found = true;
                                break;
                            }
                        }
                        last = temp;
                    } while (temp->m_parent);
                    if (!found) {
                        m_node = &m_endNode;
                    } else {
                        m_node = temp->m_right;
                    }
                }
                return *this;
            };
            SetIterator operator++(int) {
               if (m_node->m_left) {
                    // always go left
                    m_node = dynamic_cast<AbstractSet::ContainerNode*>(m_node->m_left);
                } else {
                    // we hit bottom, choose next right
                    AbstractSet::ContainerNode* temp;
                    AbstractSet::ContainerNode* last = dynamic_cast<AbstractSet::ContainerNode*>(m_node);
                    bool found = false;
                    do {
                        temp = dynamic_cast<AbstractSet::ContainerNode*>(last->m_parent);
                        if (temp->m_right) {
                            if (temp->m_right->m_id != last->m_id) {
                                found = true;
                                break;
                            }
                        }
                        last = temp;
                    } while (temp->m_parent);
                    if (!found) {
                        m_node = &m_endNode;
                    } else {
                        m_node = dynamic_cast<AbstractSet::ContainerNode*>(temp->m_right);
                    }
                }
                SetIterator ret = *this;
                return ret;
            };
            friend bool operator== (const SetIterator& a, const SetIterator& b) { return a.m_node->m_id == b.m_node->m_id; };
            friend bool operator!= (const SetIterator& a, const SetIterator& b) { return a.m_node->m_id != b.m_node->m_id; };
    };

    template <class T = Element> struct SetID_Iterator : public SetIterator<T> {
        ID operator*() {
            return this->m_node->m_id;
        };
    };

    template <class T = Element> struct ID_Set {

        template <class U, class V> friend class Set;

        private:
            AbstractSet::ContainerNode* m_root = 0;
        public:
            SetID_Iterator<T> begin() {
                SetID_Iterator<T> it;
                it.m_node = m_root;
                return it;
            };
            SetID_Iterator<T> end() {
                SetID_Iterator<T> it;
                it.m_node = &it.m_endNode;
                return it;
            };
    };
}

#endif