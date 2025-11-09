#pragma once

#include <cstdint>
#include <algorithm>

template<typename Key, typename Value, typename Compare, typename WeightFn>
class AVLTree {
public:
    struct Node {
        Key key;
        Value value;
        Node* left;
        Node* right;
        int height;
        int64_t subtreeWeight;

        Node(const Key& k, const Value& v, int64_t w)
            : key(k), value(v), left(nullptr), right(nullptr),
              height(1), subtreeWeight(w) {}
    };

    AVLTree(Compare c, WeightFn* wfn)
        : root(nullptr), comp(c), weightFn(wfn) {}

    ~AVLTree() {
        clear(root);
    }

    bool empty() const {
        return root == nullptr;
    }

    Value* find(const Key& k) {
        Node* n = findNode(root, k);
        return n ? &n->value : nullptr;
    }

    Value& insertOrGet(const Key& k, const Value& v) {
        root = insertRec(root, k, v);
        Node* n = findNode(root, k);
        return n->value;
    }

    Value& insert_or_get(const Key& k, const Value& v) {
        return insertOrGet(k, v);
    }

    bool erase(const Key& k) {
        bool erased = false;
        root = eraseRec(root, k, erased);
        return erased;
    }

    Value* minValue() {
        if (!root) return nullptr;
        Node* n = root;
        while (n->left) n = n->left;
        return &n->value;
    }

    void refreshWeight(const Key&) {}

private:
    Node* root;
    Compare comp;
    WeightFn* weightFn;

    void clear(Node* n) {
        if (!n) return;
        clear(n->left);
        clear(n->right);
        delete n;
    }

    int h(Node* n) const {
        return n ? n->height : 0;
    }

    int64_t w(Node* n) const {
        return n ? n->subtreeWeight : 0;
    }

    void update(Node* n) {
        n->height = 1 + std::max(h(n->left), h(n->right));
        n->subtreeWeight = w(n->left) + w(n->right) + (*weightFn)(n->value);
    }

    int bal(Node* n) const {
        return h(n->left) - h(n->right);
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        x->right = y;
        update(y);
        update(x);
        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        y->left = x;
        update(x);
        update(y);
        return y;
    }

    Node* insertRec(Node* cur, const Key& k, const Value& v) {
        if (!cur) return new Node(k, v, (*weightFn)(v));

        if (comp(k, cur->key)) cur->left = insertRec(cur->left, k, v);
        else if (comp(cur->key, k)) cur->right = insertRec(cur->right, k, v);
        else cur->value = v;

        update(cur);

        int b = bal(cur);
        if (b > 1) {
            if (comp(cur->key, k)) cur->left = rotateLeft(cur->left);
            return rotateRight(cur);
        }
        if (b < -1) {
            if (comp(k, cur->right->key)) cur->right = rotateRight(cur->right);
            return rotateLeft(cur);
        }
        return cur;
    }

    Node* findNode(Node* cur, const Key& k) const {
        while (cur) {
            if (!comp(k, cur->key) && !comp(cur->key, k)) return cur;
            cur = comp(k, cur->key) ? cur->left : cur->right;
        }
        return nullptr;
    }

    Node* minNode(Node* cur) {
        while (cur->left) cur = cur->left;
        return cur;
    }

    Node* eraseRec(Node* cur, const Key& k, bool& erased) {
        if (!cur) return nullptr;

        if (comp(k, cur->key))
            cur->left = eraseRec(cur->left, k, erased);
        else if (comp(cur->key, k))
            cur->right = eraseRec(cur->right, k, erased);
        else {
            erased = true;
            if (!cur->left || !cur->right) {
                Node* t = cur->left ? cur->left : cur->right;
                delete cur;
                return t;
            }
            Node* nxt = minNode(cur->right);
            cur->key = nxt->key;
            cur->value = nxt->value;
            cur->right = eraseRec(cur->right, nxt->key, erased);
        }

        update(cur);

        int b = bal(cur);
        if (b > 1 && bal(cur->left) >= 0) return rotateRight(cur);
        if (b > 1 && bal(cur->left) < 0) { cur->left = rotateLeft(cur->left); return rotateRight(cur); }
        if (b < -1 && bal(cur->right) <= 0) return rotateLeft(cur);
        if (b < -1 && bal(cur->right) > 0) { cur->right = rotateRight(cur->right); return rotateLeft(cur); }

        return cur;
    }
};
