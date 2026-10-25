//
// Created by emi on 10/17/26.
//

#include "Actor.hpp"
#include "sdk/mc/entity/components/ActorHeadRotationComponent.hpp"


ActorHeadRotationComponent* LocalPlayer::getActorHeadRotationComponent() {
    auto d =  mContext.tryGetComponent<ActorHeadRotationComponent>();
    return d.has_value() ? &d.value() : nullptr;
}
