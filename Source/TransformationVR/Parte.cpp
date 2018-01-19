// Fill out your copyright notice in the Description page of Project Settings.

#include "Parte.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Public/Math/Color.h"



// Sets default values
AParte::AParte()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Id = -1;
	ColorArticulacionNoConectada = FLinearColor(0.0f, 0.08336f, 0.234375f, 0.0f);
	ColorArticulacionSobrepuesta = FLinearColor(0.0f, 0.234375f, 0.097833f, 0.0f);
	//ColorArticulacionConectada = FLinearColor(0.0f, 0.234375, 0.212609f, 0.0f);
	ColorArticulacionConectada = FLinearColor(0.234375f, 0.0f, 0.002142f, 0.0f);

	bRotar = false;
	bRotarX = false;
	bRotarY = false;
	bRotarZ = false;
}

// Called when the game starts or when spawned
void AParte::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AParte::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*UE_LOG(LogClass, Log, TEXT("TICK"));

	UE_LOG(LogClass, Log, TEXT("bRotar: %b"), bRotar);
	if (bRotar) {

		UE_LOG(LogClass, Log, TEXT("bRotar: TRUE"));
		float DeltaX = signoX * VelocidadRotacion * DeltaTime;
		//float DeltaX = AngleXFin * DeltaTime;
		AngleXCurrent += DeltaX;
		if (AngleXCurrent <= AngleXFin) {//este if cambia si el anglefine es negativo
			FRotator DeltaRotation (0.0f, 0.0f, DeltaX);
			AddActorLocalRotation(DeltaRotation);
		}
		else {
			//llegue al angulo debo dejar de rotar
			bRotarX = false;
		}

		float DeltaY = signoY * VelocidadRotacion * DeltaTime;
		//float DeltaX = AngleXFin * DeltaTime;
		AngleYCurrent += DeltaY;
		if (AngleYCurrent <= AngleYFin) {
			FRotator DeltaRotation (DeltaY, 0.0f, 0.0f );
			AddActorLocalRotation(DeltaRotation);
		}
		else {
			//llegue al angulo debo dejar de rotar
			bRotarY = false;
		}

		float DeltaZ = signoZ * VelocidadRotacion * DeltaTime;
		//float DeltaX = AngleXFin * DeltaTime;
		AngleZCurrent += DeltaZ;
		if (AngleZCurrent <= AngleZFin) {
			FRotator DeltaRotation (0.0f, DeltaZ, 0.0f);
			AddActorLocalRotation(DeltaRotation);
		}
		else {
			//llegue al angulo debo dejar de rotar
			bRotarZ = false;
		}
		bRotar = bRotarX && bRotarY && bRotarZ;
	}*/
}
void AParte::AnimacionRotarTick(float DeltaTime) {
	//UE_LOG(LogClass, Log, TEXT("bRotar: %b"), bRotar);
	if (bRotar) {

		//UE_LOG(LogClass, Log, TEXT("bRotar: TRUE"));
		float DeltaX = signoX * VelocidadRotacion * DeltaTime;
		//float DeltaX = AngleXFin * DeltaTime;
		AngleXCurrent += DeltaX;
		if (AngleXCurrent <= AngleXFin) {//este if cambia si el anglefine es negativo
			FRotator DeltaRotation (0.0f, 0.0f, DeltaX);
			AddActorLocalRotation(DeltaRotation);
		}
		else {
			//llegue al angulo debo dejar de rotar
			bRotarX = false;
		}

		float DeltaY = signoY * VelocidadRotacion * DeltaTime;
		//float DeltaX = AngleXFin * DeltaTime;
		AngleYCurrent += DeltaY;
		if (AngleYCurrent <= AngleYFin) {
			FRotator DeltaRotation (DeltaY, 0.0f, 0.0f );
			AddActorLocalRotation(DeltaRotation);
		}
		else {
			//llegue al angulo debo dejar de rotar
			bRotarY = false;
		}

		float DeltaZ = signoZ * VelocidadRotacion * DeltaTime;
		//float DeltaX = AngleXFin * DeltaTime;
		AngleZCurrent += DeltaZ;
		if (AngleZCurrent <= AngleZFin) {
			FRotator DeltaRotation (0.0f, DeltaZ, 0.0f);
			AddActorLocalRotation(DeltaRotation);
		}
		else {
			//llegue al angulo debo dejar de rotar
			bRotarZ = false;
		}
		bRotar = bRotarX && bRotarY && bRotarZ;
	}
}

