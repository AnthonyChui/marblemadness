#ifndef HashMap_h
#define HashMap_h
#include <string>
#include <list>

template <typename T>
struct Pair {
    std::string key;
    T value;
    Pair(const std::string& k, const T& v) : key(k), value(v) {}
};

template <typename T>
class HashMap {
public:
    HashMap(double max_load = 0.75) : max_load_factor(max_load), num_elements(0), num_buckets(10), n_buckets(10) {
        buckets = new std::list<Pair<T>>[n_buckets];
    }

    ~HashMap() {
        delete[] buckets;
    }

    int size() const {
        return num_elements;
    }

    void insert(const std::string& key, const T& value) {
        std::list<Pair<T>>* bucket = &buckets[getBucketIndex(key, num_buckets)];
        for (Pair<T>& item : *bucket) {
            if (item.key == key) {
                item.value = value;
                return;
            }
        }
        bucket->push_back(Pair<T>(key, value));
        num_elements++;
        if (num_elements / num_buckets > max_load_factor) {
            rehash();
        }
    }

    T& operator[](const std::string& key) {
        std::list<Pair<T>>* bucket = &buckets[getBucketIndex(key, num_buckets)];
        for (Pair<T>& item : *bucket) {
            if (item.key == key) {
                return item.value;
            }
        }
        insert(key, T());
        return bucket->back().value;
    }

    const T* find(const std::string& key) const {
        std::list<Pair<T>>* bucket = &buckets[getBucketIndex(key, num_buckets)];
        for (const Pair<T>& item : *bucket) {
            if (item.key == key) {
                return &item.value;
            }
        }
        return nullptr;
    }

    T* find(const std::string& key) {
        const auto& hm = *this;
        return const_cast<T*>(hm.find(key));
    }

private:
    double max_load_factor;
    double num_elements;
    double num_buckets;
    int n_buckets;
    std::list<Pair<T>>* buckets;

    void rehash() {
        int new_num_buckets = num_buckets * 2;
        std::list<Pair<T>>* new_buckets = new std::list<Pair<T>>[new_num_buckets];
        for (int i = 0; i < num_buckets; ++i) {
            for (Pair<T>& pair : buckets[i]) {
                int new_index = getBucketIndex(pair.key, new_num_buckets);
                new_buckets[new_index].push_back(pair);
            }
        }
        delete[] buckets;
        buckets = new_buckets;
        num_buckets = new_num_buckets;
    }

    size_t getBucketIndex(const std::string& key, int num_buckets) const {
        std::hash<std::string> hash_function;
        return hash_function(key) % num_buckets;
    }
};

#endif /* HashMap_h */
