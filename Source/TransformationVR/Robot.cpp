// Fill out your copyright notice in the Description page of Project Settings.

#include "Robot.h"
#include "VRPawn.h"
#include "Parte.h"
#include "Jerarquia.h"
#include <stack>
#include "Transformacion.h"
#include "PilaOpenGL.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "TransformacionWidget.h"
#include "Components/BoxComponent.h"
#include "PanelBotones.h"
#include "PuntoTraslacion.h"


// Sets default values
ARobot::ARobot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    Zona = CreateDefaultSubobject<UBoxComponent>(TEXT("Zona"));
    RootComponent = Zona;
    Zona->InitBoxExtent(FVector(100.0f, 100.0f, 180.0f));
    Zona->OnComponentBeginOverlap.AddDynamic(this, &ARobot::OnBeginOverlapZona);
    Zona->OnComponentEndOverlap.AddDynamic(this, &ARobot::OnEndOverlapZona);

    static ConstructorHelpers::FClassFinder<APilaOpenGL> PilaClass(TEXT("BlueprintGeneratedClass'/Game/Trasnformation/Blueprints/Jerarquia/PilaOpenGL_BP.PilaOpenGL_BP_C'"));
    if (PilaClass.Succeeded()) {
        //if(GEngine)//no hacer esta verificación provocaba error al iniciar el editor
            //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Pila encontrado."));
        TipoPila = PilaClass.Class;
    }

    static ConstructorHelpers::FClassFinder<ANodo> NodoClass(TEXT("BlueprintGeneratedClass'/Game/Trasnformation/Blueprints/Jerarquia/Nodo_BP.Nodo_BP_C'"));
    if (NodoClass.Succeeded()) {
        if(GEngine)//no hacer esta verificación provocaba error al iniciar el editor
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("TipoNodo encontrado."));
        TipoNodo = NodoClass.Class;
    }

	DistanciaLaserMaxima = 200.0f;
    AlturaRobot = FVector(0.0f, 0.0f, 20.0f);
    VelocidadTraslacion = 25.0f;

	CurrentJerarquiaTask = EVRJerarquiaTask::EArmarTask;
    //SetJerarquiaTask(EVRJerarquiaTask::ENoTask);
    //CurrentJerarquiaTask = EVRJerarquiaTask::ENoTask;
	//CurrentJerarquiaTask = EVRJerarquiaTask::ERotationTask;
	HitEje = ETransformacionEje::ENone;
	EjeSeleccionado = ETransformacionEje::ENone;
}

// Called when the game starts or when spawned
void ARobot::BeginPlay()
{
	Super::BeginPlay();
	
	Partes.SetNum(10);
	Jerarquias.SetNum(10);
	Nodos.SetNum(10);
	Transformaciones.SetNum(10);

	TArray<AActor *> PartesEncontradas;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AParte::StaticClass(), PartesEncontradas);
	//UE_LOG(LogClass, Log, TEXT("Numero de Partes Encontradas: %d"), PartesEncontradas.Num());
	for (int i = 0; i < PartesEncontradas.Num(); i++) {
		//if(GEngine)//no hacer esta verificación provocaba error al iniciar el editor
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("PARTES ENCONTRADAS"));
		//UE_LOG(LogClass, Log, TEXT("Partes Encontradas %d"), i);
		AParte * const ParteEncontrada = Cast<AParte>(PartesEncontradas[i]);
		if(ParteEncontrada)
			Partes[ParteEncontrada->Id] = ParteEncontrada;
	}

	UWorld * const World = GetWorld();

	for (int i = 0; World && i < Partes.Num(); i++) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;

		FVector SpawnLocation(Partes[i]->Id * 5);
		
		SpawnLocation = GetTransform().TransformPosition(SpawnLocation);
		SpawnLocation += FVector(-300.0f, 0.0f, 200.0f);
		ANodo * const NodoInstanciado = World->SpawnActor<ANodo>(TipoNodo, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
		Nodos[Partes[i]->Id] = NodoInstanciado;
		NodoInstanciado->IdParte = Partes[i]->Id;//para el texto del numero, quiza este tipo de funcionalidad deberia estar encapsulada en alguna funcion de la clase nodo
		NodoInstanciado->CambiarNombreParte(Partes[i]->Id);
		//NodoInstanciado->bActualizado = false;
		NodoInstanciado->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);//segun el compilador de unral debo usar esto
	}

	for (int i = 0; i < Transformaciones.Num(); i++) {//se supone que son 1-
		Transformaciones[i] = new Transformacion;
		Transformaciones[i]->ParteAsociada = Partes[i];
		Transformaciones[i]->ActualizarDesdeParte();
	}

	UE_LOG(LogClass, Log, TEXT("Instanciadas las transformaciones"));

	//las 10 trsanformaciones ya tiene asociada su parte

	//Instanciando jerarquia
	//UWorld * const World = GetWorld();
	if (World) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		FVector SpawnLocation(-50.0f, 180.0f, 30.0f);

		//Jerarquia = World->SpawnActor<AJerarquia>(AJerarquia::StaticClass(), SpawnLocation - FVector(-30.0f, 0.0f, 0.0f), FRotator::ZeroRotator, SpawnParams);
        //esta jerarquia no uso
		//creadas las jerarquias para cada parte
		for (int i = 0; i < Jerarquias.Num(); i++) {
			Jerarquias[i] = World->SpawnActor<AJerarquia>(AJerarquia::StaticClass(), SpawnLocation + (i+1)*FVector(0.0f, -30.0f, 0.0f), FRotator::ZeroRotator, SpawnParams);
		}
        Jerarquia = Jerarquias[0];
	}

	UE_LOG(LogClass, Log, TEXT("Instanciadas las jerarquias"));

	//copiar las transformaciones a cada jerarquia
	//y establecer la trasnformacion root que le corresponde, quiza ya no tenga que tener un array de transformaciones ne la jerarquia si ya lo tengo en el robot, por ahora dejarlo en los dos
	for (int i = 0; i < Jerarquias.Num(); i++) {//asociando partes a la jerarquia
		Jerarquias[i]->Root = Transformaciones[i];
		Jerarquias[i]->CantidadPartes = 1;
		for (int j = 0; j < Transformaciones.Num(); j++) {
			Jerarquias[i]->TransformacionesPartesPunteros[j] = Transformaciones[j];
			Jerarquias[i]->Nodos[j] = Nodos[j];
		}
		Nodos[i]->AttachToActor(Jerarquias[i], FAttachmentTransformRules::KeepRelativeTransform);
        Nodos[i]->SetActorRelativeLocation(FVector::ZeroVector);
	}
	UE_LOG(LogClass, Log, TEXT("Datos copiados a las jerarquias"));

	//la pila hoara se instancia cuando queda una sola jerarquia

	if (World) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		FVector SpawnLocation(-50.0f, -110.0f, 40.0f);

		PilaCodigo = World->SpawnActor<APilaOpenGL>(TipoPila, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
        if (PilaCodigo) {
            PilaCodigo->Ocultar();
        }
		//if (Jerarquia)
			//Jerarquia->PilaCodigo = PilaCodigo;
	}

    AVRPawn * MyVRPawn = Cast<AVRPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (MyVRPawn) {
		Usuario = MyVRPawn;
		//MyVRPawn->Jerarquia = Jerarquia;
		//Aqui le copie la jerarquia al usario, es decir el necesita conocer las jerarquias, para algo, para comprobar si esta unido a algo, o si hay jerarquia
		//ahora sempre hay jeraruia, solo que necesitara acceder a todas para llmamar a las guncionces de cada una segun sea el caso, ya que no accedmos a la clase robot aun que se podria hacer por esta via.
		MyVRPawn->Jerarquias.SetNum(10);
		for (int i = 0; i < MyVRPawn->Jerarquias.Num(); i++) {
			MyVRPawn->Jerarquias[i] = Jerarquias[i];
		}

        RightController = MyVRPawn->MotionControllerRight;
        LeftController = MyVRPawn->MotionControllerLeft;
	}
	UE_LOG(LogClass, Log, TEXT("Informacion pasada al VRPawn"));

    if (World) {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = Instigator;

        APanelBotones * const PanelBotones = World->SpawnActor<APanelBotones>(APanelBotones::StaticClass(), GetActorLocation() + FVector(50.0f, 0.0f, 0.0f), FRotator(0.0f, 180.0f, 0.0f), SpawnParams);
        //PanelBotones->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);//segun el compilador de unral debo usar esto
    }
}

