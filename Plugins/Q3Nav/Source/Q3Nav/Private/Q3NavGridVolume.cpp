// Fill out your copyright notice in the Description page of Project Settings.

#include "Q3NavPrivatePCH.h"
#include "Q3NavGridVolume.h"
#include "Q3MicroPanther.h"
#include "DrawDebugHelpers.h"

DECLARE_CYCLE_STAT(TEXT("Q3NavGridVolume Tick"), STAT_Q3NavGridVolume, STATGROUP_Q3Nav);
DECLARE_CYCLE_STAT(TEXT("Q3NavGridVolume Build Grid"), STAT_Q3NavGridVolume_BuildGrid, STATGROUP_Q3Nav);
DECLARE_CYCLE_STAT(TEXT("Q3NavGridVolume Find Path"), STAT_Q3NavGridVolume_FindPath, STATGROUP_Q3Nav);


AQ3NavGridVolume::AQ3NavGridVolume(const FObjectInitializer& ObjectInitializer) : AVolume(ObjectInitializer)
{
#if !UE_BUILD_SHIPPING
	DebugDrawingDelegate = FDebugDrawDelegate::CreateUObject(this, &AQ3NavGridVolume::DrawDebug);
	DebugDrawingDelegateHandle = UDebugDrawService::Register(TEXT("Navigation"), DebugDrawingDelegate);
#endif

	GetBrushComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Graph = new Q3Graph;
	Panther = new MicroPanther::MicroPather(Graph, 250, 6, false);

	GridSize = 50.0f;
}

AQ3NavGridVolume::~AQ3NavGridVolume()
{
#if UE_BUILD_SHIPPING
	UDebugDrawService::Unregister(DebugDrawingDelegateHandle);
#endif

	delete Panther;
	delete Graph;
}

void AQ3NavGridVolume::BuildGrid()
{
	SCOPE_CYCLE_COUNTER(STAT_Q3NavGridVolume_BuildGrid);

	FBoxSphereBounds Bounds = BrushComponent->CalcBounds(BrushComponent->ComponentToWorld);
	FBox BBox = Bounds.GetBox();

	float Step = GridSize;
	
	float MinX = FPlatformMath::CeilToInt(BBox.Min.X / Step) * Step;
	float MinY = FPlatformMath::CeilToInt(BBox.Min.Y / Step) * Step;

	int CountX = FPlatformMath::FloorToInt((BBox.Max.X - BBox.Min.X) / Step);
	int CountY = FPlatformMath::FloorToInt((BBox.Max.Y - BBox.Min.Y) / Step);

	TArray<TArray<int32>> NewHeights;
	NewHeights.AddZeroed(CountX);

	for (int X = 0; X < CountX; ++X)
	{
		NewHeights[X].AddZeroed(CountY);

		for (int Y = 0; Y < CountY; ++Y)
		{
			NewHeights[X][Y] = -1;

			FVector Top(MinX + (X * Step), MinY + (Y * Step), BBox.Max.Z);
			FVector Bottom(MinX + (X * Step), MinY + (Y * Step), BBox.Min.Z);

			//DrawDebugLine(GetWorld(), Top, Bottom, FColor(0, 255, 0));

			FCollisionQueryParams CollisionQueryParams(true);
			CollisionQueryParams.AddIgnoredActor(this);

			FCollisionResponseParams CollisionResponseParams;
			CollisionResponseParams.CollisionResponse.SetAllChannels(ECR_Block);

			//FOverlapResult OverlapResult;
			FCollisionShape OverlapShape;
			OverlapShape.SetBox(FVector(1, 1, 1));

			bool bHasOverlap = GetWorld()->OverlapTest(Top, FQuat::Identity, OverlapShape, CollisionQueryParams, FCollisionObjectQueryParams(ECC_WorldStatic));

			if (bHasOverlap)
			{
				continue;
			}

			CollisionResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
			CollisionResponseParams.CollisionResponse.WorldStatic = ECR_Block;

			FHitResult HitResult;
			//bool bHasCollision = GetWorld()->LineTraceSingle(HitResult, Top, Bottom, ECC_WorldStatic, CollisionQueryParams, CollisionResponseParams);
			bool bHasCollision = GetWorld()->LineTraceSingle(HitResult, Top, Bottom, CollisionQueryParams, FCollisionObjectQueryParams(ECC_WorldStatic));

			if (bHasCollision)
			{
				//DrawDebugLine(GetWorld(), Top, HitResult.Location, FColor(255, 0, 0));
				//DrawDebugSphere(GetWorld(), HitResult.Location, 5.0f, 20, FColor(255, 0, 0));

				NewHeights[X][Y] = FPlatformMath::CeilToInt(HitResult.Location.Z - BBox.Min.Z);
			}
			else
			{
				//DrawDebugLine(GetWorld(), Top, Bottom, FColor(0, 255, 0));

				NewHeights[X][Y] = 0;
			}
		}
	}

	
	//for (int X = 0; X < CountX; ++X)
	//{
	//	for (int Y = 0; Y < CountY; ++Y)
	//	{
	//		int Node = Nodes[X][Y];

	//		if (Node >= 0)
	//		{
	//			FVector NodePosition(MinX + (X * Step), MinY + (Y * Step), BBox.Min.Z + Node);

	//			DrawDebugSolidBox(GetWorld(), NodePosition, FVector(5, 5, 5), FColor(0, 255, 0));
	//		}
	//	}
	//}

	GridMinX = MinX;
	GridMinY = MinY;
	GridMinZ = BBox.Min.Z;
	GridStep = Step;
	GridCountX = CountX;
	GridCountY = CountY;

	Heights.AddZeroed(CountX * CountY);

	for (int Y = 0; Y < CountY; ++Y)
	{
		for (int X = 0; X < CountX; ++X)
		{
			Heights[X + (Y * CountX)] = NewHeights[X][Y];
		}
	}

	Graph->SetHeights(Heights, GridCountX, GridCountY);
}

