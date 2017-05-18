//Author: Dr. Freudenthal
//Contributions: Lucia Rodriguez
/** \file shapemotion.c
 *  \brief This is a simple shape motion demo.
 *  This demo creates two layers containing shapes.
 *  One layer contains a rectangle and the other a circle.
 *  While the CPU is running the green LED is on, and
 *  when the screen does not need to be redrawn the CPU
 *  is turned off along with the green LED.
 */  
#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include <abCircle.h>
#include <buzzer.h>

#define GREEN_LED BIT6

//to keep track of scores
unsigned int score1 = 0;
unsigned int score2 = 0;

AbRect rect10 = {abRectGetBounds, abRectCheck, {2 ,10}};/**< 10x10 rectangle */

AbRectOutline fieldOutline = {	/* playing field */
		abRectOutlineGetBounds, abRectOutlineCheck,   
		{screenWidth/2 - 10, screenHeight/2 - 10}
};

//ball
Layer layer3 = {		/**< Layer with an yellow circle */
		(AbShape *)&circle4,
		{(screenWidth/2)+10, (screenHeight/2)+5}, /**< bit below & right of center */
		{0,0}, {0,0},				    /* last & next pos */
		COLOR_VIOLET
};


Layer fieldLayer = {		/* playing field as a layer */
		(AbShape *) &fieldOutline,
		{screenWidth/2, screenHeight/2},/**< center */
		{0,0}, {0,0},				    /* last & next pos */
		COLOR_BLACK,
		&layer3
};

//player1
Layer layer1 = {		/**< Layer with a red palette */
		(AbShape *)&rect10,
		//{screenWidth/14, screenHeight/30},
		//{screenWidth-14, screenHeight-30},
		{14, 30}, /**< center */
		{0,0}, {0,0},				    /* last & next pos */
		COLOR_GREEN,
		&fieldLayer,
};

//player2
Layer layer0 = {		/**< Layer with a blue palette */
		(AbShape *)&rect10,
		//{screenWidth/14, screenHeight/30},
		{screenWidth-14, screenHeight-30}, /**< on the other side of the screen */
		{0,0}, {0,0},				    /* last & next pos */
		COLOR_BLUE,
		&layer1,
};
/** Moving Layer
 *  Linked list of layer references
 *  Velocity represents one iteration of change (direction & magnitude)
 */
typedef struct MovLayer_s {
	Layer *layer;
	Vec2 velocity;
	struct MovLayer_s *next;
} MovLayer;

/* initial value of {0,0} will be overwritten */
//Changed values
MovLayer ml3 = { &layer3, {3,3}, 0 }; /**< not all layers move */
MovLayer ml1 = { &layer1, {0,0}, &ml3 }; 
MovLayer ml0 = { &layer0, {0,0}, &ml1 }; 

void movLayerDraw(MovLayer *movLayers, Layer *layers)
{
	int row, col;
	MovLayer *movLayer;
	
	and_sr(~8);/**< disable interrupts (GIE off) */
	for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
		Layer *l = movLayer->layer;
		l->posLast = l->pos;
		l->pos = l->posNext;
	}
	or_sr(8);			/**< disable interrupts (GIE on) */
	

	for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
		Region bounds;
		layerGetBounds(movLayer->layer, &bounds);
		lcd_setArea(bounds.topLeft.axes[0], bounds.topLeft.axes[1], bounds.botRight.axes[0], bounds.botRight.axes[1]);
		for (row = bounds.topLeft.axes[1]; row <= bounds.botRight.axes[1]; row++) {
			for (col = bounds.topLeft.axes[0]; col <= bounds.botRight.axes[0]; col++) {
				Vec2 pixelPos = {col, row};
				u_int color = bgColor;
				Layer *probeLayer;
				for (probeLayer = layers; probeLayer; 
						probeLayer = probeLayer->next) { /* probe all layers, in order */
					if (abShapeCheck(probeLayer->abShape, &probeLayer->pos, &pixelPos)) {
						color = probeLayer->color;
						break; 
					} /* if probe check */
				} // for checking all layers at col, row
				lcd_writeColor(color); 
			} // for col
		} // for row
	} // for moving layer being updated
}	  
Region fence = {{10,30}, {SHORT_EDGE_PIXELS-10, LONG_EDGE_PIXELS-10}}; /**< Create a fence region */

/** Advance a moving shape within a fence  
 *  param ml: shape to advance. 
 *  param fence: Boundary for ml
 */

