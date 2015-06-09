// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Volume.h"
#include "DebugRenderSceneProxy.h"
#include "Q3NavGridVolume.generated.h"

namespace MicroPanther { class MicroPather; }
class Q3Graph;

/**
 *
 */
UCLASS()
class Q3NAV_API AQ3NavGridVolume : public AVolume
{
	GENERATED_BODY()

public:
	AQ3NavGridVolume(const FObjectInitializer& ObjectInitializer);
	virtual ~AQ3NavGridVolume();

	virtual void PostLoad() override;

	void BuildGrid();
	TArray<FVector> FindPath(const FVector& Start, const FVector& End) const;

#if !UE_BUILD_SHIPPING
	void DrawDebug(UCanvas* Canvas, APlayerController*);
	void FindPathTest();
#endif

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostEditMove(bool bFinished) override;
#endif
	
private:
	UPROPERTY()
	float GridSize;

	UPROPERTY()
	float GridMinX;

	UPROPERTY()
	float GridMinY;

	UPROPERTY()
	float GridMinZ;

	UPROPERTY()
	float GridStep;

	UPROPERTY()
	int32 GridCountX;

	UPROPERTY()
	int32 GridCountY;

	// Heights of each grid cell. index = X + (Y * GridCountX). -1 means blocked.
	UPROPERTY()
	TArray<int32> Heights;

	MicroPanther::MicroPather* Panther;
	Q3Graph* Graph;

#if !UE_BUILD_SHIPPING
	FDebugDrawDelegate DebugDrawingDelegate;
	FDelegateHandle DebugDrawingDelegateHandle;
#endif
};
