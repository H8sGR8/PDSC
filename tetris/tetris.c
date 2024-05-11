#include "primlib.h"
#include <unistd.h>
#include <time.h>

#define SQUERE_SIZE 30
#define WIDTH_SQUERE_NUMBER 12
#define HEIGHT_SQUERE_NUMBER 21
#define CORDS_AND_COLOUR 3
#define GAME_SPACE_WIDTH (SQUERE_SIZE * WIDTH_SQUERE_NUMBER)
#define GAME_SPACE_HEIGHT (SQUERE_SIZE * HEIGHT_SQUERE_NUMBER)
#define NEXT_AND_HOLD_HEIGHT 7
#define NEXT_AND_HOLD_WIDTH 7
#define DISTANCE 100
#define GAME_SPACE_Y_POSITION 50
#define GAME_SPACE_X_POSITION (DISTANCE * 2 + NEXT_AND_HOLD_WIDTH * SQUERE_SIZE)
#define HOLD_X_POSITION DISTANCE
#define NEXT_X_POSITION (GAME_SPACE_X_POSITION + SQUERE_SIZE * WIDTH_SQUERE_NUMBER + DISTANCE)
#define PIECES_PARTS 4
#define NEXT_AND_HOLD_PIECE_X_POSITION 3
#define NEXT_AND_HOLD_PIECE_Y_POSITION 4
#define ROTATION_1 0
#define ROTATION_2 1
#define ROTATION_3 2
#define ROTATION_4 3
#define T_PIECE 0
#define S_PIECE 1
#define Z_PIECE 2
#define L_PIECE 3
#define I_PIECE 4
#define J_PIECE 5
#define O_PIECE 6
#define NUMBER_OF_PIECES 7
#define START_X 5
#define START_Y 0
#define POINTS_FOR_FULL_ROW 20
#define DECIMAL_POINTS 100
#define SLEEP 100000

void initLiblary()
{
	if(gfx_init()) 
	{
		exit(3);
	}
}

void drawPiecePart(int posX, int posY, int colour)
{
	gfx_filledRect(posX, posY, posX + SQUERE_SIZE, posY - SQUERE_SIZE, colour);
}


void drawHorizontalLines(int height, int width, int posX, int posY)
{
	for(int i = 1; i < height; i++)
	{
		gfx_line( posX, posY + i * SQUERE_SIZE,
		posX + SQUERE_SIZE * width, posY + i * SQUERE_SIZE, BLACK);
	}
}

void drawVerticalLines(int height, int width, int posX, int posY)
{
	for(int i = 1; i < width; i++)
	{
		gfx_line( posX + i  * SQUERE_SIZE, posY,
		posX + i  * SQUERE_SIZE, posY + SQUERE_SIZE * height, BLACK);
	}
}

void drawBlackSpace(int height, int width, int array[height][width], int posX, int posY)
{
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			drawPiecePart(posX + j * SQUERE_SIZE, posY + (i + 1) * SQUERE_SIZE, array[i][j]);
		}
	}
	drawHorizontalLines(height, width, posX, posY);
	drawVerticalLines(height, width, posX, posY);
}

