#include <deque>
#include <any>
#include "walkers_alias.hpp"

walkersAliasAlgoImplementation::walkersAliasAlgoImplementation() {}
walkersAliasAlgoImplementation::walkersAliasAlgoImplementation(std::deque<std::any> values, std::deque<float> probablilities) {
    _values = values;
    _probablilities = probablilities;
}
walkersAliasAlgoImplementation::walkersAliasAlgoImplementation(std::deque<val_prob_pair_implementation> values) {
    for (val_prob_pair_implementation v : values) {
        _values.push_back(v.val);
        _probablilities.push_back(v.weight);
    }
}
std::any walkersAliasAlgoImplementation::sample(uint64_t rand) {
    int i = rand % _values.size();
    if (rand < _probablilities[i]) return _values[i];
    else                           return _values[aliases[i]];
}

// probs = [0.2, 0.3, 0.4, 0.1]
// n = len(probs)
// 
// # pre-computation of a[] and p[]
// a = [-1]*n
// p = [0]*n
// fixed = 0
// 
// while fixed < n:
//     # block assignment of small items
//     for i in range(n):
//         if p[i] == 0 and probs[i] * n <= 1.0:
//             p[i] = probs[i] * n
//             probs[i] = 0
//             fixed += 1
//     # packing of large items
//     for i in range(n):
//         if probs[i] * n > 1.0:
//             for j in range(n):
//                 if p[j] != 0 and a[j] == -1:
//                     a[j] = i
//                     probs[i] -= (1.0 - p[j])/n
//                 if probs[i] * n <= 1.0:
//                     break
void walkersAliasAlgoImplementation::setup() {
    int n = _values.size();
    int fixed = 0;

    aliases = std::deque<int>(n, -1);
    weights = std::deque<int>(n, 0);

    while (fixed < n) {
        // block assignment of small items
        for (int i = 0; i < n; i++) {
            if (weights[i] == 0 && _probablilities[i] * n <= 1.0) {
                weights[i] = _probablilities[i] * n;
                _probablilities[i] = 0;
                fixed += 1;
            }
        }
        // packing of large items
        for (int i = 0; i < n; i++) {
            if (_probablilities[i] * n > 1.0) {
                for (int j = 0; j < n; j++) {
                    if (weights[j] != 0 && aliases[j] == -1) {
                        aliases[j] = i;
                        _probablilities[i] -= (1.0 - weights[j])/n;
                    }
                    if (_probablilities[i] * n <= 1.0) {
                        break;
                    }
                }
            }
        }
    }
}