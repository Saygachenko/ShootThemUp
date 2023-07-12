#pragma once

class AnimUtils
{
 public:
    template <typename T> T 
    static *FindNotifyByClass(UAnimSequenceBase *Animation)
    {
        if (!Animation)
        {
            return nullptr;
        }

        const auto NotifyEvents = Animation->Notifies; // получаем доступ к проперти Notify
        for (auto NotifyEvent : NotifyEvents) // проходимся по всему массиву анимационному эвенту NotifyEvents
        {
            auto AnimNotify = Cast<T>(NotifyEvent.Notify); // чтобы узнать является данный Notify нашим OnEquipFinished мы попытаемся
                                             // преобразовать данный Notify к нашему типу USTUEquipFinishedAnimNotify
            if (AnimNotify) // если это наш Notify то возвращаем AnimNotify
            {
                return AnimNotify;
            }
        }
        return nullptr; // если не нашли, возвращаем 0
    }
};