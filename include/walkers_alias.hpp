#include <deque>
#include <any>


// For implementation 
struct val_prob_pair_implementation {
    std::any val;
    float weight;
};
class walkersAliasAlgoImplementation {
    public:
        walkersAliasAlgoImplementation();
        walkersAliasAlgoImplementation(std::deque<std::any> values, std::deque<float> probablilities);
        walkersAliasAlgoImplementation(std::deque<val_prob_pair_implementation> values);
        std::any sample(uint64_t rand);

    private:
        void setup();

        std::deque<std::any> _values;
        std::deque<float> _probablilities;

        std::deque<int> aliases;
        std::deque<int> weights;
};



// User should use this interface for type safety
template<typename T>
struct val_prob_pair {
    T val;
    float weight;
};

template<typename T>
class walkersAliasAlgo {
public:
    walkersAliasAlgo(std::deque<T> values, std::deque<float> probablilities) {
        std::deque<std::any> vals;
        for (T val : values) vals.push_back(val);
        algo = walkersAliasAlgoImplementation(vals, probablilities);
    }
    walkersAliasAlgo(std::deque<val_prob_pair<T>> values) {
        std::deque<val_prob_pair_implementation> vals;
        for (val_prob_pair<T> v : values) vals.push_back( val_prob_pair_implementation{ v.val, v.weight } );
        algo = walkersAliasAlgoImplementation(vals);
    }
    T sample(uint64_t rand) {
        return std::any_cast<T>(algo.sample(rand));
    }

private:
    walkersAliasAlgoImplementation algo;
};