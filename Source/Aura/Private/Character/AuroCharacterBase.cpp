// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuroCharacterBase.h"

// Sets default values
AAuroCharacterBase::AAuroCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AAuroCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