// Called every frame
void ARobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Jerarquia->EjecutarAnimacionTick(DeltaTime);//esto esta ya que por alguna razon las partes no ejecutan sus funciones tick
	//esto tal vez tenga pque pasarse al blueprint
	//esta ejecucion de animacion se da solo en la tarea de rotar, en el armado no , por ello no necesito ejecutar essa funcion en el arreglo de jerarquias


	//BuscandoComponenteRotacionConLaser();
    switch (CurrentJerarquiaTask) {
        case EVRJerarquiaTask::EArmarTask: {//si el juego esta en Playing
            //activate the spawn volumes
			//BuscandoParteConLaser();//por ahora, pero este no va aqui

        }
        break;//no se como funciona esto
        case EVRJerarquiaTask::ERotationTask: {//si  perdimos el juego
			if (ParteSeleccionada) {
				if (EjeSeleccionado != ETransformacionEje::ENone) {
					//se supone que cuando se selecciono se grabo la posicion en el presesed
					//BuscarIntereseccionEjeRotacion();
					RotarParteEnEje();
                    //Jerarquia->Imprimir();
					Jerarquia->ActualizarCodigoConRotaciones();//esta linea esta producion errores
				}
				else {
					BuscandoComponenteRotacionConLaser();
					if (HitEje == ETransformacionEje::ENone) {
						BuscandoParteConLaser();//por ahora, pero este no va aqui
					}
				}
			}
			else {
				BuscandoParteConLaser();
			}
			//una vez que tenga una parte si la selecciono recien se ahbilita y busco componentes

        }
        break;//no se como funciona esto
        case EVRJerarquiaTask::ETraslationTask: {//si  perdimos el juego
            AnimacionTick(DeltaTime);
        }
        break;
        default:
        case EVRJerarquiaTask::ENoTask: {//unknown/ default state
            //no hacer nada
        }
        break;//no se como funciona esto
    }
    GrabRightTick();
    GrabLeftTick();
}

FVector ARobot::BuscarParte(AParte *& ParteEncontrada) {
    FCollisionQueryParams ParteTraceParams = FCollisionQueryParams(FName(TEXT("TraceParte")), true, this);
    FVector PuntoInicial = RightController->GetComponentLocation();//lo mismo que en teorioa, GetComponentTransfor().GetLocation();
    FVector Vec = RightController->GetForwardVector();
    FVector PuntoFinal = PuntoInicial + Vec*DistanciaLaserMaxima;
    //PuntoInical = PuntoInicial + Vec * 10;//para que no se choque con lo que quiero, aun que no deberia importar
    TArray<TEnumAsByte<EObjectTypeQuery> > TiposObjetos;
    TiposObjetos.Add(EObjectTypeQuery::ObjectTypeQuery7);//Nodo
    //TiposObjetos.Add(EObjectTypeQuery::ObjectTypeQuery2);//World dynamic, separado esta funcionando bien, supongo que tendre que hacer oto trace par saber si me estoy chocando con la interfaz, y no tener encuenta esta busqueda
    //podria agregar los world static y dynamic, para asi avitar siempre encontrar algun nodo que este destrar de algun menu, y que por seleccionar en el menu tambien le de click a el
    TArray<AActor*> vacio;
    FHitResult Hit;
    bool trace = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), PuntoInicial, PuntoFinal, TiposObjetos, false, vacio, EDrawDebugTrace::None, Hit, true, FLinearColor::Blue);//el none es para que no se dibuje nada
    //hit se supone que devovera al actor y el punto de impacto si encontró algo, castearlo a nodo, y listo
    if (trace) {
        //solo que al agregar el worldynamic ,tengo que castear y verificar
        ParteEncontrada = Cast<AParte>(Hit.Actor.Get());
        /*if (NodoEncontrado) {//no estaba
            //en que momento debo incluir la seccion del label? despues de todo esto, en otra funcion, o en este mismo codigo?
            return Hit.ImpactPoint;
        }*/
        //DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 1.5f, 6, FColor::Black, false, 0.0f, 0, 0.25f);
        
        return Hit.ImpactPoint;//si quito toto el if anterior debo activar esta linea, y liesto
    }
    //y si esta funcion es solo para esto, y luego ya verifico si es tal o cual cosa, en otra parte del codigo?

    //DrawDebugLine(GetWorld(), SourceNodo->GetActorLocation(), TargetNodo->GetActorLocation(), FColor::Black, false, -1.0f, 0, Radio*Escala);
    ParteEncontrada = nullptr;
    return FVector::ZeroVector;// los casos manejarlos afuera
}

void ARobot::BuscandoParteConLaser() {
    AParte * ParteEncontrada;
    FVector PuntoImpacto = BuscarParte(ParteEncontrada);
    //if (ParteEncontrada && !Interaction->IsOverHitTestVisibleWidget()) {//comprobamos la interaccion para que no se detecte lo que este detras del menu
    if (ParteEncontrada) {//comprobamos la interaccion para que no se detecte lo que este detras del menu
		//podre interactuar con los menus??
        if (HitParte && HitParte != ParteEncontrada) {//quiza se pueda hacer con el boleano, debo ocultar si es que es diferente al de ahora,
            //if (MostrarLabel) {//no sera necesario
                //HitNodo->OcultarNombre();
                //NodoEncontrado->MostrarNombre();
            //}
			//que se resalte en color blanco
            HitParte->DesactivarResaltado();//esto si, podria ser para saber que le estoy dando
            ParteEncontrada->ActivarResaltado();
        }
        else {
            //if (MostrarLabel) {
                //NodoEncontrado->MostrarNombre();
            //}
            if(!ParteEncontrada->bResaltada)
                ParteEncontrada->ActivarResaltado();
        }
        HitParte= ParteEncontrada;//podria dejar ests 3 lineas, y borrar las de adentro
        bHitParte= true;
        ImpactPoint = PuntoImpacto;
		//ParteSeleccionada = ParteEncontrada;//sacar esto , es temporal solo para prubas, pero intentar eliminarlo
    }
    else {//si no estoy golpeando algun nodo
        if (HitParte) {
            //if (MostrarLabel) {
                //HitNodo->OcultarNombre();
            //.}
            HitParte->DesactivarResaltado();
            //HitNodo = nullptr;
            //bHitNodo = false;
            //ImpactPoint = PuntoImpacto;
        }
        HitParte = nullptr;
        bHitParte = false;
        ImpactPoint = PuntoImpacto;
        //el caso contrario, seria encontrar como lo deje con el if anterior, asi que no se hace nada
    }
    //todo esto podria ser una sola funcion
    //hasta aqui he verificado si encontre algun nodo, pero no si encotnre un, menu, y tampoo he ejecutado los cambios visuales
    //hagamos algo visual, antes de incluir los menus
	if (bHitParte) {//quiza la verificacion que hago sobre si hubo cambio o no de hit nodo, ayude a evitar ciertos calculos, tal vez, por ejemplo si el laser siempre esa al maximo, no tiene mucho sentido seimpre setear con el mismo valor
		Usuario->EfectoImpacto->SetWorldLocation(ImpactPoint);
		//esto no estaba
		//if (MostrarLabel && HitNodo->Nombre->IsVisible()) {//esto podria ser util si tengo elemntos que necesito que roten en la propia parte
			//HitNodo->ActualizarRotacionNombre(Usuario->VRCamera->GetComponentLocation());
		//}
		if (Usuario->LaserActual() != 1) {
			Usuario->CambiarLaser(1);
		}
		if (!Usuario->EfectoImpacto->IsActive()) {//para el caso del puntero, no lo usao ahora
			Usuario->EfectoImpacto->ActivateSystem();
		}
		Usuario->CambiarPuntoFinal(ImpactPoint);
	}
	else {
		Usuario->EfectoImpacto->SetWorldLocation(RightController->GetComponentLocation() + RightController->GetForwardVector()*DistanciaLaserMaxima);
		//esta funcion deberia administrar le punto recbido, y verficar si acutalmente el puntero de interaccion esta sobre el menu, y tomar el adecuado para cada situacion
		if (Usuario->LaserActual() != 0) {
			Usuario->CambiarLaser(0);
		}
		if (Usuario->EfectoImpacto->IsActive()) {
			Usuario->EfectoImpacto->DeactivateSystem();
		}
		Usuario->CambiarPuntoFinal(RightController->GetComponentLocation() + RightController->GetForwardVector()*DistanciaLaserMaxima);//debieria tener un punto por defecto, pero mejor lo dejamos asi
	}

    //creo que la parte de interacion con el menu, deberia estar manajedo por el pawn, asi dentro de la funcion cambiar punto final, evaluo o verifico que no este primero algun menu
    //la pregunta es como hare con los clicks digamos para el contenido, si estoy buscando algun nodo, quiza igual deberia evitar que de algun click, si tengo algun overlap en ferente, evaluar la mejor forma de hacer todo esto
    //o usar esto en lugar de un trace solo que debo hacer esto antes de que haga cambios visuales, obtener el punto y evaluar,  antes de setear lo de hit nodo y dema
}

