// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MyCharacterControlData.h"
#include "GameFramework/CharacterMovementComponent.h"

AMyCharacterPlayer::AMyCharacterPlayer()
{
	// Create camera-related components
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// CharacterControlData Setting
	CharacterControlDataManager.SetNum(static_cast<int32>(CameraView::END));
	static ConstructorHelpers::FObjectFinder<UMyCharacterControlData> CharacterContorlDataShoulderRef(TEXT("/Script/MyProject.MyCharacterControlData'/Game/MyGame/CharacterControlData/CCD_Shoulder.CCD_Shoulder'"));
	if (CharacterContorlDataShoulderRef.Object)
	{
		CharacterControlDataManager[static_cast<int32>(CameraView::Shoulder)] = CharacterContorlDataShoulderRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMyCharacterControlData> CharacterContorlDataQuaterrRef(TEXT("/Script/MyProject.MyCharacterControlData'/Game/MyGame/CharacterControlData/CCD_Quater.CCD_Quater'"));
	if (CharacterContorlDataQuaterrRef.Object)
	{
		CharacterControlDataManager[static_cast<int32>(CameraView::Quater)] = CharacterContorlDataQuaterrRef.Object;
	}

	// Action Setting
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_ShoulderMove.IA_ShoulderMove'"));
	if (nullptr != InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_ShoulderLook.IA_ShoulderLook'"));
	if (nullptr != InputActionShoulderLookRef.Object)
	{
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuaterMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_QuaterMove.IA_QuaterMove'"));
	if (nullptr != InputActionQuaterMoveRef.Object)
	{
		QuaterMoveAction = InputActionQuaterMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionToggleCharacterControlDataRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_ToggleCharacterControlData.IA_ToggleCharacterControlData'"));
	if (nullptr != InputActionToggleCharacterControlDataRef.Object)
	{
		ToggleCharacterControlDataAction = InputActionToggleCharacterControlDataRef.Object;
	}
}

void AMyCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	UMyCharacterControlData* CharacterControlDataShoulder = GetCharacterControlData(CameraView::Quater);
	if (CharacterControlDataShoulder)
	{
		SetCharacterControlData(CharacterControlDataShoulder);
	}
}

void AMyCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AMyCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &AMyCharacterPlayer::ShoulderLook);
	EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &AMyCharacterPlayer::QuaterMove);
	EnhancedInputComponent->BindAction(ToggleCharacterControlDataAction, ETriggerEvent::Triggered, this, &AMyCharacterPlayer::ToggleCharacterControlData);
}

UMyCharacterControlData* AMyCharacterPlayer::GetCharacterControlData(CameraView View)
{
	if (CharacterControlDataManager.Num() > static_cast<int32>(View))
	{
		return CharacterControlDataManager[static_cast<int32>(View)];
	}

	return nullptr;
}

void AMyCharacterPlayer::SetCharacterControlData(const UMyCharacterControlData* MyCharacterControlData)
{
	if (!MyCharacterControlData)
	{
		return;
	}
		
	// Pawn Section
	bUseControllerRotationYaw = MyCharacterControlData->bUseControllerRotationYaw;
	
	// Character Movement Section
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	if (CharacterMovementComponent)
	{
		CharacterMovementComponent->bUseControllerDesiredRotation = MyCharacterControlData->bUseControllerDesiredRotation;
		CharacterMovementComponent->bOrientRotationToMovement = MyCharacterControlData->bOrientationRotationToMovement;
		CharacterMovementComponent->RotationRate = MyCharacterControlData->RotationRate;
	}

	// Input Section
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		DefaultMappingContext = MyCharacterControlData->InputMappingContext;
		Subsystem->ClearAllMappings();
		if (DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Camera Section
	if (CameraBoom)
	{
		CameraBoom->TargetArmLength = MyCharacterControlData->TargetArmLength;
		CameraBoom->SetRelativeRotation(MyCharacterControlData->RelativeRotation);
		CameraBoom->bUsePawnControlRotation = MyCharacterControlData->bUsePawnControlRotation;
		CameraBoom->bInheritPitch = MyCharacterControlData->bInheritPitch;
		CameraBoom->bInheritYaw = MyCharacterControlData->bInheritYaw;
		CameraBoom->bInheritRoll = MyCharacterControlData->bInheritRoll;
		CameraBoom->bDoCollisionTest = MyCharacterControlData->bDoCollisionTest;
	}
}

void AMyCharacterPlayer::ToggleCharacterControlData(const FInputActionValue& Value)
{
	if (CameraView::Quater == CurrentCameraView)
	{
		CurrentCameraView = CameraView::Shoulder;
		SetCharacterControlData(CharacterControlDataManager[static_cast<int32>(CameraView::Shoulder)]);
	}
	else
	{
		CurrentCameraView = CameraView::Quater;
		SetCharacterControlData(CharacterControlDataManager[static_cast<int32>(CameraView::Quater)]);
	}
}

void AMyCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AMyCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AMyCharacterPlayer::QuaterMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	MovementVector.Normalize();
	float MovementVectorLength = MovementVector.Size();
	
	FVector Direction(MovementVector.X, MovementVector.Y, 0.0f);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(Direction).Rotator());
	AddMovementInput(Direction, MovementVectorLength);
}