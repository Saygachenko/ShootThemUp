// Shoo Them Up Game


#include "Player/Public/STUPlayerCharacter.h"
#include "Camera/CameraComponent.h" // библиотека камеры
#include "Components/InputComponent.h" // библиотека ввода
#include "GameFramework/SpringArmComponent.h" // библиотека вращения по орбите (для камеры)
#include "Components/STUWeaponComponent.h"

ASTUPlayerCharacter::ASTUPlayerCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{// вызвали конструктор родительского калсса. Функция SetDefaultSubobjectClass для замены компонента по умолчанию
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true; // чтобы можно было вращать
    SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f); // изменили расположение камеры

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);
}

void ASTUPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);
    check(WeaponComponent);

    if (PlayerInputComponent)
    {
        PlayerInputComponent->BindAxis("MoveForward", this, &ASTUPlayerCharacter::MoveForward);
        PlayerInputComponent->BindAxis("MoveRight", this, &ASTUPlayerCharacter::MoveRight);

        PlayerInputComponent->BindAxis("LookUp", this, &ASTUPlayerCharacter::AddControllerPitchInput); // для поворота камеры по вертикали мы будем использовать функцию AddControllerPitchInput(значение угла Pit
        // ch) которая есть у каждого Pawn. Можно их подключать напрямую
        PlayerInputComponent->BindAxis("TurnAround", this, &ASTUPlayerCharacter::AddControllerYawInput); // для поворота камеры вокруг Character используем функцию AddControllerYawInput

        PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUPlayerCharacter::Jump); // функция прыжка Jump. IE_Pressed - состояние Controlle (когда клавиша нажата)

        PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUPlayerCharacter::OnStartRunning); // IE_Pressed кнопка нажата
        PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUPlayerCharacter::OnStopRunning); // IE_Released кнопка отжата

        PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASTUPlayerCharacter::OnStartFire); // начало стрельбы из оружия
        PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &USTUWeaponComponent::StopFire); // конец стрельбы из оружия

        PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &USTUWeaponComponent::NextWeapon); // бинд на смену оружия

        PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &USTUWeaponComponent::Reload); // бинд перезарядки
    }
}

void ASTUPlayerCharacter::MoveForward(float Amount)
{
    IsMovingForward = Amount > 0.0f; // true если Amount > 0 - нажата клавиша за движение вперёд
    if (Amount == 0.0f)
    {
        return;
    }
    AddMovementInput(GetActorForwardVector(), Amount); // для перемещения нашего персонажа воспользуемся функцией AddMovementInput(направление в
    // которое будет перемещать нашего персонажа, scale) - данная функция существует у каждого Pawn
    if (IsRunning() && WeaponComponent->IsFiring()) // если мы бежим и стрельба началась
    {
        WeaponComponent->StopFire(); // останавливаем стрельбу
    }
}

void ASTUPlayerCharacter::MoveRight(float Amount)
{
    if (Amount == 0.0f) return;
    IsMovingRight = true; // проверка нажата клавиша за движение в бок
    AddMovementInput(GetActorRightVector(), Amount);
}

void ASTUPlayerCharacter::OnStartRunning() // когда клавиша нажата
{
    WantsToRun = true;
    IsMovingRight = false; // если не нажата то выполняем функцию
    if (IsRunning()) // если стрельба началась
    {
        WeaponComponent->StopFire(); // останавливаем стрельбу
    }
}

void ASTUPlayerCharacter::OnStopRunning() // когда клавиша отжата
{
    WantsToRun = false;
}

bool ASTUPlayerCharacter::IsRunning() const
{
    return WantsToRun && IsMovingForward && !GetVelocity().IsZero() && !IsMovingRight; // проверка что кнопка нажата и наш персонаж двигается вперёд, и что скорость персонажа не равна 0 + персонаж не двигается в стороны
}

void ASTUPlayerCharacter::OnDeath()
{
    Super::OnDeath();
    if (Controller) // проверка на контроллер персонажа
    {
        Controller->ChangeState(NAME_Spectating); // если контроля нет, то мы меняем на NAME_Spectating (свободная камера)
    }
}