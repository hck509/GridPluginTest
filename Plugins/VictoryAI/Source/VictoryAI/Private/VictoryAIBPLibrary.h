
#pragma once

#include "VictoryAIBPLibrary.generated.h"

// BP Library for You
//
// Written by Rama

//note about UBlueprintFunctionLibrary
// This class is a base class for any function libraries exposed to blueprints.
// Methods in subclasses are expected to be static, and no methods should be added to the base class.

UCLASS()
class VICTORYAI_API UVictoryAIBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
	
	/** SQUARED distanced between actors */
	UFUNCTION(BlueprintPure, Category = "VictoryAILibrary")
	static float Calculations__SquaredDistanceBetweenActors(AActor* Actor1,AActor* Actor2);
	
	
	//~~~
	
	/** Convert Rotator to Vector */
	UFUNCTION(BlueprintPure, Category = "VictoryBPLibrary", meta = (FriendlyName = "Vector To Rotator", CompactNodeTitle = "->", Keywords = "convert vector rotator conversion"))
	static FRotator Conversions__VectorToRotator(const FVector& TheVector);
	
	/** Convert Vector to Rotator*/
	UFUNCTION(BlueprintPure, Category = "VictoryBPLibrary", meta = (FriendlyName = "Rotator to Vector", CompactNodeTitle = "->", Keywords = "convert vector rotator conversion"))
	static FVector Conversions__RotatorToVector(const FRotator& TheRotator);
	
	//~~~
	
	/** Set Max Move Speed. Supply the Character whose Character Movement to change! Returns false if operation could not occur due to invalid Character or MovementComponent could not be obtained.*/
	UFUNCTION(BlueprintCallable, Category = "VictoryAILibrary", meta=(DefaultToSelf="TheCharacter"))
	static bool CharacterMovement__SetMaxMoveSpeed(ACharacter* TheCharacter, float NewMaxMoveSpeed);
	
	/** Replicated Jump Rama-Style. Use negative values for down, backward, and left. Returns false if operation could not occur. */
	UFUNCTION(BlueprintCallable, Category = "VictoryAILibrary")
	static bool CharacterMovement__RamaJump(ACharacter* TheCharacter, float UpAmount=3000, float ForwardAmount=3000, float RightAmount=0);
	
	/** Replicated Jump Rama-Style. Directions are relative to direction to target. Forward = direct line to target. Use negative values for down, backward, and left. Returns false if operation could not occur. */
	UFUNCTION(BlueprintCallable, Category = "VictoryAILibrary")
	static bool CharacterMovement__RamaJumpRelativeToTarget(ACharacter* TheCharacter, AActor* Target, float UpAmount=3000, float ForwardAmount=3000, float RightAmount=0);
	
	
	/** Is Controller's Pawn Falling? Returns the Controller's Pawn as a Character */
	UFUNCTION(BlueprintCallable, Category = "VictoryAILibrary")
	static ACharacter* CharacterMovement__ControllerPawnIsFalling(AActor* Controller, bool& IsFalling);
};

