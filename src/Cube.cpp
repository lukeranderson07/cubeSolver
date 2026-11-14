#include "Cube.h"

using namespace std;
#include <iostream>
#include <set>
Cube::Cube() {
    for (int i = 0; i < 12; i++) {
        edges[i].perm = i;
        edges[i].orient = 0;
    }
    for (int i = 0; i < 8; i++) {
        corners[i].perm = i;
        corners[i].orient = 0;
    }
}
void Cube::setState(vector<vector<char>> faces) { // 6 faces, 8 values of 'W', 'O', 'G', 'R', 'B' 'Y' (sides are in this order)
    // faces are labled clockwise, starting from top left and containg 8 values 0-7
    // corners are even, edges are odd
    // colors are logged in cw order, starting w/ top or bottom
    corners[0].colors = {faces[0][0], faces[1][0], faces[4][2]};
    corners[1].colors = {faces[0][2], faces[4][0], faces[3][2]};
    corners[2].colors = {faces[0][4], faces[3][0], faces[2][2]};
    corners[3].colors = {faces[0][6], faces[2][0], faces[1][2]};
    corners[4].colors = {faces[5][0], faces[1][4], faces[2][6]};
    corners[5].colors = {faces[5][2], faces[2][4], faces[3][6]};
    corners[6].colors = {faces[5][4], faces[3][4], faces[4][6]};
    corners[7].colors = {faces[5][6], faces[4][4], faces[1][6]};

    edges[0].colors = {faces[0][5], faces[2][1]};
    edges[1].colors = {faces[0][3], faces[3][1]};
    edges[2].colors = {faces[0][1], faces[4][1]};
    edges[3].colors = {faces[0][7], faces[1][1]};
    edges[4].colors = {faces[2][3], faces[3][7]};
    edges[5].colors = {faces[4][7], faces[3][3]};
    edges[6].colors = {faces[4][3], faces[1][7]};
    edges[7].colors = {faces[2][7], faces[1][3]};
    edges[8].colors = {faces[5][1], faces[2][5]};
    edges[9].colors = {faces[5][3], faces[3][5]};
    edges[10].colors = {faces[5][5], faces[4][5]};
    edges[11].colors = {faces[5][7], faces[1][5]};

    for (int i = 0; i < 8; i++) { // logs current corner permutation and orientation
        Corner cur = corners[i];
        if (cur.has('W') && cur.has('O') && cur.has('B'))
            corners[i].perm = 0; // already this
        else if (cur.has('W') && cur.has('R') && cur.has('B'))
            corners[i].perm = 1;
        else if (cur.has('W') && cur.has('R') && cur.has('G'))
            corners[i].perm = 2;
        else if (cur.has('W') && cur.has('O') && cur.has('G'))
            corners[i].perm = 3;
        else if (cur.has('Y') && cur.has('O') && cur.has('G'))
            corners[i].perm = 4;
        else if (cur.has('Y') && cur.has('R') && cur.has('G'))
            corners[i].perm = 5;
        else if (cur.has('Y') && cur.has('R') && cur.has('B'))
            corners[i].perm = 6;
        else if (cur.has('Y') && cur.has('O') && cur.has('B'))
            corners[i].perm = 7;
        else {
            cout << "INVALID CUBE STATE " << endl;
            return;
        }
        char target; // for orientation
        if (cur.perm < 4) 
            target = 'W';
        else
            target = 'Y';
        if (cur.colors[1] == target)
            corners[i].orient = 1;
        else if (cur.colors[2] == target)
            corners[i].orient = 2;
        // default orientation is 0
    }
    

    for (int i = 0; i < 12; i++) { // logs current edge permutation and orientation
        Edge cur = edges[i];
        if (cur.has('W') && cur.has('G'))
            edges[i].perm = 0;
        else if (cur.has('W') && cur.has('R'))
            edges[i].perm = 1;
        else if (cur.has('W') && cur.has('B'))
            edges[i].perm = 2;
        else if (cur.has('W') && cur.has('O'))
            edges[i].perm = 3;
        else if (cur.has('G') && cur.has('R'))
            edges[i].perm = 4;
        else if (cur.has('B') && cur.has('R'))
            edges[i].perm = 5;
        else if (cur.has('B') && cur.has('O'))
            edges[i].perm = 6;
        else if (cur.has('G') && cur.has('O'))
            edges[i].perm = 7;
        else if (cur.has('Y') && cur.has('G'))
            edges[i].perm = 8;
        else if (cur.has('Y') && cur.has('R'))
            edges[i].perm = 9;
        else if (cur.has('Y') && cur.has('B'))
            edges[i].perm = 10;
        else if (cur.has('Y') && cur.has('O'))
            edges[i].perm = 11;
        else {
            cout << "INVALID CUBE STATE" << endl;
            return;
        }

        char target; // orientation
        if (cur.perm < 4)
            target = 'W';
        else if (cur.perm < 8) {// check logic
            if (cur.has('G'))
                target = 'G';
            else 
                target = 'B';
        }
        else
            target = 'Y';
        if (cur.colors[1] == target)
            edges[i].orient = 1;
        // default orientation is 0
    }
  
}
string Cube::generateScramble() {
    string scramble;
    string moves[] = {"U", "D", "F", "B", "R", "L"};
    string directions[] = {"", "'", "2"};
    int numMoves = 20 + rand() % 5;
    string prevMove = "";
    for (int i = 0; i < numMoves; i++) {
        string move = moves[rand() % 6];
        if (prevMove == move) {
            i--;
            continue;
        }

        scramble += move + directions[rand() % 3] + " ";
        prevMove = move;
    }
    return scramble;
    
}
void Cube::scramble(const string moves) {
    applyMoves(stoiMoves(moves));
}
void Cube::applyMove(const int& move) {
    if (move == 0) { // U // corners do not change orientation w/ u and d moves
        cycleCW(corners[0], corners[1], corners[2], corners[3]); // cycle corners
        cycleCW(edges[2], edges[1], edges[0], edges[3]); // cycle edges
    }
    else if (move == 1) { // U'
        cycleCCW(corners[0], corners[1], corners[2], corners[3]); 
        cycleCCW(edges[2], edges[1], edges[0], edges[3]);
    }
    else if (move == 2) { // U2 // corners do not change orientation w/ u and d moves
        cycleCW(corners[0], corners[1], corners[2], corners[3]); // cycle corners
        cycleCW(edges[2], edges[1], edges[0], edges[3]); // cycle edges
        cycleCW(corners[0], corners[1], corners[2], corners[3]); // cycle corners
        cycleCW(edges[2], edges[1], edges[0], edges[3]); // cycle edges
    }
    else if (move == 3) { // D
        cycleCW(corners[4], corners[5], corners[6], corners[7]); 
        cycleCW(edges[8], edges[9], edges[10], edges[11]);
    }
    else if (move == 4) { // D'
        cycleCCW(corners[4], corners[5], corners[6], corners[7]); 
        cycleCCW(edges[8], edges[9], edges[10], edges[11]);
    }
    else if (move == 5) { // D2
        cycleCW(corners[4], corners[5], corners[6], corners[7]); 
        cycleCW(edges[8], edges[9], edges[10], edges[11]);
        cycleCW(corners[4], corners[5], corners[6], corners[7]); 
        cycleCW(edges[8], edges[9], edges[10], edges[11]);
    }
    else if (move == 18) { // Dw
        cycleCW(corners[4], corners[5], corners[6], corners[7]);     
        cycleCW(edges[8], edges[9], edges[10], edges[11]);           
        cycleCW(edges[4], edges[5], edges[6], edges[7]);             
        flipEdges(edges[4], edges[5], edges[6], edges[7]);
    }
    else if (move == 19) { // Dw'
        cycleCCW(corners[4], corners[5], corners[6], corners[7]);
        cycleCCW(edges[8], edges[9], edges[10], edges[11]);
        cycleCCW(edges[4], edges[5], edges[6], edges[7]);
        flipEdges(edges[4], edges[5], edges[6], edges[7]);
    }
    else if (move == 20) { // Dw2
        cycleCW(corners[4], corners[5], corners[6], corners[7]);     
        cycleCW(edges[8], edges[9], edges[10], edges[11]);           
        cycleCW(edges[4], edges[5], edges[6], edges[7]);           
        cycleCW(corners[4], corners[5], corners[6], corners[7]);     
        cycleCW(edges[8], edges[9], edges[10], edges[11]);           
        cycleCW(edges[4], edges[5], edges[6], edges[7]);             
    }
    else if (move == 6) { // F
        orientCorners(corners[3], corners[2], corners[5], corners[4]);
        cycleCW(corners[3], corners[2], corners[5], corners[4]);
        flipEdges(edges[0], edges[4], edges[8], edges[7]);
        cycleCW(edges[0], edges[4], edges[8], edges[7]);
    }
    else if (move == 7) { // F'
        orientCorners(corners[3], corners[2], corners[5], corners[4]);
        cycleCCW(corners[3], corners[2], corners[5], corners[4]);
        flipEdges(edges[0], edges[4], edges[8], edges[7]);
        cycleCCW(edges[0], edges[4], edges[8], edges[7]);
    }
    else if (move == 8) { // F2
        orientCorners(corners[3], corners[2], corners[5], corners[4]);
        cycleCW(corners[3], corners[2], corners[5], corners[4]);
        cycleCW(edges[0], edges[4], edges[8], edges[7]);
        orientCorners(corners[3], corners[2], corners[5], corners[4]);
        cycleCW(corners[3], corners[2], corners[5], corners[4]);
        cycleCW(edges[0], edges[4], edges[8], edges[7]);
    }
    else if (move == 9) { // B
        orientCorners(corners[1], corners[0], corners[7], corners[6]);
        cycleCW(corners[1], corners[0], corners[7], corners[6]);
        flipEdges(edges[2], edges[6], edges[10], edges[5]);
        cycleCW(edges[2], edges[6], edges[10], edges[5]);
    }
    else if (move == 10) { // B'
        orientCorners(corners[1], corners[0], corners[7], corners[6]);
        cycleCCW(corners[1], corners[0], corners[7], corners[6]);
        flipEdges(edges[2], edges[6], edges[10], edges[5]);
        cycleCCW(edges[2], edges[6], edges[10], edges[5]);
    }
    else if (move == 11) { // B2
        orientCorners(corners[1], corners[0], corners[7], corners[6]);
        cycleCW(corners[1], corners[0], corners[7], corners[6]);
        cycleCW(edges[2], edges[6], edges[10], edges[5]);
        orientCorners(corners[1], corners[0], corners[7], corners[6]);
        cycleCW(corners[1], corners[0], corners[7], corners[6]);
        cycleCW(edges[2], edges[6], edges[10], edges[5]);
    }
    else if (move == 12) { // R
        orientCorners(corners[2], corners[1], corners[6], corners[5]);
        cycleCW(corners[2], corners[1], corners[6], corners[5]);
        cycleCW(edges[1], edges[5], edges[9], edges[4]);
    }
    else if (move == 13) { // R'
        orientCorners(corners[2], corners[1], corners[6], corners[5]);
        cycleCCW(corners[2], corners[1], corners[6], corners[5]);
        cycleCCW(edges[1], edges[5], edges[9], edges[4]);
    }
    else if (move == 14) { // R2
        orientCorners(corners[2], corners[1], corners[6], corners[5]);
        cycleCW(corners[2], corners[1], corners[6], corners[5]);
        cycleCW(edges[1], edges[5], edges[9], edges[4]);
        orientCorners(corners[2], corners[1], corners[6], corners[5]);
        cycleCW(corners[2], corners[1], corners[6], corners[5]);
        cycleCW(edges[1], edges[5], edges[9], edges[4]);
    }
    else if (move == 15) { // L
        orientCorners(corners[0], corners[3], corners[4], corners[7]);
        cycleCW(corners[0], corners[3], corners[4], corners[7]);
        cycleCW(edges[3], edges[7], edges[11], edges[6]);
    }
    else if (move == 16) { // L'
        orientCorners(corners[0], corners[3], corners[4], corners[7]);
        cycleCCW(corners[0], corners[3], corners[4], corners[7]);
        cycleCCW(edges[3], edges[7], edges[11], edges[6]);
    }
    else if (move == 17) { // L2
        orientCorners(corners[0], corners[3], corners[4], corners[7]);
        cycleCW(corners[0], corners[3], corners[4], corners[7]);
        cycleCW(edges[3], edges[7], edges[11], edges[6]);
        orientCorners(corners[0], corners[3], corners[4], corners[7]);
        cycleCW(corners[0], corners[3], corners[4], corners[7]);
        cycleCW(edges[3], edges[7], edges[11], edges[6]);
    }
    else if (move == 21) { // Lw
        orientCorners(corners[0], corners[3], corners[4], corners[7]);
        cycleCW(corners[0], corners[3], corners[4], corners[7]);
        cycleCW(edges[3], edges[7], edges[11], edges[6]);   // L layer
        cycleCW(edges[0], edges[8], edges[10], edges[2]);   // inner slice
        flipEdges(edges[0], edges[8], edges[10], edges[2]);
    }
    else if (move == 22) { // Lw'
        orientCorners(corners[0], corners[3], corners[4], corners[7]);
        cycleCCW(corners[0], corners[3], corners[4], corners[7]);
        cycleCCW(edges[3], edges[7], edges[11], edges[6]);
        cycleCCW(edges[0], edges[8], edges[10], edges[2]);
        flipEdges(edges[0], edges[8], edges[10], edges[2]);
    }
    else if (move == 23) { // L2
        orientCorners(corners[0], corners[3], corners[4], corners[7]);
        cycleCW(corners[0], corners[3], corners[4], corners[7]);
        cycleCW(edges[3], edges[7], edges[11], edges[6]);   // L layer
        cycleCW(edges[0], edges[8], edges[10], edges[2]);   // inner slice
        orientCorners(corners[0], corners[3], corners[4], corners[7]);
        cycleCW(corners[0], corners[3], corners[4], corners[7]);
        cycleCW(edges[3], edges[7], edges[11], edges[6]);   // L layer
        cycleCW(edges[0], edges[8], edges[10], edges[2]);   // inner slice
    }
    // cout << "doing move: " << move << endl;

}
void Cube::applyMoves(const vector<int>& moves)
{
    for (int i = 0; i < moves.size(); i++)
    {
        applyMove(moves.at(i));
    }
}

