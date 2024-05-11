#include "primlib.h"
#include <unistd.h>

#define SQUERE_SIZE 30
#define WIDTH_SQUERE_NUMBER 10
#define HEIGHT_SQUERE_NUMBER 20
#define PIECES_SPAWNER 4
#define CORDS_AND_COLOUR 3
#define GAME_SPACE_WIDTH (SQUERE_SIZE * WIDTH_SQUERE_NUMBER)
#define GAME_SPACE_HEIGHT (SQUERE_SIZE * HEIGHT_SQUERE_NUMBER)
#define NEXT_PIECE_SIZE (SQUERE_SIZE * 6)
#define DISTANCE 100
#define GAME_SPACE_X_POSITION (DISTANCE * 2 + NEXT_PIECE_SIZE)
#define HOLD_X_POSITION DISTANCE
#define NEXT_PIECE_CELL_X_POSITION (GAME_SPACE_X_POSITION + GAME_SPACE_WIDTH + DISTANCE)
#define GAME_SPACE_Y_POSITION 50
#define PIECES_PARTS 4
#define NEXT_PIECE_X_POSITION (NEXT_PIECE_CELL_X_POSITION + SQUERE_SIZE * 2)
#define NEXT_PIECE_Y_POSITION (GAME_SPACE_Y_POSITION + SQUERE_SIZE * 5)
#define HOLD_PIECE_X_POSITION (HOLD_X_POSITION + SQUERE_SIZE * 2)
#define HOLD_PIECE_Y_POSITION (GAME_SPACE_Y_POSITION + SQUERE_SIZE * 5)
#define ROTATION_1 1
#define ROTATION_2 2
#define ROTATION_3 3
#define ROTATION_4 4


void initLiblary()
{
	if(gfx_init()) 
	{
		exit(3);
	}
}

void drawHoleCell()
{
	gfx_filledRect(HOLD_X_POSITION, GAME_SPACE_Y_POSITION, HOLD_X_POSITION + NEXT_PIECE_SIZE,
	GAME_SPACE_Y_POSITION + NEXT_PIECE_SIZE, BLACK);
}

void drawNextPieceCell()
{
	gfx_filledRect(NEXT_PIECE_CELL_X_POSITION, GAME_SPACE_Y_POSITION,
	NEXT_PIECE_CELL_X_POSITION + NEXT_PIECE_SIZE, GAME_SPACE_Y_POSITION + NEXT_PIECE_SIZE, BLACK);
}

void drawHorizontalLines()
{
	for(int i = 1; i < HEIGHT_SQUERE_NUMBER; i++)
	{
		gfx_line( GAME_SPACE_X_POSITION, GAME_SPACE_Y_POSITION + i * SQUERE_SIZE,
		GAME_SPACE_X_POSITION + GAME_SPACE_WIDTH, GAME_SPACE_Y_POSITION + i * SQUERE_SIZE, WHITE);
	}
}

void drawVerticalLines()
{
	for(int i = 1; i < WIDTH_SQUERE_NUMBER; i++)
	{
		gfx_line( GAME_SPACE_X_POSITION + i  * SQUERE_SIZE, GAME_SPACE_Y_POSITION,
		GAME_SPACE_X_POSITION + i  * SQUERE_SIZE, GAME_SPACE_Y_POSITION + GAME_SPACE_HEIGHT, WHITE);
	}
}

void drawGameSpace()
{
	gfx_filledRect(GAME_SPACE_X_POSITION, GAME_SPACE_Y_POSITION, 
	GAME_SPACE_X_POSITION + GAME_SPACE_WIDTH, GAME_SPACE_Y_POSITION + GAME_SPACE_HEIGHT, BLACK);
	drawHorizontalLines();
	drawVerticalLines();
}

void drawBoard()
{
	gfx_filledRect(0, 0, gfx_screenWidth(), gfx_screenHeight(), WHITE);
	drawHoleCell();
	drawGameSpace();
	drawNextPieceCell();
}

void drawPiecePart(int posX, int posY, int colour)
{
	gfx_filledRect(posX, posY, posX + SQUERE_SIZE, posY - SQUERE_SIZE, colour);
}

