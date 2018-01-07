// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Parte.h"
#include "Components/SphereComponent.h"
#include "ManoDerecha.generated.h"

/**
 * 
 */
UCLASS()
class TRANSFORMATIONVR_API AManoDerecha : public AParte
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	AManoDerecha();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * ManoD;
	
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * ArticulacionMunecaD;
	
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	USphereComponent * ColisionMunecaD;
	
	
	
};
