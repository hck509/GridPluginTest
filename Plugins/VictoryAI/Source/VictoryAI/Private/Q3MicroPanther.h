#pragma once

#include "MicroPather.h"


// void* state is defined as x + (y * Column Count)
class Q3Graph : public MicroPanther::Graph
{
public:
	Q3Graph(const TArray<TArray<int>>& Heights);

	virtual int32 LeastCostEstimate(void* StartState, void* EndState) override;
	virtual void AdjacentCost(void* State, MP_VECTOR<MicroPanther::StateCost>* AdjacentCosts) override;
	virtual void PrintStateInfo(void* State) override;
	
	FIntPoint StateToVec2(void* State) const;
	void* Vec2ToState(const FIntPoint& Position) const;

private:
	int GetHeight(int32 X, int32 Y) const;

	// 2D Array of Heights
	// -1 means blocked
	TArray<TArray<int>> Heights;
};