void ARobot::BuscandoComponenteRotacionConLaser() {
    FVector PuntoInicial = RightController->GetComponentLocation();//lo mismo que en teorioa, GetComponentTransfor().GetLocation();
    FVector Vec = RightController->GetForwardVector();
    FVector PuntoFinal = PuntoInicial + Vec*DistanciaLaserMaxima;
	ETransformacionEje EjeEncontrado;
	FVector Impacto;
	//UE_LOG(LogClass, Log, TEXT("Buscando componente"));
	if (ParteSeleccionada->TWidget->ColisionRotacion(PuntoInicial, PuntoFinal, EjeEncontrado, Impacto)) {//si he colisionado con algo
		// igual que el mecanismo de las partes
		//activar el resaltadao de la rotacion, pero tambien cmaibne rl aposicino del laser
		//UE_LOG(LogClass, Log, TEXT("Componente encontrado"));
		HitEje = EjeEncontrado;
		Usuario->CambiarPuntoFinal(Impacto);
		Usuario->EfectoImpacto->SetWorldLocation(Impacto);
		if (Usuario->LaserActual() != 2) {
			Usuario->CambiarLaser(4);
		}
		if (!Usuario->EfectoImpacto->IsActive()) {//para el caso del puntero, no lo usao ahora
			Usuario->EfectoImpacto->ActivateSystem();
		}
		Usuario->CambiarPuntoFinal(Impacto);
		Usuario->EfectoImpacto->SetWorldLocation(Impacto);
	}
	else {
		HitEje = ETransformacionEje::ENone;
		Usuario->EfectoImpacto->SetWorldLocation(RightController->GetComponentLocation() + RightController->GetForwardVector()*DistanciaLaserMaxima);
		//esta funcion deberia administrar le punto recbido, y verficar si acutalmente el puntero de interaccion esta sobre el menu, y tomar el adecuado para cada situacion
		if (Usuario->LaserActual() != 0) {
			Usuario->CambiarLaser(0);
		}
		if (Usuario->EfectoImpacto->IsActive()) {
			Usuario->EfectoImpacto->DeactivateSystem();
		}
		Usuario->CambiarPuntoFinal(RightController->GetComponentLocation() + RightController->GetForwardVector()*DistanciaLaserMaxima);//debieria tener un punto por defecto, pero mejor lo dejamos asi
	}
}

bool ARobot::ColisionRotacion(FVector Inicio, FVector Fin, ETransformacionEje & EjeColisionado, FVector & Impact) {
    FCollisionQueryParams ParteTraceParams = FCollisionQueryParams(FName(TEXT("TraceX")), true);
	FHitResult Hit;
    TArray<TEnumAsByte<EObjectTypeQuery> > TiposObjetos;
    TiposObjetos.Add(EObjectTypeQuery::ObjectTypeQuery9);//Nodo
    TArray<AActor*> vacio;
    bool trace = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), Inicio, Fin, TiposObjetos, true, vacio, EDrawDebugTrace::None, Hit, true, FLinearColor::Blue);//el none es para que no se dibuje nada
	if (trace) {
		UE_LOG(LogClass, Log, TEXT("Encontrado"));
		Impact = Hit.ImpactPoint;
		return true;
	}
	Impact = FVector::ZeroVector;
	EjeColisionado = ETransformacionEje::ENone;
	return false;
}

void ARobot::SetJerarquiaTask(EVRJerarquiaTask NewJerarquiaTask) {
	CurrentJerarquiaTask = NewJerarquiaTask;
	UE_LOG(LogClass, Log, TEXT("Cambiando Tarea"));

    //para manejar la aparicion y desaparicion de elementos visuales
    switch (CurrentJerarquiaTask) {
        case EVRJerarquiaTask::EArmarTask: {//si el juego esta en Playing
            PilaCodigo->Ocultar();
        }
        break;//no se como funciona esto
        case EVRJerarquiaTask::ERotationTask: {//si  perdimos el juego
            PilaCodigo->Mostrar();
            if (Jerarquia)
                Jerarquia->ActualizarCodigoConRotaciones();
        }
        break;//no se como funciona esto
        case EVRJerarquiaTask::ETraslationTask: {//si  perdimos el juego
            //PilaCodigo->Mostrar();
            UE_LOG(LogClass, Log, TEXT("Traslation Task"));
            PuntosTraslacion.Add(FVector(0.0f, 0.0f, AlturaRobot.Z));
            CreatePuntoTraslacion(PuntosTraslacion[0]);

        }
        break;//no se como funciona esto
        default:
        case EVRJerarquiaTask::ENoTask: {//unknown/ default state
            //no hacer nada
            PilaCodigo->Ocultar();
        }
        break;//no se como funciona esto
    }
}

EVRJerarquiaTask ARobot::GetJerarquiaTask() {
	return CurrentJerarquiaTask;
}