void drawBoard(int board[HEIGHT_SQUERE_NUMBER][WIDTH_SQUERE_NUMBER],
int hold[NEXT_AND_HOLD_HEIGHT][NEXT_AND_HOLD_WIDTH], int next[NEXT_AND_HOLD_HEIGHT][NEXT_AND_HOLD_WIDTH], int points)
{
	char spoints[15];
	gfx_filledRect(0, 0, gfx_screenWidth(), gfx_screenHeight(), BLACK);
	drawBlackSpace(NEXT_AND_HOLD_HEIGHT, NEXT_AND_HOLD_WIDTH, hold, HOLD_X_POSITION, GAME_SPACE_Y_POSITION);
	drawBlackSpace(HEIGHT_SQUERE_NUMBER, WIDTH_SQUERE_NUMBER, board, GAME_SPACE_X_POSITION, GAME_SPACE_Y_POSITION);
	drawBlackSpace(NEXT_AND_HOLD_HEIGHT, NEXT_AND_HOLD_WIDTH, next, NEXT_X_POSITION, GAME_SPACE_Y_POSITION);
	sprintf(spoints, "POINTS: %d", points);
	gfx_textout(DISTANCE, gfx_screenHeight() / 2, spoints, RED);
}
int checkIfIPieceCanBeDrawn(int posX, int posY, int rotation, int array[HEIGHT_SQUERE_NUMBER][WIDTH_SQUERE_NUMBER])
{
	rotation = (rotation % 2);
	if(rotation == ROTATION_1)
	{
		for(int i = 0; i < PIECES_PARTS; i++)
		{
			if(array[posY - i][posX] != WHITE)
			{
				return 0;
			}
		}
	}
	else if(rotation == ROTATION_2)
	{
		for(int i = 0; i < PIECES_PARTS; i++)
		{
			if(array[posY][posX - i] != WHITE)
			{
				return 0;
			}
		}
	}
	return 1;
}
void drawIPiece(int posX, int posY, int rotation, int height, int width, int array[height][width], int colour)
{
	rotation = (rotation % 2);
	if(rotation == ROTATION_1)
	{
		for(int i = 0; i < PIECES_PARTS; i++)
		{
			array[posY - i][posX] = colour;
		}
	}
	else if(rotation == ROTATION_2)
	{
		for(int i = 0; i < PIECES_PARTS; i++)
		{
			array[posY][posX - i] = colour;
		}
	}
}

int checkIfOPieceCanBeDrawn(int posX, int posY, int rotation, int array[HEIGHT_SQUERE_NUMBER][WIDTH_SQUERE_NUMBER])
{
	rotation = (rotation % 1);
	if(rotation == ROTATION_1)
	{
		for(int i = 0; i < PIECES_PARTS / 2; i++)
		{
			if(array[posY - i][posX] != WHITE || array[posY - i][posX + 1] != WHITE)
			{
				return 0;
			}
		}
	}
	return 1;
}

void drawOPiece(int posX, int posY, int rotation, int height, int width, int array[height][width], int colour)
{
	rotation = (rotation % 1);
	if(rotation == ROTATION_1)
	{
		for(int i = 0; i < PIECES_PARTS / 2; i++)
		{
			array[posY - i][posX] = colour;
			array[posY - i][posX + 1] = colour;
		}
	}
}

int checkIfSPieceCanBeDrawn(int posX, int posY, int rotation, int array[HEIGHT_SQUERE_NUMBER][WIDTH_SQUERE_NUMBER])
{
	rotation = (rotation % 2);
	if(rotation == ROTATION_1)
	{
		for(int i = 0; i < PIECES_PARTS / 2; i++)
		{
			if(array[posY][posX + i -1] != WHITE || array[posY - 1][posX + i] != WHITE)
			{
				return 0;
			}
		}
	}
	else if(rotation == ROTATION_2)
	{
		for(int i = 0; i < PIECES_PARTS / 2; i++)
		{
			if(array[posY - i - 1][posX] != WHITE || array[posY - i][posX + 1] != WHITE)
			{
				return 0;
			}
		}
	}
	return 1;
}

void drawSPiece(int posX, int posY, int rotation, int height, int width, int array[height][width], int colour)
{
	rotation = (rotation % 2);
	if(rotation == ROTATION_1)
	{
		for(int i = 0; i < PIECES_PARTS / 2; i++)
		{
			array[posY][posX + i - 1] = colour;
			array[posY - 1][posX + i] = colour;
		}
	}
	else if(rotation == ROTATION_2)
	{
		for(int i = 0; i < PIECES_PARTS / 2; i++)
		{
			array[posY - i - 1][posX] = colour;
			array[posY - i][posX + 1] = colour;
		}
	}
}

