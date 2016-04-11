/*************************************************
**		Behnam Saeedi			**
**		   Saeedib			**
**		 932-217-697			**
**		 refactor.c			**
**	       cards selected:			**
**		 -Smithy			**
**		 -Adventurer			**
**		 -Feast				**
**		 -Village			**
**		 -minion			**
*************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "dominion.c"

int Smithy(int currentPlayer, int handPos, struct gameState state)
{
	// + 3 cards
	for(int i = 0; i < 3; i++)
	{
		drawCard(currentPlayer, state);
	}
	//discard card from hand
	discardCard(handPos, currentPlayer, state, 0);
	// return value is the value of getCost
	return 4;
}

int Adventurer(int currentPlayer, int handPos, struct gameState state)
{
	int drawntreasure = 0;
	int cardDrawn;
	int z =0;
	while(drawntreasure < 2)
	{
		if (state->deckCount[currentPlayer] <1)
		{
			shuffle(currentPlayer, state);
		}
		drawCard(currentPlayer, state);
		cardDrawn = state->hand[currentPlayer][state->handCount[currentPlayer]-1];
		if (cardDrawn == copper || cardDrawn == silver || cardDrawn == gold)
		{
			drawntreasure++;
		}else{
			temphand[z]=cardDrawn;
			state->handCount[currentPlayer]--;
			z++;
		}
	}
	while(z-1>=0)
	{
		state->discard[currentPlayer][state->discardCount[currentPlayer]++]=temphand[z-1];
		z=z-1;
	}
	//return value is the value of getCost()
	return 6;
}

int Feast(int currentPlayer, int handPos, struct gameState state, int * temphand)
{
	//Backup hand
	for (i = 0; i <= state->handCount[currentPlayer]; i++)
	{
		temphand[i] = state->hand[currentPlayer][i];//Backup card
		state->hand[currentPlayer][i] = -1;//Set to nothing
	}
	//Backup hand
	//Update Coins for Buy
	updateCoins(currentPlayer, state, 5);
	x = 1;//Condition to loop on
	while( x == 1)
	{
		if (state->coins < getCost(choice1))
		{
			printf("That card is too expensive!\n");
		}
		else
		{
			gainCard(choice1, state, 0, currentPlayer);//Gain the card
			x = 0;//No more buying cards
		}
	}
	//Reset Hand
	for (i = 0; i <= state->handCount[currentPlayer]; i++)
	{
		state->hand[currentPlayer][i] = temphand[i];
		temphand[i] = -1;
	}
	//Reset Hand
	return 4;
}

int Village(int currentPlayer, int handPos, struct gameState state)
{
	//+1 Card
	drawCard(currentPlayer, state);
	//+2 Actions
	state->numActions = state->numActions + 2;
	//discard played card from hand
	discardCard(handPos, currentPlayer, state, 0);
	return 3;
}

int Minion(int currentPlayer, int handPos, struct gameState state,int choice1)
{
	//+1 action
	state->numActions++;
	//discard card from hand
	discardCard(handPos, currentPlayer, state, 0);
	if (choice1)		//+2 coins
	{
		state->coins = state->coins + 2;
	}
	else
	if (choice2)		//discard hand, redraw 4, other players with 5+ cards discard hand and draw 4
	{
		//discard hand
		while(numHandCards(state) > 0)
		{
			discardCard(handPos, currentPlayer, state, 0);
		}
		//draw 4
		for (i = 0; i < 4; i++)
		{
			drawCard(currentPlayer, state);
		}
		//other players discard hand and redraw if hand size > 4
		for (i = 0; i < state->numPlayers; i++)
		{
			if (i != currentPlayer)
			{
				if ( state->handCount[i] > 4 )
				{
					//discard hand
					while( state->handCount[i] > 0 )
					{
						discardCard(handPos, i, state, 0);
					}
					//draw 4
					for (j = 0; j < 4; j++)
					{
						drawCard(i, state);
					}
				}
			}
		}
	}
	return 5;
}
