// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Parte.h"
#include "Components/SphereComponent.h"
#include "Cabeza.generated.h"

/**
 * 
 */
UCLASS()
class TRANSFORMATIONVR_API ACabeza : public AParte
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ACabeza();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * Cabeza;
	
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * ArticulacionCuello;
	
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	USphereComponent * ColisionCuello;
	

};
