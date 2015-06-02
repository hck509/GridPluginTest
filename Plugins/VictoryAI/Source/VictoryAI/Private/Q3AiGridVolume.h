// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Volume.h"
#include "Q3AiGridVolume.generated.h"

/**
 *
 */
UCLASS()
class VICTORYAI_API AQ3AiGridVolume : public AVolume
{
	GENERATED_BODY()

public:
	AQ3AiGridVolume(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void PostRenderFor(class APlayerController* PC, class UCanvas* Canvas, FVector CameraPosition, FVector CameraDir) override;

	void BuildGrid();
	TArray<FVector> FindPath(const FVector& Start, const FVector& End) const;



	void FindPathTest();

private:
	float GridMinX, GridMinY, GridMinZ, GridStep;
	TArray<TArray<int>> Nodes;
};