void drawIPiece(int posX, int posY, int rotation)
{
	if(rotation == ROTATION_1)
	{
		for(int i = 0; i < PIECES_PARTS; i++)
		{
			drawPiecePart(posX, posY - i * SQUERE_SIZE, CYAN);
		}
	}
	else if(rotation == ROTATION_2)
	{
		for(int i = -1; i < PIECES_PARTS - 1; i++)
		{
			drawPiecePart(posX + i * SQUERE_SIZE, posY, CYAN);
		}
	}
}

void drawOPiece(int posX, int posY)
{
	for(int i = 0; i < PIECES_PARTS / 2; i++)
	{
		drawPiecePart(posX, posY - i * SQUERE_SIZE, YELLOW);
	}
	for(int i = 0; i < PIECES_PARTS / 2; i++)
	{
		drawPiecePart(posX + SQUERE_SIZE, posY - i * SQUERE_SIZE, YELLOW);
	}
}

void drawSPiece(int posX, int posY, int rotation)
{
	if(rotation == ROTATION_1)
	{
		for(int i = -1; i < PIECES_PARTS / 2 - 1; i++)
		{
			drawPiecePart(posX + i * SQUERE_SIZE, posY, RED);
		}
		for(int i = 0; i < PIECES_PARTS / 2; i++)
		{
			drawPiecePart(posX + i * SQUERE_SIZE, posY - SQUERE_SIZE, RED);
		}
	}
	else if(rotation == ROTATION_2)
	{
		for(int i = 0; i < PIECES_PARTS / 2; i++)
		{
			drawPiecePart(posX, posY - i * SQUERE_SIZE, RED);
		}
		for(int i = 1; i < PIECES_PARTS / 2 + 1; i++)
		{
			drawPiecePart(posX + SQUERE_SIZE, posY - i * SQUERE_SIZE, RED);
		}
	}
}

void drawZPiece(int posX, int posY, int rotation)
{
	if(rotation == ROTATION_1)
	{
		for(int i = 0; i < PIECES_PARTS / 2; i++)
		{
			drawPiecePart(posX + i * SQUERE_SIZE, posY, GREEN);
		}
		for(int i = -1; i < PIECES_PARTS / 2 - 1; i++)
		{
			drawPiecePart(posX + i * SQUERE_SIZE, posY - SQUERE_SIZE, GREEN);
		}
	}
	else if(rotation == ROTATION_2)
	{
		for(int i = 1; i < PIECES_PARTS / 2 + 1; i++)
		{
			drawPiecePart(posX, posY - i * SQUERE_SIZE, GREEN);
		}
		for(int i = 0; i < PIECES_PARTS / 2; i++)
		{
			drawPiecePart(posX + SQUERE_SIZE, posY - i * SQUERE_SIZE, GREEN);
		}
	}
}

void drawLPiece(int posX, int posY, int rotation)
{
	if(rotation == ROTATION_1)
	{
		for(int i = 0; i < PIECES_PARTS - 1; i++)
		{
			drawPiecePart(posX, posY - i * SQUERE_SIZE, BLUE);
		}
		drawPiecePart(posX + SQUERE_SIZE, posY, BLUE);
	}
	else if(rotation == ROTATION_2)
	{
		for(int i = -1; i < PIECES_PARTS / 2; i++)
		{
			drawPiecePart(posX + i * SQUERE_SIZE, posY - SQUERE_SIZE, BLUE);
		}
		drawPiecePart(posX - SQUERE_SIZE, posY, BLUE);
	}
	else if(rotation == ROTATION_3)
	{
		for(int i = 0; i < PIECES_PARTS - 1; i++)
		{
			drawPiecePart(posX + SQUERE_SIZE, posY - i * SQUERE_SIZE, BLUE);
		}
		drawPiecePart(posX, posY - 2 * SQUERE_SIZE, BLUE);
	}
	else if(rotation == ROTATION_4)
	{
		for(int i = -1; i < PIECES_PARTS / 2; i++)
		{
			drawPiecePart(posX + i * SQUERE_SIZE, posY, BLUE);
		}
		drawPiecePart(posX + SQUERE_SIZE, posY - SQUERE_SIZE, BLUE);
	}
}

