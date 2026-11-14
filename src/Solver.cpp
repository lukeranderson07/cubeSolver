#include "Solver.h"
#include "Cube.h"
using namespace std;
#include <iostream>

Solver::Solver(const Cube& scrambled) : cube(scrambled) {
    mode = "json";
}

void Solver::setMode(string m) {
    mode = m;
}

string Solver::solveEdges() {
    string moves;
    bool flippedPivot = false;
    int iterations = 0;

    while (!cube.edgesSolved() && iterations < 20) {
        Edge cur = cube.edges[1];   // buffer = UR
    
        if (cur.perm == 1) { // buffer is solved
            if (cur.orient == 1)
                flippedPivot = true;

            for (int i = 0; i < 12; i++) {
                if (i == 1) continue;          // skip buffer
                if (cube.edges[i].perm != i) {   // unsolved edge
                    cur = cube.edges[i];
                    break;
                }
            }
        }
        if (cube.edgesPermuted()) {
            for (int i = 0; i < 12; i++) {
                if (i == 1) continue; // skips buffer
                if (cube.edges[i].orient != 0) { // fixes flipped edges
                    cur = cube.edges[i];
                    moves += swapEdge(cur);
                    cur.orient = 0;             
                    break;
                }
            }
        }

        moves += swapEdge(cur);
        iterations++;
    }
   
    return moves;
}

string Solver::solveCorners() {
    string moves;
    int numSwaps = 0;
    int iterations = 0;

    while (!cube.cornersSolved() && iterations < 20) {
        Corner cur = cube.corners[0];   // buffer = ULB
        
        if (cur.perm == 0) { // buffer is solved
            for (int i = 1; i < 8; i++) {
                if (cube.corners[i].perm != i) {
                    cur = cube.corners[i];
                    break;
                }
            }
        }
        if (cube.cornersPermuted()) {
            for (int i = 1; i < 8; i++) {
                if (cube.corners[i].orient != 0) { // fixes rotated corners
                    cur = cube.corners[i];
                    moves += swapCorner(cur);
                    cur.orient = 0;             
                    break;
                }
            }
        }
        moves += swapCorner(cur);
        numSwaps++;
        iterations++;
    }
    return moves;
}

string Solver::solveCube() {
    if (mode != "D" && mode != "json") {
        cout << "Edge Trigger: " << "R U R' U' R' F R2 U' R' U' R U R' F' " << endl;
        cout << "Corner Trigger: " << "R U' R' U' R U R' F' R U R' U' R' F R " << endl;
        cout << "----------------------------------------------------------\n";
    }
    string moves = solveEdges();
    moves += solveCorners();
    if (!cube.edgesSolved() || !cube.cornersSolved()) {
        cout << "SOLUTION FAILED";
    }
    return moves;
}

char Solver::getEdgeLetter(Edge e) {
    switch (e.perm) {
        case 0:  // UF
            return (e.orient == 0 ? 'C' : 'I');
        case 1:  // UR
            return (e.orient == 0 ? 'B' : 'M');
        case 2:  // UB
            return (e.orient == 0 ? 'A' : 'Q');
        case 3:  // UL
            return (e.orient == 0 ? 'D' : 'E');
        case 4:  // FR
            return (e.orient == 0 ? 'J' : 'P');
        case 5:  // BR
            return (e.orient == 0 ? 'T' : 'N');
        case 6:  // BL
            // B sticker = T? No : BL = H on L face, R on B face
            return (e.orient == 0 ? 'R' : 'H');
        case 7:  // FL
            // F sticker = L, L sticker = F
            return (e.orient == 0 ? 'L' : 'F');
        case 8:  // DF
            return (e.orient == 0 ? 'U' : 'K');
        case 9:  // DR
            return (e.orient == 0 ? 'V' : 'O');
        case 10: // DB
            return (e.orient == 0 ? 'W' : 'S');
        case 11: // DL
            return (e.orient == 0 ? 'X' : 'G');
        default:
            return '?';
    }
}