TArray<FVector> AQ3NavGridVolume::FindPath(const FVector& Start, const FVector& End) const
{
	SCOPE_CYCLE_COUNTER(STAT_Q3NavGridVolume_FindPath);

	int StartX = FPlatformMath::FloorToInt((Start.X - GridMinX) / GridStep);
	int StartY = FPlatformMath::FloorToInt((Start.Y - GridMinY) / GridStep);
	int EndX = FPlatformMath::FloorToInt((End.X - GridMinX) / GridStep);
	int EndY = FPlatformMath::FloorToInt((End.Y - GridMinY) / GridStep);

	if (StartX < 0 || StartX >= GridCountX)
	{
		UE_LOG(Q3Nav, Error, TEXT("Start X is out of range. X : %d, Range : %d"), StartX, GridCountX);
		return TArray<FVector>();
	}

	if (StartY < 0 || StartY >= GridCountY)
	{
		UE_LOG(Q3Nav, Error, TEXT("Start Y is out of range. Y : %d, Range : %d"), StartY, GridCountY);
		return TArray<FVector>();
	}

	if (EndX < 0 || EndX >= GridCountX)
	{
		UE_LOG(Q3Nav, Error, TEXT("End X is out of range. X : %d, Range : %d"), EndX, GridCountX);
		return TArray<FVector>();
	}

	if (EndY < 0 || EndY >= GridCountY)
	{
		UE_LOG(Q3Nav, Error, TEXT("End Y is out of range. Y : %d, Range : %d"), EndY, GridCountY);
		return TArray<FVector>();
	}

	int32 TotalCost = 0;
	MP_VECTOR<void*> Path;
	Panther->Solve(Graph->Vec2ToState(FIntPoint(StartX, StartY)), Graph->Vec2ToState(FIntPoint(EndX, EndY)), &Path, &TotalCost);

	TArray<FVector> ResultPath;

	for (uint32 i = 0; i < Path.size(); ++i)
	{
		FIntPoint Position = Graph->StateToVec2(Path[i]);
		int Height = Heights[Position.X + (Position.Y * GridCountX)];

		FVector NodePosition(GridMinX + (Position.X * GridStep), GridMinY + (Position.Y * GridStep), GridMinZ + Height);

		ResultPath.Add(NodePosition);
	}

	return ResultPath;
}