void ARobot::BuscarIntereseccionEjeRotacion() {
    switch (EjeSeleccionado) {
        case ETransformacionEje::EEjeX: {
			PuntoInterseccion = ParteSeleccionada->TWidget->InterseccionLineaPlanoYZ(RightController->GetComponentLocation(), RightController->GetForwardVector());
			if (PuntoInterseccion.X != -1.0f) {//punto imposible
				Usuario->CambiarLaser(1);
				//este trasnfor debe ser con el trasnform del la parte el transform del objeto twidget
				//deberia encapsular eso en una funcion para trasnforar en funcion del trasnfor copia o la del objeto adecuado
				//FVector PuntoInterseccionWorld = ParteSeleccionada->TWidget->GetComponentTransform().TransformPosition(PuntoInterseccion);
				FVector PuntoInterseccionWorld = ParteSeleccionada->TWidget->TransformTemporal.TransformPosition(PuntoInterseccion);
				Usuario->CambiarPuntoFinal(PuntoInterseccionWorld);//convierte de local a global, para que sea usado por el vrpawn que lo usa en espacio global
				Usuario->EfectoImpacto->SetWorldLocation(PuntoInterseccionWorld);
				//dibujar laser apropiado
			}
        }
        break;//no se como funciona esto
        case ETransformacionEje::EEjeY: {
			PuntoInterseccion = ParteSeleccionada->TWidget->InterseccionLineaPlanoXZ(RightController->GetComponentLocation(), RightController->GetForwardVector());
			if (PuntoInterseccion.Y != -1.0f) {//punto imposible
				Usuario->CambiarLaser(1);
				//este trasnfor debe ser con el trasnform del la parte el transform del objeto twidget
				//deberia encapsular eso en una funcion para trasnforar en funcion del trasnfor copia o la del objeto adecuado
				//FVector PuntoInterseccionWorld = ParteSeleccionada->TWidget->GetComponentTransform().TransformPosition(PuntoInterseccion);
				FVector PuntoInterseccionWorld = ParteSeleccionada->TWidget->TransformTemporal.TransformPosition(PuntoInterseccion);
				Usuario->CambiarPuntoFinal(PuntoInterseccionWorld);//convierte de local a global, para que sea usado por el vrpawn que lo usa en espacio global
				Usuario->EfectoImpacto->SetWorldLocation(PuntoInterseccionWorld);
				//dibujar laser apropiado
			}
        }
        break;//no se como funciona esto
        case ETransformacionEje::EEjeZ: {
			PuntoInterseccion = ParteSeleccionada->TWidget->InterseccionLineaPlanoXY(RightController->GetComponentLocation(), RightController->GetForwardVector());
			if (PuntoInterseccion.Z != -1.0f) {//punto imposible
				Usuario->CambiarLaser(1);
				//este trasnfor debe ser con el trasnform del la parte el transform del objeto twidget
				//deberia encapsular eso en una funcion para trasnforar en funcion del trasnfor copia o la del objeto adecuado
				//FVector PuntoInterseccionWorld = ParteSeleccionada->TWidget->GetComponentTransform().TransformPosition(PuntoInterseccion);
				FVector PuntoInterseccionWorld = ParteSeleccionada->TWidget->TransformTemporal.TransformPosition(PuntoInterseccion);
				Usuario->CambiarPuntoFinal(PuntoInterseccionWorld);//convierte de local a global, para que sea usado por el vrpawn que lo usa en espacio global
				Usuario->EfectoImpacto->SetWorldLocation(PuntoInterseccionWorld);
				//dibujar laser apropiado
			}
        }
        break;//no se como funciona esto
        default:
        case ETransformacionEje::ENone: {//unknown/ default state
            //no hacer nada
        }
        break;//no se como funciona esto
    }

	UE_LOG(LogClass, Log, TEXT("Punto Intereseccino Inicial: %f, %f, %f"), PuntoInterseccion.X, PuntoInterseccion.Y, PuntoInterseccion.Z);
}

