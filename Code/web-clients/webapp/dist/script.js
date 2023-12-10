const cssProperties = new Map ([
  ["colsn", "--mapNoOfCol"],
  ["rowsn", "--mapNoOfRows"],
  ["cellColor", "--mapElemBg"],
  [`cellHover`, '--mapElemHover'],
  ["pX", "--pointerLeft"],
  ["pY", "--pointerTop"],
  ["pShadow", "--pointerShadow"],
  ["turnRight", "--turnRightColor"],
  ["turnLeft", "--turnLeftColor"]
]);
const pShadow = {active: "#73C2FB", sel: "#e3242b"};
const classNames = new Map ([
  ["cell", "map-cell"],
])
const IDs = new Map ([
  ["map", "parent"],
  ["mouse", 'mouse-pos'],
  ["pointer-pos", 'cell-txt'],
  ["form", "form"],
  ["send", "send-coords"]
])
const cells = new Map ([
  ["(0, 0)", "p 1"],
  ["(1, 0)", "p 2"],
  ["(0, 1)", "p 3"],
  ["(1, 1)", "p 4"],
  ["(0, 2)", "p 5"],
  ["(1, 2)", "p 6"],
  ["(0, 3)", "p 7"],
  ["(1, 3)", "p 8"]
])

/* Set the mode of our interface */
var selecting = true;

const pointer = document.getElementById("pointer");
const root = document.querySelector(':root');
var grid; //Map
const main = document.getElementById("main");

const form = document.getElementById("form");
const log = document.getElementById("log");
let mousePos = { x: undefined, y: undefined };
let pointerPos = { x: undefined, y: undefined };
let selCell = { x: undefined, y: undefined };

/* Helper Functions */

/* root: has all the constants of our css code */
class Root{
  // Get a variable's value
  static get(propName) {
    var rs = getComputedStyle(root);
    return rs.getPropertyValue(propName);
  }
  // Set a variable's value
  static set(propName, value) {
    root.style.setProperty(propName, value);
  }
}

/* Compute coordinates of elements in the window */
class Coords{
  /* Find the absolute position of the element el */
  static getOffset(el) {
    const rect = el.getBoundingClientRect();
    return {left: rect.left + window.scrollX, top: rect.top + window.scrollY};
  }
  
  static getSize(el){
    const rect = el.getBoundingClientRect();
    return {width: rect.width, height: rect.height};
  }
  
}

/* Grid and Pointer */

/* Table that contains the map */
class Grid {
  /*  (0,0) (1,0) (2,0) ... (cols - 1, 0)
  *   (0,1) (1,1) (2,1) ... (cols - 1, 1)
  *    ...                    ...
  *   (0,rows - 1) (1,rows - 1) (2,rows - 1) ... (cols - 1, rows - 1) 
  */
  constructor(){
    this.rows = Root.get(cssProperties.get("rowsn"));
    this.cols = Root.get(cssProperties.get("colsn"));
    
    
    this.parent = document.getElementById(IDs.get("map"));
    this.top = Coords.getOffset(this.parent).top;
    this.left = Coords.getOffset(this.parent).left;
    this.pos = Coords.getOffset(this.parent);
  }
  
  getParent(){return this.parent;}
  getCols(){return this.cols;}
  getRows(){return this.rows;}
  getPos(){return this.pos;}
  setPos(pos){this.pos = pos;}
  
  pointerCoords(){
    let gridSize = Coords.getSize(this.parent);
     // Find cell position
    if(mousePos.x == 0 || mousePos.y == 0 || mousePos.x == gridSize.width || mousePos.y == gridSize.height){
       return { x: -1, y: -1};
    } else {
      let cellwidth = gridSize.width / this.cols;
      let cellheight = gridSize.height / this.rows;
      let pointerSize = Coords.getSize(pointer);
      let pos_x = this.pos.left + mousePos.x - (mousePos.x % cellwidth) + cellwidth/2 - pointerSize.width/2;
      let pos_y = this.pos.top + mousePos.y - (mousePos.y % cellheight) + cellheight/2 - pointerSize.height/2;
      return { x: pos_x, y: pos_y};
    }
  }
  
