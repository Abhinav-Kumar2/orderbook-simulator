#include "orderbook.h"
#include "../data_handling/logger.h"
#include <algorithm>
#include <sstream>
#include <iostream>
#include <chrono>

using namespace std;

void OrderBook::addOrder(const Order& order) { // Add new order to the order book
    OrderNode* node = nodePool.create(order);
    orderIndex[order.id] = node;

    if (order.side == Side::BUY) { // Buy order
        PriceLevel* level = bids.find(order.price);
        if (!level) {
            bids.insert_or_get(order.price, PriceLevel(order.price));
            level = bids.find(order.price);
        }
        level->addOrder(node);
        bids.refreshWeight(order.price);
    } else { // Sell order
        PriceLevel* level = asks.find(order.price);
        if (!level) {
            asks.insert_or_get(order.price, PriceLevel(order.price));
            level = asks.find(order.price);
        }
        level->addOrder(node);
        asks.refreshWeight(order.price);
    }

    // Log order 
    log("Added order ID=" + to_string(order.id) +
        " Ticker=" + order.ticker +
        " Price=" + to_string(order.price) + " Qty=" + to_string(order.qty));
}

bool OrderBook::cancelOrder(OrderID id) { // Cancel an existing order
    auto it = orderIndex.find(id);
    if (it == orderIndex.end()) return false;

    OrderNode* node = it->second;

    if (node->order.side == Side::BUY) { // Buy order
        PriceLevel* level = bids.find(node->order.price);
        if (level) {
            level->removeOrder(node);
            if (level->empty()) {
                bids.erase(node->order.price);
            } else {
                bids.refreshWeight(node->order.price);
            }
        }
    } else { // Sell order
        PriceLevel* level = asks.find(node->order.price);
        if (level) {
            level->removeOrder(node);
            if (level->empty()) {
                asks.erase(node->order.price);
            } else {
                asks.refreshWeight(node->order.price);
            }
        }
    }

    // Remove order
    nodePool.destroy(node);
    orderIndex.erase(it);

    // Log cancellation
    log("Cancelled order ID=" + to_string(id));
    return true;
}

void OrderBook::match() { // Match and execute orders
    while (!bids.empty() && !asks.empty()) {
        // Match order
        PriceLevel* bestBid = bids.minValue();
        PriceLevel* bestAsk = asks.minValue();

        if (!bestBid || !bestAsk) break;

        Price bidPrice = bestBid->price;
        Price askPrice = bestAsk->price;

        if (!(bidPrice >= askPrice)) break;

        OrderNode* bidNode = bestBid->head;
        OrderNode* askNode = bestAsk->head;

        if (!bidNode || !askNode) break;

        Qty traded = min<Qty>(bidNode->order.qty, askNode->order.qty);

        bidNode->order.qty -= traded;
        askNode->order.qty -= traded;
        bestBid->totalQty -= traded;
        bestAsk->totalQty -= traded;
        
        // Record trade
        Trade trade;
        trade.buyId = (bidNode->order.side == Side::BUY) ? bidNode->order.id : askNode->order.id;
        trade.sellId = (askNode->order.side == Side::SELL) ? askNode->order.id : bidNode->order.id;
        trade.ticker = bidNode->order.ticker;
        trade.price = bestAsk->price;
        trade.qty = traded;
        trade.ts = static_cast<Timestamp>(chrono::steady_clock::now().time_since_epoch().count());

        trades.push_back(trade);
        
        // Log trade
        log("Trade " + to_string(traded) + " at " + to_string(bestAsk->price) +
            " Ticker=" + bidNode->order.ticker +
            " bidID=" + to_string(bidNode->order.id) +
            " askID=" + to_string(askNode->order.id));

        // Order cleanup
        if (bidNode->order.qty == 0) {
            bestBid->removeOrder(bidNode);
            orderIndex.erase(bidNode->order.id);
            nodePool.destroy(bidNode);
        }

        if (askNode->order.qty == 0) {
            bestAsk->removeOrder(askNode);
            orderIndex.erase(askNode->order.id);
            nodePool.destroy(askNode);
        }

        if (bestBid->empty()) {
            bids.erase(bidPrice);
        } else {
            bids.refreshWeight(bidPrice);
        }

        if (bestAsk->empty()) {
            asks.erase(askPrice);
        } else {
            asks.refreshWeight(askPrice);
        }
    }
}