// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS_CharacterBase.h"


// Sets default values
AGAS_CharacterBase::AGAS_CharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGAS_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGAS_CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AGAS_CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