void ARobot::RotarParteEnEje() {
	BuscarIntereseccionEjeRotacion();//buscando punto, retorna el punto local en pUnto Interseccion
	//en funcon del eje selccionado acutal debo hacer calculos para saber el angulo de diferencia
    switch (EjeSeleccionado) {
        case ETransformacionEje::EEjeX: {
			FVector PuntoInterseccionWorld = ParteSeleccionada->TWidget->TransformTemporal.TransformPosition(PuntoInterseccion);

			//esta forma funciona bien, pero gira en sentido diferente
			//float DeltaAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(PuntoInterseccion.GetClampedToSize(1.0f, 1.0f), PuntoRotacionInicial.GetClampedToSize(1.0f, 1.0f))));
			//float DeltaSing = FVector::CrossProduct(PuntoInterseccion.GetClampedToSize(1.0f, 1.0f), PuntoRotacionInicial.GetClampedToSize(1.0f, 1.0f)).Y;//esto es por que el signo es impotante para saber si fue un angulo mayor de 180 o no

			float DeltaAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(PuntoInterseccion.GetSafeNormal(), PuntoRotacionInicial.GetSafeNormal())));
			float DeltaSing = FVector::CrossProduct(PuntoInterseccion.GetSafeNormal(), PuntoRotacionInicial.GetSafeNormal()).X;//esto es por que el signo es impotante para saber si fue un angulo mayor de 180 o no

			//float DeltaAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(PuntoRotacionInicial.GetClampedToSize(1.0f, 1.0f), PuntoInterseccion.GetClampedToSize(1.0f, 1.0f))));
			//float DeltaSing = FVector::CrossProduct(PuntoRotacionInicial.GetClampedToSize(1.0f, 1.0f), PuntoInterseccion.GetClampedToSize(1.0f, 1.0f)).Y;//esto es por que el signo es impotante para saber si fue un angulo mayor de 180 o no
			if (DeltaSing >= 0) {
				DeltaAngle = 360-DeltaAngle;
			}
			//UE_LOG(LogClass, Log, TEXT("DeltaAngle: %f"), DeltaAngle);
			//UE_LOG(LogClass, Log, TEXT("Rotator Inicial: %f, %f, %f"), RotacionInicial.Roll, RotacionInicial.Pitch, RotacionInicial.Yaw);
			//FRotator VariacionRotation(DeltaAngle, 0.0f, 0.0f);
			//AngleTemp = DeltaAngle;
			//AngleTemp = -DeltaAngle;
			FRotator VariacionRotation(0.0f, 0.0f, -DeltaAngle);
			//UE_LOG(LogClass, Log, TEXT("Variacion Rotacion: %f, %f, %f"), VariacionRotation.Roll, VariacionRotation.Pitch, VariacionRotation.Yaw);
			ParteSeleccionada->SetActorRelativeRotation(RotacionInicial);
			ParteSeleccionada->AddActorLocalRotation(VariacionRotation);
			//consigue la rotacion sin errores pero siempre en sentido contrario!!! 
			FRotator RotacionCambiada = ParteSeleccionada->GetRootComponent()->GetRelativeTransform().GetRotation().Rotator();
			//UE_LOG(LogClass, Log, TEXT("Rotator Cambiado: %f, %f, %f"), RotacionCambiada.Roll, RotacionCambiada.Pitch, RotacionCambiada.Yaw);


			AnguloActual = DeltaAngle;
			//UE_LOG(LogClass, Log, TEXT("AnguloAnterior: %f"), AnguloAnterior);
			//UE_LOG(LogClass, Log, TEXT("AnguloActual: %f"), AnguloActual);
			float DeltaFrame = AnguloActual - AnguloAnterior;
			//UE_LOG(LogClass, Log, TEXT("DeltaFrame: %f"), DeltaFrame);

			if (DeltaFrame > 180.0f) {
				DeltaFrame = 360.0f - DeltaFrame;
			}
			else if (DeltaFrame < -180.0f ) {
				DeltaFrame = DeltaFrame + 360.0f;
			}
			//UE_LOG(LogClass, Log, TEXT("DeltaFrame: %f"), DeltaFrame);
			AnguloAcum += DeltaFrame;

			//UE_LOG(LogClass, Log, TEXT("AnguloAcum: %f"), AnguloAcum);

			AnguloAnterior = AnguloActual;



        }
        break;//no se como funciona esto
        case ETransformacionEje::EEjeY: {
			FVector PuntoInterseccionWorld = ParteSeleccionada->TWidget->TransformTemporal.TransformPosition(PuntoInterseccion);

			//esta forma funciona bien, pero gira en sentido diferente
			//float DeltaAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(PuntoInterseccion.GetClampedToSize(1.0f, 1.0f), PuntoRotacionInicial.GetClampedToSize(1.0f, 1.0f))));
			//float DeltaSing = FVector::CrossProduct(PuntoInterseccion.GetClampedToSize(1.0f, 1.0f), PuntoRotacionInicial.GetClampedToSize(1.0f, 1.0f)).Y;//esto es por que el signo es impotante para saber si fue un angulo mayor de 180 o no

			float DeltaAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(PuntoInterseccion.GetSafeNormal(), PuntoRotacionInicial.GetSafeNormal())));
			float DeltaSing = FVector::CrossProduct(PuntoInterseccion.GetSafeNormal(), PuntoRotacionInicial.GetSafeNormal()).Y;//esto es por que el signo es impotante para saber si fue un angulo mayor de 180 o no

			//float DeltaAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(PuntoRotacionInicial.GetClampedToSize(1.0f, 1.0f), PuntoInterseccion.GetClampedToSize(1.0f, 1.0f))));
			//float DeltaSing = FVector::CrossProduct(PuntoRotacionInicial.GetClampedToSize(1.0f, 1.0f), PuntoInterseccion.GetClampedToSize(1.0f, 1.0f)).Y;//esto es por que el signo es impotante para saber si fue un angulo mayor de 180 o no
			if (DeltaSing >= 0) {
				DeltaAngle = 360-DeltaAngle;
			}
			//if (DeltaSing >= 0) {
				//DeltaAngle = DeltaAngle * -1;
			//}
			//por ahora aplicar la rotacion
			//AngleTemp = DeltaAngle;
			//AngleTemp = -DeltaAngle;
			//UE_LOG(LogClass, Log, TEXT("DeltaAngle: %f"), DeltaAngle);
			//UE_LOG(LogClass, Log, TEXT("Rotator Inicial: %f, %f, %f"), RotacionInicial.Roll, RotacionInicial.Pitch, RotacionInicial.Yaw);
			//FRotator VariacionRotation(DeltaAngle, 0.0f, 0.0f);
			FRotator VariacionRotation(-DeltaAngle, 0.0f, 0.0f);
			//ParteSeleccionada->SetActorRelativeRotation(RotacionInicial + VariacionRotation);
			//ParteSeleccionada->AddActorLocalRotation(VariacionRotation);

			//FRotator VariacionRotation = RotacionInicial;
			//VariacionRotation.Add(DeltaAngle, 0.0f, 0.0f);
			//UE_LOG(LogClass, Log, TEXT("Variacion Rotacion: %f, %f, %f"), VariacionRotation.Roll, VariacionRotation.Pitch, VariacionRotation.Yaw);
			//ParteSeleccionada->SetActorRelativeRotation(VariacionRotation);


			ParteSeleccionada->SetActorRelativeRotation(RotacionInicial);
			ParteSeleccionada->AddActorLocalRotation(VariacionRotation);
			//consigue la rotacion sin errores pero siempre en sentido contrario!!! 


			FRotator RotacionCambiada = ParteSeleccionada->GetRootComponent()->GetRelativeTransform().GetRotation().Rotator();
			//UE_LOG(LogClass, Log, TEXT("Rotator Cambiado: %f, %f, %f"), RotacionCambiada.Roll, RotacionCambiada.Pitch, RotacionCambiada.Yaw);

			AnguloActual = DeltaAngle;
			//UE_LOG(LogClass, Log, TEXT("AnguloAnterior: %f"), AnguloAnterior);
			//UE_LOG(LogClass, Log, TEXT("AnguloActual: %f"), AnguloActual);
			float DeltaFrame = AnguloActual - AnguloAnterior;
			//UE_LOG(LogClass, Log, TEXT("DeltaFrame: %f"), DeltaFrame);

			if (DeltaFrame > 180.0f) {
				DeltaFrame = 360.0f - DeltaFrame;
			}
			else if (DeltaFrame < -180.0f ) {
				DeltaFrame = DeltaFrame + 360.0f;
			}
			//UE_LOG(LogClass, Log, TEXT("DeltaFrame: %f"), DeltaFrame);
			AnguloAcum += DeltaFrame;

			//UE_LOG(LogClass, Log, TEXT("AnguloAcum: %f"), AnguloAcum);

			AnguloAnterior = AnguloActual;

        }
        break;//no se como funciona esto
        case ETransformacionEje::EEjeZ: {
			FVector PuntoInterseccionWorld = ParteSeleccionada->TWidget->TransformTemporal.TransformPosition(PuntoInterseccion);

			//esta forma funciona bien, pero gira en sentido diferente
			//float DeltaAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(PuntoInterseccion.GetClampedToSize(1.0f, 1.0f), PuntoRotacionInicial.GetClampedToSize(1.0f, 1.0f))));
			//float DeltaSing = FVector::CrossProduct(PuntoInterseccion.GetClampedToSize(1.0f, 1.0f), PuntoRotacionInicial.GetClampedToSize(1.0f, 1.0f)).Y;//esto es por que el signo es impotante para saber si fue un angulo mayor de 180 o no

			float DeltaAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(PuntoInterseccion.GetSafeNormal(), PuntoRotacionInicial.GetSafeNormal())));
			float DeltaSing = FVector::CrossProduct(PuntoInterseccion.GetSafeNormal(), PuntoRotacionInicial.GetSafeNormal()).Z;//esto es por que el signo es impotante para saber si fue un angulo mayor de 180 o no

			//float DeltaAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(PuntoRotacionInicial.GetClampedToSize(1.0f, 1.0f), PuntoInterseccion.GetClampedToSize(1.0f, 1.0f))));
			//float DeltaSing = FVector::CrossProduct(PuntoRotacionInicial.GetClampedToSize(1.0f, 1.0f), PuntoInterseccion.GetClampedToSize(1.0f, 1.0f)).Y;//esto es por que el signo es impotante para saber si fue un angulo mayor de 180 o no
			if (DeltaSing >= 0) {
				DeltaAngle = 360-DeltaAngle;
			}
			//AngleTemp = DeltaAngle;
			//AngleTemp = -DeltaAngle;
			//UE_LOG(LogClass, Log, TEXT("DeltaAngle: %f"), DeltaAngle);
			//UE_LOG(LogClass, Log, TEXT("Rotator Inicial: %f, %f, %f"), RotacionInicial.Roll, RotacionInicial.Pitch, RotacionInicial.Yaw);
			//FRotator VariacionRotation(DeltaAngle, 0.0f, 0.0f);
			FRotator VariacionRotation(0.0f, DeltaAngle, 0.0f);
			//UE_LOG(LogClass, Log, TEXT("Variacion Rotacion: %f, %f, %f"), VariacionRotation.Roll, VariacionRotation.Pitch, VariacionRotation.Yaw);
			ParteSeleccionada->SetActorRelativeRotation(RotacionInicial);
			ParteSeleccionada->AddActorLocalRotation(VariacionRotation);
			//consigue la rotacion sin errores pero siempre en sentido contrario!!! 
			FRotator RotacionCambiada = ParteSeleccionada->GetRootComponent()->GetRelativeTransform().GetRotation().Rotator();
			//UE_LOG(LogClass, Log, TEXT("Rotator Cambiado: %f, %f, %f"), RotacionCambiada.Roll, RotacionCambiada.Pitch, RotacionCambiada.Yaw);

			AnguloActual = DeltaAngle;
			//UE_LOG(LogClass, Log, TEXT("AnguloAnterior: %f"), AnguloAnterior);
			//UE_LOG(LogClass, Log, TEXT("AnguloActual: %f"), AnguloActual);
			float DeltaFrame = AnguloActual - AnguloAnterior;
			//UE_LOG(LogClass, Log, TEXT("DeltaFrame: %f"), DeltaFrame);

			if (DeltaFrame > 180.0f) {
				DeltaFrame = 360.0f - DeltaFrame;
			}
			else if (DeltaFrame < -180.0f ) {
				DeltaFrame = DeltaFrame + 360.0f;
			}
			//UE_LOG(LogClass, Log, TEXT("DeltaFrame: %f"), DeltaFrame);
			AnguloAcum += DeltaFrame;

			//UE_LOG(LogClass, Log, TEXT("AnguloAcum: %f"), AnguloAcum);

			AnguloAnterior = AnguloActual;
        }
        break;//no se como funciona esto
        default:
        case ETransformacionEje::ENone: {//unknown/ default state
            //no hacer nada
        }
        break;//no se como funciona esto
    }

}

void ARobot::SelectPressed() {
}

void ARobot::SelectReleased() {
}

void ARobot::CreatePuntoTraslacion(FVector PuntoSpawn) {
    UWorld * const World = GetWorld();
    if (World) {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = Instigator;

        APuntoTraslacion * PuntoT = World->SpawnActor<APuntoTraslacion>(APuntoTraslacion::StaticClass(), PuntoSpawn - AlturaRobot, FRotator::ZeroRotator, SpawnParams);
        //BotonTrasladar->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);//segun el compilador de unral debo usar esto
        PuntosTraslacionActors.Add(PuntoT);
    }
}

