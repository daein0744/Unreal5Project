// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MyCharacterControlData.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UMyCharacterControlData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	// Pawn Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn)
	uint32 bUseControllerRotationYaw : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterMovement)
	uint32 bOrientationRotationToMovement : 1;

	// Character Movement Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterMovement)
	uint32 bUseControllerDesiredRotation : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterMovement)
	FRotator RotationRate;

	// Input Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	// Camera Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpringArm)
	float TargetArmLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpringArm)
	FRotator RelativeRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpringArm)
	uint32 bUsePawnControlRotation : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpringArm)
	uint32 bInheritPitch : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpringArm)
	uint32 bInheritYaw : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpringArm)
	uint32 bInheritRoll : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpringArm)
	uint32 bDoCollisionTest : 1;
};
