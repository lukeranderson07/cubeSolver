//import { Cube } from './cube.js';
import * as THREE from 'three';


const colors = {
    white: 0xffffff,
    yellow: 0xffff00,
    red: 0xff0000,
    orange: 0xff8000,
    blue: 0x0000ff,
    green: 0x00ff00,
    black: 0x111111
};

export function renderCube(cube) {
    for (let x = 1; x >= -1; x--) {
        for (let y = 1; y >= -1; y--) {
            for (let z = 1; z >= -1; z--) {
                if (cube.cubeType == "colored")
                    createColoredPiece(x, y, z, cube);
                else if (cube.cubeType == "mirror")
                    createMirrorPiece(x, y, z, cube);
                }
            }
        }         
}

function createColoredPiece(x, y, z, cube) {
    const geometry = new THREE.BoxGeometry(1, 1, 1);
    const materials = [
        new THREE.MeshBasicMaterial({ color : x == 1 ? colors.red : colors.black}),     // right
        new THREE.MeshBasicMaterial({ color : x == -1 ? colors.orange : colors.black}),   // left
        new THREE.MeshBasicMaterial({ color : y == 1 ? colors.white : colors.black}),   // top
        new THREE.MeshBasicMaterial({ color : y == -1 ? colors.yellow : colors.black}),   // bottom
        new THREE.MeshBasicMaterial({ color : z == 1 ? colors.green : colors.black}),  // front
        new THREE.MeshBasicMaterial({ color : z == -1 ? colors.blue : colors.black}),   // back
    ];
    
    const piece = new THREE.Mesh(geometry, materials);
    const eGeometry = new THREE.BoxGeometry();
    const eMaterial = new THREE.MeshBasicMaterial({color: colors.black});
    const edges = new THREE.EdgesGeometry(eGeometry);
    const line = new THREE.LineSegments(edges, eMaterial); 
    line.position.set(x,y,z);
    
    cube.scene.add(line);
    piece.position.set(x,y,z);
    cube.scene.add(piece);
    piece.attach(line);
    const type = Math.abs(x) + Math.abs(y) + Math.abs(z);
    if (type == 3)
      cube.corners.push(piece);
    else if (type == 2)
      cube.edges.push(piece);
    else if (type == 1)
      cube.centers.push(piece);
}

function createMirrorPiece(px, py, pz, cube) {
  const scalar = 1;
  
  let dx = scalar, dy = scalar, dz = scalar;
  if (px == 1)
    dx = .25*scalar;
  else if (px == -1)
    dx = 1*scalar;
  if (py == 1)
    dy = .45*scalar;
  else if (py == -1)
    dy = .85*scalar;
  if (pz == 1)
    dz = .6*scalar;
  else if (pz == -1)
    dz = .7*scalar;
  
  let ox = (scalar - dx) / 2;
  let oy = (scalar - dy) / 2;
  let oz = (scalar - dz) / 2;
  if (px == -1) ox *= -1;
  if (py == -1) oy *= -1;
  if (pz == -1) oz *= -1;
  const geometry = new THREE.BoxGeometry(dx, dy, dz);
  const material = new THREE.MeshBasicMaterial({ color : colors.white})
  
  const piece = new THREE.Mesh(geometry, material);
  const eGeometry = new THREE.BoxGeometry(dx, dy, dz);
  const eMaterial = new THREE.MeshBasicMaterial({color: colors.black});
  const edges = new THREE.EdgesGeometry(eGeometry);
  const line = new THREE.LineSegments(edges, eMaterial); 
  line.position.set(px-ox,py-oy,pz-oz);
  
  cube.scene.add(line);
  piece.position.set(px-ox,py-oy,pz-oz);
 
  cube.scene.add(piece);
  piece.attach(line);

  const type = Math.abs(px) + Math.abs(py) + Math.abs(pz);
  if (type == 3)
      cube.corners.push(piece);
  else if (type == 2)
      cube.edges.push(piece);
  else if (type == 1)
      cube.centers.push(piece);
}