int checkIfZPieceCanBeDrawn(int posX, int posY, int rotation, int array[HEIGHT_SQUERE_NUMBER][WIDTH_SQUERE_NUMBER])
{
	rotation = (rotation % 2);
	if(rotation == ROTATION_1)
	{
		for(int i = 0; i < PIECES_PARTS / 2; i++)
		{
			if(array[posY][posX + i] != WHITE || array[posY - 1][posX + i - 1] != WHITE)
			{
				return 0;
			}
		}
	}
	else if(rotation == ROTATION_2)
	{
		for(int i = 0; i < PIECES_PARTS / 2; i++)
		{
			if(array[posY - i][posX] != WHITE || array[posY - i - 1][posX + 1] != WHITE)
			{
				return 0;
			}
		}
	}
	return 1;
}

void drawZPiece(int posX, int posY, int rotation, int height, int width, int array[height][width], int colour)
{
	rotation = (rotation % 2);
	if(rotation == ROTATION_1)
	{
		for(int i = 0; i < PIECES_PARTS / 2; i++)
		{
			array[posY][posX + i] = colour;
			array[posY - 1][posX + i - 1] = colour;
		}
	}
	else if(rotation == ROTATION_2)
	{
		for(int i = 0; i < PIECES_PARTS / 2; i++)
		{
			array[posY - i][posX] = colour;
			array[posY - i - 1][posX + 1] = colour;
		}
	}
}

int checkIfLPieceCanBeDrawn(int posX, int posY, int rotation, int array[HEIGHT_SQUERE_NUMBER][WIDTH_SQUERE_NUMBER])
{
	rotation = (rotation % 4);
	if(rotation == ROTATION_1)
	{
		for(int i = 0; i < PIECES_PARTS - 1; i++)
		{
			if(array[posY - i][posX] != WHITE || array[posY][posX + 1] != WHITE)
			{
				return 0;
			}
		}
	}
	else if(rotation == ROTATION_2)
	{
		for(int i = -1; i < PIECES_PARTS / 2; i++)
		{
			if(array[posY - 1][posX + i] != WHITE || array[posY][posX - 1] != WHITE)
			{
				return 0;
			}
		}
	}
	else if(rotation == ROTATION_3)
	{
		for(int i = 0; i < PIECES_PARTS - 1; i++)
		{
			if(array[posY - i][posX + 1] != WHITE || array[posY - 2][posX] != WHITE)
			{
				return 0;
			}
		}
	}
	else if(rotation == ROTATION_4)
	{
		for(int i = -1; i < PIECES_PARTS / 2; i++)
		{
			if(array[posY][posX + i] != WHITE || array[posY - 1][posX + 1] != WHITE)
			{
				return 0;
			}
		}
	}
	return 1;
}

void drawLPiece(int posX, int posY, int rotation, int height, int width, int array[height][width], int colour)
{
	rotation = (rotation % 4);
	if(rotation == ROTATION_1)
	{
		for(int i = 0; i < PIECES_PARTS - 1; i++)
		{
			array[posY - i][posX] = colour;
		}
		array[posY][posX + 1] = colour;
	}
	else if(rotation == ROTATION_2)
	{
		for(int i = -1; i < PIECES_PARTS / 2; i++)
		{
			array[posY - 1][posX + i] = colour;
		}
		array[posY][posX - 1] = colour;
	}
	else if(rotation == ROTATION_3)
	{
		for(int i = 0; i < PIECES_PARTS - 1; i++)
		{
			array[posY - i][posX + 1] = colour;
		}
		array[posY - 2][posX] = colour;
	}
	else if(rotation == ROTATION_4)
	{
		for(int i = -1; i < PIECES_PARTS / 2; i++)
		{
			array[posY][posX + i] = colour;
		}
		array[posY - 1][posX + 1] = colour;
	}
}

