#pragma once
#include "order.h"

struct OrderNode { // Doubly linked list node
    Order order;
    OrderNode* prev;
    OrderNode* next;
    OrderNode(const Order& o) : order(o), prev(nullptr), next(nullptr) {}
};

struct PriceLevel { // Doubly linked list maintaining price level
    Price price;
    OrderNode* head;
    OrderNode* tail;
    Qty totalQty;

    PriceLevel() : price(0), head(nullptr), tail(nullptr), totalQty(0) {}
    PriceLevel(Price p) : price(p), head(nullptr), tail(nullptr), totalQty(0) {}

    void addOrder(OrderNode* node) {
        if (!head) head = tail = node;
        else {
            tail->next = node;
            node->prev = tail;
            tail = node;
        }
        totalQty += node->order.qty;
    }

    void removeOrder(OrderNode* node) {
        if (node->prev) node->prev->next = node->next;
        else head = node->next;

        if (node->next) node->next->prev = node->prev;
        else tail = node->prev;

        totalQty -= node->order.qty;
    }

    bool empty() {
        return head == nullptr;
    }
};