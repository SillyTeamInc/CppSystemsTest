//
// Created by emi on 2/23/2026.
//

#pragma once

class generic_system;

class systems_manager {
    // messy shit
    // this also requires rtti
    std::vector<std::unique_ptr<generic_system>> m_owned;
    std::vector<generic_system*> m_all;
    std::unordered_map<std::type_index, generic_system*> m_by_type;

    void index(generic_system& s);

    void unindex(generic_system& s);

public:
    static std::unique_ptr<systems_manager> s_instance;
    static systems_manager* i();
    static void destroy_instance();

    systems_manager() = default;
    ~systems_manager();


    systems_manager(const systems_manager&) = delete;
    systems_manager& operator=(const systems_manager&) = delete;

    void initialize_systems() const;
    void shutdown_systems();

    void attach(generic_system& s);
    void detach(generic_system& s);

    template<std::derived_from<generic_system> T, typename... Args>
    T* add_system(Args&&... args) {
        auto ownedSys = std::make_unique<T>(std::forward<Args>(args)...);
        T& refSys = *ownedSys;
        m_owned.push_back(std::move(ownedSys));
        attach(refSys);
        return get<T>();
    }

    template<typename T>
    T* get() {
        if (auto it = m_by_type.find(typeid(T)); it != m_by_type.end()) {
            return static_cast<T*>(it->second);
        }
        for (auto* s: m_all) {
            if (auto sys = dynamic_cast<T*>(s)) {
                return sys;
            }
        }
        return nullptr;
    }

    template<typename T>
    T& require_system() {
        T* system = get<T>();
        assert(system && "required system is not registered");
        return *system;
    }
};