int checkIfJPieceCanBeDrawn(int posX, int posY, int rotation, int array[HEIGHT_SQUERE_NUMBER][WIDTH_SQUERE_NUMBER])
{
	rotation = (rotation % 4);
	if(rotation == ROTATION_1)
	{
		for(int i = 0; i < PIECES_PARTS - 1; i++)
		{
			if(array[posY - i][posX + 1] != WHITE || array[posY][posX] != WHITE)
			{
				return 0;
			}
		}
	}
	else if(rotation == ROTATION_2)
	{
		for(int i = -1; i < PIECES_PARTS / 2; i++)
		{
			if(array[posY][posX + i] != WHITE || array[posY - 1][posX - 1] != WHITE)
			{
				return 0;
			}
		}
	}
	else if(rotation == ROTATION_3)
	{
		for(int i = 0; i < PIECES_PARTS - 1; i++)
		{
			if(array[posY - i][posX] != WHITE || array[posY - 2][posX + 1] != WHITE)
			{
				return 0;
			}
		}
	}
	else if(rotation == ROTATION_4)
	{
		for(int i = -1; i < PIECES_PARTS / 2; i++)
		{
			if(array[posY - 1][posX + i] != WHITE || array[posY][posX + 1] != WHITE)
			{
				return 0;
			}
		}
	}
	return 1;
}

void drawJPiece(int posX, int posY, int rotation, int height, int width, int array[height][width], int colour)
{
	rotation = (rotation % 4);
	if(rotation == ROTATION_1)
	{
		for(int i = 0; i < PIECES_PARTS - 1; i++)
		{
			array[posY - i][posX + 1] = colour;
		}
		array[posY][posX] = colour;
	}
	else if(rotation == ROTATION_2)
	{
		for(int i = -1; i < PIECES_PARTS / 2; i++)
		{
			array[posY][posX + i] = colour;
		}
		array[posY - 1][posX - 1] = colour;
	}
	else if(rotation == ROTATION_3)
	{
		for(int i = 0; i < PIECES_PARTS - 1; i++)
		{
			array[posY - i][posX] = colour;
		}
		array[posY - 2][posX + 1] = colour;
	}
	else if(rotation == ROTATION_4)
	{
		for(int i = -1; i < PIECES_PARTS / 2; i++)
		{
			array[posY - 1][posX + i] = colour;
		}
		array[posY][posX + 1] = colour;
	}
}

int checkIfTPieceCanBeDrawn(int posX, int posY, int rotation, int array[HEIGHT_SQUERE_NUMBER][WIDTH_SQUERE_NUMBER])
{
	rotation = (rotation % 4);
	if(rotation == ROTATION_1)
	{
		for(int i = -1; i < PIECES_PARTS / 2; i++)
		{
			if(array[posY][posX + i] != WHITE || array[posY - 1][posX] != WHITE)
			{
				return 0;
			}
		}
	}
	else if(rotation == ROTATION_2)
	{
		for(int i = 0; i < PIECES_PARTS -1; i++)
		{
			if(array[posY - i][posX] != WHITE || array[posY - 1][posX + 1] != WHITE)
			{
				return 0;
			}
		}
	}
	else if(rotation == ROTATION_3)
	{
		for(int i = -1; i < PIECES_PARTS / 2; i++)
		{
			if(array[posY - 1][posX + i] != WHITE || array[posY][posX] != WHITE)
			{
				return 0;
			}
		}
	}
	else if(rotation == ROTATION_4)
	{
		for(int i = 0; i < PIECES_PARTS -1; i++)
		{
			if(array[posY - i][posX + 1] != WHITE || array[posY - 1][posX] != WHITE)
			{
				return 0;
			}
		}
	}
	return 1;
}

