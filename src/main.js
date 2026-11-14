import * as THREE from 'three';
import { Group, Tween, Easing } from '@tweenjs/tween.js';
//import data from './data.json';
import { Cube } from './cube.js';
import './style.css';
import { renderCube } from './cubeRenderer.js'

const solverUrl = new URL(`${import.meta.env.BASE_URL}solver.js`, window.location.origin).href;
const { default: createSolverModule } = await import(/* @vite-ignore */ solverUrl);

const solver = await createSolverModule();

// Scene
const scene = new THREE.Scene();
scene.background = new THREE.Color(0x222222);

// Camera
const camera = new THREE.PerspectiveCamera(
    75,
    window.innerWidth / window.innerHeight,
    0.1,
    1000
);

camera.position.set(4,4,6);
camera.lookAt(0,0,0);

// Renderer
const renderer = new THREE.WebGLRenderer({ antialias: true });
renderer.setSize(window.innerWidth, window.innerHeight);
document.body.style.margin = '0';
document.body.appendChild(renderer.domElement);

// create cube
const cubeType = "colored";
const cube = new Cube(scene, cubeType);

renderCube(cube);



// const block = new THREE.Mesh(new THREE.BoxGeometry(.54,.69,.85), new THREE.MeshBasicMaterial({color : 0xffffff}));
// block.position.set(0,0,0);

// scene.add(block);
// const group = new Group();
// const tween = new Tween(block.position, group).to({x:-4, y:4, z:4}, 6000)
//   .easing(Easing.Quadratic.InOut)
//   .start();



// stalling helper
const wait = (ms) => new Promise((resolve) => setTimeout(resolve, ms));
const solveBtn = document.getElementById("solveBtn");
const scrambleInput = document.getElementById("scrambleInput");
const moveSpeedInput = document.getElementById("moveSpeedInput");
const generateScrambleBtn = document.getElementById("generateScrambleBtn");
const scrambleText = document.getElementById("scrambleText");
const moveSpeedText = document.getElementById("moveSpeedText");
moveSpeedText.textContent = cube.moveSpeed;

//const solveBtn = document.getElementById("solveBtn");

let isTurning = false;
let scrambled = false;
// npm run build:wasm for shortcut when modifying cpp files

scrambleInput.addEventListener('keydown', (event) => {
    if (event.key == 'Enter') {
      const newScramble = event.target.value;
      event.target.value = "";
      const customData = JSON.parse(solver.solveCustomScramble(newScramble));
      cube.scramble = cube.simplifyMoves(customData.scramble);
      cube.solution = cube.simplifyMoves(customData.solution);
      if (!isTurning) {
        if (!scrambled) {
          scrambled = true;
          scrambleCube();
          
          solveBtn.textContent = "Solve";
        }
        else {
          solveCube();
          scrambled = false;
          //scrambleBtn.textContent = "Scramble";
        }
      }
    }
});

moveSpeedInput.addEventListener('keydown', (event) => {
    if (event.key == 'Enter') {
      if (!isTurning) {
        cube.moveSpeed = event.target.value;
        event.target.value = "";
        moveSpeedText.textContent = cube.moveSpeed;
      }
    }
});

generateScrambleBtn.addEventListener("click", () => {
      const customData = JSON.parse(solver.solveRandomScramble());
      cube.scramble = cube.simplifyMoves(customData.scramble);
      cube.solution = cube.simplifyMoves(customData.solution);
      if (!isTurning) {
        if (!scrambled) {
          scrambled = true;
          scrambleCube();
          
          
          //scrambleBtn.textContent = "Solve";
        }
        else {
          solveCube();
          scrambled = false;
          //scrambleBtn.textContent = "Scramble";
        }
      }
});

solveBtn.addEventListener("click", () => {
  if (!isTurning) {
      solveCube();
      scrambled = false;
      //scrambleBtn.textContent = "Scramble";
    
  }
});


// cube.moveSpeed = 100;
async function scrambleCube() {
  isTurning = true;
  for (const move of cube.scramble) {
    
    await cube.applyMove(move);
    scrambleText.textContent = cube.scramble;
  }   
  isTurning = false;
}
async function solveCube() {
  isTurning = true;
  for (const move of cube.solution) {
    await cube.applyMove(move);
  }   
  isTurning = false;
}


// Resize handling
window.addEventListener('resize', () => {
    camera.aspect = window.innerWidth / window.innerHeight;
    camera.updateProjectionMatrix();

    renderer.setSize(window.innerWidth, window.innerHeight);
});

function animate(time) {
  requestAnimationFrame(animate);
  cube.group.update(time);
  renderer.render(scene, camera);
}
requestAnimationFrame(animate);

