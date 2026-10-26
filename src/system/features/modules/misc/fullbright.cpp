//
// Created by emi on 10/20/26.
//

#include "fullbright.hpp"
#include "events/base_tick_event.hpp"
#include "sdk/mc/options/option_types/FloatOption.hpp"
#include "sdk/mc/client/game/ClientInstance.hpp"
#include "core/core.hpp"
#include "sdk/mc/options/IOptionsReader.hpp"

void fullbright::on_enable() {
    this->gamma = static_cast<FloatOption*>(core::get_client_instance()->getOptions().getIfValid(OptionID::GfxGamma).value());
    this->m_prev_gamma = this->gamma->mValue;
    this->gamma->VALUE_MAX = 10.0f;
    this->gamma->mValue = 10.0f;

    dispatcher().listen<base_tick_event, &fullbright::on_base_tick>(this);
    log->info("fullbright enabled");
}

void fullbright::on_disable() {
    if (!this->gamma) {
        log->warn("help me");
    }
    this->gamma->mValue = this->m_prev_gamma;
    this->gamma->VALUE_MAX = 1.0f;

    dispatcher().deafen<base_tick_event, &fullbright::on_base_tick>(this);
}

void fullbright::on_base_tick(base_tick_event &event) {
    if (this->gamma->mValue != 10.0f)
        this->gamma->mValue = 10.0f;
}