void AParte::AnimacionRotar(FVector CantidadRotacion) {// es mejor recibir los angulos como vectores, para manejra loas negativos coreectamente, ya que debo recibir el delta
	//separar por ejes, calculad pasos, y 
	//necsito 3 valores, 
	//necisto que la animaicon ocurra en 1 segudno de preferencia,
	//neceisto el valor actual
	UE_LOG(LogClass, Log, TEXT("Ejecutando Animacion %s"), *NombreParte);

	VelocidadRotacion = 45.0f;

	bRotar = true;

	bRotarX = true;
	AngleXIni = 0;
	AngleXCurrent = AngleXIni;
	AngleXFin = CantidadRotacion.X;
	//este signo x sale del valor angle fin
	if (AngleXFin >= 0.0f)
		signoX = 1; //o -1
	else
		signoX = -1;

	bRotarY = true;
	AngleYIni = 0;
	AngleYCurrent = AngleYIni;
	AngleYFin = CantidadRotacion.Y;
	//este signo x sale del valor angle fin
	if (AngleYFin >= 0.0f)
		signoY = 1; //o -1
	else
		signoY = -1;

	bRotarZ = true;
	AngleZIni = 0;
	AngleZCurrent = AngleZIni;
	AngleZFin = CantidadRotacion.Z;
	//este signo x sale del valor angle fin
	if (AngleZFin >= 0.0f)
		signoZ = 1; //o -1
	else
		signoZ = -1;
}

int AParte::IndiceColisionArticulacion(USphereComponent * ArticulacionSphere) {
	int Index;
	if (ColisionesArticualciones.Find(ArticulacionSphere, Index)) {
		return Index;
	}
	return -1;
}

void AParte::BuscarArticulacion() {
	bBuscarArticulacion = true;
}

void AParte::NoBuscarArticulacion() {
	bBuscarArticulacion = false;
}

void AParte::BuscandoArticulacion() {
	//creo que esta funcion esta demas, ya que la busqueda estara controlada por la funcion everlaping, de cada articulacion, que reaccionara siempre solo si el bool de buscar articulacion es verdadero
	//si al final no llego a usar esta funcion, la debo eliminar
}

void AParte::UnirConParteSobrepuesta() {//esta union deberia estar en la calse vrpawn o en otro lado, como la jerarquia, el punto es que si necesito esta union fisica de partes, dado que utilizo las partes, 
	//debo pensar com otratar esto junto con las trasnformaciones de forma coherente
	//quiza las trasnformaciones deberian utilizar l ainformacion aqui contenida, 
	if (bArticulacionSobrepuesta) {//si hay una articulacion sobrepuesta entoces si o si habra un overlaperparte, tener en cuenta que esta funcion deberia manerja los caso de articulaciones con varias partes por ahora dejar sencillo
		Padre = OverlapedParte;
		OverlapedParte->Hijos.Add(this);
		//aqui deberia haber mas partes de color
		bConectado = true;
		if (IndiceArticulacionSobrepuesta != -1 && IndiceArticulacionSobrepuestaOtro != -1) {
			CambiarColorArticulacion(IndiceArticulacionSobrepuesta, ColorArticulacionConectada);
			OverlapedParte->CambiarColorArticulacion(IndiceArticulacionSobrepuestaOtro, ColorArticulacionConectada);
			FVector OffsetWorld = GetActorLocation() - ColisionesArticualciones[IndiceArticulacionSobrepuesta]->GetComponentLocation();
			SetActorLocation(OverlapedParte->ColisionesArticualciones[IndiceArticulacionSobrepuestaOtro]->GetComponentLocation() + OffsetWorld);
			//la actualizacion del trasnform de la parte se hace fuera de esta funcon, ya que aqui no tengo acceso a la jerarquia

		}
		//tambien se deben inhabilitar los de alguna forma estas articulaciones, dejarlo asi por ahora

		//aqui debo actualizar las matrices de la transformaicon con los nuevos valores
		//para que se le pueda plicar los cambios en la jerarquia
	}
}

void AParte::CambiarColorArticulacion(int IndiceArticulacion, FLinearColor NuevoColor) {
	if (IndiceArticulacion >= 0 && IndiceArticulacion < ArticulacionesMaterialDynamic.Num()) {
		ArticulacionesMaterialDynamic[IndiceArticulacion]->SetVectorParameterValue(TEXT("Color"), NuevoColor);
	}
	//usar 3 colores, azul para las articulaciones no unidas, turquesa para cuando se este buscando y haya sobreposicion, y verde para cuando la articulacion este unida
}