void ARobot::AnimacionTick(float DeltaTime) {
    if (bAnimacionTrasladar) {//boleano podria estar controlado por por el witch ya que este funcion se llama solocuando este en esta tarea
        UE_LOG(LogClass, Log, TEXT("bAnimacionTrasladar true"));
        if (bTrasladar) {//para cuando se termina de traslacdar hacia el punto objetivo

            //traslado con lerp
            UE_LOG(LogClass, Log, TEXT("bTrasladar: true"));

            //Jerarquia->Root->SetWorldLocation(FMath::Lerp(PuntoInicialTraslacion, PuntoFinalTraslacion, DeltaTime * VelocidadTraslacion / (PuntoFinalTraslacion - PuntoInicialTraslacion).Size()));
            //Jerarquia->Root->SetWorldLocation(FMath::Lerp(PuntoInicialTraslacion, PuntoFinalTraslacion, Alpha));
            Alpha += DeltaTime * VelocidadTraslacion / (PuntoFinalTraslacion - PuntoInicialTraslacion).Size();
            Jerarquia->Root->ParteAsociada->SetActorLocation(FMath::Lerp(PuntoInicialTraslacion, PuntoFinalTraslacion, Alpha));
            Jerarquia->Root->ActualizarDesdeParte();
            //Alpha += 0.02f;
            //esta funcionando? o me falta actualizar algo?
            //if (Jerarquia->Root->GetWorldLocation() == PuntoFinalTraslacion) {
            //if (Jerarquia->Root->ParteAsociada->GetActorLocation() == PuntoFinalTraslacion) {
            UE_LOG(LogClass, Log, TEXT("Alpha: %f"), Alpha);
            if (Alpha >= 1.0f) {
                UE_LOG(LogClass, Log, TEXT("bTrasladar: false"), Alpha);
                bTrasladar = false;
            }
        }
        else if (!bTrasladar && IdPuntoTrasladoActual < PuntosTraslacion.Num() ) {
            UE_LOG(LogClass, Log, TEXT("Llmando animacon traslacion punto %d"), IdPuntoTrasladoActual);
            UE_LOG(LogClass, Log, TEXT("Punto Inicial (%f, %f, %f)"), PuntosTraslacion[IdPuntoTrasladoActual-1].X, PuntosTraslacion[IdPuntoTrasladoActual-1].Y, PuntosTraslacion[IdPuntoTrasladoActual-1].Z);
            UE_LOG(LogClass, Log, TEXT("Punto Final (%f, %f, %f)"), PuntosTraslacion[IdPuntoTrasladoActual].X, PuntosTraslacion[IdPuntoTrasladoActual].Y, PuntosTraslacion[IdPuntoTrasladoActual].Z);
            AnimacionTrasladar(PuntosTraslacion[IdPuntoTrasladoActual - 1], PuntosTraslacion[IdPuntoTrasladoActual]);
            IdPuntoTrasladoActual++;
        }
        else if(IdPuntoTrasladoActual == PuntosTraslacion.Num()) {
            bAnimacionTrasladar = false;
            Jerarquia->DetenerCicloAnimacion(Jerarquia->Root->ParteAsociada->Id);
        }
    }
}

void ARobot::IniciarAnimacion() {
    UE_LOG(LogClass, Log, TEXT("Iniciando animacion"));
    bAnimacionTrasladar = true;
    IdPuntoTrasladoActual = 1;
    //usando el siclo de jerarquia, cuando llegue al final debo detenerla
    Jerarquia->EjecutarCicloAnimacion(Jerarquia->Root->ParteAsociada->Id);
}

void ARobot::AnimacionTrasladar(FVector PuntoInicial, FVector PuntoFinal) {
    bTrasladar = true;
    PuntoInicialTraslacion = PuntoInicial;
    PuntoFinalTraslacion = PuntoFinal;
    Alpha = 0.0f;
    //debo hacer que mire en esa direccion
    Jerarquia->Root->ParteAsociada->SetActorLocation(PuntoInicialTraslacion);
    FRotator NewRotation = FRotationMatrix::MakeFromX(PuntoFinalTraslacion - PuntoInicialTraslacion).Rotator();
    Jerarquia->Root->ParteAsociada->SetActorRotation(NewRotation);
    Jerarquia->Root->ActualizarDesdeParte();
}

void ARobot::GrabRightPressed() {
    switch (CurrentJerarquiaTask) {
        case EVRJerarquiaTask::EArmarTask: {
			//GrabRightArmarPressed();
        }
        break;
        case EVRJerarquiaTask::ERotationTask: {
			//GrabRightRotarPressed();
			if (ParteSeleccionada) {
				if (HitEje != ETransformacionEje::ENone) {
					EjeSeleccionado = HitEje;
					ParteSeleccionada->TWidget->SeleccionarEjeRotacion(EjeSeleccionado);
					//ParteSeleccionada->TWidget->CopiarTransform();
					ParteSeleccionada->CopiarTransform();
					RotacionInicial = ParteSeleccionada->GetRootComponent()->GetRelativeTransform().GetRotation().Rotator();
					BuscarIntereseccionEjeRotacion();
					PuntoRotacionInicial = PuntoInterseccion;
					AnguloAnterior = 0.0f;
					AnguloAcum = 0.0f;

				}
                else {
					if (HitParte && HitParte == ParteSeleccionada) {
						ParteSeleccionada->DesactivarResaltado();
						ParteSeleccionada->TWidget->OcultarWidgetRotacion();
						ParteSeleccionada = nullptr;
					}
				}
				//rotar, es decir ver si hay componete, para capturar su plano, y empezar a hacer los calculos, aqui entra lo de la rotacion de ramas
				//si hay algun componente seleccionado, o algo, debo marcarlo por ahora
			}
			else {
				if (bHitParte && HitParte) {
					EjeSeleccionado = ETransformacionEje::ENone;
					HitEje = ETransformacionEje::ENone;
					ParteSeleccionada = HitParte;
					ParteSeleccionada->ActivarResaltado();
					ParteSeleccionada->TWidget->MostrarWidgetRotacion();
				}
			}
			//una vez que tenga una parte si la selecciono recien se ahbilita y busco componentes

        }
        break;
        case EVRJerarquiaTask::ETraslationTask: {
			//GrabRightTrasladarPressed();
			if (Usuario->OverlapedRightParte) {//encapsularlo en la funcion anterior para manter un orden
				Usuario->bBuscarParteRight = false;
				Usuario->bGrabRightParte = true;
				//if (OverlapedRightParte->Id != OverlapedLeftParte->IdParteRaiz) { //esta condicion no es necesaria
					//si no es la raiz de su jerarquia debo 
					//en relaidad no encesito este if, defrente tomar el IdParteRaiz y trabanr con esa parte
					//como hago en los demas, y da igual si
                Usuario->OffsetRightParte = Jerarquias[Usuario->OverlapedRightParte->IdParteRaiz]->Root->ParteAsociada->GetActorLocation() - Usuario->MotionControllerRight->GetComponentLocation();//creo que esto no sera necesario
                //establecer la posicion y rotacion del punto de referenciay ponerlo de hijo de este control
                Usuario->PuntoReferenciaRight->SetWorldLocation(Jerarquias[Usuario->OverlapedRightParte->IdParteRaiz]->Root->ParteAsociada->GetActorLocation());
                Usuario->PuntoReferenciaRight->SetWorldRotation(Jerarquias[Usuario->OverlapedRightParte->IdParteRaiz]->Root->ParteAsociada->GetActorRotation());
                //no hacer nada mas
                //La posicion grab robot debe ser calculada de la mims forma a como calculo todo, mapeando solo los valores x y y, y usando la altura de altura robot
                PosicionGrabRobot = Jerarquias[Usuario->OverlapedRightParte->IdParteRaiz]->Root->ParteAsociada->GetActorLocation();
                PosicionGrabRobot.Z = AlturaRobot.Z;
                //aun que esto de cambiar el z no deberia estar, es decir si el robot se ubica en el centro, entonces, ya etara en alguna posicn con ese z adecuado, entonces no se necesita
                //tambien deberia haber una posicion inical 0, 0 el conejo deberia estar alli desde el comienzo
                //y deberia ser el primer punto de traslaccion

					//OverlapedRightParte->BuscarArticulacion();//en realidad esto deberia ser para todas las partes de la jerarquia, todo el tiempo, si es que esta en el modo de armado
					//si para toda la jerarquia aplicarlo recursivo quiza solo en las partes que tenga articulaciones libres
				//}
			}
        }
        break;
        default:
        case EVRJerarquiaTask::ENoTask: {
			//GrabRightNingunoPressed();
        }
        break;
    }
}

