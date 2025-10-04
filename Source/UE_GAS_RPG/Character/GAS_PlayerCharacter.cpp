// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS_PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UE_GAS_RPG/Player/GAS_PlayerState.h"


AGAS_PlayerCharacter::AGAS_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
}

void AGAS_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGAS_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGAS_PlayerCharacter::PossessedBy(AController* NewController)
{
	// 서버에서 호출
	Super::PossessedBy(NewController);

	InitAbilitySystemActorInfo();
	GiveAbility();
}

void AGAS_PlayerCharacter::OnRep_PlayerState()
{
	// 클라에서 호출
	Super::OnRep_PlayerState();

	InitAbilitySystemActorInfo();
}

void AGAS_PlayerCharacter::InitAbilitySystemActorInfo()
{
	AGAS_PlayerState* PS = Cast<AGAS_PlayerState>(GetPlayerState());
	check(PS);

	UAbilitySystemComponent* AbilitySystemComponent = PS->GetAbilitySystemComponent();
	check(AbilitySystemComponent);

	ASC = AbilitySystemComponent;
	ASC->InitAbilityActorInfo(PS, this);

	AttributeSet = PS->GetAttributeSet();

	
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (IsValid(PC))
	{
		PC->ConsoleCommand(TEXT("showdebug abilitysystem"));
	}
}

void AGAS_PlayerCharacter::GiveAbility()
{
	if (!IsValid(ASC))
	{
		UE_LOG(LogTemp, Error, TEXT("AGAS_PlayerCharacter::GiveAbility : ASC is not valid"));
	}
	for (const auto& Ability : DefaultAbilities)
	{
		FGameplayAbilitySpec AbilitySpec(Ability);
		ASC->GiveAbility(AbilitySpec);
	}
	for (const auto& Ability : InputAbilities)
	{
		FGameplayAbilitySpec AbilitySpec(Ability.Value);
		AbilitySpec.InputID = static_cast<int>(Ability.Key);
		ASC->GiveAbility(AbilitySpec);
	}
}

void AGAS_PlayerCharacter::SetupGASInputComponent()
{
	if (IsValid(ASC) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AGAS_PlayerCharacter::GASInputPressed, EInputId::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AGAS_PlayerCharacter::GASInputReleased, EInputId::Jump);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AGAS_PlayerCharacter::GASInputPressed, EInputId::Attack);
	}
}

void AGAS_PlayerCharacter::GASInputPressed(EInputId InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(static_cast<int>(InputId));
	if (Spec)
	{
		Spec->InputPressed = true;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			ASC->TryActivateAbility(Spec->Handle);
		}
	}
}

void AGAS_PlayerCharacter::GASInputReleased(EInputId InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(static_cast<int>(InputId));
	if (Spec)
	{
		Spec->InputPressed = false;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputReleased(*Spec);
		}
	}
}

void AGAS_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (IsValid(EnhancedInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGAS_PlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGAS_PlayerCharacter::Look);

		// 점프
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AGAS_PlayerCharacter::Jump);
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AGAS_PlayerCharacter::StopJumping);
	}

	SetupGASInputComponent();
}

void AGAS_PlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D Vector = Value.Get<FVector2D>();
	
	if (GetController())
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardVector, Vector.X);
		AddMovementInput(RightVector, Vector.Y);
	}
}

void AGAS_PlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D Vector = Value.Get<FVector2D>();

	if (GetController())
	{
		AddControllerYawInput(Vector.X);
		//AddControllerPitchInput(-Vector.Y);
	}
}

void AGAS_PlayerCharacter::JumpStart(const FInputActionValue& Value)
{
	Jump();
}

void AGAS_PlayerCharacter::JumpEnd(const FInputActionValue& Value)
{
	StopJumping();
}



