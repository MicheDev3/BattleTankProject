// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "TankAimingComponent.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UTankAimingComponent::SetBarrelReference(UTankBarrel* BarrelToSet)
{
	if (!BarrelToSet) { return; }
	Barrel = BarrelToSet;
}

void UTankAimingComponent::SetTurretReference(UTankTurret* TurretToSet)
{
	if (!TurretToSet) { return; }
	Turret = TurretToSet;
}

void UTankAimingComponent::AimAt(FVector WorldSpaceAim, float LaunchSpeed)
{
	if (!Barrel || !Turret) { return; }

	FVector OutLunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile")); // if can't find the socket returns barrel position
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutLunchVelocity,
		StartLocation,
		WorldSpaceAim,
		LaunchSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);
	auto Time = GetWorld()->GetTimeSeconds();
	// Calculate the OutLaunchVelocity
	if (bHaveAimSolution)
	{
		auto AimDirection = OutLunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
		MoveTurretAround(AimDirection);
		//UE_LOG(LogTemp, Warning, TEXT("%f: Aim solve found"), Time)
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("%f: No aim solve found"), Time)
	}

}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	// Work-out difference between current barrel reaction, and AimDirection
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimRotator = AimDirection.Rotation();
	auto DeltaRotator = AimRotator - BarrelRotator;

	Barrel->Elevate(DeltaRotator.Pitch);
}

void UTankAimingComponent::MoveTurretAround(FVector AimDirection)
{
	// Work-out difference between current barrel reaction, and AimDirection
	auto TurretRotation = Turret->GetForwardVector().Rotation();
	auto AimRotator = AimDirection.Rotation();
	auto DeltaRotator = AimRotator - TurretRotation;

	Turret->Rotate(DeltaRotator.Yaw);
}

