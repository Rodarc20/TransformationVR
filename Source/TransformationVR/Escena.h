// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Materials/Material.h"
#include "TransformacionWidget.h"
#include "Escena.generated.h"

UCLASS()
class TRANSFORMATIONVR_API AEscena : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEscena();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    //usar felchas mas grandes
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UTransformacionWidget * TWidget;

    void MostrarWidget();

    void OcultarWidget();
};
