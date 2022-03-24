#ifndef COMMANDER_MODULE_H
#define COMMANDER_MODULE_H

#include <string>
#include <jsonrpccxx/server.hpp>


namespace commander
{

    struct Module;

    template<typename In>
    struct ModuleObject
    {
        Module& m;
        std::string prefix;
        In in;

        template<typename Fn, typename... Args>
        ModuleObject& def(const std::string& name, Fn&& fn, const std::string& description = "", Args&&... args);

    };

    template<typename In>
    ModuleObject(Module&, std::string, In) -> ModuleObject<In>;

    struct Module
    {

        std::unordered_map<std::string, std::function<void(Module&)>> functions;

        jsonrpccxx::JsonRpc2Server server;
        std::unordered_map<std::string, Command> commands;

        Module():
            server(),
            commands()
        {
            register_functions(*this);
            auto fn = jsonrpccxx::GetHandle(std::function([this]() {
                return get_help();
            }));
            server.Add("help", fn);

        }

        std::vector<std::string> get_help() const {
            std::vector<std::string> helps; helps.reserve(commands.size());
            for (auto& [name, cmd]: commands)
                helps.push_back(fmt::format("{}: {}", name, cmd));
            return helps;
        }

        std::vector<std::string> command_names() const {
            std::vector<std::string> names; names.reserve(commands.size());
            for(auto& e : commands | boost::adaptors::map_keys)
                names.push_back(e);
            return names;
        }

        std::string description(const std::string& name) {
            return commands.at(name).description;
        }

        template<typename Fn, typename... Args>
        Module& def(const std::string& name, Fn&& fn, const std::string &description = "", Args&&... args)
        {
            Command command{description}; //, parse_args(fn, std::forward<Args>(args)...)};

            auto res = server.Add(name, jsonrpccxx::GetHandle(fn));//, command.names());

            if ( not res)
                fmt::print("Error, cannot add {}\n", name);

            commands.emplace(name, command);
            return *this;
        }

        template<typename T>
        auto instance(std::string name)
        {
            auto static_instance = []() -> T& {
                static T instance;
                return instance;
            };
            return ModuleObject{*this, name, static_instance};
        }

        template<typename T, typename Fn>
        auto instance(std::string name, Fn&& fn)
        {
            return ModuleObject{*this, name, fn};
        }
    };

    template<typename In>
    template<typename Fn, typename... Args>
    ModuleObject<In>& ModuleObject<In>::def(const std::string& name, Fn&& fn, const std::string& description, Args&&... args)
    {
        m.def(fmt::format("{}:{}", prefix, name), get_handle(fn, in), description, std::forward<Args>(args)...);
        return *this;
    }

} // namespace commander

#endif //COMMANDER_MODULE_H