// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "Tank.h"
#include "TankAIController.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("AIController Begin Play"));

	auto PlayerTank = GetPlayerTank();
	if (!PlayerTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController can't find player tank"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("AIController found: %s"), *(PlayerTank->GetName()))

}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ATank* ControlledTank = GetControlledTank();
	ATank* PlayerTank = GetPlayerTank();

	if (!ControlledTank || !PlayerTank) { return; }

	// TODO Move towards the player
	ControlledTank->AimAt(PlayerTank->GetActorLocation());

	// Fire if ready


}

ATank* ATankAIController::GetPlayerTank() const
{
	auto PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (!PlayerPawn) return nullptr;

	return Cast<ATank>(PlayerPawn);
}

ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