void ARobot::GrabRightTick() {
    switch (CurrentJerarquiaTask) {
        case EVRJerarquiaTask::EArmarTask: {
			//GrabRightArmarTick();
        }
        break;
        case EVRJerarquiaTask::ERotationTask: {
			//GrabRightRotarTick();
        }
        break;
        case EVRJerarquiaTask::ETraslationTask: {
			//GrabRightTrasladarTick();
			if (Usuario->bGrabRightParte && Usuario->OverlapedRightParte) {
                //FVector PosicionLocal = GetActorTransform().InverseTransformPosition(Usuario->PuntoReferenciaRight->GetComponentLocation());
                //las partes estan trabajando en el espacion global, que coincide co la posicion de robot
                //trabajar con global
                FVector PosicionNueva = Usuario->PuntoReferenciaRight->GetComponentLocation() - PosicionGrabRobot;
                //esta posicion nueva deberia ser un vector desede la posicion que se guardo al inicio, al momeno del press
                if (FMath::Abs(PosicionNueva.X) >= FMath::Abs(PosicionNueva.Y)) {
                    PosicionNueva.Y = 0.0f;
                }
                else {
                    PosicionNueva.X = 0.0f;
                }
                PosicionNueva.Z = 0.0f;
                
				//Jerarquias[Usuario->OverlapedRightParte->IdParteRaiz]->Root->SetWorldLocation(Usuario->PuntoReferenciaRight->GetComponentLocation());
				Jerarquias[Usuario->OverlapedRightParte->IdParteRaiz]->Root->SetWorldLocation(PosicionGrabRobot + PosicionNueva);
				Jerarquias[Usuario->OverlapedRightParte->IdParteRaiz]->Root->SetWorldRotation(Usuario->PuntoReferenciaRight->GetComponentRotation());
				UE_LOG(LogClass, Log, TEXT("Actualizando Poisicion jerarquia tick"));
                //el robot debe avanzar por umbrales, de uno en uno, para que sean valores enteros,
                //asi mesmo la direccion es respecto al punto donde esta, sea solo se puede mover otrogonalmente no en diagoneles
                //para ello determinar dependendo si el valor absoluto es de un eje es mayor que el del otro, entonces lo traslado al del mayor, estoy moviendome en ese eje.
                //hay variaces al mecanismo, com oque solo funcione en un radio pequeño desde la ubicacion del objeto, despues de ese radio ya se determino un eje, y sobre ese eje se muve, para cambiar de direccion , el usuario debe regresar al punto de inico e ir e otra.
				//Jerarquias[Usuario->OverlapedRightParte->IdParteRaiz]->ActualizarNodos();
				//Jerarquias[Usuario->OverlapedRightParte->IdParteRaiz]->ActualizarPila();
			}
        }
        break;
        default:
        case EVRJerarquiaTask::ENoTask: {
			//GrabRightNingunoTick();
        }
        break;
    }
}

void ARobot::GrabRightReleased() {
    switch (CurrentJerarquiaTask) {
        case EVRJerarquiaTask::EArmarTask: {
			//GrabRightArmarReleased();
        }
        break;
        case EVRJerarquiaTask::ERotationTask: {
			//GrabRightRotarReleased();
			if (ParteSeleccionada) {
				if (EjeSeleccionado != ETransformacionEje::ENone)
					//Jerarquia->EstablecerRotacionEje(ParteSeleccionada->Id, AngleTemp, EjeSeleccionado);
					Jerarquia->EstablecerRotacionEje(ParteSeleccionada->Id, AnguloAcum, EjeSeleccionado);

                //aqui debo decirle a la jerarquia que cree un nuevo alemento, que agregue una nueva rotacion, pero la pregunta es donde esta la rotacion termporal?
                //donde se muestra

					ParteSeleccionada->TWidget->DeseleccionarEjeRotacion(EjeSeleccionado);
				//se edberian aplicar las rotaciones permanentemente
				EjeSeleccionado = ETransformacionEje::ENone;
				//rotar, es decir ver si hay componete, para capturar su plano, y empezar a hacer los calculos, aqui entra lo de la rotacion de ramas
				//si hay algun componente seleccionado, o algo, debo marcarlo por ahora
			}
			else {
			}
        }
        break;
        case EVRJerarquiaTask::ETraslationTask: {
			//GrabRightTrasladarReleased();
			if (Usuario->OverlapedRightParte) {
				Usuario->bBuscarParteRight = true;
                PuntosTraslacion.Add(Jerarquias[Usuario->OverlapedRightParte->IdParteRaiz]->Root->GetWorldLocation());
                CreatePuntoTraslacion(PuntosTraslacion[PuntosTraslacion.Num() - 1]);
				//Jerarquias[OverlapedRightParte->IdParteRaiz]->RealizarUniones();
                //crear punto para traslacion, en la ubicacion del robot
				Usuario->bGrabRightParte = false;
                Jerarquia->ActualizarCodigoTraslacion();
			}
        }
        break;
        default:
        case EVRJerarquiaTask::ENoTask: {
			//GrabRightNingunoReleased();
        }
        break;
    }
}

void ARobot::GrabLeftPressed() {
    switch (CurrentJerarquiaTask) {
        case EVRJerarquiaTask::EArmarTask: {
			//GrabLeftArmarPressed();
        }
        break;
        case EVRJerarquiaTask::ERotationTask: {
			//GrabLeftRotarPressed();
        }
        break;
        case EVRJerarquiaTask::ETraslationTask: {
			//GrabLeftTrasladarPressed();
            //podria hacer que compruebe si usario tiene una parte sobrepuesta en su control izquierdo, y listo entonces a partir de ahi lo tomo el control auiq,
            //es decir solo dbor poner los mismo s if aquie haciendo refernica alos boools del usuario
        }
        break;
        default:
        case EVRJerarquiaTask::ENoTask: {
			//GrabLeftNingunoPressed();
        }
        break;
    }
}

void ARobot::GrabLeftTick() {
    switch (CurrentJerarquiaTask) {
        case EVRJerarquiaTask::EArmarTask: {
			//GrabLeftArmarTick();
        }
        break;
        case EVRJerarquiaTask::ERotationTask: {
			//GrabLeftRotarTick();
        }
        break;
        case EVRJerarquiaTask::ETraslationTask: {
			//GrabLeftTrasladarTick();
        }
        break;
        default:
        case EVRJerarquiaTask::ENoTask: {
			//GrabLeftNingunoTick();
        }
        break;
    }
}

void ARobot::GrabLeftReleased() {
    switch (CurrentJerarquiaTask) {
        case EVRJerarquiaTask::EArmarTask: {
			//GrabLeftArmarReleased();
        }
        break;
        case EVRJerarquiaTask::ERotationTask: {
			//GrabLeftRotarReleased();
        }
        break;
        case EVRJerarquiaTask::ETraslationTask: {
			//GrabLeftTrasladarReleased();
        }
        break;
        default:
        case EVRJerarquiaTask::ENoTask: {
			//GrabLeftNingunoReleased();
        }
        break;
    }
}

