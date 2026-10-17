//
// Created by emi on 10/16/26.
//
#pragma once

#include <utility>
#include "magic_enum.hpp"
#include "util/hash.hpp"

enum class category {
    misc
};
class module_system;

struct module_info {
    std::string_view name;
    std::string_view description = "No description provided";
    category module_category = category::misc;
    int default_key_code = 0;
    bool default_enabled = false;
    bool toggle = true;
};

class module {
    friend class module_system; // wow look at me using this correctly i think..?
protected:
    // i hate this.
    static inline systems_manager* systems() { return systems_manager::i(); }
    [[nodiscard]] static module_system* get_module_system();
    [[nodiscard]] static hvs::event_dispatcher& dispatcher();

    module_info m_info;

    // dont manually set this or you'll fuck things up lol
    bool m_enabled = false;
    bool m_wanted_state = false;
    bool m_state_locked = false;

    bool lock_state(bool locked);

public:
    explicit module(const module_info& info) : m_info(info) {
        key_code = info.default_key_code;
    }
    int key_code = 0; // todo: getter for keycode string without horrible code

    // virtual functions
    virtual ~module() = default;
    virtual void on_enable() {}
    virtual void on_disable() {}
    virtual void on_manager_init() {}
    virtual void on_manager_exit() {}
    // this should not be overridden by modules
    virtual std::uint32_t get_type_id() = 0;

    //getters
    [[nodiscard]] int get_default_key_code() const;
    [[nodiscard]] std::string_view get_category_name() const;
    [[nodiscard]] const std::string_view& get_name() const;
    [[nodiscard]] const std::string_view& get_description() const;
    [[nodiscard]] ::category get_category() const;
    [[nodiscard]] bool is_enabled() const;
    [[nodiscard]] bool is_wanted_state() const;
    [[nodiscard]] bool is_state_locked() const;
    [[nodiscard]] bool is_toggle() const;

    //other stuff
    void set_key_code(int kc);
    void set_enabled(bool enabled, bool silent = false);

    void toggle();
    void enable();
    void disable();

    // for the love of god do not put ui-based variables here.
    // use some kind of state typed by module or similar
    // to keep it seperate.
};

template<typename T>
class module_base : public module {
protected:
    explicit module_base(const module_info& info) : module(info) {}
public:
    // this is why constexpr is epic and useful
    static constexpr std::uint32_t type_id() {
        return util::hash::fnv1a(typeid(T).name());
    }
    [[nodiscard]] std::uint32_t get_type_id() override {
        return type_id();
    }
};

