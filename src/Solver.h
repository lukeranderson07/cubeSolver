#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <string>
#include "Cube.h"



class Solver 
{
    private:
        Cube cube;
        std::string mode; 

    public:
        Solver(const Cube& c);
        
        void setMode(std::string m);
        std::string solveEdges();
        std::string solveCorners();
        std::string solveCube();
        void displayState();
        std::string swapEdge(Edge& e);
        std::string swapCorner(Corner& e);
        char getEdgeLetter(Edge e);
        char getCornerLetter(Corner c);
        std::string edgeTrigger();
        std::string cornerTrigger();
};

#endif 