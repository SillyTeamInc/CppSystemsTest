//
// Created by emi on 10/17/26.
//

#include "sprint.hpp"
#include "events/base_tick_event.hpp"
#include "sdk/mc/client/game/ClientInstance.hpp"
#include "sdk/mc/entity/components/MoveInputComponent.hpp"

void sprint::on_enable() {
    dispatcher().listen<base_tick_event, &sprint::on_base_tick>(this);
    log->info("sprint enabled");
}

void sprint::on_disable() {
    dispatcher().deafen<base_tick_event, &sprint::on_base_tick>(this);
}

// this event assumes only localplayer is passed into this event
// if it fires with a non-lp actor it is a bug
void sprint::on_base_tick(base_tick_event& event) {
    auto lp = static_cast<LocalPlayer*>(event.actor); // Eat my entire ass
    auto mic = lp->mContext.tryGetComponent<MoveInputComponent>();
    if (!mic.has_value()) {
        log->warn("[sprint] MoveInputComponent not found..?");
        return;
    }

    // this is fucking horrible this bitfield sucks
    mic->mInputState.mFlagValues.set(static_cast<std::size_t>(MoveInputState::Flag::SprintDown), true);
    mic->mRawInputState.mFlagValues.set(static_cast<std::size_t>(MoveInputState::Flag::SprintDown), true);
}