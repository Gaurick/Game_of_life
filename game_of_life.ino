


#include "LedControl.h"
//library for the display.

LedControl lc=LedControl(12, 10, 11, 1);
//set the pins that the led matrix will use.
//dataPin, clkPin, csPin, numDevices)

int startArray[102];
//initial array in game of life.
int finishArray[102];
//end array that displays in game of life.

int aliveCounter = 0;
//counter to track living neighbors.
int arrayCounter = 0;
//counter to track position in the array for lighting up the led's.
long delayCounter = 0;
//counter to work as a delay for everything.
int repeatCounter = 0;
//counter to make sure that the game doesn't get stuck on one screen.

void setup() {
  Serial.begin(9600);
  //startup the serial input/output.
  randomSeed(analogRead(0));
  //start the random numbers with noise on pin 0.
  
  lc.shutdown(0, false);
  //wakeup the display.
  lc.setIntensity(0, 1);
  //set the brightness to the lowest value.
  lc.clearDisplay(0); 
  //clear the display.
  
  for(int a = 0; a <= 102; a++){
    startArray[a] = random(2);
    //seed random data in the start array.
  }
  

  for(int b = 0; b <= 102; b++){
    finishArray[b] = 0;
    //clear the end array.
  }
}

void loop() {
  if(delayCounter == 100000){
    //use a delay counter since delay caused issues and it runs too fast to see.
    delayCounter = 0;
    wholeProgram();
  }

  else{
    //if it's not time to do the program stuff, then continue counting.
    delayCounter++;
  }
}

void wholeProgram(){
  //testRow function tests each cell to see if it lives or it dies for the next generation.
  //values pushing to the function are the start and end location of the cells per row.
  //do that because there needs to be a row of nul data around the area.
  testRow(12, 19);
  testRow(22, 29);
  testRow(32, 39);
  testRow(42, 49);
  testRow(52, 59);
  testRow(62, 69);
  testRow(72, 79);
  testRow(82, 89);

  //now that we know who lives and dies, turn the lights on for those who live,
  //off for those that die.
  //lightLeds function takes the start and end location of the rows.
  //also sends the row number since the led matrix needs it differently.
  lightLeds(12, 19, 0);
  lightLeds(22, 29, 1);
  lightLeds(32, 39, 2);
  lightLeds(42, 49, 3);
  lightLeds(52, 59, 4);
  lightLeds(62, 69, 5);
  lightLeds(72, 79, 6);
  lightLeds(82, 89, 7);

  //function checks to see if it's repeating, if so reset the startArray.
  repeatChecker();
}

void copyArray(int first, int last){
  //function to copy the data from the finishArray to the startArray so it can go through again.
  //also clears the finishArray to be ready for the 
  //loops ftw.
  for(first; first <= last; first++){
    startArray[first] = finishArray[first];
    finishArray[first] = 0;
  }
}

void testRow(int startPosition, int endPosition){
  //test each of the 8 neighbors of each of cell, then apply the Game of Life rules
  //to see if it lives or not.
  for(startPosition; startPosition <= endPosition; startPosition++){
    testCellLife(startPosition, -11);
    //upper left.
    testCellLife(startPosition, -10);
    //top.
    testCellLife(startPosition, -9);
    //upper right.
    testCellLife(startPosition, -1);
    //left.
    testCellLife(startPosition, 1);
    //right.
    testCellLife(startPosition, 9);
    //lower left.
    testCellLife(startPosition, 10);
    //bottom.
    testCellLife(startPosition, 11);
    //bottom right.

    if(aliveCounter == 2){
      //if the cell is living it will keep living if it has 2 neighbors.
      if(startArray[startPosition] == 1){
        finishArray[startPosition] = 1;
      }
    }

    else if(aliveCounter == 3){
      //a living cell with 3 living neighbors and/or a dead cell with 3 living neighbors
      //lives.
      finishArray[startPosition] = 1;
    }

    else{
      //otherwise the cell dies.
      finishArray[startPosition] = 0;
    }

    aliveCounter = 0;
    
  }
}

void testCellLife(int cell, int locMod){
  //actual function to test a cell's life.
  int z = cell + locMod;
  if(startArray[z] == 1){
    aliveCounter++;
  }
}

void lightLeds(int start, int finish, int row){
  //function to light up the led's in the matrix if the cell is alive,
  //turn the led off if the cell is dead.
  for(start; start <= finish; start++){
    lc.setLed(0, row, arrayCounter, finishArray[start]);
    arrayCounter++;
  }

  arrayCounter = 0;
}

void repeatChecker(){
  //check to make sure that the game isn't stuck in a repeating loop.
  //resets only if it is stuck at the same screen.
  //need bigger brains to make something to check for a non-static repeat loop thing.
  for(int c = 0; c <= 102; c++){
    if(startArray[c] == finishArray[c]){
      repeatCounter++;
    }
  }

  if(repeatCounter == 102){
    //if it's stuck then re-seed the startArray with random data to make it do stuff.
    for(int a = 0; a <= 102; a++){
      startArray[a] = random(2);
    }
  }

  else{
    //if it's not stuck then copy the data from the finishArray to the startArray,
    //so it can loop and go through again.
    copyArray(12, 19);
    copyArray(22, 29);
    copyArray(32, 39);
    copyArray(42, 49);
    copyArray(52, 59);
    copyArray(62, 69);
    copyArray(72, 79);
    copyArray(82, 89);
  }

  for(int b = 0; b <= 102; b++){
    finishArray[b] = 0;
    //clear the end array.
  }

  repeatCounter = 0;
  //also don't forget to reset the counter.
}