void drawTPiece(int posX, int posY, int rotation, int height, int width, int array[height][width], int colour)
{
	rotation = (rotation % 4);
	if(rotation == ROTATION_1)
	{
		for(int i = -1; i < PIECES_PARTS / 2; i++)
		{
			array[posY][posX + i] = colour;
		}
		array[posY - 1][posX] = colour;
	}
	else if(rotation == ROTATION_2)
	{
		for(int i = 0; i < PIECES_PARTS -1; i++)
		{
			array[posY - i][posX] = colour;
		}
		array[posY - 1][posX + 1] = colour;
	}
	else if(rotation == ROTATION_3)
	{
		for(int i = -1; i < PIECES_PARTS / 2; i++)
		{
			array[posY - 1][posX + i] = colour;
		}
		array[posY][posX] = colour;
	}
	else if(rotation == ROTATION_4)
	{
		for(int i = 0; i < PIECES_PARTS -1; i++)
		{
			array[posY - i][posX + 1] = colour;
		}
		array[posY - 1][posX] = colour;
	}
}

void cleanBoard(int height, int width, int array[height][width])
{
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			if(x == 0 || x == width - 1 || y == height - 1)
			{
				array[y][x] = BLACK;
			}
			else
			{
				array[y][x] = WHITE;
			}
		}
	}
}

int checkIfPieceCanBeDrawn(int posX, int posY, int rotation, int array[HEIGHT_SQUERE_NUMBER][WIDTH_SQUERE_NUMBER], int type)
{
	switch(type)
	{
	case I_PIECE:
		return checkIfIPieceCanBeDrawn(posX, posY, rotation, array);
	case O_PIECE:
		return checkIfOPieceCanBeDrawn(posX, posY, rotation, array);
	case S_PIECE:
		return checkIfSPieceCanBeDrawn(posX, posY, rotation, array);
	case Z_PIECE:
		return checkIfZPieceCanBeDrawn(posX, posY, rotation, array);
	case L_PIECE:
		return checkIfLPieceCanBeDrawn(posX, posY, rotation, array);
	case J_PIECE:
		return checkIfJPieceCanBeDrawn(posX, posY, rotation, array);
	case T_PIECE:
		return checkIfTPieceCanBeDrawn(posX, posY, rotation, array);
	}
	return 1;
}

void drawOrDeletePiece(int posX, int posY, int rotation, int height, int width, int array[height][width], int type, int draw)
{
	int colour = 7;
	if(draw)
	{
		colour = type;
	}
	switch(type)
	{
	case I_PIECE:
		drawIPiece(posX, posY, rotation, height, width, array, colour);
		break;
	case O_PIECE:
		drawOPiece(posX, posY, rotation, height, width, array, colour);
		break;
	case S_PIECE:
		drawSPiece(posX, posY, rotation, height, width, array, colour);
		break;
	case Z_PIECE:
		drawZPiece(posX, posY, rotation, height, width, array, colour);
		break;
	case L_PIECE:
		drawLPiece(posX, posY, rotation, height, width, array, colour);
		break;
	case J_PIECE:
		drawJPiece(posX, posY, rotation, height, width, array, colour);
		break;
	case T_PIECE:
		drawTPiece(posX, posY, rotation, height, width, array, colour);
		break;
	}
}

void rotatePiece(int* R, int* kUp, int posX, int posY, int array[HEIGHT_SQUERE_NUMBER][WIDTH_SQUERE_NUMBER], int type)
{
	if(!(gfx_isKeyDown(SDLK_UP) || gfx_isKeyDown(SDLK_z)) && *kUp == 0)
	{ 
		*kUp = 1;
	}
	if((gfx_isKeyDown(SDLK_UP) || gfx_isKeyDown(SDLK_z)) && *kUp == 1)
	{
		drawOrDeletePiece(posX, posY, *R, HEIGHT_SQUERE_NUMBER, WIDTH_SQUERE_NUMBER, array, type, 0);
		if(gfx_isKeyDown(SDLK_UP) && checkIfPieceCanBeDrawn(posX, posY, *R + 1, array, type))
		{
			*R += 1;
		}
		if(gfx_isKeyDown(SDLK_z) && checkIfPieceCanBeDrawn(posX, posY, *R - 1, array, type))
		{
			*R -= 1;
		}
		*kUp = 0;
	}
}

