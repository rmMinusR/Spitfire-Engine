#include "Game/Generic.h"

#include "Spitfire-Framework/sfio.h"
#include "Spitfire-Framework/textstyle.h"

#include <time.h>
#include <string>

namespace game_generic {

#pragma region Constants and helper microfunctions

	#define max(a, b) ( ((a)>(b))?(a):(b) )
	#define min(a, b) ( ((a)<(b))?(a):(b) )

	const int PLAYER_COUNT = 2;
	const int BOARD_WIDTH  = 3;
	const int BOARD_HEIGHT = 3;

#pragma endregion

#pragma region Custom data types

	enum space_state
	{
		// invalid space state
		space_invalid = -1,

		// states visible to both players
		space_open,	// space is open
		space_o,		// space is taken by O player
		space_x,		// space is taken by X player
	};

#ifndef __cplusplus
	typedef		enum space_state		space_state;
#endif	// !__cplusplus

#define SPACE_STATE space_state

	// game state
	typedef		unsigned char	game_grid_index;
	typedef		SPACE_STATE		game_grid[BOARD_WIDTH][BOARD_HEIGHT];

#pragma endregion

#pragma region Array accessor/etc functions

#define	IS_WITHIN(x,y,w,h)			(x < w && y < h)
#define VALID(x,y)					IS_WITHIN(x,y,BOARD_WIDTH,BOARD_HEIGHT)


	inline SPACE_STATE getSpaceState(game_grid const game, game_grid_index const xpos, game_grid_index const ypos)
	{
		if (VALID(xpos, ypos))
			return (game[xpos][ypos]);
		return space_invalid;
	}

	inline SPACE_STATE setSpaceState(game_grid game, SPACE_STATE const state, game_grid_index const xpos, game_grid_index const ypos)
	{
		if (VALID(xpos, ypos))
			return (game[xpos][ypos] = state);
		return space_invalid;
	}

	inline game_grid_index reset(game_grid game)
	{
		game_grid_index xpos, ypos, total;
		for (xpos = 0; xpos < BOARD_WIDTH; ++xpos)
			for (ypos = 0; ypos < BOARD_HEIGHT; ++ypos)
				game[xpos][ypos] = space_open;
		total = (xpos * ypos);
		return total;
	}

#pragma endregion

#pragma region Runtime variables and resources

	char render_chars[3];
	TextStyle render_styles[3];
	StyledChar render_sc[3];

	SPACE_STATE whoseTurn = space_o;

	//Previous Console Width/Height
	int pcw = 0;
	int pch = 0;

#pragma endregion

	void render(game_grid game, const int& highlightedX = -1, const int& highlightedY = -1); //Conversion function from tictactoe[][] to renderarr-formatted

