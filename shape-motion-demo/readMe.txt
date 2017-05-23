
Author: Lucia Rodriguez

Pong Game

Description:
This is a pong game that allows two players to play with each other. There are several buttons that will be directed to each of the two players. 

Player1 which display with a Green padlle:
Moves up with switch1
Moves down with switch2

Play2 which is display with a Blue paddle:
Moves up with switch3
Moves down with switch4

The game is supposed to make a sound whenever the ball touches any of the paddles. Unfortunately I was not able to implement that 
feature since it was giving me alot of errors when running.

I was not able to get my jump table to work in assemble language. But here you can see you I translated from c to assembly

.data 
state: .byte 0

.text 
Jump: 
.word case0 
.word case1 
.word case2

case0: 
MOV ___, r12 
CALL #buzzer_init, 
MOV.b #1, &state JMP DONE 
case1: 
MOV ___, r12 
CALL #buzzer_init, 
MOV.b #2, &state JMP DONE 
case2: 
MOV ___, r12 
CALL #buzzer_init, 
MOV.b #2, &state JMP DONE

default: 
MOV #0, r12 
CALL #buzzer_init

DONE: 
pop R0

Contributors:
Dr. Freudenthal, I used a functions that were provided by Dr. Freudenthal
Shammir Ibarra, we helped each other with the logic of my game and his game. We also shared 
some code that helped us complete our assignment. 
  in shapemotion.c file functions mlAdvance
  
  In this part of the readme I will explain the collaborations I had for a few lines of code that were implemented. Such as the part of code that was written by my as well as the part of code that was written by my classmate Shammir Ibarra.
For this lab, Dr. Freudenthal provided us with sample code and demos that will be helpful for the completion of the lab. The files “buzzer.c”, “buzzer.h”, “Shapemotion.c” and “Makefile” were files that were provided by the instructor. I contributed to his program by making small modifications to the files, and by adding functions and variables to the file shapemotion.c.
Shapemotion.c
First I started by adding two variables to keep track of the scores. This will help determine which player is winning when playing the pong game. This part of code you can find on lines 24-25.
Next, I modified the code given for the layers. This part of code was written by Dr. Freudenthal and I just modified the values of some of the variables that he had provided to make the output as how I wanted it. I chose to have the ball for the game to be red. This you can find in line 39. Where I declare the color red for that layer. I also modified layers 1 and 0 which correspond to the paddles that will be displayed for the two different players. In line 55 and 56 (for player 1) and in line 66 (for player 2) you can see how I was trying to make my paddles work a different way, which was not working. After looking at the code that was given by Dr. Freudenthal I was able to kind of understand it kind of worked.
The code that was implemented from line 76 to line 123 was the code that was given by Dr. Freudenthal.
Next comes the function “void mlAdvance(MovLayer *ml, Region *fence, Region *player1, Region *player2)” initially this function was given by Dr. Freudenthal, but I added some modifications to allow me to take into account the paddles for the players. This will allows me to determine which player was winning when and to move them in the screen. It was explained that this function was used to advance a moving shape within a fence. The code implemented from line 132 to line 144 was given by Dr. Freudenthal. Line 147-155 was written by me. In this part of code I use to determine which player wins. It will set a boundary to the screen and it will compare the left and right axes. Every time the ball touches the boundary on the left it will increment the score for player two which is located in the opposite side. Here it should also play a sound everytime the ball touches the boundary. Unfortunately the function for songs is not working. For the following lines of code, I worked with my classmate Shammir Ibarra to set the boundaries and velocity for the paddles of the players. 
Next comes the function “void main()” this function was also given by Dr. Freudenthal. The only modification I made to this function was to initialize the layers for the players. This you can find in lines 221-222. In the previous line I noticed how Dr. Freudenthal was initializing the fence layer by calling the function layerGetBounds (which was implemented by Dr. Freudenthal) and passing the layer that needed to be initialize and the Region variable that I was representing. I had to previously create two Region variables for player1 and for player2 to be able to properly initialize it. Once the variables had been created I called the function layerGetBounds two times, one time for player1 and another time for player2; passing the required parameters (layer1 and player1 for player1 and layer0 and player2 for player 2). 
Next comes the function wdt_c_handles() which was also provided by Dr. Freudenthal along with a file that contain its assemble translation. In this function I had to add a variable to allow me to initialize the input of when a switch was being pressed. I also added a str array variable to allow me to store the information for the possible switches (moves). Inside the if statements that was provided by Dr. Freudenthal I had to recall the function that was previously called in the main function due to errors. I thought that since it had already been called and therefore initialize in the main function I would have no problems. Here is where Shammir Ibarra also helped me. He advised me to do the function call there as well. I was so desperate to finish at this point that I didn’t really asked him for an explanation. All I wanted was my code to work because the deadline was already close. 

Francisco Landa, Roman Martinez we worked together with the assembly translation. We all got together and studied and 
translated a jump table that we had in common.

