// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "Tank.h"
#include "TankAIController.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto ControlledTank = Cast<ATank>(GetPawn());
	auto PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (!ControlledTank || !PlayerTank) { return; }

	//Move towards the player
	MoveToActor(PlayerTank, AcceptanceRadius); // TODO check radius is in cm

	ControlledTank->AimAt(PlayerTank->GetActorLocation());
	
	ControlledTank->Fire(); // TODO limit firing rate

}