int keepRotationInInterval(int R)
{
	if(R < 0)
	{
		R = 3;
	}
	if(R > 3)
	{
		R = 0;
	}
	return R;
}

void movePieceSiedways(int rotation, int* kUp, int* posX, int posY, int array[HEIGHT_SQUERE_NUMBER][WIDTH_SQUERE_NUMBER], int type)
{
	if(!(gfx_isKeyDown(SDLK_RIGHT) || gfx_isKeyDown(SDLK_LEFT)) && *kUp == 0)
	{ 
		*kUp = 1;
	}
	if((gfx_isKeyDown(SDLK_RIGHT) || gfx_isKeyDown(SDLK_LEFT)) && *kUp == 1)
	{
		drawOrDeletePiece(START_X + *posX, posY, rotation, HEIGHT_SQUERE_NUMBER, WIDTH_SQUERE_NUMBER, array, type, 0);
		if(gfx_isKeyDown(SDLK_RIGHT) && checkIfPieceCanBeDrawn(START_X + *posX + 1, posY, rotation, array, type))
		{
			*posX += 1;
		}
		if(gfx_isKeyDown(SDLK_LEFT) && checkIfPieceCanBeDrawn(START_X + *posX - 1, posY, rotation, array, type))
		{
			*posX -= 1;
		}
		*kUp = 0;
	}
}

int setStartPosition(int posY, int type)
{
	if(type == I_PIECE)
	{
		posY += 2;
	}
	if(type == L_PIECE || type == J_PIECE)
	{
		posY += 1;
	}
	return posY;
}

void movePieceDown(int posX, int* posY, int rotation, int array[HEIGHT_SQUERE_NUMBER][WIDTH_SQUERE_NUMBER], int type, clock_t stop, clock_t* start, int* shouldBreak)
{
	if(checkIfPieceCanBeDrawn(START_X + posX, START_Y + *posY + 1, rotation, array, type))
	{
		if((roundf(DECIMAL_POINTS * (stop - *start)/CLOCKS_PER_SEC)/DECIMAL_POINTS) >= 0.50)
		{
			*posY += 1;
			*start = clock();
		}
	}
	else
	{
		if((roundf(DECIMAL_POINTS * (stop - *start)/CLOCKS_PER_SEC)/DECIMAL_POINTS) >= 1.00)
		{
			drawOrDeletePiece(START_X + posX, START_Y + *posY, rotation, HEIGHT_SQUERE_NUMBER, WIDTH_SQUERE_NUMBER, array, type, 1);
			*shouldBreak = 1;
		}
	}
}

void movePieceDownInstantly(int posX, int* posY, int rotation, int array[HEIGHT_SQUERE_NUMBER][WIDTH_SQUERE_NUMBER], int type, int* shouldBreak, int* instantDownKUp)
{
	if(!gfx_isKeyDown(SDLK_SPACE) && *instantDownKUp == 0)
	{ 
		*instantDownKUp = 1;
	}
	if(gfx_isKeyDown(SDLK_SPACE) && *instantDownKUp == 1)
	{
		drawOrDeletePiece(START_X + posX, START_Y + *posY, rotation, HEIGHT_SQUERE_NUMBER, WIDTH_SQUERE_NUMBER, array, type, 0);
		while(1)
		{
			if(checkIfPieceCanBeDrawn(START_X + posX, START_Y + *posY + 1, rotation, array, type))
			{
				*posY += 1;
			}
			else
			{
				break;
			}
		}
		drawOrDeletePiece(START_X + posX, START_Y + *posY, rotation, HEIGHT_SQUERE_NUMBER, WIDTH_SQUERE_NUMBER, array, type, 1);
		*shouldBreak = 1;
	}
}

