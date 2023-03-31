#include <fstream>
#include <iostream>
#include "L_NFA.h"

void file_mode() {
    L_NFA_Compiler compiler;
    std::ifstream in("input.txt");
    std::ofstream out("output.txt");
    int n, m, word_nr;
    std::string word_input;
    in >> n;
    {
        int state;
        for (int x = 0; x < n; x++) {
            in >> state;
            compiler.add_state(state);
        }
    }
    in>>m;
    {
        int a, b;
        char c;
        for (int x = 0; x < m; x++) {
            in >> a >> b >> c;
            compiler.add_transition(a, c, b);
        }
    }
    {
        int final_nr, init;
        in >> init >> final_nr;
        compiler.set_start_state(init);
        for (int x = 0; x < final_nr; x++) {
            int final_st;
            in >> final_st;
            compiler.set_final(final_st, true);
        }
    }
    auto automat = compiler.compile();
    in>>word_nr;
    for(int x = 0;x<word_nr;x++){
        automat->reset();
        in>>std::ws;
        std::getline(in, word_input);
        std::cout<<"Init states: " << states_to_string(*automat) << std::endl;
        for(char letter : word_input) {
            automat->consume(letter);
            std::cout<<"States after "<<letter<<": ";
            std::cout<<states_to_string(*automat)<<std::endl;
        }
        out << (automat->is_final() ? "NO" : "YES") << std::endl;
    }
}