char Solver::getCornerLetter(Corner c) {
    switch (c.perm) {
        case 0:  // ULB
            if (c.orient == 0) return 'E';
            if (c.orient == 1) return 'R';
            return 'A';
        case 1:  // URB
            if (c.orient == 0) return 'N';
            if (c.orient == 1) return 'B';
            return 'Q';
        case 2:  // UFR
            if (c.orient == 0) return 'M';
            if (c.orient == 1) return 'J';
            return 'C';
        case 3:  // ULF
            if (c.orient == 0) return 'F';
            if (c.orient == 1) return 'D';
            return 'I';
        case 4:  // DFL
            if (c.orient == 0) return 'G';
            if (c.orient == 1) return 'L';
            return 'U';
        case 5:  // DFR
            if (c.orient == 0) return 'P';
            if (c.orient == 1) return 'V';
            return 'K';
        case 6:  // DRB
            if (c.orient == 0) return 'O';
            if (c.orient == 1) return 'T';
            return 'W';
        case 7:  // DBL
            if (c.orient == 0) return 'H';
            if (c.orient == 1) return 'X';
            return 'S';
        default: // something is wrong...
            return '?';
    }
}

string Solver::swapEdge(Edge& e) {
    string setup;
    string unwind;
    char letter = getEdgeLetter(e);
    if (letter == 'A') {
        setup = "Lw2 D' L2 ";
        unwind = "L2 D Lw2 ";
    }
    else if (letter == 'B') {
        // should never get here
    }
    else if (letter == 'C') {
        setup = "Lw2 D L2 ";
        unwind = "L2 D' Lw2 ";
    }
    else if (letter == 'D') {
        // in place already
        setup = "";
        unwind = "";
    }
    else if (letter == 'E') {
        setup = "L' Dw L' ";
        unwind = "L Dw' L ";
    }
    else if (letter == 'F') {
        setup = "Dw' L ";
        unwind = "L' Dw ";
    }
    else if (letter == 'G')
    {
        setup = "L' Dw' L ";
        unwind = "L' Dw L ";
    }
    else if (letter == 'H')
    {
        setup = "Dw L' ";
        unwind = "L Dw' ";
    }
    else if (letter == 'I')
    {
        setup = "Lw D' L2 ";
        unwind = "L2 D Lw' ";
    }
    else if (letter == 'J')
    {
        setup = "Dw2 L ";
        unwind = "L' Dw2 ";
    }
    else if (letter == 'K')
    {
        setup = "Lw D L2 ";
        unwind = "L2 D' Lw' ";
    }
    else if (letter == 'L')
    {
        setup = "L' ";
        unwind = "L ";
    }
    else if (letter == 'M')
    {
        // should not get here
    }
    else if (letter == 'N')
    {
        setup = "Dw L ";
        unwind = "L' Dw' ";
    }
    else if (letter == 'O')
    {
        setup = "Dw' Lw D L2 ";
        unwind = "L2 D' Lw' Dw ";
    }
    else if (letter == 'P')
    {
        setup = "Dw' L' ";
        unwind = "L Dw ";
    }
    else if (letter == 'Q')
    {
        setup = "Lw' D L2 ";
        unwind = "L2 D' Lw ";
    }
    else if (letter == 'R')
    {
        setup = "L ";
        unwind = "L' ";
    }
    else if (letter == 'S')
    {
        setup = "Lw' D' L2 ";
        unwind = "L2 D Lw ";
    }
    else if (letter == 'T')
    {
        setup = "Dw2 L' ";
        unwind = "L Dw2 ";
    }
    else if (letter == 'U')
    {
        setup = "D' L2 ";
        unwind = "L2 D ";
    }
    else if (letter == 'V')
    {
        setup = "D2 L2 ";
        unwind = "L2 D2 ";
    }
    else if (letter == 'W')
    {
        setup = "D L2 ";
        unwind = "L2 D' ";
    }
    else if (letter == 'X')
    {
        setup = "L2 ";
        unwind = "L2 ";
    }
    cube.applyMoves(cube.stoiMoves(setup));
    string moves;
    if (mode != "U")
    {
        moves = setup + edgeTrigger() + unwind;
        if (mode == "D") moves += "\n";
    }
    else
    {
        edgeTrigger();
        moves = setup + "ET " + unwind + "\n";
    }
    cube.applyMoves(cube.stoiMoves(unwind));
    return moves;
}

