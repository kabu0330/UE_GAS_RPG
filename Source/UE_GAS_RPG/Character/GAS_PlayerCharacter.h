// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS_Character.h"
#include "GAS_PlayerCharacter.generated.h"

class UGameplayAbility;
struct FInputActionValue;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;

UENUM(BlueprintType)
enum class EInputId : uint8
{
	Jump,
	Attack
};

UCLASS()
class UE_GAS_RPG_API AGAS_PlayerCharacter : public AGAS_Character
{
	GENERATED_BODY()

public:
	AGAS_PlayerCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void InitAbilitySystemActorInfo() override;
	void GiveAbility();

	virtual void SetupGASInputComponent();
	
protected:
	virtual void BeginPlay() override;

	// Input Action
	UPROPERTY(EditDefaultsOnly, Category = "InputAction")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "InputAction")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "InputAction")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "InputAction")
	TObjectPtr<UInputAction> AttackAction;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void JumpStart(const FInputActionValue& Value);
	void JumpEnd(const FInputActionValue& Value);

	// GAS
	UPROPERTY(EditDefaultsOnly, Category = "GAS|Ability")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "GAS|Ability")
	TMap<EInputId, TSubclassOf<UGameplayAbility>> InputAbilities;

	void GASInputPressed(EInputId InputId);
	void GASInputReleased(EInputId InputId);
	
private:
	// Camera 
	UPROPERTY(EditDefaultsOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	UPROPERTY(EditDefaultsOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> CameraComponent;
	
};
