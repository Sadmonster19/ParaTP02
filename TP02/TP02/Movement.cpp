#include "Movement.h"

Position Movement::getPositionForDirection(Direction d) {
	Position p;

	switch (d) {
		case NORTH_WEST:
			p = Position{ 1,-1 };
			break;

		case NORTH:
			p = Position{ 1,0 };
			break;

		case NORTH_EAST:
			p = Position{ 1,1 };
			break;

		case EAST:
			p = Position{ 0,1 };
			break;

		case SOUTH_EAST:
			p = Position{ -1,1 };
			break;

		case SOUTH:
			p = Position{ -1,0 };
			break;

		case SOUTH_WEST:
			p = Position{ -1,-1 };
			break;

		case WEST:
			p = Position{ 0,-1 };
			break;

		default:
			throw new std::exception("No position associated for this direction");
	}

	return p;
}