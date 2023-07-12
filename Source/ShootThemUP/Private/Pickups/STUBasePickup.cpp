// Shoo Them Up Game


#include "Pickups/STUBasePickup.h"
#include "Components/SphereComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBasePickup, All, All);

// Sets default values
ASTUBasePickup::ASTUBasePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    CollisionComponent->InitSphereRadius(50.0f); // размер сферы
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // физических взаимодействий не будет с этой колизией
    CollisionComponent->SetCollisionResponseToChannels(ECollisionResponse::ECR_Overlap); // реакция колизии с другими
    SetRootComponent(CollisionComponent); // сделали его корневым

}

// Called when the game starts or when spawned
void ASTUBasePickup::BeginPlay()
{
	Super::BeginPlay();
    
    check(CollisionComponent); // проверка колизии пикапа

    GenerateRotationYaw(); // вращение вызывается при старте игры
	
}

// Called every frame
void ASTUBasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    AddActorLocalRotation(FRotator(0.0f, RotationYaw, 0.0f)); // угол вращения добавляется каждый фрейм с помощью функции AddActorLocalRotation() - она у всех акторов есть

    for (const auto OverlapPawn : OverlappingPawns) // пробегаемся по массиву OverlappingPawns
    {
        if (GivePickupTo(OverlapPawn)) // проверяем, можем-ли мы отдать кому-нибудь пикап из паунов в массиве (те кто пересекают в данный момент коллизию пикапа)
        {
            PickupWasTaken(); // подбираем пикап
            break; // выход из цикла
        }
    }
}

void ASTUBasePickup::NotifyActorBeginOverlap(AActor *OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    const auto Pawn = Cast<APawn>(OtherActor); // указатель на павн
    if (GivePickupTo(Pawn)) // если пикап удалось забрать
    {
        PickupWasTaken(); // вызываем функцию взятия пикапа
    }
    else if (Pawn) // если пикап неудалось поднять
    {
        OverlappingPawns.Add(Pawn); // добавляем указатель на пауна в массив
    }
}

void ASTUBasePickup::NotifyActorEndOverlap(AActor *OtherActor)
{
    Super::NotifyActorEndOverlap(OtherActor);

    const auto Pawn = Cast<APawn>(OtherActor); // указатель на павн
    OverlappingPawns.Remove(Pawn); // удаляем укащатель на павна из массива
}

bool ASTUBasePickup::GivePickupTo(APawn *PlayerPawn)
{
    return false; // пикап базового класса взять невозможно
}

void ASTUBasePickup::PickupWasTaken()
{
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // отключаем все коллизии (перестанет взаимодействовать с окружающим миром)
    if (GetRootComponent()) // проверка на 0
    {
        GetRootComponent()->SetVisibility(false, true); // делаем пикап невидимым. Функция SetVisibility(рендер компонента, новое значение Visibility применено ко всем дочерним компонентам) что актор перестал рендерится
    }

    FTimerHandle RespawnTimerHandle; // запускаем таймер респавна, таймер локальный будет срабатывать 1 раз (отключать таймер не будем, поэтому его не нужно создавать как переменну класса не нужно)
    GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ASTUBasePickup::Respawn, RespawnTime);
}

void ASTUBasePickup::Respawn()
{
    GenerateRotationYaw(); // ещё один вызов вращение во время респавна пикапа
    if (GetRootComponent()) // проверка на 0
    {
        GetRootComponent()->SetVisibility(true, true); // делаем пикап видимым
    }
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap); // возвращаем коллизию
}

void ASTUBasePickup::GenerateRotationYaw()
{
    const auto Direction = FMath::RandBool() ? 1.0f : -1.0f; // генерация булевого значения RandBool()
    RotationYaw = FMath::RandRange(1.0f, 2.0f) * Direction; // функция генерации рандомных чисел RandRange(начало отрезка, конец отрезка)
}

