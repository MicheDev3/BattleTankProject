// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "TankAimingComponent.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::BeginPlay()
{
	// So that first fire is after initial reload
	LastFireTime = GetWorld()->GetTimeSeconds();
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (AmmoCount <= 0)
	{
		FiringState = EFiringStare::OutOFAmmo;
	} else if ((GetWorld()->GetTimeSeconds() - LastFireTime) < ReloadTimeInSecond)
	{
		FiringState = EFiringStare::RELOADING;
	} else if (IsBarrelMoving())
	{
		FiringState = EFiringStare::AIMING;
	} else
	{
		FiringState = EFiringStare::LOCKED;
	}
}

void UTankAimingComponent::Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

EFiringStare UTankAimingComponent::GetFiringState() const
{
	return FiringState;
}

void UTankAimingComponent::Fire()
{
	if (FiringState == EFiringStare::AIMING || FiringState == EFiringStare::LOCKED)
	{

		if (!ensure(Barrel)) { return; }

		if (!ensure(ProjectileBlueprint)) { return; }

		// Spawn a projectile at the socket location on the barrel

		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetSocketRotation(FName("Projectile"))
			);

		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = GetWorld()->GetTimeSeconds();
		AmmoCount--;
	}

}

int32 UTankAimingComponent::GetAmmoCount() const
{
	return AmmoCount;
}

void UTankAimingComponent::AimAt(FVector WorldSpaceAim)
{
	if (!ensure(Barrel && Turret)) { return; }

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
		AimDirection = OutLunchVelocity.GetSafeNormal();
		MoveBarrelToAim(AimDirection);
	} 

}

void UTankAimingComponent::MoveBarrelToAim(FVector Direction)
{
	// Work-out difference between current barrel reaction, and AimDirection
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimRotator = Direction.Rotation();
	auto DeltaRotator = AimRotator - BarrelRotator;

	// Always yaw the shortest way
	Barrel->Elevate(DeltaRotator.Pitch);
	if (FMath::Abs(DeltaRotator.Yaw) < 180)
	{
		Turret->Rotate(DeltaRotator.Yaw);
	} else // Avoid going the long-way round
	{
		Turret->Rotate(-DeltaRotator.Yaw);
	}
}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel)) { return false; }

	auto BarrelForward = Barrel->GetForwardVector();
	
	return !BarrelForward.Equals(AimDirection, 0.01);
}