void AParte::OnBeginOverlapArticulacion(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
	if (bBuscarArticulacion) {
		AParte * const ParteEncontrada = Cast<AParte>(OtherActor);
		if (ParteEncontrada) {
			USphereComponent * const ArticulacionEncontrada = Cast<USphereComponent>(OtherComp);//en realidad deberia manejar canles para que esto solo se active con los spherecomponents ocnfigurados como articulacion
			if (ArticulacionEncontrada) {
				//debo cambiar el color de esta articulacion y de la otra, parte para ello debo darle, el componente encontrado, y que lo busque en su array de sphere components, el indice me dira cual es su mesh corresondiente para cambiarlo de color
				bArticulacionSobrepuesta = true;
				OverlapedParte = ParteEncontrada;

				if(GEngine)//no hacer esta verificación provocaba error al iniciar el editor
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Articulacion Sobrepuesta"));
				//asi mismo debo saber cual de todas mis articulaciones es la que esta en contacto, para ello esta overlappedcomponent, que debo guardarlo despues de castear para luego buscar y saber su inidice para guardar el puntero de la parte padre, e inhabilitar en ese componente el contacto
				//debo deshabilitar para que en una nueva busqueda de alguna otra parte, no tome en cuenta las rticualciones unidas
				IndiceArticulacionSobrepuesta = IndiceColisionArticulacion(Cast<USphereComponent>(OverlappedComponent));// que busque mejor la parte en lugar de yo hacerlo aqui, por un lado reduzco busquedas si lo hago antes
				if(IndiceArticulacionSobrepuesta != -1)
					CambiarColorArticulacion(IndiceArticulacionSobrepuesta, ColorArticulacionSobrepuesta);
				IndiceArticulacionSobrepuestaOtro = ParteEncontrada->IndiceColisionArticulacion(ArticulacionEncontrada);
				if(IndiceArticulacionSobrepuestaOtro != -1)
					ParteEncontrada->CambiarColorArticulacion(IndiceArticulacionSobrepuestaOtro, ColorArticulacionSobrepuesta);
			}
		}
	}
}

void AParte::OnEndOverlapArticulacion(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex) {
	if (bBuscarArticulacion) {
		AParte * const ParteEncontrada = Cast<AParte>(OtherActor);
		if (ParteEncontrada) {
			USphereComponent * const ArticulacionEncontrada = Cast<USphereComponent>(OtherComp);//en realidad deberia manejar canles para que esto solo se active con los spherecomponents ocnfigurados como articulacion
			if (ArticulacionEncontrada) {
				bArticulacionSobrepuesta = false;
				OverlapedParte = nullptr;
				if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Saliendo Articulacion Sobrepuesta"));
				IndiceArticulacionSobrepuesta = IndiceColisionArticulacion(Cast<USphereComponent>(OverlappedComponent));// que busque mejor la parte en lugar de yo hacerlo aqui, por un lado reduzco busquedas si lo hago antes
				if(IndiceArticulacionSobrepuesta != -1)
					CambiarColorArticulacion(IndiceArticulacionSobrepuesta, ColorArticulacionNoConectada);
				int IndiceArticulacionSobrepuestaOtro = ParteEncontrada->IndiceColisionArticulacion(ArticulacionEncontrada);
				if(IndiceArticulacionSobrepuestaOtro != -1)
					ParteEncontrada->CambiarColorArticulacion(IndiceArticulacionSobrepuestaOtro, ColorArticulacionNoConectada);
			}
		}
		//tambien aplicar los cambios de color adecuados, por ahora solo dejare mensajes en consola
	}
}

void AParte::ActivarResaltado() {
	ParteMesh->SetCustomDepthStencilValue(255);
	bResaltada = true;
}

void AParte::DesactivarResaltado() {
	ParteMesh->SetCustomDepthStencilValue(0);
	bResaltada = false;
}

void AParte::CopiarTransform() {
	TWidget->TransformTemporal = GetActorTransform();
}

//creo que las articulaciones deberian ser un nuevo componente llamado articulacion, el cual tendria un pshere component y un statickmesh
//asi cuando este reaacione en el overlap, y deba cambiar el color por ejemplo, solo debo llamar a una funcion de este componente, lo mismo cuando usceda el endoverlap


