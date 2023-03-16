#include <stdexcept>
#include <queue>
#include "L_NFA.h"

L_NFA::L_NFA() : start_state(-1), in_final_state(false) {}

void L_NFA::lambda_close_states() {
    std::unordered_set<int> new_states;
    for(int state : current_states)
        new_states.insert(lambda_closure[state].begin(), lambda_closure[state].end());
    current_states = new_states;
    in_final_state = false;
    for(int state : current_states)
        if(final_states[state]) {
            in_final_state = true;
            break;
        }
}

void L_NFA::consume(char c) {
    std::unordered_set<int> new_states;
    for(int state : current_states)
        for(int next_state : transitions[c - ALPHABET_START][state])
            new_states.insert(lambda_closure[next_state].begin(), lambda_closure[next_state].end());
    current_states = new_states;
    lambda_close_states();
}

bool L_NFA::is_final() const {
    return in_final_state;
}

std::vector<int> L_NFA::get_current_states() const {
    std::vector<int> numbers;
    for(int state : current_states)
        numbers.push_back(state_index_to_number[state]);
    return numbers;
}

void L_NFA::reset() {
    current_states.clear();
    current_states.insert(start_state);
    lambda_close_states();
}

L_NFA_Compiler::L_NFA_Compiler(L_NFA_Compiler &&other)  noexcept {
    this->l_nfa = std::move(other.l_nfa);
    this->state_number_to_index = std::move(other.state_number_to_index);
}

L_NFA_Compiler &L_NFA_Compiler::operator=(L_NFA_Compiler &&other) noexcept {
    this->l_nfa = std::move(other.l_nfa);
    this->state_number_to_index = std::move(other.state_number_to_index);
    return *this;
}

void L_NFA_Compiler::add_state(int state_number, bool is_final) {
    state_number_to_index[state_number] = next_id++;
    l_nfa->state_index_to_number.push_back(state_number);
    l_nfa->final_states.push_back(is_final);
    l_nfa->lambda_transitions.emplace_back();
    for(int i = 0; i < ALPHABET_SIZE; i++)
        l_nfa->transitions[i].emplace_back();
}

void L_NFA_Compiler::add_transition(int from, char c, int to) {
    if((c > ALPHABET_END || c < ALPHABET_START) && c != LAMBDA)
        throw std::invalid_argument("Invalid character");
    from = state_number_to_index.at(from);
    to = state_number_to_index.at(to);
    if(c == LAMBDA)
        l_nfa->lambda_transitions[from].push_back(to);
    else
        l_nfa->transitions[c - ALPHABET_START][from].push_back(to);
}

void L_NFA_Compiler::set_start_state(int state_number) {
    l_nfa->start_state = state_number_to_index.at(state_number);
}

void L_NFA_Compiler::reset() {
    l_nfa = std::unique_ptr<L_NFA>(new L_NFA());
    state_number_to_index.clear();
    next_id = 0;
}

L_NFA_Compiler::L_NFA_Compiler() {
    reset();
}

std::vector<int> bfs(std::vector<std::vector<int> > &graph, int start) {
    std::vector<bool> visited(graph.size(), false);
    std::vector<int> result;
    std::queue<int> q;
    q.push(start);
    while(!q.empty()) {
        int current = q.front();
        q.pop();
        if(visited[current])
            continue;
        visited[current] = true;
        result.push_back(current);
        for(int neighbor : graph[current])
            q.push(neighbor);
    }
    return std::move(result);
}

std::unique_ptr<L_NFA> L_NFA_Compiler::compile() {
    if(l_nfa->start_state == -1)
        throw std::runtime_error("No start state");
    for(int state = 0;state<next_id;state++){
        l_nfa->lambda_closure[state] = bfs(l_nfa->lambda_transitions, state);
    }
    std::unique_ptr<L_NFA> tmp = std::move(l_nfa);
    reset();
    return std::move(tmp);
}