string Solver::swapCorner(Corner& c)
{
    string setup;
    string unwind;
    char letter = getCornerLetter(c);
    if (letter == 'A')
    {
        // should never get here
    }
    else if (letter == 'B')
    {
        setup = "R2 ";
        unwind = "R2 ";
    }
    else if (letter == 'C')
    {
        setup = "R2 D' ";
        unwind = "D R2 ";
    }
    else if (letter == 'D')
    {
        setup = "F2 ";
        unwind = "F2 ";
    }
    else if (letter == 'E')
    {
        // should never get here
    }
    else if (letter == 'F')
    {
        setup = "F' D ";
        unwind = "D' F ";
    }
    else if (letter == 'G')
    {
        setup = "F' ";
        unwind = "F ";
    }
    else if (letter == 'H')
    {
        setup = "D' R ";
        unwind = "R' D ";
    }
    else if (letter == 'I')
    {
        setup = "F R' ";
        unwind = "R F' ";
    }
    else if (letter == 'J')
    {
        setup = "R' ";
        unwind = "R ";
    }
    else if (letter == 'K')
    {
        setup = "R' D' ";
        unwind = "D R ";
    }
    else if (letter == 'L')
    {
        setup = "F' R' D' ";
        unwind = "D R F ";
    }
    else if (letter == 'M')
    {
        setup = "F ";
        unwind = "F' ";
    }
    else if (letter == 'N')
    {
        setup = "R' F ";
        unwind = "F' R ";
    }
    else if (letter == 'O')
    {
        setup = "R2 F ";
        unwind = "F' R2 ";
    }
    else if (letter == 'P')
    {
        setup = "R F ";
        unwind = "F' R' ";
    }
    else if (letter == 'Q')
    {
        setup = "R D' ";
        unwind = "D R' ";
    }
    else if (letter == 'R')
    {
        // should never get here
    }
    else if (letter == 'S')
    {
        setup = "D F' ";
        unwind = "F D' ";
    }
    else if (letter == 'T')
    {
        setup = "R ";
        unwind = "R' ";
    }
    else if (letter == 'U')
    {
        setup = "D ";
        unwind = "D' ";
    }
    else if (letter == 'V')
    {
        // already there
        setup = "";
        unwind = "";
    }
    else if (letter == 'W')
    {
        setup = "D' ";
        unwind = "D ";
    }
    else if (letter == 'X')
    {
        setup = "D2 ";
        unwind = "D2 ";
    }
    cube.applyMoves(cube.stoiMoves(setup));
    string moves;
    if (mode != "U")
    {
        moves = setup + cornerTrigger() + unwind;
        if (mode == "D") moves += "\n";
    }
    else
    {
        cornerTrigger();
        moves = setup + "CT " + unwind + "\n";
    }
    cube.applyMoves(cube.stoiMoves(unwind));
    return moves;
}

void Solver::displayState()
{
    cout << "CORNERS:" << endl;
    for (int i = 0; i < 8; i++)
    {
        cout << cube.corners[i].perm << " " << cube.corners[i].orient << ", ";
    }
    cout << endl << "EDGES:" << endl;
    for (int i = 0; i < 12; i++)
    {
        cout << cube.edges[i].perm << " " << cube.edges[i].orient << ", ";
    }
    cout << endl;
}

string Solver::edgeTrigger()
{
    string moves = "R U R' U' R' F R2 U' R' U' R U R' F' ";
    cube.applyMoves(cube.stoiMoves(moves));
    return moves;
}
string Solver::cornerTrigger()
{
    string moves = "R U' R' U' R U R' F' R U R' U' R' F R ";
    cube.applyMoves(cube.stoiMoves(moves));
    return moves;
}