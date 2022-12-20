const canvas = document.getElementById('canvas');
var ctx = canvas.getContext("2d");

// Align the canvas to the window size and create a grid of cells
function alignToWindow() {
    // Get the width and height of the canvas element
    canvasWidth = canvas.offsetWidth;
    canvasHeight = canvas.offsetHeight;

    // Calculate the size of the grid and the individual cells
    gridSize = 24; // Set the number of cells in the grid
    cellSize = canvasWidth > canvasHeight ? (canvasHeight * 0.8) / gridSize : (canvasWidth * 0.8) / gridSize;

    // Calculate the aspect ratio of the canvas
    aspectRatio = canvasWidth / canvasHeight;

    // Set the width and height of the canvas to the size of the window
    canvas.width = window.innerWidth;
    canvas.height = window.innerHeight;

    // Calculate the coordinates of the top-left corner of the grid
    startX = canvasWidth / 2 - (cellSize * gridSize) / 2;
    startY = canvasHeight / 2 - (cellSize * gridSize) / 2;

    // Create the grid
    createGrid();
}

function createGrid() {
    // Fill the canvas with a solid color
    ctx.fillRect(0, 0, canvas.width, canvas.height);

    // Set the stroke style and line width for the grid lines
    ctx.strokeStyle = '#FFFFFF';
    ctx.lineWidth = 2;

    // Begin a new path for the grid lines
    ctx.beginPath();

    // Loop through the cells and draw the vertical and horizontal lines
    for (let i = 0; i <= 24; i++) {
        ctx.moveTo(startX + i * cellSize, startY);
        ctx.lineTo(startX + i * cellSize, startY + gridSize * cellSize);

        ctx.moveTo(startX, startY + i * cellSize);
        ctx.lineTo(startX + gridSize * cellSize, startY + i * cellSize);
    }

    // Stroke the path to draw the grid lines on the canvas
    ctx.stroke();
}


let isDragging = false;
let startColumn = -1;
let startRow = -1;

canvas.addEventListener('mousedown', event => {
    // Calculate the column and row of the starting cell for the drag operation
    startColumn = Math.floor((event.offsetX - startX) / cellSize);
    startRow = Math.floor((event.offsetY - startY) / cellSize);

    isDragging = true; // Set the dragging flag to true
});

canvas.addEventListener('mousemove', event => {
    if (!isDragging) return; // Do nothing if the mouse is not being dragged

    // Calculate the column and row of the current cell
    const column = Math.floor((event.offsetX - startX) / cellSize);
    const row = Math.floor((event.offsetY - startY) / cellSize);

    // Print the column and row to the console
    console.log(`Column: ${column}, Row: ${row}`);
});

canvas.addEventListener('mouseup', event => {
    isDragging = false; // Set the dragging flag to false
});

// Listen for window resize events and align the canvas and grid to the new window size
window.addEventListener('resize', alignToWindow);

window.addEventListener('load', alignToWindow);