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

    //DrawDebugPoint(GEngine->GetWorldContexts().Last().World(), BBox.Min, 100.0f, FColor(0, 255, 0, 255));

	DrawDebugBox(GetWorld(), BBox.GetCenter(), BBox.GetExtent(), FQuat::Identity, FColor(255, 255, 255, 255));

	BuildGrid();
}

void AQ3AiGridVolume::PostRenderFor(class APlayerController* PC, class UCanvas* Canvas, FVector CameraPosition, FVector CameraDir)
{

}

void AQ3AiGridVolume::BuildGrid()
{
	FBoxSphereBounds Bounds = BrushComponent->CalcBounds(BrushComponent->ComponentToWorld);
	FBox BBox = Bounds.GetBox();

	const float Step = 50.0f;
	
	float MinX = FPlatformMath::CeilToInt(BBox.Min.X / Step) * Step;
	float MinY = FPlatformMath::CeilToInt(BBox.Min.Y / Step) * Step;

	int CountX = FPlatformMath::FloorToInt((BBox.Max.X - BBox.Min.X) / Step);
	int CountY = FPlatformMath::FloorToInt((BBox.Max.Y - BBox.Min.Y) / Step);

	Nodes.Empty();
	Nodes.AddZeroed(CountX);

	for (int X = 0; X < CountX; ++X)
	{
		Nodes[X].AddZeroed(CountY);

		for (int Y = 0; Y < CountY; ++Y)
		{
			Nodes[X][Y] = -1;

			FVector Top(MinX + (X * Step), MinY + (Y * Step), BBox.Max.Z);
			FVector Bottom(MinX + (X * Step), MinY + (Y * Step), BBox.Min.Z);

			//DrawDebugLine(GetWorld(), Top, Bottom, FColor(0, 255, 0));

			FCollisionQueryParams CollisionQueryParams(true);
			FCollisionResponseParams CollisionResponseParams;
			CollisionResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
			CollisionResponseParams.CollisionResponse.WorldStatic = ECR_Block;

			FOverlapResult OverlapResult;
			FCollisionShape OverlapShape;
			OverlapShape.SetSphere(1.0f);

			bool bHasOverlap = GetWorld()->OverlapSingle(OverlapResult, Top, FQuat::Identity, ECC_WorldStatic, OverlapShape, CollisionQueryParams, CollisionResponseParams);

			if (bHasOverlap)
			{
				continue;
			}

			FHitResult HitResult;
			bool bHasCollision = GetWorld()->LineTraceSingle(HitResult, Top, Bottom, ECC_WorldStatic, CollisionQueryParams, CollisionResponseParams);

			if (bHasCollision)
			{
				//DrawDebugLine(GetWorld(), Top, HitResult.Location, FColor(255, 0, 0));
				//DrawDebugSphere(GetWorld(), HitResult.Location, 5.0f, 20, FColor(255, 0, 0));

				Nodes[X][Y] = FPlatformMath::CeilToInt(HitResult.Location.Z - BBox.Min.Z);
			}
			else
			{
				//DrawDebugLine(GetWorld(), Top, Bottom, FColor(0, 255, 0));

				Nodes[X][Y] = 0;
			}
		}
	}

	
	for (int X = 0; X < CountX; ++X)
	{
		for (int Y = 0; Y < CountY; ++Y)
		{
			int Node = Nodes[X][Y];

			if (Node >= 0)
			{
				FVector NodePosition(MinX + (X * Step), MinY + (Y * Step), BBox.Min.Z + Node);

				DrawDebugSphere(GetWorld(), NodePosition, 5.0f, 20, FColor(0, 255, 0));
			}
		}
	}
}
