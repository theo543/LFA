#include <fstream>
#include "L_NFA.h"

void file_mode() {
    L_NFA_Compiler compiler;
    std::ifstream in("input.txt");
    std::ofstream out("output.txt");
    int n, m, init, final_nr, state, a, b, word_nr;
    char c;
    std::string word_input;
    in>>n;
    for(int x = 0;x<n;x++) {
        in>>state;
        compiler.add_state(state);
    }
    in>>m;
    for(int x = 0;x<m;x++){
        in>>a>>b>>c;
        compiler.add_transition(a, c, b);
    }
    in>>init>>final_nr;
    compiler.set_start_state(init);
    for(int x = 0;x<final_nr;x++){
        int final_st;
        in>>final_st;
        compiler.set_final(final_st, true);
    }
    auto automat = compiler.compile();
    in>>word_nr;
    for(int x = 0;x<word_nr;x++){
        in>>std::ws;
        std::getline(in, word_input);
        for(char letter : word_input)
            automat->consume(letter);
        out << (automat->is_final() ? "YES" : "NO") << std::endl;
        automat->reset();
    }
}