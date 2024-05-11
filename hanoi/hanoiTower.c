#include "primlib.h"
#include <unistd.h>

#define DISCS_NUMBER 5
#define PEGS_NUMBER 3
#define DISTANCE_BETWEEN_PEGS (gfx_screenWidth() / (PEGS_NUMBER + 1))
#define DISC_HEIGHT (gfx_screenHeight() / (2 * DISCS_NUMBER))
#define DISC_WIDTH_INCREMENT (gfx_screenWidth() / (2 * PEGS_NUMBER * DISCS_NUMBER))
#define FLOOR_HEIGHT (9 * gfx_screenHeight() / 10)
#define PEG_HEIGHT (3 * gfx_screenHeight() / 5)
#define PEG_WIDTH (DISC_WIDTH_INCREMENT / 2)
#define MOVING_HEIGHT (gfx_screenHeight() / 5)
#define SLEEP 10000
#define ZERO 48
#define ZERO_INTO_TEN 10
#define ASCII_INTO_NUMBER 49
#define CYAN 4
#define MAGENTA 5
#define DISC_SPEED 10

void initLiblary()
{
	if(gfx_init()) 
	{
		exit(3);
	}
}

void exitGame()
{
	if(gfx_isKeyDown(SDLK_ESCAPE))
	{
		exit(0);
	}
}

void drawBackGroung()
{
	gfx_filledRect(0, 0, gfx_screenWidth(), gfx_screenHeight(), WHITE);
	gfx_filledRect(0, FLOOR_HEIGHT, gfx_screenWidth(), gfx_screenHeight(), BLACK);
	for (int i = 1; i <= PEGS_NUMBER; i++)
	{
		gfx_filledRect(i * DISTANCE_BETWEEN_PEGS, FLOOR_HEIGHT - PEG_HEIGHT,
		i * DISTANCE_BETWEEN_PEGS + PEG_WIDTH, FLOOR_HEIGHT, BLACK);
	}
}

int setColour(int disc)
{
	if (disc % 2 == 0)
	{
		return CYAN;
	}
	return MAGENTA;
}

void drawDiscs(int array[PEGS_NUMBER][DISCS_NUMBER])
{
	for (int j = 0; j < PEGS_NUMBER; j++)
	{
		for (int i = 0; i < DISCS_NUMBER; i++)
		{
			if (array[j][i] != '\0')
			{
				gfx_filledRect(
				(j + 1) * DISTANCE_BETWEEN_PEGS + PEG_WIDTH / 2 - (DISC_WIDTH_INCREMENT * array[j][i]) / 2,
				FLOOR_HEIGHT - DISC_HEIGHT * (i + 1),
				(j + 1) * DISTANCE_BETWEEN_PEGS + PEG_WIDTH / 2 + (DISC_WIDTH_INCREMENT * array[j][i]) / 2,
				FLOOR_HEIGHT - DISC_HEIGHT * i,
				setColour(i));
			}
		}
	}
}

int checkLengthOfArray(int array[])
{
	int len = 0;
	for (int i = 0; i < DISCS_NUMBER; i++)
	{
		if (array[i] != '\0')
		{
			len ++;
		}
	}
	return len;
}

int changeInputIntoPegNumber(int key)
{
	if(key == ZERO)
	{
		key += ZERO_INTO_TEN;
	}
	return (key - ASCII_INTO_NUMBER);
}

int checkIfPegIsNotEmpty(int array[])
{
	if (checkLengthOfArray(array) != 0)
	{
		return 1;
	}
	return 0;
}

int setKeyFrom(int array[PEGS_NUMBER][DISCS_NUMBER])
{
	int from;
	do{
		from = changeInputIntoPegNumber(gfx_getkey());
		exitGame();
	}while(!(from >= 0 && from < PEGS_NUMBER && checkIfPegIsNotEmpty(array[from])));
	return from;
}

int setKeyTo(int array[PEGS_NUMBER][DISCS_NUMBER], int discSize)
{
	int to;
	do
	{
		to = changeInputIntoPegNumber(gfx_getkey());
		exitGame();
	}while(!(to >= 0 && to < PEGS_NUMBER &&
	(checkLengthOfArray(array[to]) == 0 ||
	array[to][checkLengthOfArray(array[to]) - 1] > discSize)));
	return to;
}

void moveDiscUp(int current_height, int size, int array[PEGS_NUMBER][DISCS_NUMBER], int from)
{
	int posY = FLOOR_HEIGHT - DISC_HEIGHT * current_height;
	while(posY > MOVING_HEIGHT)
	{
		exitGame();
		drawBackGroung();
		drawDiscs(array);
		gfx_filledRect(
		(from + 1) * DISTANCE_BETWEEN_PEGS + PEG_WIDTH / 2 - (DISC_WIDTH_INCREMENT * size) / 2,
		posY - DISC_HEIGHT,
		(from + 1) * DISTANCE_BETWEEN_PEGS + PEG_WIDTH / 2 + (DISC_WIDTH_INCREMENT * size) / 2,
		posY,
		setColour(current_height));
		gfx_updateScreen();
		usleep(SLEEP);
		posY -= DISC_SPEED;
	}
}

