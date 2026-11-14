import { Group, Tween, Easing } from '@tweenjs/tween.js';
import * as THREE from 'three';
//import data from './data.json';
"use strict";
export class Cube {
    constructor(scene, cubeType) {
        this.cubeType = cubeType;
        this.group = new Group();
        this.scene = scene;
        this.moveSpeed = 250;
        this.corners = [];
        this.edges = [];
        this.centers = [];
        this.scramble = "";
        this.solution = ""; 
    }
    

    simplifyMoves(moves) { // moves is a string
        let arrMoves = moves ? moves.trim().split(/\s+/) : []; // converts to array
        if (arrMoves.length == 0) return [];
        const stack = [arrMoves[0]];
        for (let i = 1; i < arrMoves.length; i ++) {
            let top = stack[stack.length-1];
            let next =  arrMoves[i];
            if (top && top[0] == arrMoves[i][0]) {
                if ((top[1] != 'w' && next[1] != 'w') || 
                    (top[1] == 'w' && next[1] == 'w')) { 
                    if ((top[top.length-1] == 2 && next[next.length-1] == 2) || 
                        (top[top.length-1] != 2 && next[next.length-1] !=2 && top != next)) { // both are doubles or opposites
                            stack.pop();
                            continue;
                    }
                    else if (top == next) {
                        stack.pop();
                        top[top.length-1] == '\'' ? top = top.substr(0, top.length-1) + '2' : top += "2";
                        stack.push(top);
                        continue;
                    }
                    else if (next[next.length-1] == 2) { // fixes next being double and top being single
                        stack.pop();
                        top[top.length-1] == '\'' ? top = top.substr(0, top.length-1) : top += "'";
                        stack.push(top);
                        continue;
                    }
                    else if (top[top.length-1] == 2) { // fixes top being double and next being single
                        stack.pop();
                        next[next.length-1] == '\'' ? next = next.substr(0, next.length-1) : next += "'";
                        stack.push(next);
                        continue;
                    }
                }
            }
            stack.push(arrMoves[i]);
            
        }
        
        return stack;
    }

    async solve() {
        for (let i = 0; i < this.solution.length; i++){
            if (this.solution[i])
                await this.applyMove(this.solution[i]);
        }
    }
    async applyScramble() {
        for (let i = 0; i < this.scramble.length; i++){
            if (this.scramble[i])
                await this.applyMove(this.scramble[i]);
            
        }
    }
    
    applyMove(move) {
        // pieces[0,3] are corners, pieces[4,7] are edges, pieces[8] is the center
        // pieces are the pieces in the face that are being turned
        
            let pieces = [];
            let gDir, lDir, axis;

            // gDir is the direction the animation needs (global), while lDir is for the cube cycle (local to the face)
            // corners are mapped clockwise starting at lower right corner, edged clockwise starting at lower edge
            if      (move == "U") pieces = [0, 4, 5, 1, 4, 8, 5, 0, 1], gDir = "ccw", lDir = "cw", axis = 'y';
            else if (move == "U'") pieces = [0, 4, 5, 1, 4, 8, 5, 0, 1], gDir = "cw", lDir = "ccw", axis = 'y';
            else if (move == "U2") pieces = [0, 4, 5, 1, 4, 8, 5, 0, 1], gDir = "2", lDir = "2", axis = 'y';
            else if (move == "D") pieces = [3, 7, 6, 2, 7, 11, 6, 3, 4], gDir = "cw", lDir = "cw", axis = 'y';
            else if (move == "D'") pieces = [3, 7, 6, 2, 7, 11, 6, 3, 4], gDir = "ccw", lDir = "ccw", axis = 'y';
            else if (move == "D2") pieces = [3, 7, 6, 2, 7, 11, 6, 3, 4], gDir = "2", lDir = "2", axis = 'y';
            else if (move == "F") pieces = [2, 6, 4, 0, 6, 9, 4, 1, 2], gDir = "ccw", lDir = "cw", axis = 'z';
            else if (move == "F'") pieces = [2, 6, 4, 0, 6, 9, 4, 1, 2], gDir = "cw", lDir = "ccw", axis = 'z';
            else if (move == "F2") pieces = [2, 6, 4, 0, 6, 9, 4, 1, 2], gDir = "2", lDir = "2", axis = 'z';
            else if (move == "B") pieces = [7, 3, 1, 5, 7, 2, 5, 10, 3], gDir = "cw", lDir = "cw", axis = 'z';
            else if (move == "B'") pieces = [7, 3, 1, 5, 7, 2, 5, 10, 3], gDir = "ccw", lDir = "ccw", axis = 'z';
            else if (move == "B2") pieces = [7, 3, 1, 5, 7, 2, 5, 10, 3], gDir = "2", lDir = "2", axis = 'z';
            else if (move == "R") pieces = [3, 2, 0, 1, 3, 1, 0, 2, 0], gDir = "ccw", lDir = "cw", axis = 'x';
            else if (move == "R'") pieces = [3, 2, 0, 1, 3, 1, 0, 2, 0], gDir = "cw", lDir = "ccw", axis = 'x';
            else if (move == "R2") pieces = [3, 2, 0, 1, 3, 1, 0, 2, 0], gDir = "2", lDir = "2", axis = 'x';
            else if (move == "L") pieces = [6, 7, 5, 4, 11, 10, 8, 9, 5], gDir = "cw", lDir = "cw", axis = 'x';
            else if (move == "L'") pieces = [6, 7, 5, 4, 11, 10, 8, 9, 5], gDir = "ccw", lDir = "ccw", axis = 'x';
            else if (move == "L2") pieces = [6, 7, 5, 4, 11, 10, 8, 9, 5], gDir = "2", lDir = "2", axis = 'x';
            else if (move == "Dw") pieces = [3, 7, 6, 2, 7, 11, 6, 3, 4, 2, 10, 9, 1, 3, 5, 2, 0], gDir = "cw", lDir = "cw", axis = 'y';
            else if (move == "Dw'") pieces = [3, 7, 6, 2, 7, 11, 6, 3, 4, 2, 10, 9, 1, 3, 5, 2, 0], gDir = "ccw", lDir = "ccw", axis = 'y';
            else if (move == "Dw2") pieces = [3, 7, 6, 2, 7, 11, 6, 3, 4, 2, 10, 9, 1, 3, 5, 2, 0], gDir = "2", lDir = "2", axis = 'y';
            else if (move == "Lw") pieces = [6, 7, 5, 4, 11, 10, 8, 9, 5, 6, 7, 5, 4, 4, 3, 1, 2], gDir = "cw", lDir = "cw", axis = 'x';
            else if (move == "Lw'") pieces = [6, 7, 5, 4, 11, 10, 8, 9, 5, 6, 7, 5, 4, 4, 3, 1, 2], gDir = "ccw", lDir = "ccw", axis = 'x';
            else if (move == "Lw2") pieces = [6, 7, 5, 4, 11, 10, 8, 9, 5, 6, 7, 5, 4, 4, 3, 1, 2], gDir = "2", lDir = "2", axis = 'x';
            else 
                return Promise.resolve();

            return this.animateMove(pieces, gDir, lDir, axis);
            
    }

