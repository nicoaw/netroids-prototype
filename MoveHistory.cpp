#include "MoveHistory.h"

#include "GameClient.h"

void MoveHistory::correction(GameClient& game, uint32_t time, const Ship::State& state, const Ship::Input& input)
{
	while (!importantMoves.empty() && importantMoves.front().time < time)
		importantMoves.pop_front();

	while (!moves.empty() && moves.front().time < time)
		moves.pop_front();

	if (moves.empty())
		return;

	if (state != moves.front().state)
	{
		moves.pop_front();
		
		//uint32_t savedTime = game.time;
		Ship::Input savedInput = game.input;

		game.time = time;
		game.input = input;
		game.ship.snap(state);
		game.replaying = true;

		for (Ship::Move& move : moves)
		{
			while (game.time < move.time)
				game.update(game.time);

			game.input = move.input;
			move.state = game.ship.state();
		}

		game.update(game.time);
		game.input = savedInput;
		game.replaying = false;
	}
}

void MoveHistory::getImportantMoves(std::deque<Ship::Move>& importantMoves)
{
	importantMoves = this->importantMoves;
}

void MoveHistory::push(const Ship::Move& move)
{
	bool important = true;

	if (!moves.empty())
		important = move.input.forward != moves.back().input.forward || move.input.left != moves.back().input.left || move.input.right != moves.back().input.right;

	if (important)
		importantMoves.push_back(move);

	moves.push_back(move);
}