vector<int> Cube::stoiMoves(const string moves) {
    vector<int> intMoves;
    for (int i = 0; i < moves.size(); i++) {
        if (moves[i] == ' ' || moves[i] == '\n')
            continue;

        
        if (i + 1 >= moves.size()) {
            intMoves.push_back(stoiMove(moves.substr(i, 1)));
            break;
        }

        // --- PRIME MOVE (e.g., R') ---
        if (moves[i + 1] == '\'') {
            intMoves.push_back(stoiMove(moves.substr(i, 2)));
            i += 1;
            continue;
        }

        // --- DOUBLE MOVE (e.g., R2) ---
        if (moves[i + 1] == '2') {
            intMoves.push_back(stoiMove(moves.substr(i, 2))); // "R2"
            i += 1;
            continue;
        }

        // --- WIDE MOVE (e.g., Rw, Rw', Rw2) ---
        if (moves[i + 1] == 'w') {
            // Check for Rw'
            if (i + 2 < moves.size() && moves[i + 2] == '\'')
            {
                intMoves.push_back(stoiMove(moves.substr(i, 3))); // "Rw'"
                i += 2;
                continue;
            }

            // Check for Rw2
            if (i + 2 < moves.size() && moves[i + 2] == '2') {
                intMoves.push_back(stoiMove(moves.substr(i, 3))); // "Rw2"
                i += 2;
                continue;
            }

            // Just "Rw"
            intMoves.push_back(stoiMove(moves.substr(i, 2)));
            i += 1;
            continue;
        }

        // --- NORMAL MOVE (e.g., R, U, F) ---
        intMoves.push_back(stoiMove(moves.substr(i, 1)));
    }
    return intMoves;
}
int Cube::stoiMove(const string move) {
    if (move == "U" || move == "u") return 0;
    if (move == "U'" || move == "u'") return 1;
    if (move == "U2" || move == "u2") return 2;
    if (move == "D" || move == "d") return 3;
    if (move == "D'" || move == "d'") return 4;
    if (move == "D2" || move == "d2") return 5;
    if (move == "F" || move == "f") return 6;
    if (move == "F'" || move == "f'") return 7;
    if (move == "F2" || move == "f2") return 8;
    if (move == "B" || move == "b") return 9;
    if (move == "B'" || move == "b'") return 10;
    if (move == "B2" || move == "b2") return 11;
    if (move == "R" || move == "r") return 12;
    if (move == "R'" || move == "r'") return 13;
    if (move == "R2" || move == "r2") return 14;
    if (move == "L" || move == "l") return 15;
    if (move == "L'" || move == "l'") return 16;
    if (move == "L2" || move == "l2") return 17;
    if (move == "Dw" || move == "dw") return 18;
    if (move == "Dw'" || move == "dw'") return 19;
    if (move == "Dw2" || move == "dw2") return 20;
    if (move == "Lw" || move == "lw") return 21;
    if (move == "Lw'" || move == "lw'") return 22;
    if (move == "Lw2" || move == "lw2") return 23;
    else return -1;
}

