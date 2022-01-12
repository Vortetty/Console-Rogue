#include <deque>
#include <any>
#include <stdint.h>
#include <random>

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
class randomDistribution {
public:
    randomDistribution(std::deque<val_prob_pair<T>> values) {
        int total_weight = 0;
        for (auto& vp : values) {
            internal_val_prob_pair<T> vp_internal;
            vp_internal.val = vp.val;
            vp_internal.min_weight = total_weight;

            total_weight += vp.weight;

            vp_internal.max_weight = total_weight;
            
            m_values.push_back(vp_internal);
        }
    }

    template<typename T1, class = typename std::enable_if_t<std::is_unsigned<T>::value>>
    T sample(T1 rand) {
        return vals[rand % vals.size()].val;
    }

private:
    template<typename T>
    struct internal_val_prob_pair {
        T val;
        int min_weight;
        int max_weight;
    };
    std::deque<internal_val_prob_pair<T>> vals;
};