void movePiece(int array[HEIGHT_SQUERE_NUMBER][WIDTH_SQUERE_NUMBER], int hold[NEXT_AND_HOLD_HEIGHT][NEXT_AND_HOLD_WIDTH], int next[NEXT_AND_HOLD_HEIGHT][NEXT_AND_HOLD_WIDTH], int type, int instantDownKUp, int points)
{
	int posX = 0;
	int posY = 1;
	int shouldBreak = 0;
	int rotation = 0;
	int rotationKeyUp = 0;
	int movementKeyUp = 0;
	posY = setStartPosition(posY, type);
	clock_t start, stop;
	start = clock();
	while(1)
	{
		drawBoard(array, hold, next, points);
		rotatePiece(&rotation, &rotationKeyUp, START_X + posX, START_Y + posY, array, type);
		rotation = keepRotationInInterval(rotation);
		movePieceSiedways(rotation, &movementKeyUp, &posX, START_Y + posY, array, type);
		stop = clock();
		drawOrDeletePiece(START_X + posX, START_Y + posY, rotation, HEIGHT_SQUERE_NUMBER, WIDTH_SQUERE_NUMBER, array, type, 0);
		movePieceDown(posX, &posY, rotation, array, type, stop, &start, &shouldBreak);
		movePieceDownInstantly(posX, &posY, rotation, array, type, &shouldBreak, &instantDownKUp);
		if(shouldBreak == 1)
		{
			break;
		}
		drawOrDeletePiece(START_X + posX, START_Y + posY, rotation, HEIGHT_SQUERE_NUMBER, WIDTH_SQUERE_NUMBER, array, type, 1);
		gfx_updateScreen();
	}
}

int generateNextPiece()
{
	int randomNumber = 0;
	srand(time(NULL));
	randomNumber = rand() % NUMBER_OF_PIECES;
	return randomNumber;
}

int checkIfAnyRowFull(int array[HEIGHT_SQUERE_NUMBER][WIDTH_SQUERE_NUMBER])
{
	int fullCells = 0;
	for(int i = HEIGHT_SQUERE_NUMBER - 2; i >= 0; i--)
	{
		fullCells = 0;
		for(int j = 1; j < WIDTH_SQUERE_NUMBER - 1; j++)
		{
			if(array[i][j] != WHITE)
			{
				fullCells++;
			}
		}
		if(fullCells == WIDTH_SQUERE_NUMBER - 2)
		{
			return i;
		}
	}
	return -1;
}

void deleteRow(int row, int array[HEIGHT_SQUERE_NUMBER][WIDTH_SQUERE_NUMBER], int hold[NEXT_AND_HOLD_HEIGHT][NEXT_AND_HOLD_WIDTH], int next[NEXT_AND_HOLD_HEIGHT][NEXT_AND_HOLD_WIDTH], int points)
{
	for(int i = 0; i < 5; i++)
	{
		drawBoard(array, hold, next, points);
		for(int j = 1; j < WIDTH_SQUERE_NUMBER - 1; j++)
		{
			array[row][j] = RED;
		}
		gfx_updateScreen();
		usleep(SLEEP);
		drawBoard(array, hold, next, points);
		for(int j = 1; j < WIDTH_SQUERE_NUMBER - 1; j++)
		{
			array[row][j] = WHITE;
		}
		gfx_updateScreen();
		usleep(SLEEP);
	}
}

void colapseTetris(int row, int array[HEIGHT_SQUERE_NUMBER][WIDTH_SQUERE_NUMBER], int hold[NEXT_AND_HOLD_HEIGHT][NEXT_AND_HOLD_WIDTH], int next[NEXT_AND_HOLD_HEIGHT][NEXT_AND_HOLD_WIDTH], int points)
{
	drawBoard(array, hold, next, points);
	for(int i = row; i > 0; i--)
	{
		for(int j = 1; j < WIDTH_SQUERE_NUMBER - 1; j++)
		{
			array[i][j] = array[i - 1][j];
		}
	}
	gfx_updateScreen();
	usleep(SLEEP);
}

