#include <iostream>
#include "L_NFA.h"
#include <sstream>

static const int SPACING = 100;

void add_keyboard_input(L_NFA_Compiler &compiler, int state_start) {
    std::cout<< "You must add the start state 0."<< std::endl;
    std::cout<< "Do not add states beyond " << SPACING - 1 << " to avoid collisions (unless this is the last or only test)" << std::endl;
    std::cout<< "(All states for this test will be incremented by " << state_start << " to give space for other tests)" << std::endl;
    std::cout<<"Enter the states. Final states should be marked with a * at the end. Use -1 to stop adding states." << std::endl;
    for(;;) {
        std::string state_number;
        std::cin >> state_number;
        bool final = false;
        if(state_number.back() == '*') {
            state_number.pop_back();
            final = true;
        }
        if(std::stoi(state_number) == -1)
            break;
        compiler.add_state(std::stoi(state_number) + state_start, final);
    }
    std::cout<<"Enter the transitions: " << std::endl;
    std::cout<<"Enter the transitions as number + letter + number (with spaces). Lambda transitions should be marked with a L." << std::endl;
    std::cout<<"Enter a * to automatically add 26 transitions for each alphabet character. (If you also want a lambda you must add it manually.)" << std::endl;
    std::cout<<"Enter the number -1 to stop adding transitions." << std::endl;
    for(;;) {
        int from, to;
        char c;
        std::cin >> from;
        if(from == -1)
            break;
        std::cin >> c >> to;
        if(c != '*')
            compiler.add_transition(from + state_start, c, to + state_start);
        else {
            for(char j = A_START; j <= A_END; j++)
                compiler.add_transition(from + state_start, j, to + state_start);
        }
    }
}

void add_number_matching(L_NFA_Compiler &compiler, int state_start) {
    int current_state = state_start;
    compiler.add_state(current_state++, false);
    const std::vector<std::string> numbers = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    for(const auto& nr : numbers) {
        compiler.add_state(current_state, false);
        compiler.add_transition(state_start, 'L', current_state);
        current_state++;
        for(int i = 0; i < nr.size(); i++) {
            compiler.add_state(current_state, i == (nr.size() - 1));
            compiler.add_transition(current_state - 1, nr[i], current_state);
            current_state++;
        }
    }
}

void add_even_length(L_NFA_Compiler &compiler, int state_start){
    compiler.add_state(state_start, true)
            .add_state(state_start + 1, false);
    for (char c = A_START; c <= A_END; c++) {
        compiler.add_transition(state_start, c, state_start + 1)
                .add_transition(state_start + 1, c, state_start);
    }
}

void add_odd_length(L_NFA_Compiler &compiler, int state_start){
    compiler.add_state(state_start, false)
            .add_state(state_start + 1, true);
    for (char c = A_START; c <= A_END; c++) {
        compiler.add_transition(state_start, c, state_start + 1)
                .add_transition(state_start + 1, c, state_start);
    }
}

void add_tema_2_ex_3(L_NFA_Compiler &compiler, int state_start) {
    int s0 = state_start;
    int s1 = state_start + 1;
    int s2 = state_start + 2;
    int s3 = state_start + 3;
    int s4 = state_start + 4;
    compiler.add_state(state_start, true)
            .add_state(state_start + 1, false)
            .add_state(state_start + 2, false)
            .add_state(state_start + 3, false)
            .add_state(state_start + 4, false)
            .add_transition(s0, 'b', s1)
            .add_transition(s0, 'L', s3)
            .add_transition(s1, 'b', s2)
            .add_transition(s2, 'a', s1)
            .add_transition(s3, 'b', s0)
            .add_transition(s3, 'L', s1)
            .add_transition(s3, 'L', s2)
            .add_transition(s3, 'a', s4)
            .add_transition(s3, 'a', s3)
            .add_transition(s4, 'b', s4)
            .add_transition(s4, 'L', s2)
            .add_transition(s4, 'b', s4);
}

void add_abc_loop_plus_ac(L_NFA_Compiler &compiler, int state_start) {
    // match (abc)*ac
    compiler.add_state(state_start, false)
            .add_state(state_start + 1, false)
            .add_state(state_start + 2, false)
            .add_state(state_start + 3, false)
            .add_state(state_start + 4, true)
            .add_transition(state_start, 'a', state_start + 1)
            .add_transition(state_start + 1, 'b', state_start + 2)
            .add_transition(state_start + 2, 'c', state_start)
            .add_transition(state_start, 'a', state_start + 3)
            .add_transition(state_start + 3, 'c', state_start + 4);
}

typedef void (*add_function)(L_NFA_Compiler&, int);
const std::vector<std::pair<std::string, add_function>> tests = {
        {"Number matching", add_number_matching},
        {"Even length", add_even_length},
        {"Odd length", add_odd_length},
        {"Tema 2 ex 3", add_tema_2_ex_3},
        {"(abc)*ac", add_abc_loop_plus_ac},
        {"Keyboard input", add_keyboard_input}
};

void interactive_testing_mode() {
    L_NFA_Compiler compiler;
    for(;;) {
        try {
            std::cout<<"Available tests:"<<std::endl;
            for(int i = 0; i < tests.size(); i++)
                std::cout<<i<<". "<<tests[i].first<<std::endl;
            std::cout<<"Choose which tests to add (can input multiple numbers)"<<std::endl;
            std::cout<<"Add non-number character to stop"<<std::endl;
            std::cin>>std::ws;
            std::string input;
            std::getline(std::cin, input);
            compiler.add_state(0, false);
            int offset = 1;
            for(char c : input) {
                if(c >= '0' && c <= '9') {
                    int test_index = c - '0';
                    if (test_index < tests.size()) {
                        tests[test_index].second(compiler, offset * SPACING);
                        if (offset == 2) {
                            compiler.add_transition(0, 'L', 1 * SPACING);
                            compiler.add_transition(0, 'L', 2 * SPACING);
                            compiler.set_start_state(0);
                        } else if (offset > 2) {
                            compiler.add_transition(0, 'L', offset * SPACING);
                        } else compiler.set_start_state(offset * SPACING);
                        offset++;
                    }
                } else if(c == ' ') {
                    continue;
                } else goto break_uppermost;
            }
            auto l_nfa = compiler.compile();
            if(l_nfa->get_type() == DFA) std::cout<<"DFA"<<std::endl;
            else if (l_nfa->get_type() == NFA) std::cout<<"NFA"<<std::endl;
            else std::cout<<"L-NFA"<<std::endl;
            for(;;) {
                l_nfa->reset();
                std::cout<<"Input string: (add non-letter character to stop) (use L for empty string)"<<std::endl;
                std::cin>>std::ws;
                std::getline(std::cin, input);
                if(input == "L") input = "";
                for(char c : input) {
                    if(c < A_START || c > A_END) {
                        goto break_upper;
                    }
                }
                std::cout << "Initial states: " << states_to_string(*l_nfa) << std::endl;
                for(char c : input) {
                    if(c >= A_START && c <= A_END) {
                        l_nfa->consume(c);
                        std::cout<<"Consuming: "<<c<<std::endl;
                        std::cout << "Current states: " << states_to_string(*l_nfa) << std::endl;
                    } else break;
                }
                std::cout<<"Accepting: "<<l_nfa->is_final()<<std::endl;
            }
            break_upper:;
        } catch (std::exception &e) {
            std::cout<<"Error: "<<e.what()<<std::endl;
            std::cout<<"Restarting with new automaton..." << std::endl;
            compiler.reset();
        }
    }
    break_uppermost:;
}
