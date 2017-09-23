// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankTurret.h"




void UTankTurret::Rotate(float RelativeSpeed)
{
	// Move the turret the right amount this frame
	// Given a max rotation speed, and the frame time
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, 1);
	auto RotationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto RawNewRotation = RelativeRotation.Yaw + RotationChange;
	RawNewRotation = FMath::Clamp<float>(RawNewRotation, 0, 360);
	SetRelativeRotation(FRotator(0, RawNewRotation, 0));
}
