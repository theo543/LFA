#ifndef LFA_L_NFA_H
#define LFA_L_NFA_H

#include <vector>
#include <array>
#include <memory>
#include <unordered_map>
#include <unordered_set>

constexpr char LAMBDA = 'L';
constexpr char ALPHABET_START = 'a';
constexpr char ALPHABET_END = 'z';
constexpr int ALPHABET_SIZE = ALPHABET_END - ALPHABET_START + 1;

// can be used as a NFA or DFA
class L_NFA {
    std::vector<int> state_index_to_number;
    std::vector<bool> final_states;
    std::array<std::vector<std::vector<int> >, ALPHABET_SIZE> transitions;
    std::vector<std::vector<int> > lambda_transitions;
    std::vector<std::vector<int> > lambda_closure;
    std::unordered_set<int> current_states; // used for NFA and L-NFA
    int start_state;
    bool in_final_state;
    L_NFA();
    void lambda_close_states();
public:
    void consume(char c);
    bool is_final() const;
    std::vector<int> get_current_states() const;
    void reset();
    friend class L_NFA_Compiler;
};

class L_NFA_Compiler {
    std::unique_ptr<L_NFA> l_nfa;
    std::unordered_map<int, int> state_number_to_index;
    int next_id = 0;
public:
    L_NFA_Compiler(const L_NFA_Compiler &other) = delete;
    L_NFA_Compiler &operator=(const L_NFA_Compiler &other) = delete;
    L_NFA_Compiler(L_NFA_Compiler &&other) noexcept;
    L_NFA_Compiler &operator=(L_NFA_Compiler &&other) noexcept;
    L_NFA_Compiler();
    std::unique_ptr<L_NFA> compile();
    void add_state(int state_number, bool is_final);
    void add_transition(int from, char c, int to);
    void set_start_state(int state_number);
    void reset();
};

#endif //LFA_L_NFA_H
