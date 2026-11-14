#ifndef CUBE_H
#define CUBE_H

#include <vector>
#include <string>
#include <set>

struct Edge 
{
    int perm;
    int orient;
    std::vector<char> colors;
    bool has(char c)
    {
        if (colors[0] == c || colors[1] == c)
            return true;
        return false;
    }
};

struct Corner 
{
    int perm;
    int orient;
    std::vector<char> colors;
    bool has(char c)
    {
        if (colors[0] == c || colors[1] == c || colors[2])
            return true;
        return false;
    }
};

class Solver;
class SolverIDA;
class Cube 
{
    friend class Solver;
    friend class SolverIDA;
private:
    
    template<typename T>
    static void cycleCW(T& a, T& b, T& c, T& d) // by default ccw
    {
        T temp = a;
        a = d;
        d = c;
        c = b;
        b = temp;
    }
    template<typename T>
    static void cycleCCW(T& a, T& b, T& c, T& d) // by default ccw
    {
        T temp = a;
        a = b;
        b = c;
        c = d;
        d = temp;
    }
    
    static void orientCorners(Corner& a, Corner& b, Corner& c, Corner& d)
    {
        a.orient = (a.orient + 1) % 3;
        b.orient = (b.orient + 2) % 3;
        c.orient = (c.orient + 1) % 3;
        d.orient = (d.orient + 2) % 3;
    }

    static void flipEdges(Edge& a, Edge& b, Edge& c, Edge& d)
    {
        a.orient ^= 1;
        b.orient ^= 1;
        c.orient ^= 1;
        d.orient ^= 1;
    }

public:
    Edge edges[12];
    Corner corners[8];  
    Cube();
    std::string generateScramble();
    void setState(std::vector<std::vector<char>>);
    void scramble(std::string moves);
    void applyMove(const int& move);
    void applyMoves(const std::vector<int>& moves);

    bool isSolved() const;
    bool edgesSolved() const;
    bool cornersSolved() const;
    bool edgesPermuted() const;
    bool cornersPermuted() const;

    std::vector<int> stoiMoves(const std::string moves);
    int stoiMove(std::string move);
    std::string itosMoves(std::vector<int> moves);
    std::string itosMove(int move);
    void undoMove(int move);
    bool sameAxis(int, int);
    bool conflictsWide(int, int);
    
};

#endif