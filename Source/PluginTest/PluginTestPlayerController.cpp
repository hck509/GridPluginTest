// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "PluginTest.h"
#include "PluginTestPlayerController.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Q3AiGridVolume.h"
#include "EngineUtils.h"

APluginTestPlayerController::APluginTestPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void APluginTestPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void APluginTestPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &APluginTestPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &APluginTestPlayerController::OnSetDestinationReleased);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &APluginTestPlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &APluginTestPlayerController::MoveToTouchLocation);
}

void APluginTestPlayerController::MoveToMouseCursor()
{
	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(Hit.ImpactPoint);
	}
}

void APluginTestPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void APluginTestPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	//APawn* const Pawn = GetPawn();
	//if (Pawn)
	//{
	//	UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
	//	float const Distance = FVector::Dist(DestLocation, Pawn->GetActorLocation());

	//	// We need to issue move command only if far enough in order for walk animation to play correctly
	//	if (NavSys && (Distance > 120.0f))
	//	{
	//		NavSys->SimpleMoveToLocation(this, DestLocation);
	//	}
	//}

	APawn* Pawn = GetPawn();
	if (Pawn)
	{
		float Distance = FVector::Dist(DestLocation, Pawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if (Distance > 120.0f)
		{
			for (TActorIterator<AActor> It(GetWorld()); It; ++It)
			{

			}

		}
	}
}

void APluginTestPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void APluginTestPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}