    animateMove(pieces, gDir, lDir, axis) {
        return new Promise((resolve) => {
            const face = new THREE.Group();
            this.scene.add(face);
            let rad, dur = this.moveSpeed;
            if (gDir == "cw") rad = Math.PI/2;
            else if (gDir == "ccw") rad = -Math.PI/2;
            else rad = -Math.PI, dur *= 2;

            // attaching every piece to the face
            for (let i = 0; i < 4; i++) {
                face.attach(this.corners[pieces[i]]);
                face.attach(this.edges[pieces[i+4]]);
                if (pieces.length == 17) {
                    face.attach(this.edges[pieces[i+9]]);
                    face.attach(this.centers[pieces[i+13]]);
                }
            }
            face.attach(this.centers[pieces[8]]);

            const coords = {angle: 0};

            new Tween(coords, this.group).to({angle : rad}, dur) 
            .easing(Easing.Quadratic.InOut)
            .onUpdate(() => {
                //console.log("Tween is actively running!", coords.angle); 
                face.rotation[axis] = coords.angle;
            })
            .onComplete(() => { // reattaches to the scene to keep updated location of the piece
                for (let i = 0; i < 4; i++) {
                    this.scene.attach(this.corners[pieces[i]]);
                    this.scene.attach(this.edges[pieces[i+4]]);
                    if (pieces.length == 17) {
                        this.scene.attach(this.edges[pieces[i+9]]);
                        this.scene.attach(this.centers[pieces[i+13]]);
                    }
                }
                this.scene.attach(this.centers[pieces[8]]);

                this.scene.remove(face);

                if (lDir == "cw")
                    this.cycleCW(pieces)
                else if (lDir == "ccw")
                    this.cycleCCW(pieces);
                else {
                    this.cycleCW(pieces);
                    this.cycleCW(pieces);
                }
                resolve();
            })
            .start();
        });
    }
    

    cycleCW(pieces) {
        // permuting corners
        [this.corners[pieces[0]], this.corners[pieces[1]], this.corners[pieces[2]], this.corners[pieces[3]]] = 
        [this.corners[pieces[3]], this.corners[pieces[0]], this.corners[pieces[1]], this.corners[pieces[2]]];

        [this.edges[pieces[4]], this.edges[pieces[5]], this.edges[pieces[6]], this.edges[pieces[7]]] = 
        [this.edges[pieces[7]], this.edges[pieces[4]], this.edges[pieces[5]], this.edges[pieces[6]]];

        if (pieces.length == 17) {
            [this.edges[pieces[9]], this.edges[pieces[10]], this.edges[pieces[11]], this.edges[pieces[12]]] = 
            [this.edges[pieces[12]], this.edges[pieces[9]], this.edges[pieces[10]], this.edges[pieces[11]]];

            [this.centers[pieces[13]], this.centers[pieces[14]], this.centers[pieces[15]], this.centers[pieces[16]]] = 
            [this.centers[pieces[16]], this.centers[pieces[13]], this.centers[pieces[14]], this.centers[pieces[15]]];
        }
    }

    cycleCCW(pieces) {
        // permuting corners
        [this.corners[pieces[0]], this.corners[pieces[1]], this.corners[pieces[2]], this.corners[pieces[3]]] = 
        [this.corners[pieces[1]], this.corners[pieces[2]], this.corners[pieces[3]], this.corners[pieces[0]]];

        [this.edges[pieces[4]], this.edges[pieces[5]], this.edges[pieces[6]], this.edges[pieces[7]]] = 
        [this.edges[pieces[5]], this.edges[pieces[6]], this.edges[pieces[7]], this.edges[pieces[4]]];

        if (pieces.length == 17) {
            [this.edges[pieces[9]], this.edges[pieces[10]], this.edges[pieces[11]], this.edges[pieces[12]]] = 
            [this.edges[pieces[10]], this.edges[pieces[11]], this.edges[pieces[12]], this.edges[pieces[9]]];

            [this.centers[pieces[13]], this.centers[pieces[14]], this.centers[pieces[15]], this.centers[pieces[16]]] = 
            [this.centers[pieces[14]], this.centers[pieces[15]], this.centers[pieces[16]], this.centers[pieces[13]]];
        }
    }
    
};