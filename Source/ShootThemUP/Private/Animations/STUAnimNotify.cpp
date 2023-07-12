// Shoo Them Up Game


#include "Animations/STUAnimNotify.h"

void USTUAnimNotify::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation)
{
    OnNotified.Broadcast(MeshComp);     // вызываем наш делегат
    Super::Notify(MeshComp, Animation); // вызываем родительскую функцию
}