void moveLeftRight(int pos, int size,int colour, int array[PEGS_NUMBER][DISCS_NUMBER])
{
	exitGame();
	drawBackGroung();
	drawDiscs(array);
	gfx_filledRect(
	pos + PEG_WIDTH / 2 - (DISC_WIDTH_INCREMENT * size) / 2,
	MOVING_HEIGHT - DISC_HEIGHT,
	pos + PEG_WIDTH / 2 + (DISC_WIDTH_INCREMENT * size) / 2,
	MOVING_HEIGHT,
	setColour(colour));
	gfx_updateScreen();
	usleep(SLEEP);
}

void moveDiscSideway(int from, int to, int size,int colour, int array[PEGS_NUMBER][DISCS_NUMBER])
{
	int posX = DISTANCE_BETWEEN_PEGS * (from + 1);
	if(from > to)
	{
		while(posX > DISTANCE_BETWEEN_PEGS * (to + 1))
		{
			moveLeftRight(posX ,size, colour, array);
			posX -= DISC_SPEED;
		}
	}else
	{
		while(posX < DISTANCE_BETWEEN_PEGS * (to + 1))
		{
			moveLeftRight(posX, size, colour, array);
			posX += DISC_SPEED;
		}
	}
}

void moveDiscDown(int to, int size, int end_height,int array[PEGS_NUMBER][DISCS_NUMBER])
{
	int posY = MOVING_HEIGHT;
	while(posY < FLOOR_HEIGHT - DISC_HEIGHT * end_height)
	{
		exitGame();
		drawBackGroung();
		drawDiscs(array);
		gfx_filledRect(
		(to + 1) * DISTANCE_BETWEEN_PEGS + PEG_WIDTH / 2 - (DISC_WIDTH_INCREMENT * size) / 2,
		posY - DISC_HEIGHT,
		(to + 1) * DISTANCE_BETWEEN_PEGS + PEG_WIDTH / 2 + (DISC_WIDTH_INCREMENT * size) / 2,
		posY,
		setColour(end_height));
		gfx_updateScreen();
		usleep(SLEEP);
		posY += DISC_SPEED;
	}
}

void changePositionOfDiscs(int array[PEGS_NUMBER][DISCS_NUMBER])
{
	int keyFrom = setKeyFrom(array);
	int pickedDisc = array[keyFrom][(checkLengthOfArray(array[keyFrom]) - 1)];
	array[keyFrom][(checkLengthOfArray(array[keyFrom]) - 1)] = '\0';
	moveDiscUp(checkLengthOfArray(array[keyFrom]), pickedDisc, array, keyFrom);
	int keyTo = setKeyTo(array, pickedDisc);
	moveDiscSideway(keyFrom, keyTo, pickedDisc, checkLengthOfArray(array[keyFrom]), array);
	moveDiscDown(keyTo, pickedDisc, checkLengthOfArray(array[keyTo]), array);
	array[keyTo][checkLengthOfArray(array[keyTo])] = pickedDisc;
}

void setUpArray(int array[PEGS_NUMBER][DISCS_NUMBER])
{
	for (int i = 0; i < PEGS_NUMBER; i++)
	{
		for(int j = 0; j < DISCS_NUMBER; j++)
		{
			array[i][j] = '\0';
			if(i == 0)
			{
				array[0][j] = DISCS_NUMBER - j;
			}
		}
	}
}

int playAndWin(int array[PEGS_NUMBER][DISCS_NUMBER], int win)
{
	if(!win)
		{	
			gfx_updateScreen();
			changePositionOfDiscs(array);
			if(array[PEGS_NUMBER - 1][DISCS_NUMBER - 1] == 1)
			{
				return 1;
			}
		}else
		{
			gfx_textout(gfx_screenWidth()/2, (FLOOR_HEIGHT + gfx_screenHeight()) / 2, "CONGRATS", RED);
			gfx_updateScreen();
			return 1;
		}
		return 0;
}

int main(int argc, char* argv[])
{
	initLiblary();
	int win = 0;
	int pegs[PEGS_NUMBER][DISCS_NUMBER];
	setUpArray(pegs);
	
	while(1)
	{
		exitGame();
		drawBackGroung();
		drawDiscs(pegs);
		win = playAndWin(pegs, win);
		usleep(SLEEP);
	}
	return 0;
}
