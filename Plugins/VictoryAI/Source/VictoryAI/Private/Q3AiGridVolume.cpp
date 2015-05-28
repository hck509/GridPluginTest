// Fill out your copyright notice in the Description page of Project Settings.

#include "VictoryAIPrivatePCH.h"
#include "Q3AiGridVolume.h"
#include "DrawDebugHelpers.h"

AQ3AiGridVolume::AQ3AiGridVolume(const FObjectInitializer& ObjectInitializer) : AVolume(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = true;
}

void AQ3AiGridVolume::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    FBoxSphereBounds Bounds = BrushComponent->CalcBounds(BrushComponent->ComponentToWorld);
    FBox BBox = Bounds.GetBox();

    DrawDebugPoint(GEngine->GetWorldContexts()[0].World(), BBox.Min, 100.0f, FColor(0, 255, 0, 255));
}

void AQ3AiGridVolume::PostRenderFor(class APlayerController* PC, class UCanvas* Canvas, FVector CameraPosition, FVector CameraDir)
{

}
