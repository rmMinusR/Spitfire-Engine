#include "Spitfire-Framework/sfio.h"
#include "Spitfire-Framework/textstyle.h"

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
TextStyle render_styles[3];
StyledChar render_sc[3];

void renderarr_cvt(gs_tictactoe game); //Conversion function from gs_tictactoe[][] to renderarr-formatted
int launchTicTacToe()
{
	render_chars[gs_tictactoe_space_state::gs_tictactoe_space_open] = '.';
	render_chars[gs_tictactoe_space_state::gs_tictactoe_space_o   ] = 'O';
	render_chars[gs_tictactoe_space_state::gs_tictactoe_space_x   ] = 'X';
	
	render_styles[gs_tictactoe_space_state::gs_tictactoe_space_open] = TextStyle(0, 0, 0, 1);
	render_styles[gs_tictactoe_space_state::gs_tictactoe_space_o   ] = TextStyle(1, 0, 0, 1);
	render_styles[gs_tictactoe_space_state::gs_tictactoe_space_x   ] = TextStyle(0, 0, 1, 1);

	render_sc[gs_tictactoe_space_state::gs_tictactoe_space_open] = StyledChar(render_chars[gs_tictactoe_space_state::gs_tictactoe_space_open], render_styles[gs_tictactoe_space_state::gs_tictactoe_space_open]);
	render_sc[gs_tictactoe_space_state::gs_tictactoe_space_o   ] = StyledChar(render_chars[gs_tictactoe_space_state::gs_tictactoe_space_o   ], render_styles[gs_tictactoe_space_state::gs_tictactoe_space_o   ]);
	render_sc[gs_tictactoe_space_state::gs_tictactoe_space_x   ] = StyledChar(render_chars[gs_tictactoe_space_state::gs_tictactoe_space_x   ], render_styles[gs_tictactoe_space_state::gs_tictactoe_space_x   ]);

	gs_tictactoe game;

	bool doExitGame = false;
	while (!doExitGame) {
		
		//Initialize game
		gs_tictactoe_reset(game);
		gs_tictactoe_space_state whoseTurn = gs_tictactoe_space_o;
		
		//Main game loop
		while (true) {
			//Render the game board
			cclear();
			renderarr_cvt(game);

			//Swap whose turn it is, and display it
			whoseTurn = (whoseTurn == gs_tictactoe_space_o) ? gs_tictactoe_space_x : gs_tictactoe_space_o;
			
			std::cout << "Player " << render_chars[whoseTurn] << "'s turn" << std::endl;

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

			//Do win/lose logic
			{
				gs_tictactoe_space_state whoWon = gs_tictactoe_space_state::gs_tictactoe_space_open;
				
				//Check for horizontal win
				if(whoWon == gs_tictactoe_space_state::gs_tictactoe_space_open)
				for (int y = 0; y < GS_TICTACTOE_BOARD_HEIGHT; y++) {
					bool _won = true;
					for (int x = 1; x < GS_TICTACTOE_BOARD_WIDTH; x++) {
						_won = _won && (game[x][y] == game[0][y]);
					}
					if (_won) whoWon = game[0][y];
				}

				//Check for vertical win
				if (whoWon == gs_tictactoe_space_state::gs_tictactoe_space_open)
				for (int x = 0; x < GS_TICTACTOE_BOARD_HEIGHT; x++) {
					bool _won = true;
					for (int y = 1; y < GS_TICTACTOE_BOARD_WIDTH; y++) {
						_won = _won && (game[x][y] == game[y][0]);
					}
					if (_won) whoWon = game[x][0];
				}

				//Check for win along diagonals
				if (whoWon == gs_tictactoe_space_state::gs_tictactoe_space_open) {
					bool _won = true;
					for (int i = 1; i < GS_TICTACTOE_BOARD_HEIGHT && i < GS_TICTACTOE_BOARD_WIDTH; i++) {
						_won = _won && (game[i][i] == game[0][0]);
					}
					if (_won) whoWon = game[0][0];
				}

				if (whoWon == gs_tictactoe_space_state::gs_tictactoe_space_open) {
					bool _won = true;
					for (int i = 1; i < GS_TICTACTOE_BOARD_HEIGHT && i < GS_TICTACTOE_BOARD_WIDTH; i++) {
						_won = _won && (game[GS_TICTACTOE_BOARD_WIDTH - 1 - i][i] == game[GS_TICTACTOE_BOARD_WIDTH - 1][0]);
					}
					if (_won) whoWon = game[0][GS_TICTACTOE_BOARD_WIDTH - 1];
				}

				//Check for tie
				if (whoWon == gs_tictactoe_space_state::gs_tictactoe_space_open) {
					bool _tied = true;
					for (int x = 0; x < GS_TICTACTOE_BOARD_HEIGHT; x++) {
						for (int y = 1; y < GS_TICTACTOE_BOARD_WIDTH; y++) {
							_tied = _tied && (game[x][y] != gs_tictactoe_space_state::gs_tictactoe_space_open);
						}
					}
					if (_tied) whoWon = gs_tictactoe_space_state::gs_tictactoe_space_invalid;
					std::cout << "Tied? " << _tied << std::endl;
				}

				std::cout << "Winner: " << render_chars[whoWon] << std::endl;

				if (whoWon != gs_tictactoe_space_state::gs_tictactoe_space_open) gs_tictactoe_reset(game);

			} //End of win/lose logic

		} //End of main game loop
		
	} //End of program
	

	return 0;
}

void renderarr_cvt(gs_tictactoe game) {
	const int border = 3;

	StyledTextBlock canvas(GS_TICTACTOE_BOARD_WIDTH * (1 + border * 2), GS_TICTACTOE_BOARD_HEIGHT * (1 + border * 2));
	
	for (int x = 0; x < GS_TICTACTOE_BOARD_WIDTH; x++) {
		for (int y = 0; y < GS_TICTACTOE_BOARD_HEIGHT; y++) {
			int cell_x = border + x * (1 + border * 2);
			int cell_y = border + y * (1 + border * 2);
			
			canvas.setStyledChar(render_sc[game[x][y]], cell_x, cell_y);
			
			for (int i = 1; i <= border; i++) canvas.drawBox(render_styles[game[x][y]], cell_x - i, cell_y - i, cell_x + i, cell_y + i);
		}
	}

	canvas.renderAt(0, 0);
}

//-----------------------------------------------------------------------------
