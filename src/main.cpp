#include "Cube.h"
#include "Solver.h"
//#include "Cube.cpp"
//#include "Solver.cpp"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <emscripten/bind.h>
using namespace std;

// npm run build:wasm for shortcut when modifying cpp files
std::string generateCustomSolution(std::string scramble) {
    Cube cube;
    vector<int> scramInt = cube.stoiMoves(scramble);
    scramble = cube.itosMoves(scramInt);
    
    cube.scramble(scramble);
    Solver solver(cube);
    string solution = solver.solveCube();
    
    return "{\"scramble\":\"" + scramble + "\",\"solution\":\"" + solution + "\"}";
}

std::string generateRandomSolution() {
    Cube cube;
    srand(static_cast<unsigned int>(time(nullptr)));
    string scramble = cube.generateScramble();
    cube.scramble(scramble);
    Solver solver(cube);
    string solution = solver.solveCube();
    return "{\"scramble\":\"" + scramble + "\",\"solution\":\"" + solution + "\"}";
}

EMSCRIPTEN_BINDINGS(cube_module) {
    emscripten::function("solveCustomScramble", &generateCustomSolution);
    emscripten::function("solveRandomScramble", &generateRandomSolution);
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    string mode = "json";
    
    Cube c;
   
    string scramble = "R";
    // scramble = "U";
    c.scramble(scramble);
    Solver s(c);
    s.setMode(mode);
    
    if (mode == "json")
    {
        string solution = s.solveCube();
        std::ofstream out("data.json");
        out << "{\n";
        out << "  \"scramble\": \"" << scramble << "\",\n";
        out << "  \"solution\": \"" << solution << "\"\n";
        out << "}";

    }
    else {
        std::ofstream file("output.txt");
        std::streambuf* oldCout = std::cout.rdbuf();
        std::cout.rdbuf(file.rdbuf());
        cout << "Scramble: " << scramble << 
        "\n----------------------------------------------------------\n";
        cout << s.solveCube();
    }
    
    
    return 0;
    
}