void drawJPiece(int posX, int posY, int rotation)
{
	if(rotation == ROTATION_1)
	{
		for(int i = 0; i < PIECES_PARTS - 1; i++)
		{
			drawPiecePart(posX + SQUERE_SIZE, posY - i * SQUERE_SIZE, MAGENTA);
		}
		drawPiecePart(posX, posY, MAGENTA);
	}
	else if(rotation == ROTATION_2)
	{
		for(int i = -1; i < PIECES_PARTS / 2; i++)
		{
			drawPiecePart(posX + i * SQUERE_SIZE, posY, MAGENTA);
		}
		drawPiecePart(posX - SQUERE_SIZE, posY - SQUERE_SIZE, MAGENTA);
	}
	else if(rotation == ROTATION_3)
	{
		for(int i = 0; i < PIECES_PARTS - 1; i++)
		{
			drawPiecePart(posX, posY - i * SQUERE_SIZE, MAGENTA);
		}
		drawPiecePart(posX + SQUERE_SIZE, posY - 2 * SQUERE_SIZE, MAGENTA);
	}
	else if(rotation == ROTATION_4)
	{
		for(int i = -1; i < PIECES_PARTS / 2; i++)
		{
			drawPiecePart(posX + i * SQUERE_SIZE, posY- SQUERE_SIZE, MAGENTA);
		}
		drawPiecePart(posX + SQUERE_SIZE, posY, MAGENTA);
	}
}

void drawTPiece(int posX, int posY, int rotation)
{
	if(rotation == ROTATION_1)
	{
		for(int i = -1; i < PIECES_PARTS / 2; i++)
		{
			drawPiecePart(posX + i * SQUERE_SIZE, posY, WHITE);
		}
		drawPiecePart(posX, posY - SQUERE_SIZE, WHITE);
	}
	else if(rotation == ROTATION_2)
	{
		for(int i = 0; i < PIECES_PARTS -1; i++)
		{
			drawPiecePart(posX, posY - i * SQUERE_SIZE, WHITE);
		}
		drawPiecePart(posX + SQUERE_SIZE, posY - SQUERE_SIZE, WHITE);
	}
	else if(rotation == ROTATION_3)
	{
		for(int i = -1; i < PIECES_PARTS / 2; i++)
		{
			drawPiecePart(posX + i * SQUERE_SIZE, posY - SQUERE_SIZE, WHITE);
		}
		drawPiecePart(posX, posY, WHITE);
	}
	else if(rotation == ROTATION_4)
	{
		for(int i = 0; i < PIECES_PARTS -1; i++)
		{
			drawPiecePart(posX + SQUERE_SIZE, posY - i * SQUERE_SIZE, WHITE);
		}
		drawPiecePart(posX, posY - SQUERE_SIZE, WHITE);
	}
}

void cleanBoard(int array[HEIGHT_SQUERE_NUMBER + PIECES_SPAWNER][WIDTH_SQUERE_NUMBER]/*[CORDS_AND_COLOUR]*/)
{
	for(int y = 0; y < HEIGHT_SQUERE_NUMBER + PIECES_SPAWNER; y++)
	{
		for(int x = 0; x < WIDTH_SQUERE_NUMBER; x++)
		{
			/*for(int cc = 0; cc < CORDS_AND_COLOUR; cc++)
			{
				if(cc == 0)
				{
					array[y][x][cc] = y;
				}
				else if(cc == 1)
				{
					array[y][x][cc] = x;
				}
				else if(cc == 2)
				{
					array[y][x][cc] = '\0';
				}
			}*/
			array[y][x] = '\0';
		}
	}
}

int main(int argc, char* argv[])
{
	initLiblary();
	int board[HEIGHT_SQUERE_NUMBER + PIECES_SPAWNER][WIDTH_SQUERE_NUMBER]/*[CORDS_AND_COLOUR]*/;
	cleanBoard(board);
	while(!gfx_isKeyDown(SDLK_ESCAPE))
	{
		drawBoard();
		gfx_updateScreen();
	}
	return 0;
}
//zrobic figury uzaleznione od pol w tablicy a nia pola w tablicy od figur!!! hold i next powinny byc tablicami plansza powstaje kulorujac poszczegolne pola kolory przypisane do komorek
