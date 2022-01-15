#include <deque>
#include <any>
#include <stdint.h>
#include <random>
#include <iostream>

template <class _Ty = int>
class dummyRng {
public:
    using result_type = _Ty;

    void setValue(result_type value) {
        m_value = value;
    }

    constexpr result_type operator()() {
        return m_value;
    }

    constexpr result_type min() {
        return std::numeric_limits<result_type>::min();
    }

    constexpr result_type max() {
        return std::numeric_limits<result_type>::max();
    }

private:
    result_type m_value;
};

// User should use this interface for type safety
template<typename T>
struct val_prob_pair {
    T val;
    int weight;
};

template<typename T>
class weightedDistribution {
public:
    weightedDistribution(std::deque<val_prob_pair<T>> values) {
        uint64_t total_weight = 0;
        for (auto& vp : values) {
            internal_val_prob_pair<T> vp_internal;
            vp_internal.val = vp.val;
            vp_internal.min_weight = total_weight;

            total_weight += vp.weight;

            vp_internal.max_weight = total_weight;
            
            vals.push_back(vp_internal);
        }
        highest_weight = total_weight;
    }

    template<typename T1, class = typename std::enable_if<std::is_integral<T1>::value>::type>
    T sample(T1 rand) {
        T1 rand_int = rand % highest_weight;
        return std::find_if(vals.begin(), vals.end(), [rand_int](const internal_val_prob_pair<T>& vp) {
            return rand_int >= vp.min_weight && rand_int < vp.max_weight;
        })->val;
    }

private:
    template<typename T1>
    struct internal_val_prob_pair {
        T1 val;
        int min_weight;
        int max_weight;
    };
    uint64_t highest_weight;
    std::deque<internal_val_prob_pair<T>> vals;
};