#ifndef LFA_L_NFA_H
#define LFA_L_NFA_H

#include <vector>
#include <array>
#include <memory>
#include <unordered_map>
#include <unordered_set>

constexpr char A_START = 'a';
constexpr char A_END = 'z';
constexpr int A_SIZE = A_END - A_START + 1;

enum FA_Type {NFA, LNFA, DFA};

class L_NFA {
    std::vector<int> state_index_to_number;
    std::vector<bool> final_states;
    std::array<std::vector<std::vector<int> >, A_SIZE> transitions;
    std::vector<std::vector<int> > lambda_transitions;
    std::vector<std::vector<int> > lambda_closure;
    std::unordered_set<int> current_states;
    int start_state;
    bool in_final_state;
    L_NFA();
    void lambda_close_states();
    void update_final();
    FA_Type type;
public:
    void consume(char c);
    bool is_final() const;
    std::vector<int> get_current_states() const;
    void reset();
    FA_Type get_type() const;
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
    L_NFA_Compiler & add_state(int state_number, bool is_final = false);
    void set_final(int state_number, bool is_final);
    L_NFA_Compiler & add_transition(int from, char c, int to);
    L_NFA_Compiler & set_start_state(int state_number);
    L_NFA_Compiler & reset();
};

std::string states_to_string(L_NFA const &l_nfa);

#endif //LFA_L_NFA_H