	int game_main()
	{
		render_chars[SPACE_STATE::space_open] = '.';
		render_chars[SPACE_STATE::space_o] = 'O';
		render_chars[SPACE_STATE::space_x] = 'X';

		render_styles[SPACE_STATE::space_open] = TextStyle(0, 0, 0, 1);
		render_styles[SPACE_STATE::space_o] = TextStyle(1, 0, 0, 1);
		render_styles[SPACE_STATE::space_x] = TextStyle(0, 0, 1, 1);

		render_sc[SPACE_STATE::space_open] = StyledChar(render_chars[SPACE_STATE::space_open], render_styles[SPACE_STATE::space_open]);
		render_sc[SPACE_STATE::space_o] = StyledChar(render_chars[SPACE_STATE::space_o], render_styles[SPACE_STATE::space_o]);
		render_sc[SPACE_STATE::space_x] = StyledChar(render_chars[SPACE_STATE::space_x], render_styles[SPACE_STATE::space_x]);

		game_grid game;

		bool doExitGame = false;
		while (!doExitGame) {

			//Initialize game
			reset(game);
			whoseTurn = space_o;

			csetcurvis(false);

			//Main game loop
			while (true) {
				//Input logic + rendering
				int selX = BOARD_WIDTH / 2, selY = BOARD_HEIGHT / 2;
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
							selY = min(selY + 1, BOARD_HEIGHT - 1);
						}
						else if (c == 77) { // RIGHT
							selX = min(selX + 1, BOARD_WIDTH - 1);
						}

					}
					else { // Non-arrow key input

						if (input == ' ' || input == '\n') {

							if (getSpaceState(game, selX, selY) == SPACE_STATE::space_open) {
								setSpaceState(game, whoseTurn, selX, selY);
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
				whoseTurn = (whoseTurn == space_o) ? space_x : space_o;

				//Do win/lose logic
				{
					SPACE_STATE whoWon = SPACE_STATE::space_open;

					//Check for horizontal win
					for (int y = 0; y < BOARD_HEIGHT; y++) {
						if (whoWon == SPACE_STATE::space_open) {
							bool _won = true;
							for (int x = 1; x < BOARD_WIDTH; x++) if (game[x][y] != game[0][y]) _won = false;
							if (_won) whoWon = game[0][y];
						}
					}

					//Check for vertical win
					for (int x = 0; x < BOARD_HEIGHT; x++) {
						if (whoWon == SPACE_STATE::space_open) {
							bool _won = true;
							for (int y = 1; y < BOARD_WIDTH; y++) if (game[x][y] != game[x][0]) _won = false;
							if (_won) whoWon = game[x][0];
						}
					}

#pragma warning (push)
#pragma warning (disable: 6287) // Get rid of warning on "i < BOARD_HEIGHT && i < BOARD_WIDTH"

					//Check for win along diagonals
					if (whoWon == SPACE_STATE::space_open) {
						bool _won = true;
						for (int i = 1; i < BOARD_HEIGHT && i < BOARD_WIDTH; i++) {
							_won = _won && (game[i][i] == game[0][0]);
						}
						if (_won) whoWon = game[0][0];
					}

					if (whoWon == SPACE_STATE::space_open) {
						bool _won = true;
						for (int i = 1; i < BOARD_HEIGHT && i < BOARD_WIDTH; i++) {
							_won = _won && (game[BOARD_WIDTH - 1 - i][i] == game[BOARD_WIDTH - 1][0]);
						}
						if (_won) whoWon = game[0][BOARD_WIDTH - 1];
					}

#pragma warning (pop)

					//Check for tie
					if (whoWon == SPACE_STATE::space_open) {
						bool _tied = true;
						for (int x = 0; x < BOARD_HEIGHT; x++) {
							for (int y = 0; y < BOARD_WIDTH; y++) {
								_tied = _tied && (game[x][y] != SPACE_STATE::space_open);
							}
						}
						if (_tied) whoWon = SPACE_STATE::space_invalid;
					}

					//If game is over, show it
					if (whoWon == SPACE_STATE::space_invalid) {
						showDialog("The game has reached a tie!", cgetw() / 2, cgeth() / 2);
						cquerych();

						reset(game);
						cclear();
					}
					else if (whoWon != SPACE_STATE::space_open) {
						std::string winstr = "Player ";
						winstr += render_chars[whoWon];
						winstr += " has won the game!";
						showDialog(winstr, cgetw() / 2, cgeth() / 2);
						cquerych();

						reset(game);
						cclear();
					}

				} //End of win/lose logic

			} //End of main game loop

		} //End of program


		return 0;
	}

	void render(game_grid game, const int& highlightedX, const int& highlightedY) {
		const int border = 3;
		
		StyledTextBlock gameCanvas(BOARD_WIDTH * (1 + border * 2), BOARD_HEIGHT * (1 + border * 2));

		for (int x = 0; x < BOARD_WIDTH; x++) {
			for (int y = 0; y < BOARD_HEIGHT; y++) {
				int cell_x = border + x * (1 + border * 2);
				int cell_y = border + y * (1 + border * 2);

				gameCanvas.setStyledChar(render_sc[game[x][y]], cell_x, cell_y);

				int i = border;
				if (border > 0) gameCanvas.drawBox(render_styles[game[x][y]], cell_x - i, cell_y - i, cell_x + i, cell_y + i);

				if (x == highlightedX && y == highlightedY) {
					gameCanvas.fillStyle(TextStyle(1, 1, 0, 1), cell_x - border, cell_y - border, cell_x + border, cell_y + border);
				}
			}
		}

		gameCanvas.renderAt(cgetw() / 2 - gameCanvas.width / 2, cgeth() / 2 - gameCanvas.height / 2);


		showDialog("Arrow keys to move selector   Space to place tile", cgetw() / 2, cgeth() / 2 + gameCanvas.height / 2 + 3);


		//Display whose turn it is
		csetcurpos(0, 0);
		std::cout << render_styles[whoseTurn] << "Player " << render_chars[whoseTurn] << "'s turn" << std::endl;
	}

}