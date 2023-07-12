// Shoo Them Up Game


#include "Weapon/STUProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/Components/STUWeaponFXComponent.h" // файл нашего компонента ниагары

// Sets default values
ASTUProjectile::ASTUProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    CollisionComponent->InitSphereRadius(5.0f); // размер сферы
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // физических взаимодействий не будет с этой колизией
    CollisionComponent->SetCollisionResponseToChannels(ECollisionResponse::ECR_Block); // реакция колизии с другими
    CollisionComponent->bReturnMaterialOnMove = true; // физический материал по умолчанию не передаётся в параметр HitResult (для оптимизации), явно указываем чтобы физический материал попадал в HitResult. Возврат физического материала bReturnMaterialOnMove
    SetRootComponent(CollisionComponent); // сделали его корневым

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent"); // чисто логический аттачить не нужно
    MovementComponent->InitialSpeed = 2000.0f; // скорость выстрела
    MovementComponent->ProjectileGravityScale = 0.0f; // притяжение к земле
  
    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent"); // создаём компонент
}

// Called when the game starts or when spawned
void ASTUProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	check(MovementComponent); // проверка что наш компонент существует
    check(CollisionComponent); // проверка что наш компонент существует
    check(WeaponFXComponent); // проверка что наш компонент существует

    MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed; // единичный вектор движения * на значение скорости
    CollisionComponent->OnComponentHit.AddDynamic(this, &ASTUProjectile::OnProjectileHit); // делегат вызывается тогда, когда произошло столкновение с актором или другим компонентом
    CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true); // игнорирование акторов
    SetLifeSpan(LifeSeconds); // удаляем наш снаряд через 5 секунд
}

void ASTUProjectile::OnProjectileHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
    if (!GetWorld()) // проверка на мир
    {
        return;
    }
    MovementComponent->StopMovementImmediately(); // останавливаем движение

    // наносим урон
    UGameplayStatics::ApplyRadialDamage(GetWorld(), DamageAmount, GetActorLocation(), DamageRadius, UDamageType::StaticClass(), {GetOwner()}, this, GetController(), DoFullDamage); // UGameplayStatics функция ApplyRadialDamage(указатель на мир, количество урона, центр урона,
                       // радиус, тип урона, можем указать массив акторов которым не будем наносить урон, актор который
                       // наносит ущерб, контроллер, полный урон (true) или по приближению к центру урона (false))
    // DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, 5.0f); // сфера взрыва
    WeaponFXComponent->PlayImpactFX(Hit); // вызывает наш эффект Ниагару
    Destroy(); // уничтожение актора
}

AController *ASTUProjectile::GetController() const
{
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr; // проверка на указатель GetOwner являится-ли он котроллером, если является вернём его контроллер
}