  selCell(){
    let gridSize = Coords.getSize(this.parent);
     // Find cell position
    if(mousePos.x == 0 || mousePos.y == 0 || mousePos.x == gridSize.width || mousePos.y == gridSize.height){
       return { x: -1, y: -1};
    } else {
      let cellwidth = gridSize.width / this.cols;
      let cellheight = gridSize.height / this.rows;
      let i_x = parseInt((pointerPos.x - this.pos.left) /cellwidth);
      let i_y = parseInt((pointerPos.y - this.pos.top) /cellheight);
      return { x: i_x, y: i_y};
    }
  }
 
}

function drawPointer(){
  if(selecting && pointerPos.x != -1 && pointerPos.y != -1){
    let left = pointerPos.x + "px";
    let top = pointerPos.y + "px";
    Root.set(cssProperties.get("pX"), left);
    Root.set(cssProperties.get("pY"), top);
    pointer.setAttribute("class", "pulse sel");
    Root.set(cssProperties.get("pShadow"), pShadow.sel);
  } else if(!selecting){
    pointer.setAttribute("class", "pulse act");
    Root.set(cssProperties.get("pShadow"), pShadow.active);
  } else {
    pointer.setAttribute("class", "none");
  }
}

/* Control panel style */

function mouseOverLeft(){
  Root.set(cssProperties.get("turnLeft"), Root.get(cssProperties.get("cellHover")));
}

function mouseOutLeft(){
  Root.set(cssProperties.get("turnLeft"), "white");
}

function mouseOverRight(){
  Root.set(cssProperties.get("turnRight"), Root.get(cssProperties.get("cellHover")));
}

function mouseOutRight(){
  Root.set(cssProperties.get("turnRight"), "white");
}

/* Communication with the server */

function button_clicked(id) {
    const ip = document.getElementById("ip-txt").value;
    var message = {};
    
    message[id] = true

    const url = "http://" + ip + "/commands"
    postData(url, message)
}

function position_clicked() {
    if(selecting) {
      
        const ip = document.getElementById("ip-txt").value;
        var message = {};
      
        message["position"] = cells.get(`(${selCell.x}, ${selCell.y})`);
      
        const url = "http://" + ip + "/commands"
        postData(url, message)
    }

}

function postData(url = '', data = {}) {
    var xhr = new XMLHttpRequest();
    xhr.open("POST", url);
    xhr.send(JSON.stringify(data));
    console.log("Done POST: " + data + " " + JSON.stringify(data));
}

/* Events and Event Handlers */

window.onload = (event) => {
  grid = new Grid();
};

window.addEventListener('mousemove', (event) => {
  
  /* Find position of mouse within the map */
  let mainPos = Coords.getOffset(main);
  let xp = Math.max(0, Math.min(event.clientX - grid.getPos().left, Coords.getSize(grid.getParent()).width));
  let yp = Math.max(0, Math.min(event.clientY - grid.getPos().top, Coords.getSize(grid.getParent()).height));
  
  /* Update mousePos */
  mousePos = { x: xp, y: yp };
  
  if(selecting){
    /* Update the pointer */
    let p = grid.pointerCoords();
    pointerPos ={ x: p.x, y: p.y};
    /* Update the selected cell */
    p = grid.selCell();
    selCell ={ x: p.x, y: p.y};
    drawPointer();
    
    /* Change the coords we send to the server */
    //changeURL();
  }
  
});

window.addEventListener('click', (event) => {
  if(selecting && pointerPos.x % pointerPos.x != -1 && pointerPos.y != -1){
    selecting = false;
    drawPointer();
    Root.set(cssProperties.get('cellHover'), '#ccc');
    Root.get()
    /* Send messages to server */
  } else {
    selecting = true;
    drawPointer();
    Root.set(cssProperties.get('cellHover'), '#ffa500');
  }
});