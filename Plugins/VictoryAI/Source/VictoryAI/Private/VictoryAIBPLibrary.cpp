/*

	By Rama

*/

#include "VictoryAIPrivatePCH.h"
#include "VictoryAIBPLibrary.h"
//////////////////////////////////////////////////////////////////////////
// UVictoryAIBPLibrary

UVictoryAIBPLibrary::UVictoryAIBPLibrary(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	
}

FRotator UVictoryAIBPLibrary::Conversions__VectorToRotator(const FVector& TheVector)
{
	return TheVector.Rotation();
}
FVector UVictoryAIBPLibrary::Conversions__RotatorToVector(const FRotator& TheRotator)
{
	return TheRotator.Vector();
}

float UVictoryAIBPLibrary::Calculations__SquaredDistanceBetweenActors(AActor* Actor1,AActor* Actor2)
{
	if(!Actor1 || !Actor2)
	{
		return -1;
	}
	
	return FVector::DistSquared(Actor1->GetActorLocation(),Actor2->GetActorLocation());
}

//~~~

ACharacter* UVictoryAIBPLibrary::CharacterMovement__ControllerPawnIsFalling(AActor* Controller,bool& IsFalling)
{
	AController* TheController = Cast<AController>(Controller);
	if(!TheController)
	{
		return NULL;
	}
	
	ACharacter* TheCharacter = Cast<ACharacter>(TheController->GetPawn());
	
	if(!Controller || ! TheCharacter )
	{
		return NULL;
	}
	
	if(!TheCharacter->CharacterMovement)
	{
		return NULL;
	}
	
	IsFalling = TheCharacter->CharacterMovement->IsFalling();
	return TheCharacter;
}

bool UVictoryAIBPLibrary::CharacterMovement__SetMaxMoveSpeed(ACharacter* TheCharacter, float NewMaxMoveSpeed)
{
	if(!TheCharacter)
	{
		return false;
	}
	if(!TheCharacter->CharacterMovement)
	{
		return false;
	}
	
	TheCharacter->CharacterMovement->MaxWalkSpeed = NewMaxMoveSpeed;
	
	return true;
}


bool UVictoryAIBPLibrary::CharacterMovement__RamaJump(
	ACharacter* TheCharacter, 
	float UpAmount, 
	float ForwardAmount, 
	float RightAmount
){
	if(!TheCharacter)
	{
		return false;
	}
	if(!TheCharacter->GetRootComponent())
	{
		return false;
	}
	if(!TheCharacter->CharacterMovement)
	{
		return false;
	}
	
	const USceneComponent* RootComp = TheCharacter->GetRootComponent();
	
	const FVector Up			= RootComp->GetUpVector() * UpAmount;
	const FVector Forward	= RootComp->GetForwardVector() * ForwardAmount;
	const FVector Right 		= RootComp->GetRightVector() * RightAmount;
	
	FVector& Laaaaunch = TheCharacter->CharacterMovement->PendingLaunchVelocity;
	Laaaaunch = FVector::ZeroVector;
	Laaaaunch += Up;
	Laaaaunch += Forward;
	Laaaaunch += Right;
	
	return true;
}
bool UVictoryAIBPLibrary::CharacterMovement__RamaJumpRelativeToTarget(ACharacter* TheCharacter, AActor* Target, float UpAmount, float ForwardAmount, float RightAmount)
{
	if(!TheCharacter || !Target)
	{
		return false;
	}
	if(!TheCharacter->GetRootComponent())
	{
		return false;
	}
	if(!TheCharacter->CharacterMovement)
	{
		return false;
	}
	
	const USceneComponent* RootComp = TheCharacter->GetRootComponent();
	
	const FRotator AngleToTarget = (Target->GetActorLocation() - TheCharacter->GetActorLocation() ).Rotation();
	
	const FRotator RightRot 	= FRotator(0,AngleToTarget.Yaw + 90,0);
	
	const FVector Up			= RootComp->GetUpVector() * UpAmount;
	const FVector Forward	= AngleToTarget.Vector() * ForwardAmount;
	const FVector Right 		= RightRot.Vector() * RightAmount;
	
	
	
	//Launch after move finishes
	FVector& Laaaaunch = TheCharacter->CharacterMovement->PendingLaunchVelocity;
	Laaaaunch = FVector::ZeroVector;
	Laaaaunch += Up;
	Laaaaunch += Forward;
	Laaaaunch += Right;
	
	//Launch Immediate
	TheCharacter->CharacterMovement->Velocity += Laaaaunch;
	
	return true;
}
	
