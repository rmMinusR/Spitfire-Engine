#include "Spitfire-Framework/sfio.h"
#include "Spitfire-Framework/textstyle.h"
#include <time.h>

// MACROS

#define max(a, b) ( ( (a) > (b) )?(a):(b) )
#define min(a, b) ( ( (a) < (b) )?(a):(b) )

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

void render(gs_tictactoe game, const int& highlightedX = -1, const int& highlightedY = -1); //Conversion function from gs_tictactoe[][] to renderarr-formatted
void showDialog(std::string str, int x, int y);

//-----------------------------------------------------------------------------
// RUNTIME VARIABLES

gs_tictactoe_space_state whoseTurn = gs_tictactoe_space_o;

//Previous Console Width/Height
int pcw = 0;
int pch = 0;

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
		whoseTurn = gs_tictactoe_space_o;

		csetcurvis(false);

		//Main game loop
		while (true) {
			//Input logic + rendering
			int selX = GS_TICTACTOE_BOARD_WIDTH / 2, selY = GS_TICTACTOE_BOARD_HEIGHT / 2;
			bool hasMoved = false;
			while (!hasMoved) {

				char input = cquerycht(50);

				if (input == -32) { //Charcode preceeding arrow keys

					char c = cquerycht(5);
					/**/ if (c == 72) { // UP
						selY = max(selY - 1, 0);
					}
					else if (c == 75) { // LEFT
						selX = max(selX - 1, 0);
					}
					else if (c == 80) { // DOWN
						selY = min(selY + 1, GS_TICTACTOE_BOARD_HEIGHT - 1);
					}
					else if (c == 77) { // RIGHT
						selX = min(selX + 1, GS_TICTACTOE_BOARD_WIDTH - 1);
					}

				}
				else { // Non-arrow key input

					if (input == ' ' || input == '\n') {

						if (gs_tictactoe_getSpaceState(game, selX, selY) == gs_tictactoe_space_state::gs_tictactoe_space_open) {
							gs_tictactoe_setSpaceState(game, whoseTurn, selX, selY);
							hasMoved = true;
						}

					}

				}

				//If the window's size has changed, clear the buffer
				if (pcw != cgetw() || pch != cgeth()) {
					pcw = cgetw();
					pch = cgeth();

					cclear();
				}

				//Render the game board, flashing the selected cell
				if (clock() % 500 < 250 && !hasMoved) {
					render(game, selX, selY);
				}
				else {
					render(game);
				}
			}

			//Swap whose turn it is
			whoseTurn = (whoseTurn == gs_tictactoe_space_o) ? gs_tictactoe_space_x : gs_tictactoe_space_o;

			//Do win/lose logic
			{
				gs_tictactoe_space_state whoWon = gs_tictactoe_space_state::gs_tictactoe_space_open;
				
				//Check for horizontal win
				for (int y = 0; y < GS_TICTACTOE_BOARD_HEIGHT; y++) {
					if (whoWon == gs_tictactoe_space_state::gs_tictactoe_space_open) {
						bool _won = true;
						for (int x = 1; x < GS_TICTACTOE_BOARD_WIDTH; x++) if (game[x][y] != game[0][y]) _won = false;
						if (_won) whoWon = game[0][y];
					}
				}
				
				//Check for vertical win
				for (int x = 0; x < GS_TICTACTOE_BOARD_HEIGHT; x++) {
					if (whoWon == gs_tictactoe_space_state::gs_tictactoe_space_open) {
						bool _won = true;
						for (int y = 1; y < GS_TICTACTOE_BOARD_WIDTH; y++) if (game[x][y] != game[x][0]) _won = false;
						if (_won) whoWon = game[x][0];
					}
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
				}
				
				//If game is over, show it
				if (whoWon == gs_tictactoe_space_state::gs_tictactoe_space_invalid) {
					showDialog("Tied!", cgetw() / 2, cgeth() / 2);
					cquerych();

					gs_tictactoe_reset(game);
					cclear();
				}
				else if (whoWon != gs_tictactoe_space_state::gs_tictactoe_space_open) {
					std::string winstr = "Winner: ";
					winstr += render_chars[whoWon];
					showDialog(winstr, cgetw() / 2, cgeth() / 2);
					cquerych();

					gs_tictactoe_reset(game);
					cclear();
				}

			} //End of win/lose logic

		} //End of main game loop
		
	} //End of program
	

	return 0;
}

void render(gs_tictactoe game, const int& highlightedX, const int& highlightedY) {
	const int border = 2;

	StyledTextBlock gameCanvas(GS_TICTACTOE_BOARD_WIDTH * (1 + border * 2), GS_TICTACTOE_BOARD_HEIGHT * (1 + border * 2));
	
	for (int x = 0; x < GS_TICTACTOE_BOARD_WIDTH; x++) {
		for (int y = 0; y < GS_TICTACTOE_BOARD_HEIGHT; y++) {
			int cell_x = border + x * (1 + border * 2);
			int cell_y = border + y * (1 + border * 2);
			
			gameCanvas.setStyledChar(render_sc[game[x][y]], cell_x, cell_y);
			
			for (int i = 1; i <= border; i++) gameCanvas.drawBox(render_styles[game[x][y]], cell_x - i, cell_y - i, cell_x + i, cell_y + i);

			if (x == highlightedX && y == highlightedY) {
				gameCanvas.fillStyle(TextStyle(1, 1, 0, 1), cell_x - border, cell_y - border, cell_x + border, cell_y + border);
			}
		}
	}

	gameCanvas.renderAt(cgetw()/2 - gameCanvas.width/2, cgeth()/2 - gameCanvas.height/2);


	showDialog("Arrow keys to move selector   Space to place tile", cgetw()/2, cgeth()/2 + gameCanvas.height/2 + 3);
	

	//Display whose turn it is
	csetcurpos(0, 0);
	std::cout << render_styles[whoseTurn] << "Player " << render_chars[whoseTurn] << "'s turn" << std::endl;
}

void showDialog(std::string str, int x, int y) {
	StyledTextBlock dialog((int)str.length() + 2, 3);
	dialog.drawBox(TextStyle(), 0, 0, dialog.width - 1, dialog.height - 1);
	dialog.putStr(str, 1, 1);
	dialog.renderAt(x - dialog.width/2, y - dialog.height/2);
}

//-----------------------------------------------------------------------------