int getPoints(int points, int array[HEIGHT_SQUERE_NUMBER][WIDTH_SQUERE_NUMBER], int hold[NEXT_AND_HOLD_HEIGHT][NEXT_AND_HOLD_WIDTH], int next[NEXT_AND_HOLD_HEIGHT][NEXT_AND_HOLD_WIDTH])
{
	int noFullRows;
	do
		{
			noFullRows = checkIfAnyRowFull(array);
			if(noFullRows != -1)
			{
				deleteRow(noFullRows, array, hold, next, points);
				colapseTetris(noFullRows, array, hold, next, points);
				points += POINTS_FOR_FULL_ROW;
			}
		}while(noFullRows != -1);
	return points;
}

void cleanAll(int array[HEIGHT_SQUERE_NUMBER][WIDTH_SQUERE_NUMBER], int hold[NEXT_AND_HOLD_HEIGHT][NEXT_AND_HOLD_WIDTH], int next[NEXT_AND_HOLD_HEIGHT][NEXT_AND_HOLD_WIDTH])
{
	cleanBoard(HEIGHT_SQUERE_NUMBER, WIDTH_SQUERE_NUMBER, array);
	cleanBoard(NEXT_AND_HOLD_HEIGHT, NEXT_AND_HOLD_WIDTH, hold);
	cleanBoard(NEXT_AND_HOLD_HEIGHT, NEXT_AND_HOLD_WIDTH, next);
}

int checkIfLost(int array[HEIGHT_SQUERE_NUMBER][WIDTH_SQUERE_NUMBER])
{
	for(int i = 1; i < WIDTH_SQUERE_NUMBER - 1; i ++)
	{
		if(array[0][i] != WHITE)
		{
			return 0;
		}
	}
	return 1;
}

int main(int argc, char* argv[])
{
	initLiblary();
	int board[HEIGHT_SQUERE_NUMBER][WIDTH_SQUERE_NUMBER];
	int hold[NEXT_AND_HOLD_HEIGHT][NEXT_AND_HOLD_WIDTH];
	int next[NEXT_AND_HOLD_HEIGHT][NEXT_AND_HOLD_WIDTH];
	int currentPiece;
	int nextPiece = generateNextPiece();
	int instantlyDownKeyUp = 0;
	int points = 0;
	int game = 1;
	cleanAll(board, hold, next);
	while(!(gfx_isKeyDown(SDLK_ESCAPE)))
	{
		if(game)
		{
			currentPiece = nextPiece;
			drawOrDeletePiece(NEXT_AND_HOLD_PIECE_X_POSITION, NEXT_AND_HOLD_PIECE_Y_POSITION, ROTATION_1, NEXT_AND_HOLD_HEIGHT, NEXT_AND_HOLD_WIDTH, next, nextPiece, 0);
			nextPiece = generateNextPiece();
			drawOrDeletePiece(NEXT_AND_HOLD_PIECE_X_POSITION, NEXT_AND_HOLD_PIECE_Y_POSITION, ROTATION_1, NEXT_AND_HOLD_HEIGHT, NEXT_AND_HOLD_WIDTH, next, nextPiece, 1);
			movePiece(board, hold, next, currentPiece, instantlyDownKeyUp, points);
			points = getPoints(points, board, hold, next);
			game = checkIfLost(board);
		}
		else
		{
			drawBoard(board, hold, next, points);
			gfx_textout(NEXT_X_POSITION, gfx_screenHeight() / 2, "LOST", RED);
			gfx_updateScreen();
		}
	}
	return 0;
}
