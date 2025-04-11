// UMBC - CMSC 341 - Fall 2023 - Proj2
#include "satnet.h"
SatNet::SatNet(){
    m_root = nullptr;
}

SatNet::~SatNet(){
    clear();
}

// GET ROOT
Sat* SatNet::getRoot() const {
    return m_root;
}

// ROTATIONS
// RIGHT ROTATION
Sat* SatNet::rightRotate(Sat *node)  {
    Sat* newRoot = node->getLeft();
    Sat* temp = newRoot->getRight();

    newRoot->setRight(node);
    node->setLeft(temp);

    // Update heights
    node->setHeight(1 + std::max(
            (node->getLeft() ? node->getLeft()->getHeight() : 0),
            (node->getRight() ? node->getRight()->getHeight() : 0)
    ));

    newRoot->setHeight(1 + std::max(
            (newRoot->getLeft() ? newRoot->getLeft()->getHeight() : 0),
            (newRoot->getRight() ? newRoot->getRight()->getHeight() : 0)
    ));

    return newRoot;
}
// LEFT ROTATION
Sat* SatNet::leftRotate(Sat *node) {
    Sat* newRoot = node->getRight();
    Sat* temp = newRoot->getLeft();

    newRoot->setLeft(node);
    node->setRight(temp);

    // Update heights
    node->setHeight(1 + std::max(
            (node->getLeft() ? node->getLeft()->getHeight() : 0),
            (node->getRight() ? node->getRight()->getHeight() : 0)
    ));

    newRoot->setHeight(1 + std::max(
            (newRoot->getLeft() ? newRoot->getLeft()->getHeight() : 0),
            (newRoot->getRight() ? newRoot->getRight()->getHeight() : 0)
    ));

    return newRoot;
}

// AVL TREE METHODS
// BALANCE
int SatNet::getBalance(Sat *node) {
    if (node == nullptr) {
        return 0;
    }

    int leftHeight = (node->getLeft() ? node->getLeft()->getHeight() : 0);
    int rightHeight = (node->getRight() ? node->getRight()->getHeight() : 0);

    return leftHeight - rightHeight;
}

// UPDATE HEIGHT
void SatNet::updateHeight(Sat *node) {
    if (node != nullptr) {
        node->setHeight(1 + std::max(
                (node->getLeft() ? node->getLeft()->getHeight() : 0),
                (node->getRight() ? node->getRight()->getHeight() : 0)
        ));
    }
}


// BST VALIDITY
bool SatNet::getBSTValidity(Sat* node, int min, int max) const {
    if (node == nullptr) {
        return true;
    }

    int key = node->m_id;

    // Check if the key value is within the valid range
    if (key < min || key > max) {
        return false; // BST property is violated
    }

    // Recursively check the left and right subtrees
    return getBSTValidity(node->getLeft(), min, key - 1) &&
            getBSTValidity(node->getRight(), key + 1, max);
}

// BALANCE AVL TREE
Sat* SatNet::balance(Sat *node) {
    if (node == nullptr) {
        return node;
    }

    int balance = getBalance(node);

    // Left-heavy
    if (balance > 1) {
        if (getBalance(node->getLeft()) < 0) {
            // Left-Right case: Left rotation on left child, then right rotation on current node
            node->setLeft(leftRotate(node->getLeft()));
        }
        // Right rotation on current node
        return rightRotate(node);
    }
    // Right-heavy
    if (balance < -1) {
        if (getBalance(node->getRight()) > 0) {
            // Right-Left case: Right rotation on right child, then left rotation on current node
            node->setRight(rightRotate(node->getRight()));
        }
        // Left rotation on current node
        return leftRotate(node);
    }

    return node;
}

// INSERTION
Sat* SatNet::insertRecursive(Sat *node, const Sat &satellite) {
    if (node == nullptr) {
        return new Sat(satellite);
    }

    if (satellite.getID() < node->getID()) {
        node->setLeft(insertRecursive(node->getLeft(), satellite));
    } else if (satellite.getID() > node->getID()) {
        node->setRight(insertRecursive(node->getRight(), satellite));
    } else {
        return node;
    }

    // Update height and balance
    updateHeight(node);
    return balance(node);
}

// CLEAR
void SatNet::clearRecursive(Sat *node) {
    if (node == nullptr) {
        return;
    }

    clearRecursive(node->getLeft());
    clearRecursive(node->getRight());

    delete node;
    node = nullptr;
}

// REMOVE
Sat* SatNet::removeRecursive(Sat *node, int id) {
    if (node == nullptr) {
        return node;
    }

    // standard bst removal
    if (id < node->getID()) {
        node->setLeft(removeRecursive(node->getLeft(), id));
    } else if (id > node->getID()) {
        node->setRight(removeRecursive(node->getRight(), id));
    } else {
        // node with only one child or no child
        if (node->getLeft() == nullptr || node->getRight() == nullptr) {
            Sat* temp = (node->getLeft() == nullptr) ? node->getRight() : node->getLeft();
            // delete node but check if it's the root
            // if it is, overwrite the root with the new node
            if (node->getID() == m_root->getID()) {
                m_root = temp;
            }
            delete node;
            return temp;

        }
        else {
            // node with two children: Get the inorder successor (smallest in the right subtree)
            Sat* temp = findMin(node->getRight());

            // Copy the inorder successor's data to this node
            node->setID(temp->getID());

            // Delete the inorder successor
            node->setRight(removeRecursive(node->getRight(), temp->getID()));
        }
    }

    // Update height and balance
    updateHeight(node);
    return balance(node);
}