string Cube::itosMove(int move) {
    if (move == 0) return "U";
    if (move == 1) return "U'";
    if (move == 2) return "U2";
    if (move == 3) return "D";
    if (move == 4) return "D'";
    if (move == 5) return "D2";
    if (move == 6) return "F";
    if (move == 7) return "F'";
    if (move == 8) return "F2";
    if (move == 9) return "B";
    if (move == 10) return "B'";
    if (move == 11) return "B2";
    if (move == 12) return "R";
    if (move == 13) return "R'";
    if (move == 14) return "R2";
    if (move == 15) return "L";
    if (move == 16) return "L'";
    if (move == 17) return "L2";
    if (move == 18) return "Dw";
    if (move == 19) return "Dw'";
    if (move == 20) return "Dw2";
    if (move == 21) return "Lw";
    if (move == 22) return "Lw'";
    if (move == 23) return "Lw2";
    return "ERROR";

}
string Cube::itosMoves(vector<int> moves) {
    string result;
    for (int m : moves)
    {
        result += itosMove(m) + " ";
    }
    return result;
}
void Cube::undoMove(int move) {
    if (move % 3 == 0) // cw move
        applyMove(move + 1);
    else if (move % 3 == 1) // ccw move
        applyMove(move - 1);
    else  // double move
        applyMove(move);
}


bool Cube::isSolved() const {
    if (edgesSolved() && cornersSolved())
        return true;
    return false;
}
bool Cube::edgesSolved() const {
    for (int i = 0; i < 12; i++)
        if (edges[i].perm != i || edges[i].orient != 0)
            return false;
    return true;
}
bool Cube::cornersSolved() const {
    for (int i = 0; i < 8; i++)
        if (corners[i].perm != i || corners[i].orient != 0)
            return false;
    return true;
}
bool Cube::cornersPermuted() const {
    for (int i = 0; i < 8; i++)
        if (corners[i].perm != i)
            return false;
    return true;
}
bool Cube::edgesPermuted() const {
    for (int i = 0; i < 12; i++)
        if (edges[i].perm != i)
            return false;
    return true;
}