void ARobot::UnirJerarquiaPadreHijo(int IdPadre, int IdHijo) {
	//ya que esta funcion se llama desde la funcion unir con parte de la clase parte
	//estos ids son de las partes que se unira, que comaprten la misma articulaciones, por lo tanto hay que buscar quien es raiz de quien, sin mebargo el hijo es quien si es raiz, si o si, es raiz
	//de cualquier forma corresponde obtener su parte rai
	//estos ids son de las partes que el vrpawn ha sujetado, etos idse se consegiue primero por la jerarquia que estoy sujetando, y el segund a travez del componente colision, 
	//aun que otro enfoque seria que las partes sean los que envien las señáles de union, en la funcon contectar en cada parte
	//si es que esta una articulacion disponible solapada con l a que le corresponde, pues esta llama a robot y le dice que sze unana, asi vrpawn no tiene que hacer nada
	//para esta funcon ser llamada se debe activar las articulaciones solo en las partes que he sujetado, pero que detecte cualquier articulacion solapada.
	//es decir todas las articulaciones terminales de la jerarquia deben detectar solapamientos con sus contrapartes
	//ya que esto esta restringido, no iporta quien se une a quien

	int IdRaizPadre = Partes[IdPadre]->IdParteRaiz;
	int IdRaizHijo = Partes[IdHijo]->IdParteRaiz;
	//estos son paa acceder a la jas jerarquias a las que pertencen
	//o lo hago desde afuera, sin la funcion absorber?
	//Jerarquias[IdRaizPadre]->AbsorberJerarquia(Jerarquias[IdRaizHijo]);

	Transformaciones[IdHijo]->Padre = Transformaciones[IdPadre];
	Transformaciones[IdPadre]->Hijos.Add(Transformaciones[IdHijo]);
	//se supone que el hijo es el nuevo nodo a crear, y el padre ya esta creado

	Jerarquias[IdRaizHijo]->ActualizarIdRaizParte(IdRaizPadre);
	//raiz actuazada

	//debo actualizar el IdParteRaiz de la jerarquia hijo a todas sus partes
	
	Transformaciones[IdHijo]->ParteAsociada->AttachToActor(Transformaciones[IdPadre]->ParteAsociada, FAttachmentTransformRules::KeepWorldTransform);

    //la raiz es el ultimo nodo

    /*
    *debo unir los nodos, attt los nodos de la jerarquia hijo a la jerarquia padre
    */
    std::stack<Transformacion *> pila;
	UE_LOG(LogClass, Log, TEXT("Actualizando Attach Nodos"));
	pila.push(Jerarquias[IdRaizHijo]->Root);
    while (!pila.empty()) {
        Transformacion * V = pila.top();
        pila.pop();
		//ejecutar animacion
        if (V) {
            Nodos[V->ParteAsociada->Id]->AttachToActor(Jerarquias[IdRaizPadre], FAttachmentTransformRules::KeepRelativeTransform);
			if (V->Hijos.Num()) {
				for (int i = V->Hijos.Num()-1; i >= 0; i--) {
					pila.push(V->Hijos[i]);
				}
			}
		}
    }

	//listo hora a solo lajerar
	Jerarquias[IdRaizPadre]->CantidadPartes += Jerarquias[IdRaizHijo]->CantidadPartes;
	//debo dejar sin raiz a la otra jerarquia
	Jerarquias[IdRaizHijo]->Root = nullptr;
	Jerarquias[IdRaizHijo]->CantidadPartes = 0;



	Jerarquias[IdRaizPadre]->Imprimir();
	Jerarquias[IdRaizHijo]->Imprimir();

	Jerarquias[IdRaizPadre]->Layout();
	Jerarquias[IdRaizPadre]->AplicarLayout();
	Jerarquias[IdRaizPadre]->ActualizarNodos();

	//cuando haga desuniones debo recontablilizar a mano, las partes en la jerarquias
	//y quiza sea mejor usar esa funcion aqui, esa funcon contara desde la razi todas las partes
	
}

void ARobot::VerificarRotaciones() {
    bool res = true;
    for (int i = 0; i < Partes.Num(); i++) {
        res = res & Partes[i]->RotacionesConfirmadas();
    }
    if (res) {
        SetJerarquiaTask(EVRJerarquiaTask::ETraslationTask);
        Usuario->SetJerarquiaTask(EVRJerarquiaTask::ETraslationTask);
    }

}

void ARobot::OnBeginOverlapZona(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
    if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor != GetOwner())) { //no es necesario el ultimo, solo para este caso particular en el que no quiero que el propio conejo active esta funconalidad
        //en realidad deberia haber un if afuera de cada uno de estos, verifcando que este en alguna tarea de casa o robot, si esttoy en no task de alguno de ellos no deberia estar conviriendo
        //ni comprobando
        AVRPawn * const VRPawn = Cast<AVRPawn>(OtherActor);
        if (VRPawn && !VRPawn->IsPendingKill()) {
            USphereComponent * const ColisionHead = Cast<USphereComponent>(OtherComp);//para la casa no necesito verificar que haya tocado su staticmesh
            if (ColisionHead) {
                if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Overlap Zona VRPawn"));
                VRPawn->SetJerarquiaTask(CurrentJerarquiaTask);
            }
        }
    }
}

void ARobot::OnEndOverlapZona(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex) {
    if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor != GetOwner())) { //no es necesario el ultimo, solo para este caso particular en el que no quiero que el propio conejo active esta funconalidad
        //en realidad deberia haber un if afuera de cada uno de estos, verifcando que este en alguna tarea de casa o robot, si esttoy en no task de alguno de ellos no deberia estar conviriendo
        //ni comprobando
        AVRPawn * const VRPawn = Cast<AVRPawn>(OtherActor);
        if (VRPawn && !VRPawn->IsPendingKill()) {
            USphereComponent * const ColisionHead = Cast<USphereComponent>(OtherComp);//para la casa no necesito verificar que haya tocado su staticmesh
            if (ColisionHead) {
                if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
                    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("End Overlap Zona VRPawn"));
                VRPawn->SetJerarquiaTask(EVRJerarquiaTask::ENoTask);
            }
        }
    }
}

//todo estara organizado en las funciones grab en las 3 funciones grab!

//las 10 jerarquias trabajan con las misma 10 trasnformaciones, encontces cuando uno jerarquia, y por dentro uno sus transformaciones, en el fondo estoy alterando en todas las jerarquias,
//lo unico que las diferencia es que tienen cada jerarquia un raiz diferente, entonce al unir jeraruias, unire las transformaciones que tienen como raiz.
//ademas de hacer los attachment apropiados.
//y actulaizar los calculos, de la matrices locales y globales, tal cual esta sucediendo ahora, tener cudado con ello, aunque suficiente con los atachments, ya que actualizo por ahora las amtrices en fucnion de unreal
//
//el problema es identificar la raiz o la jerarquia a la que pertenece una parte, quiza deba tener un identificador de a que jerarqui estoy conectada, la parte, en lugar del bool bConectado.


//las articulaciones deben tener un nombre exatamente igual y unco con su contraparte, para que se resalte al entrar en contacto solo con esa parte, y quiza un efecto cnouando entre en contacto con una parte que no debe
//que esto se exxprese omo mayor ilumincaion o menor ilumnacion, es decir intensidad, 
//y cuando la articulacion estes conectadad que se ponga de color blanco, asi tendra al final todas las articulaciones del mismo color.
//lo mismo al soltar se verfica que este sobreponiendo la articulacion, ahora debe ser solotrue cuando sobreponga la articulacion que le corresponde
//o quiza solo unir, si la articulacion sobre puesta es la que le corresponde

//los nodos, destaran todos dispuestos en el excenario, al igula que las trasnformaciones estaran asociados a sus partes y todas las jerarquias.
//el layout depende de cada jerarquias, listo, facil.


//el desunir, debo ser capaza de detectar un parte a desunir, poder encontrarla en la jerarquia de la que voy a desunir, y luego tambien generar una nueva jerarquia con ese sub arbol.
//para esto se puede usar los arreglos de jerarquias y trasnformaciones que se tenian, poniendo punteros como deberian ser, etc.

//cuando se termine la tarea de armar el robot, debo destruir las otras jerarquias y quedarme con la que contiene al robot
//tener en cuanta esta desunion, al momento de implementar el armado del robot


//la desunion sujetando, para entrar en el modo desunion se debe sujetar en cada mano alguna parte que esten en la misma jerarquia y que ademas una sea hija de la otra. revisando esto
//las articualaciones entran en un modo especia en el que el movimeintos es libre y la aarticulacion se estira.

//la desunion con puntero seraia apuntar a ala articulacion de unio seleccionar y desunir .