// FIND MIN
Sat* SatNet::findMin(Sat *node) {
    if (node == nullptr) {
        return node;
    }

    while (node->getLeft() != nullptr) {
        node = node->getLeft();
    }

    return node;
}

// FIND SATELLITE
bool SatNet::findSatelliteRecursive(Sat* node, int id) const {
    if (node == nullptr) {
        return false;
    }

    if (id == node->getID()) {
        return true;
    } else if (id < node->getID()) {
        return findSatelliteRecursive(node->getLeft(), id);
    } else {
        return findSatelliteRecursive(node->getRight(), id);
    }
}

// SET STATE
bool SatNet::setStateRecursive(Sat* node, int id, STATE state) {
    if (node == nullptr) {
        return false;
    }

    if (id == node->getID()) {
        node->setState(state);
        return true;
    } else if (id < node->getID()) {
        return setStateRecursive(node->getLeft(), id, state);
    } else {
        return setStateRecursive(node->getRight(), id, state);
    }
}

// LIST SATELLITES
void SatNet::listSatellitesRecursive(Sat* node) const {
    if (node != nullptr) {
        listSatellitesRecursive(node->getLeft());
        // Print the satellite information
        cout << node->getID() << ": " << node->getStateStr() << ": " << node->getInclinStr() << ": " << node->getAltStr() << endl;
        listSatellitesRecursive(node->getRight());
    }
}

// REMOVE DEORBITED
Sat* SatNet::removeDeorbitedRecursive(Sat* node) {
    if (node == nullptr) {
        return node;
    }

    // Recursively process the left and right subtrees
    node->setLeft(removeDeorbitedRecursive(node->getLeft()));
    node->setRight(removeDeorbitedRecursive(node->getRight()));

    // Check and remove satellites with DEORBITED state
    if (node->getState() == DEORBITED) {
        // Node has DEORBITED state, remove it
        return removeNode(node);
    }

    // Update height and balance
    updateHeight(node);
    return balance(node);
}

// REMOVE NODE
Sat* SatNet::removeNode(Sat* node) {
    if (node == nullptr) {
        return node;
    }

    if (node->getLeft() == nullptr) {
        Sat* temp = node->getRight();
        delete node;
        return temp; // Return the new root (or nullptr if the tree is empty)
    } else if (node->getRight() == nullptr) {
        Sat* temp = node->getLeft();
        delete node;
        return temp; // Return the new root (or nullptr if the tree is empty)
    } else {
        Sat* temp = findMin(node->getRight());
        node->setID(temp->getID());
        node->setRight(removeRecursive(node->getRight(), temp->getID()));
    }

    return node;
}

// COUNT SATELLITES
int SatNet::countSatellitesRecursive(Sat* node, INCLIN degree) const {
    if (node == nullptr) {
        return 0;
    }

    int count = 0;
    if (node->getInclin() == degree) {
        count++;
    }

    count += countSatellitesRecursive(node->getLeft(), degree);
    count += countSatellitesRecursive(node->getRight(), degree);

    return count;
}

// COPY TREE
Sat* SatNet::copyTree(Sat *node) {
    if (node == nullptr) {
        return nullptr;
    }

    // Create a deep copy of the current node
    Sat* newSat = new Sat(node->getID(), node->getAlt(), node->getInclin(), node->getState());
    newSat->setLeft(copyTree(node->getLeft()));
    newSat->setRight(copyTree(node->getRight()));

    return newSat;
}

// BASE METHODS

void SatNet::insert(const Sat& satellite){
    if (satellite.getID() >= MINID && satellite.getID() <= MAXID) {
        if (!findSatellite(satellite.getID())) {
            m_root = insertRecursive(m_root, satellite);
        }
        else {
            // throw exception
            throw std::runtime_error("Satellite already exists (ID: " + std::to_string(satellite.getID()) + ")");
        }
    } else {
        // throw exception
        throw std::runtime_error("Satellite ID out of range (ID: " + std::to_string(satellite.getID()) + ")");
    }
}

void SatNet::clear(){
    clearRecursive(m_root);
    m_root = nullptr;
}

void SatNet::remove(int id){
    if (id >= MINID && id <= MAXID) {
        if (findSatellite(id)) {
            removeRecursive(m_root, id);
        }
    }
}

void SatNet::dumpTree() const {
    dump(m_root);
}

void SatNet::dump(Sat* satellite) const{
    if (satellite != nullptr){
        cout << "(";
        dump(satellite->m_left);//first visit the left child
        cout << satellite->m_id << ":" << satellite->m_height;//second visit the node itself
        dump(satellite->m_right);//third visit the right child
        cout << ")";
    }
}

void SatNet::listSatellites() const {
    listSatellitesRecursive(m_root);
}

bool SatNet::setState(int id, STATE state){
    return setStateRecursive(m_root, id, state);
}

void SatNet::removeDeorbited(){
    removeDeorbitedRecursive(m_root);
}

bool SatNet::findSatellite(int id) const {
    return findSatelliteRecursive(m_root, id);
}

const SatNet & SatNet::operator=(const SatNet & rhs){
    // Check for self-assignment
    if (this == &rhs) {
        return *this;
    }

    // Clear the current tree to avoid memory leaks
    clear();

    // Create a deep copy of the rhs tree
    m_root = copyTree(rhs.m_root);

    return *this;
}

int SatNet::countSatellites(INCLIN degree) const{
    return countSatellitesRecursive(m_root, degree);
}