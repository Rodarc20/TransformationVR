// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Parte.generated.h"

UCLASS()
class TRANSFORMATIONVR_API AParte : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AParte();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UCapsuleComponent * Colision;

	
	
};


//creo que sera mejor poner como centro un scene component, y al com hijos a colision los static mes etc, de esa forma no hay escalas