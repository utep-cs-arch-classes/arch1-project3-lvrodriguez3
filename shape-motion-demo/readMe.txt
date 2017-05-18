
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
Francisco Landa, Roman Martinez