#if !UE_BUILD_SHIPPING

void AQ3NavGridVolume::DrawDebug(UCanvas* Canvas, APlayerController*)
{
	const bool bVisible = (Canvas && Canvas->SceneView && !!Canvas->SceneView->Family->EngineShowFlags.Navigation);

    FBoxSphereBounds Bounds = BrushComponent->CalcBounds(BrushComponent->ComponentToWorld);
    FBox BBox = Bounds.GetBox();

    //DrawDebugPoint(GEngine->GetWorldContexts().Last().World(), BBox.Min, 100.0f, FColor(0, 255, 0, 255));

	DrawDebugBox(GetWorld(), BBox.GetCenter(), BBox.GetExtent(), FQuat::Identity, FColor(0, 255, 0));

	TArray<FVector> Verts;
	TArray<int32> Indices;
	Verts.Reserve(GridCountX * GridCountY * 4);
	Indices.Reserve(GridCountX * GridCountY * 6);

	// Config
	const float Size = GridSize * 0.2f;
	const float ZOffset = 10;

	for (int X = 0; X < GridCountX; ++X)
	{
		for (int Y = 0; Y < GridCountY; ++Y)
		{
			int Height = Heights[X + (Y * GridCountX)];

			if (Height != -1)
			{
				FVector Position(GridMinX + (X * GridStep), GridMinY + (Y * GridStep), GridMinZ + Height);
				//DrawDebugSolidBox(GetWorld(), Position, FVector(5, 5, 5), FColor(0, 255, 0));

				int32 Index = Verts.Num();

				Verts.Add(Position + FVector(-Size, -Size, ZOffset));
				Verts.Add(Position + FVector(Size, -Size, ZOffset));
				Verts.Add(Position + FVector(Size, Size, ZOffset));
				Verts.Add(Position + FVector(-Size, Size, ZOffset));

				Indices.Add(Index);
				Indices.Add(Index + 1);
				Indices.Add(Index + 2);
				Indices.Add(Index);
				Indices.Add(Index + 2);
				Indices.Add(Index + 3);
			}
		}
	}

	DrawDebugMesh(GetWorld(), Verts, Indices, FColor(0, 255, 0));
}

void AQ3NavGridVolume::FindPathTest()
{
	FIntPoint StartPoint(0, 0);
	FIntPoint EndPoint(6, 8);

	int32 TotalCost = 0;
	MP_VECTOR<void*> Path;
	Panther->Solve(Graph->Vec2ToState(StartPoint), Graph->Vec2ToState(EndPoint), &Path, &TotalCost);

	for (uint32 i = 0; i < Path.size(); ++i)
	{
		FIntPoint Position = Graph->StateToVec2(Path[i]);
		int Height = Heights[Position.X + (Position.Y * GridCountX)];

		FVector NodePosition(GridMinX + (Position.X * GridStep), GridMinY + (Position.Y * GridStep), GridMinZ + Height);

		DrawDebugSolidBox(GetWorld(), NodePosition, FVector(8, 8, 8), FColor(255, 0, 0));
	}
}

#endif

void AQ3NavGridVolume::PostLoad()
{
	Super::PostLoad();

	Graph->SetHeights(Heights, GridCountX, GridCountY);
}

#if WITH_EDITOR

void AQ3NavGridVolume::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	BuildGrid();
}

void AQ3NavGridVolume::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	BuildGrid();
}

#endif