void mlAdvance(MovLayer *ml, Region *fence, Region *player1, Region *player2)
{
	Vec2 newPos;
	u_char axis;
	Region shapeBoundary;
	for (; ml; ml = ml->next) {
		vec2Add(&newPos, &ml->layer->posNext, &ml->velocity);
		abShapeGetBounds(ml->layer->abShape, &newPos, &shapeBoundary);
		for(axis = 0; axis < 2; axis++){
			if ((shapeBoundary.topLeft.axes[axis] < fence -> topLeft.axes[axis]) ||
					(shapeBoundary.botRight.axes[axis] > fence -> botRight.axes[axis])){
				int velocity = ml->velocity.axes[axis] = -ml->velocity.axes[axis];
				newPos.axes[axis] += (2*velocity);
			}/**< if outside of fence */
		}

		//my code
		if(shapeBoundary.topLeft.axes[0] < fence->topLeft.axes[0]){
			buzzer_init(1000);
			score2++;//updates score for player2
		}if(shapeBoundary.botRight.axes[0] > fence->botRight.axes[0]){
			buzzer_init(1000);
			score1++;//updates score for player1
		}else{
			buzzer_init(0);
		}

		//player1
		if ((shapeBoundary.topLeft.axes[0] < player1->botRight.axes[0]) && 
				(shapeBoundary.topLeft.axes[0] > player1->topLeft.axes[0])) {
			if((shapeBoundary.botRight.axes[1] >= player1->topLeft.axes[1]) && 
					(shapeBoundary.topLeft.axes[1] <= player1->botRight.axes[1])) {
				ml->velocity.axes[0] = -ml->velocity.axes[0];
			}
		}

		//player2
		if ((shapeBoundary.botRight.axes[0] > player2->topLeft.axes[0]) 
				&& (shapeBoundary.botRight.axes[0] < player2->botRight.axes[0])) {
			if((shapeBoundary.botRight.axes[1] >= player2->topLeft.axes[1]) 
					&& (shapeBoundary.topLeft.axes[1] <= player2->botRight.axes[1])) {
				ml->velocity.axes[0] = -ml->velocity.axes[0];
			}
		}
		ml->layer->posNext = newPos;
	} /**< for ml */
}

u_int bgColor = COLOR_BLACK;     /**< The BG color */
int redrawScreen = 1;           /**< Boolean for whether screen needs to be redrawn */

Region fieldFence;		/**< fence around playing field  */

//My code
//Green for player1
Region player1;
//Blue for player2
Region player2;       

/** Initializes everything, enables interrupts and green LED, 
 *  and handles the rendering for the screen
 */

void main()
{
	P1DIR |= GREEN_LED;		/**< Green led on when CPU on */		
	P1OUT |= GREEN_LED;

	configureClocks();
	lcd_init();
	shapeInit();
	p2sw_init(15);

	shapeInit();

	layerInit(&layer0);
	layerDraw(&layer0);
	layerGetBounds(&fieldLayer, &fieldFence);
	
	//my code
	//initialize layer for players with corresponding colors
	layerGetBounds(&layer1, &player1);//green          
	layerGetBounds(&layer0, &player2);//blue          

	enableWDTInterrupts();      /**< enable periodic interrupt */
	or_sr(0x8);	              /**< GIE (enable interrupts) */

	for(;;) { 
		while (!redrawScreen) { /**< Pause CPU if screen doesn't need updating */
			P1OUT &= ~GREEN_LED;    /**< Green led off witHo CPU */
			or_sr(0x10);	      /**< CPU OFF */
		}
		P1OUT ^= GREEN_LED;       /**< Green led on when CPU on */
		redrawScreen = 0;
		movLayerDraw(&ml0, &layer0);
	}
}

void wdt_c_handler()
{
	static short count = 0;
	P1OUT |= GREEN_LED;		      /**< Green LED on when cpu on */
	count ++;

	//my code
	u_int switches = p2sw_read(), i;
	
	char str[5];//for number of possible moves 
	if (count == 15) {
	  //my code
	  layerGetBounds(&layer1, &player1);//green
	  layerGetBounds(&layer0, &player2);//blue

		mlAdvance(&ml0, &fieldFence, &player1, &player2);
		
		for (i = 0; i < 4; i++)
		  //Ternary conditional operator ?
		  //it was giving me an error when using if/else so I researched
		  //and found out that a conditional operator works on expressions
		  //better than if/else statements
			str[i] = (switches & (1<<i)) ? 0: 1;
		
		//to move bars
		//button4 moves green down
		if(str[0]) {
			ml1.velocity.axes[1] = -5;
			ml1.velocity.axes[0] = 0;
		}
		//nothing happens when not toggled
		else{
			ml1.velocity.axes[1] = 0;
			ml1.velocity.axes[0] = 0; 
		}
		//button4 moves green up
		if(str[1]) {
			ml1.velocity.axes[1] = 5;
			ml1.velocity.axes[0] = 0;
		}
		//button3 moves blue down
		if(str[2]){
			ml0.velocity.axes[1] = -5;
			ml0.velocity.axes[0] = 0;
		}
		//nothing happens when not toggle
		else{
			ml0.velocity.axes[1] = 0;
			ml0.velocity.axes[0] = 0; 
		}
		//button43 moves blue up
		if(str[3]){
			ml0.velocity.axes[1] = 5;
			ml0.velocity.axes[0] = 0;
		}

		//compare scored to see whos winning
		//score1 = player1 = green
		if(score1 > score2){
		  //if player 1/green is winning displays message in color of player that wins
			drawString5x7(11, screenHeight - 8, "Player1 WINS", COLOR_GREEN, COLOR_BLACK); 
		}
		else if(score2 > score1){
		  //if player 2/blue is winning displays message in color of player that wins
			drawString5x7(11, screenHeight - 8, "Player2 WINS", COLOR_BLUE, COLOR_BLACK);
		}
		else{
		  //When both players have same score
			drawString5x7(11, screenHeight - 8, "BOTH WIN", COLOR_RED, COLOR_BLACK);
		}




		if (p2sw_read())
			redrawScreen = 1;
		count = 0;
	}
	P1OUT &= ~GREEN_LED;		    /**< Green LED off when cpu off */
}
