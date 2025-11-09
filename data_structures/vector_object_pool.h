#pragma once

#include <vector>
#include <cstddef>
#include <mutex>
#include <new>
#include <unordered_set>

template<typename T>
class VectorObjectPool {
public:
    explicit VectorObjectPool(size_t initialSize = 512) {
        addMore(initialSize);
    }

    ~VectorObjectPool() {
        std::lock_guard<std::mutex> lk(lock);

        for (void* p : active) {
            T* t = (T*)p;
            t->~T();
        }

        for (void* p : blocks) {
            ::operator delete(p);
        }

        freeList.clear();
        blocks.clear();
        active.clear();
    }

    template<typename... Args>
    T* create(Args&&... args) {
        std::lock_guard<std::mutex> lk(lock);

        if (freeList.empty()) {
            size_t grow = (capacity == 0 ? 1 : capacity / 2);
            if (grow < 1) grow = 1;
            addMore(grow);
        }

        void* mem = freeList.back();
        freeList.pop_back();

        T* obj = new(mem) T(std::forward<Args>(args)...);
        active.insert(mem);
        ++usedCount;

        return obj;
    }

    void destroy(T* obj) {
        if (!obj) return;

        obj->~T();

        void* rawPtr = (void*)obj;

        std::lock_guard<std::mutex> lk(lock);

        auto it = active.find(rawPtr);
        if (it != active.end()) {
            active.erase(it);
            freeList.push_back(rawPtr);
            --usedCount;
        }
    }

    size_t capacitySize() const { return capacity; }
    size_t used() const { return usedCount; }

private:
    void addMore(size_t n) {
        for (size_t i = 0; i < n; i++) {
            void* mem = ::operator new(sizeof(T));
            freeList.push_back(mem);
            blocks.push_back(mem);
        }
        capacity += n;
    }

    std::vector<void*> freeList;
    std::vector<void*> blocks;
    std::unordered_set<void*> active;

    size_t capacity = 0;
    size_t usedCount = 0;

    std::mutex lock;
};