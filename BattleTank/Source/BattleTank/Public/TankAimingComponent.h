// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

//Enum for aiming state
UENUM()
enum class EFiringStare : uint8
{
	LOCKED,
	AIMING,
	RELOADING,
	OutOFAmmo
};

// Forward Declaration 
class UTankBarrel; 
class UTankTurret;
class AProjectile;

// Holds barrel's properties and Elevate method
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable, Category = Input)
	void Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);

	UFUNCTION(BlueprintCallable, Category = Fire)
	void Fire();

	UFUNCTION(BlueprintCallable, Category = Fire)
	int32 GetAmmoCount() const;

	void AimAt(FVector WorldSpaceAim);

	EFiringStare GetFiringState() const;

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = State)
	EFiringStare FiringState = EFiringStare::RELOADING;

private:
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<AProjectile> ProjectileBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = Firing)
	float LaunchSpeed = 4000;

	UPROPERTY(EditDefaultsOnly, Category = Firing)
	float ReloadTimeInSecond = 3;

	UPROPERTY(EditDefaultsOnly, Category = Firing)
	int32 AmmoCount = 3;

	UTankBarrel* Barrel = nullptr;
	UTankTurret* Turret = nullptr;

	double LastFireTime = 0;

	FVector AimDirection = FVector::ZeroVector;

	// Sets default values for this component's properties
	UTankAimingComponent();

	void MoveBarrelToAim(FVector AimDirection);

	bool IsBarrelMoving();
	
};
