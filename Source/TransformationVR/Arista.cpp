// Fill out your copyright notice in the Description page of Project Settings.

#include "Arista.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Engine/StaticMesh.h"


// Sets default values
AArista::AArista()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    Radio = 0.5f;
    Distancia = 6.0f;//esta distancia, inclue las semisferas
    //hay que instanciar y al final recien rotar, lamentablement estas aristas dependeran de los nodos, una vez instanciasdas debo llamar a una funcion apra que actualice sus posicion y rotación, etc
    AristaMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AristaMesh"));
    RootComponent = AristaMesh;
    //AristaMesh->SetupAttachment(RootComponent);

    //este estatic mesh es de 100 x100x100
    //static ConstructorHelpers::FObjectFinder<UStaticMesh> AristaMeshAsset(TEXT("/Engine/BasicShapes/Cylinder"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    static ConstructorHelpers::FObjectFinder<UStaticMesh> AristaMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/AristaPrueba2.AristaPrueba2'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (AristaMeshAsset.Succeeded()) {
        AristaMesh->SetCollisionProfileName(FName(TEXT("Arista")));
        AristaMesh->SetStaticMesh(AristaMeshAsset.Object);//este objeto tiene el pivot en la parte inferior
        static ConstructorHelpers::FObjectFinder<UMaterial> AristaMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaZ.FlechaZ'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (AristaMaterialAsset.Succeeded()) {
            AristaMesh->SetMaterial(0, AristaMaterialAsset.Object);
        }
        AristaMesh->SetWorldScale3D(FVector(Radio, Radio, Distancia/6));//0.06f//este valor se debe calcular en base al radio,  y escalas, esta funcoin toma el diametro, por lo tnto seria algo como 2*radio/100
        //AristaMesh->SetWorldScale3D(FVector(2*Radio*Escala/100, 2*Radio*Escala/100, Distancia/100));//0.06f//este valor se debe calcular en base al radio,  y escalas, esta funcoin toma el diametro, por lo tnto seria algo como 2*radio/100
        //estos 2*, es por que el pivot esta enel centro de los static mesh
        //la distancia no necesita ser escalada, se basa en las posiciones de otros
    }
}

// Called when the game starts or when spawned
void AArista::BeginPlay() {
	Super::BeginPlay();
	
}

// Called every frame
void AArista::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void AArista::Actualizar() {
    FVector Diferencia = Target->GetComponentLocation() - Source->GetComponentLocation();//deberia ser solo los valores de y y z, funciona ahora por que en ambos x es 0, pero falla en el de abajo
    FVector NewLocation(Source->GetComponentLocation() + Diferencia/2);//ejes invertidos a los recibidos
    FRotator NewRotation = FRotationMatrix::MakeFromZ(Diferencia).Rotator();
    SetActorLocation(NewLocation);
    //SetActorRelativeRotation(NewRotation);// debria ser world?
    SetActorRotation(NewRotation);

    Distancia = GetTransform().InverseTransformVector(Diferencia).Size();
    //Distancia = Diferencia.Size();//el -1 deberia sser otro valor dianmico en funcion del tamaño de los nodos y la escala
    //AristaCollision->SetCapsuleHalfHeight(Distancia/2);//modificar el tamaño del componete que hace la coslision, en este aso el capsule componente hace que actualizar el overlap aumente, en ese caso como no lo necesito por el momento al realizar traslaciones, esto puede estar desactivado hasta que lo necesite en laguna interaccion, y llamar a este cambio recien cuado suelte el boton de traslado
    //AristaMesh->SetWorldScale3D(FVector(2*Radio*Escala/2, 2*Radio*Escala/2, Distancia/6));//funciona bien para el largo de la arsita, pero no para el grosor
    //AristaMesh->SetWorldScale3D(FVector(Radio, Radio, Distancia/6));//funciona bien para el grosor de la arista pero no para el largo
    AristaMesh->SetWorldScale3D(FVector(Radio, Radio, Diferencia.Size()/6));//funciona bien para el grosor de la arista pero no para el largo
}

