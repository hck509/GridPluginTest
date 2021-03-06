#include "Q3NavPrivatePCH.h"
#include "Q3MicroPanther.h"


Q3Graph::Q3Graph()
{
	GridCountX = 0;
	GridCountY = 0;
}

int32 Q3Graph::LeastCostEstimate(void* StartState, void* EndState)
{
	FIntPoint StartPosition = StateToVec2(StartState);
	FIntPoint EndPosition = StateToVec2(EndState);

	// TODO : need square root here, but we are trying not to use floating point. so...
	// WARN : without square root, comparison can be wrong. (a + b)^2 != a^2 + b^2
	int32 Cost = (StartPosition - EndPosition).SizeSquared();

	return Cost;
}

void Q3Graph::AdjacentCost(void* State, MP_VECTOR<MicroPanther::StateCost>* AdjacentCosts)
{
	const int32 AdjacentX[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
	const int32 AdjacentY[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
	const int32 Costs[8] = { 1, 2, 1, 2, 1, 2, 1, 2 };

	FIntPoint Position = StateToVec2(State);

	for (int i = 0; i < 8; ++i)
	{
		FIntPoint AdjacentPosition(Position.X + AdjacentX[i], Position.Y + AdjacentY[i]);

		int Height = GetHeight(AdjacentPosition.X, AdjacentPosition.Y);

		if (Height != -1)
		{
			// Not Blocked
			void* State = Vec2ToState(AdjacentPosition);

			MicroPanther::StateCost Cost = { State, Costs[i] };

			AdjacentCosts->push_back(Cost);
		}
	}
}

void Q3Graph::PrintStateInfo(void* /*State*/)
{
	// TODO : ...
}

FIntPoint Q3Graph::StateToVec2(void* State) const
{
	if (GridCountX == 0)
	{
		ensure(0);
		return FIntPoint(0, 0);
	}

	intptr_t Index = (intptr_t)State;

	return FIntPoint(Index % GridCountX, Index / GridCountX);
}

void* Q3Graph::Vec2ToState(const FIntPoint& Position) const
{
	intptr_t Index = Position.X + (Position.Y * GridCountX);

	return (void*)Index;
}

int Q3Graph::GetHeight(int32 X, int32 Y) const
{
	if (X < 0 || X >= GridCountX)
	{
		return -1;
	}

	if (Y < 0 || Y >= GridCountY)
	{
		return -1;
	}

	return Heights[X + (Y * GridCountX)];
}
