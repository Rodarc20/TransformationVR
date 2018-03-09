// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Malla.generated.h"

UCLASS()
class TRANSFORMATIONVR_API AMalla : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMalla();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * Plano;

    void Mostrar();

    void Ocultar();
	
	
};
