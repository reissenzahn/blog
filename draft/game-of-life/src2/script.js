
class Pixel {

  constructor(alive) {
    this.alive = alive;
    
    this.lifeStyle = '#000000';
    this.deathStyle = '#FFFFFF';
    
    this.underpopulation = 2;
    this.overpopulation = 3;
    this.reproductionMin = 3;
    this.reproductionMax = 3;

    this.neighbors = [];
    this.nextState = null;
    this.previousState = null;
    this.forceRepaint = true;
  }

  prepareUpdate() {
    let sum = 0;
    let nextState = this.alive;

    for(let n of this.neighbors){
      if(n.alive && n !== this) sum++;
    }

    if(nextState && sum < this.underpopulation){
      nextState = false;
    }
    else if(nextState && sum > this.overpopulation) {
      nextState = false;
    }
    else if(!nextState && sum >= this.reproductionMin && sum <= this.reproductionMax) {
      nextState = true;
    }

    this.nextState = nextState;
  }

  update() {
    this.previousState = this.alive;
    this.alive = this.nextState;
    this.nextState = null;
  }

  handleClick() {
    this.alive = true;
  }

  setPaintStyles(canvasCtx) {
    canvasCtx.fillStyle = this.alive ? this.lifeStyle : this.deathStyle;
  }
}


class Simulator {

  constructor(rows, cols, pixelSize, interRoundDelay, initialChanceOfLife) {
    this.rows = rows;
    this.cols = cols;
    this.pixelSize = pixelSize;
    this.interRoundDelay = interRoundDelay;
    this.mouseIsDown = false;
    this.paused = false;
    this.intervalId = null;
    this.grid = [];
    this.initialChanceOfLife = initialChanceOfLife;

    // add pixels to the grid
    for (let i = 0; i < this.rows; i++) {
      this.grid.push([]);
      for (let j = 0; j < this.cols; j++) {
        const alive = Math.random() < this.initialChanceOfLife;
        this.grid[i].push(new Pixel(alive));
      }
    }

    // set the appropriate neighbors for each pixel
    for (let i = 0; i < this.rows; i++) {
      for (let j = 0; j < this.cols; j++) {

        this.grid[i][j].neighbors = this.getNeighbors(i, j);
      }
    }

    
    // setup canvas
    this.canvas = document.createElement('canvas');
    this.canvas.width = this.pixelSize * this.cols;
    this.canvas.height = this.pixelSize * this.rows;
    this.canvasCtx = this.canvas.getContext('2d', { alpha: false});
    
    this.registerMouseListeners();
  }

  getNeighbors(row, col) {
    const neighbors = [];
    for (let i = row - 1; i <= row + 1; i++) {
      for (let j = col - 1; j <= col + 1; j++) {
        if (i === row && j === col)
         continue;
        if (this.grid[i] && this.grid[i][j]) {
            neighbors.push(this.grid[i][j]);
        }
      }
    }
    return neighbors;
  }
  
  start() {
    if (this.intervalId) {
      return;
    }

    this.intervalId = setInterval(() => {
      this.advanceRound();
      this.repaint();
    }, this.interRoundDelay);
  }

  stop() {
    if (this.intervalId) {
      clearInterval(this.intervalId);
      this.intervalId = null;
    }
  }

  advanceRound() {
    if(this.mouseIsDown)
      return;

    for (let i = 0; i < this.rows; i++) {
      for (let j = 0; j < this.cols; j++) {
        this.grid[i][j].prepareUpdate();
      }
    }

    for (let i = 0; i < this.rows; i++) {
      for (let j = 0; j < this.cols; j++) {
        this.grid[i][j].update();
      }
    }
  }

  repaint(force = false) {
    if(this.mouseIsDown && !force) return;

    // Canvas optimization -- it's faster to paint by color than placement.
    let byColor = {};
    for(let i = 0; i < this.rows; i++) {
      for(let j = 0; j < this.cols; j++) {
        let pixel = this.grid[i][j];

        if(!force && !pixel.forceRepaint && pixel.alive === pixel.previousState){
          continue; // No need to repaint if the pixel didn't change
        }

        let color = pixel.alive ? pixel.lifeStyle : pixel.deathStyle;
        if(byColor[color] === undefined) {
          byColor[color] = []
        }

        byColor[color].push([i, j]);
        pixel.forceRepaint = false; // Once a pixel is painted, reset it's forced state
      }
    }

    for(let color in byColor) {
      this.canvasCtx.fillStyle = color;
      for(let [row, col] of byColor[color]){
        this.canvasCtx.fillRect(
          col * this.pixelSize,
          row * this.pixelSize,
          this.pixelSize,
          this.pixelSize
        );
      }
    }
  }

  paintPixel(row, col) {
    this.grid[row][col].setPaintStyles(this.canvasCtx);
    this.canvasCtx.fillRect(
      col * this.pixelSize,
      row * this.pixelSize,
      this.pixelSize,
      this.pixelSize
    );
  }

  resetLife(chanceOfLife) {
    this.grid.forEach((row) => {
      row.forEach((pixel) => {
        pixel.previousState = pixel.alive;
        pixel.alive = Math.random() < chanceOfLife;
      });
    });

    this.repaint();
  }

  registerMouseListeners() {
    bindMultipleEventListener(this.canvas, ['mousemove', 'touchmove'], (e) =>  {
      e.preventDefault();
      if(this.mouseIsDown) {
        let x, y;
        if(e.touches) {
          let rect = e.target.getBoundingClientRect();
          x = Math.floor((e.touches[0].pageX - rect.left) / this.pixelSize);
          y = Math.floor((e.touches[0].pageY - rect.top) / this.pixelSize);
        }
        else {
          x = Math.floor(e.offsetX / this.pixelSize);
          y = Math.floor(e.offsetY / this.pixelSize);
        }

        this.grid[y][x].handleClick();
        this.paintPixel(y, x);
      }
    });

    // Capture mouse state for click and drag features
    bindMultipleEventListener(this.canvas, ['mousedown', 'touchstart'], (e) => {
      e.preventDefault();
      let rect = e.target.getBoundingClientRect();
      let x, y;
      if(e.touches) {
        let rect = e.target.getBoundingClientRect();
        x = Math.floor((e.touches[0].pageX - rect.left) / this.pixelSize);
        y = Math.floor((e.touches[0].pageY - rect.top) / this.pixelSize);
      }
      else {
        x = Math.floor(e.offsetX / this.pixelSize);
        y = Math.floor(e.offsetY / this.pixelSize);
      }

      this.grid[y][x].handleClick();
      this.paintPixel(y, x);
      this.mouseIsDown = true;
    });

    bindMultipleEventListener(this.canvas, ['mouseup', 'touchend'], (e) => {
      e.preventDefault();
      this.mouseIsDown = false;
    });
  }
}

function bindMultipleEventListener(element, eventNames, f) {
  eventNames.forEach((eventName) => {
    element.addEventListener(eventName, f);
  });
}
