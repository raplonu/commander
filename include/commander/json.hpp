#pragma once

#include <boost/json.hpp>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <string_view>

namespace commander::json
{
    namespace bj = boost::json;
    using bj::value;
    using bj::object;
    using bj::array;
    using bj::string;

    using bj::serialize;
    using bj::parse;

    using bj::value_to;
    using bj::value_from;

} // namespace commander::json

namespace fmt
{

    template<typename CharT>
    struct formatter<boost::core::basic_string_view<CharT>> : formatter<std::basic_string_view<CharT>> {
        template <typename FormatContext>
        auto format(boost::core::basic_string_view<CharT> data, FormatContext &ctx) const {
            return formatter<std::basic_string_view<CharT>>::format({data.data(), data.size()}, ctx);
        }
    };

    // Specialization for boost::json::string
    template <>
    struct formatter<boost::json::string> : formatter<std::string_view> {
        auto format(const boost::json::string& json_str, format_context& ctx) const {
            return formatter<std::string_view>::format(static_cast<std::string_view>(json_str), ctx);
        }
    };

    // Specialization for boost::json::value
    template <>
    struct formatter<boost::json::value> {

        constexpr auto parse(format_parse_context& ctx) {
            return ctx.begin();
        }

        auto format(const boost::json::value& json_val, format_context& ctx) const {
            if (json_val.is_string()) {
                return fmt::format_to(ctx.out(), "\"{}\"", json_val.as_string());
            } else if (json_val.is_object()) {
                return fmt::format_to(ctx.out(), "{{{}}}", fmt::join(json_val.as_object(), ", "));
            } else if (json_val.is_array()) {
                return fmt::format_to(ctx.out(), "[{}]", fmt::join(json_val.as_array(), ", "));
            } else if (json_val.is_int64()) {
                return fmt::format_to(ctx.out(), "{}", json_val.as_int64());
            } else if (json_val.is_double()) {
                return fmt::format_to(ctx.out(), "{}", json_val.as_double());
            } else if (json_val.is_bool()) {
                return fmt::format_to(ctx.out(), "{}", json_val.as_bool());
            } else if (json_val.is_null()) {
                return fmt::format_to(ctx.out(), "null");
            }
            return fmt::format_to(ctx.out(), "<unknown>");
        }
    };


    // template <>
    // struct formatter<boost::json::key_value_pair> {

    //     constexpr auto parse(format_parse_context& ctx) {
    //         return ctx.begin();
    //     }

    //     auto format(const boost::json::key_value_pair& kvp, format_context& ctx) {
    //         return format_to(ctx.out(), "{{{}, {}}}", kvp.key(), kvp.value());
    //     }
    // };

    // Specialization for boost::json::array
    // template <>
    // struct formatter<boost::json::array> {
    //     auto format(const boost::json::array& json_array, format_context& ctx) {
    //         return format_to(ctx.out(), "[{}]", fmt::join(json_array.begin(), json_array.end());
    //     }
    // };

    // Specialization for boost::json::object
    // template <>
    // struct formatter<boost::json::object> {
    //     auto format(const boost::json::object& json_object, format_context& ctx) {
    //         auto it = fmt::format_to(ctx.out(), "{{");
    //         bool first = true;
    //         for (const auto& [key, val] : json_object) {
    //             if (!first) result += ", ";
    //             result += fmt::format("\"{}\": {}", key, val);
    //             first = false;
    //         }
    //         result += "}";
    //         return fmt::format_to(ctx.out(), "{}", result);
    //     }
    // };

} // namespace fmt
