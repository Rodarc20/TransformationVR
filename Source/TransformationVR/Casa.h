// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VRPawn.h"
#include "Bloque.h"
#include "Components/BoxComponent.h"
#include "Casa.generated.h"

UCLASS()
class TRANSFORMATIONVR_API ACasa : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACasa();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    TArray<ABloque *> Bloques;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    EVRCasaTask LastCasaTask;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    EVRCasaTask CurrentCasaTask;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    EVRCasaTask OldCasaTask;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    UBoxComponent * Zona;

    UFUNCTION(BlueprintCallable, Category = "TransformationVR")
    bool CasaArmada();

    UFUNCTION(BlueprintCallable, Category = "TransformationVR")
    void SetCasaTask(EVRCasaTask NewCasaTask);

    UFUNCTION(BlueprintCallable, Category = "TransformationVR")
    EVRCasaTask GetCasaTask();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR - Referencias")
    AVRPawn * Usuario;

    UFUNCTION()
    void OnBeginOverlapZona(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

    UFUNCTION()
	void OnEndOverlapZona(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex);

	
};
