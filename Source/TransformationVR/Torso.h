// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Parte.h"
#include "Components/SphereComponent.h"
#include "Torso.generated.h"

/**
 * 
 */
UCLASS()
class TRANSFORMATIONVR_API ATorso : public AParte
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ATorso();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * Torso;
	
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * ArticulacionCuello;
	
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	USphereComponent * ColisionCuello;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * ArticulacionHombroD;
	
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	USphereComponent * ColisionHombroD;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * ArticulacionCaderaD;
	
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	USphereComponent * ColisionCaderaD;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * ArticulacionHombroI;
	
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	USphereComponent * ColisionHombroI;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * ArticulacionCaderaI;
	
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	USphereComponent * ColisionCaderaI;

	
	
	
};
