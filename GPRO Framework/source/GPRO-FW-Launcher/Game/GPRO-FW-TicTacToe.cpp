#include "Spitfire-Framework/sfio.h"

//-----------------------------------------------------------------------------
// DECLARATIONS

#define GS_TICTACTOE_PLAYERS					2
#define GS_TICTACTOE_BOARD_WIDTH				3
#define GS_TICTACTOE_BOARD_HEIGHT				3

enum gs_tictactoe_space_state
{
	// invalid space state
	gs_tictactoe_space_invalid = -1,

	// states visible to both players
	gs_tictactoe_space_open,	// space is open
	gs_tictactoe_space_o,		// space is taken by O player
	gs_tictactoe_space_x,		// space is taken by X player
};
#ifndef __cplusplus
typedef		enum gs_tictactoe_space_state		gs_tictactoe_space_state;
#endif	// !__cplusplus

// tic-tac-toe game state
typedef		unsigned char						gs_tictactoe_index;
typedef		gs_tictactoe_space_state			gs_tictactoe[GS_TICTACTOE_BOARD_WIDTH][GS_TICTACTOE_BOARD_HEIGHT];


#define	GS_VALIDATE_COORDINATE(x,y,w,h)			(x < w && y < h)
#define GS_TICTACTOE_VALID(x,y)					GS_VALIDATE_COORDINATE(x,y,GS_TICTACTOE_BOARD_WIDTH,GS_TICTACTOE_BOARD_HEIGHT)


inline gs_tictactoe_space_state gs_tictactoe_getSpaceState(gs_tictactoe const game, gs_tictactoe_index const xpos, gs_tictactoe_index const ypos)
{
	if (GS_TICTACTOE_VALID(xpos, ypos))
		return (game[xpos][ypos]);
	return gs_tictactoe_space_invalid;
}

inline gs_tictactoe_space_state gs_tictactoe_setSpaceState(gs_tictactoe game, gs_tictactoe_space_state const state, gs_tictactoe_index const xpos, gs_tictactoe_index const ypos)
{
	if (GS_TICTACTOE_VALID(xpos, ypos))
		return (game[xpos][ypos] = state);
	return gs_tictactoe_space_invalid;
}

inline gs_tictactoe_index gs_tictactoe_reset(gs_tictactoe game)
{
	gs_tictactoe_index xpos, ypos, total;
	for (xpos = 0; xpos < GS_TICTACTOE_BOARD_WIDTH; ++xpos)
		for (ypos = 0; ypos < GS_TICTACTOE_BOARD_HEIGHT; ++ypos)
			game[xpos][ypos] = gs_tictactoe_space_open;
	total = (xpos * ypos);
	return total;
}


//-----------------------------------------------------------------------------
// DEFINITIONS

char render_chars[3];
unsigned char render_styles[3];

void renderarr_cvt(gs_tictactoe game); //Conversion function from gs_tictactoe[][] to renderarr-formatted
int launchTicTacToe()
{
	render_chars[gs_tictactoe_space_state::gs_tictactoe_space_open] = '.';
	render_chars[gs_tictactoe_space_state::gs_tictactoe_space_o   ] = 'O';
	render_chars[gs_tictactoe_space_state::gs_tictactoe_space_x   ] = 'X';

	render_styles[gs_tictactoe_space_state::gs_tictactoe_space_open] = ascol(0, 0, 0, 1);
	render_styles[gs_tictactoe_space_state::gs_tictactoe_space_o   ] = ascol(1, 0, 0, 1);
	render_styles[gs_tictactoe_space_state::gs_tictactoe_space_x   ] = ascol(0, 0, 1, 1);

	gs_tictactoe game;

	{ //Main game loop
		gs_tictactoe_reset(game);
		gs_tictactoe_space_state whoseTurn = gs_tictactoe_space_o;
		while (true) {
			whoseTurn = (whoseTurn==gs_tictactoe_space_o) ? gs_tictactoe_space_x : gs_tictactoe_space_o;
			renderarr_cvt(game);

			csetcolb(render_styles[whoseTurn]);
			if (whoseTurn == gs_tictactoe_space_x) {
				std::cout << "Player X's turn" << std::endl;
			}
			else {
				std::cout << "Player O's turn" << std::endl;
			}

			//Try to move
			bool failedmove = true;
			while (failedmove) {
				csetcolc(1, 1, 1, 1);
				std::cout << "Move in position (ex. C3): ";

				char x = 0;
				while (!('a' <= x && x <= 'c')) x = tolower(cquerych());
				std::cout << x;

				char y = 0;
				while (!('1' <= y && y <= '3')) y = cquerych();
				std::cout << y;

				if (gs_tictactoe_getSpaceState(game, x - 'a', y - '1') == gs_tictactoe_space_state::gs_tictactoe_space_open) {
					gs_tictactoe_setSpaceState(game, whoseTurn, x - 'a', y - '1');
					failedmove = false;
				}
				else {
					failedmove = true;
					std::cout << std::endl << "Space already occupied!";
				}
				std::cout << std::endl;
			}
		}
	}

	return 0;
}

void renderarr_cvt(gs_tictactoe game) {
	char as_renderable[GS_TICTACTOE_BOARD_WIDTH][GS_TICTACTOE_BOARD_HEIGHT];
	unsigned char styles[GS_TICTACTOE_BOARD_WIDTH][GS_TICTACTOE_BOARD_HEIGHT];

	for (int x = 0; x < GS_TICTACTOE_BOARD_WIDTH; x++) {
		for (int y = 0; y < GS_TICTACTOE_BOARD_HEIGHT; y++) {
			as_renderable[x][y] = render_chars[ game[x][y] ];
			styles[x][y] = render_styles[game[x][y]];
		}
	}
	
	renderarr_styled(*as_renderable, *styles, GS_TICTACTOE_BOARD_WIDTH, GS_TICTACTOE_BOARD_HEIGHT, 0, 0);
	//renderarr(*as_renderable, GS_TICTACTOE_BOARD_WIDTH, GS_TICTACTOE_BOARD_HEIGHT, 10, 10);
}



//-----------------------------------------------------------------------------
