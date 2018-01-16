// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "TransformacionWidget.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TRANSFORMATIONVR_API UTransformacionWidget : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTransformacionWidget();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * FlechaX;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * FlechaY;
		
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * FlechaZ;
	
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * Origen;
		
	UFUNCTION(BlueprintCallable, Category = "Transformaciones")
	void MostrarWidget();

	UFUNCTION(BlueprintCallable, Category = "Transdormaciones")
	void OcultarWidget();

	UFUNCTION(BlueprintCallable, Category = "Transformaciones")
	void MostrarResaltado();
	
	UFUNCTION(BlueprintCallable, Category = "Transformaciones")
	void OculatarResaltado();
};
