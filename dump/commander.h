#pragma once

namespace commander
{

    struct Argument
    {
        std::string name;
        std::string description;
        std::string type;
        std::string default_value;
    };

    struct Command
    {
        std::string description;

        // std::vector<Argument> arguments;

        // std::vector<std::string> arg_names() {
        //     std::vector<std::string> names;
        //     for (auto &arg : arguments) {
        //         names.push_back(arg.name);
        //     }
        //     return names;
        // }
    };

}

template <> struct fmt::formatter<commander::Command> {

  constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
    // Parse the presentation format and store it in the formatter:
    auto it = ctx.begin(), end = ctx.end();

    // Check if reached the end of the range:
    // if (it != end && *it != '}') throw fmt::format_error("invalid format");

    // Return an iterator past the end of the parsed range:
    return it;
  }

  // Formats the point p using the parsed format specification (presentation)
  // stored in this formatter.
  template <typename FormatContext>
  auto format(const commander::Command& cmd, FormatContext& ctx) -> decltype(ctx.out()) {
    // ctx.out() is an output iterator to write to.
    return format_to(ctx.out(), "{}", cmd.description);
  }
};


namespace commander
{

namespace detail
{

    /// C++17 version of std::type_identity
    /// Replace by std::type_identity when C++20.
    template< class T >
    struct type_identity {
        using type = T;
    };

    template< class T >
    using type_identity_t = typename type_identity<T>::type;

    namespace ct = boost::callable_traits;

    template <typename Fn, typename ReturnType, typename T, typename... ParamTypes, typename In>
    auto get_handle_impl(Fn&& fn, In&& instance, type_identity<ReturnType>, type_identity<std::tuple<T, ParamTypes...>>) {
        std::function<ReturnType(ParamTypes...)> function = [instance, fn](ParamTypes &&... params) -> ReturnType {
            return std::invoke(fn, instance(), std::forward<ParamTypes>(params)...);
        };
        return jsonrpccxx::GetHandle(function);
    }

}

    template <typename Fn, typename In>
    auto get_handle(Fn&& fn, In&& instance) {
        return detail::get_handle_impl(
            std::forward<Fn>(fn), std::forward<In>(instance),
            detail::type_identity<detail::ct::return_type_t<Fn>>{},
            detail::type_identity<detail::ct::args_t<Fn>>{}
        );
    }

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

    using RegisterFunction = std::function<void(Module&)>;

    std::vector<RegisterFunction>& register_functions_instance();

    void register_function(RegisterFunction fn);

    void register_functions(Module& module);

}

#define CMD_CAT(A, B) A ## B

#define COMMANDER_REGISTER_IMPL(MOD, CNT)                                   \
namespace commander{namespace{                                              \
struct CMD_CAT(ModuleRegister,CNT){                                         \
    CMD_CAT(ModuleRegister,CNT)(){                                          \
        commander::register_function(register_module);                      \
    }                                                                       \
    static inline void register_module(Module& MOD);                        \
} CMD_CAT(module_register_,CNT);                                            \
}}                                                                          \
void commander::CMD_CAT(ModuleRegister,CNT)::register_module(Module& MOD)

#define COMMANDER_REGISTER(MOD) COMMANDER_REGISTER_IMPL(MOD, __COUNTER__)
