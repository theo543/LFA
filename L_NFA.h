#ifndef LFA_L_NFA_H
#define LFA_L_NFA_H

#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>

// can be used as a NFA or DFA
class L_NFA {
    std::unordered_map<int, int> state_number_to_index;
    std::vector<int> state_index_to_number;
    std::vector<bool> final_states;
    std::vector<std::vector<std::pair<char, int> > > transitions;
    std::vector<std::vector<int> > lambda_closure;
    std::unordered_set<int> current_states; // used for NFA and L-NFA
    int start_state;
    int current_state; // used only for DFA
    L_NFA();
public:
    void consume(char c);
    bool is_final();
    void reset();
};

class L_NFA_Compiler {
    std::unique_ptr<L_NFA> l_nfa;
public:
    L_NFA_Compiler(const L_NFA_Compiler &other) = delete;
    L_NFA_Compiler &operator=(const L_NFA_Compiler &other) = delete;
    L_NFA_Compiler(L_NFA_Compiler &&other);
    L_NFA_Compiler &operator=(L_NFA_Compiler &&other);
    L_NFA_Compiler();
    std::unique_ptr<L_NFA> compile();
    void add_state(int state_number, bool is_final);
    void add_transition(int from, char c, int to);
    void set_start_state(int state_number);
    void reset();
};

#endif //LFA_L_NFA_H
