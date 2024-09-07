#ifndef COMMANDER_CLIENT_SOCKET_H
#define COMMANDER_CLIENT_SOCKET_H

#include <commander/json.hpp>
#include <zmq.hpp>
#include <fmt/core.h>

#include <future>

namespace commander::client
{

    struct Socket
    {
        Socket(std::string socket);

        ~Socket() = default;

        template <typename Res, typename... Ts>
        Res send(std::string_view command_name, Ts... ts) {
            json::array args({json::value(ts)...});

            send(command_name, args);

            json::value result = recv();

            return json::value_to<Res>(result);

        }

        template <typename Res, typename... Ts>
        std::future<Res> send_async(std::string_view command_name, Ts... ts) {
            return std::async(std::launch::async, [this, command_name, ts...]() {
                return send<Res>(command_name, ts...);
            });

        }

        void send(std::string_view command_name, json::array args);

        json::value recv();

        zmq::context_t ctx;
        zmq::socket_t sock;
    };

} // namespace commander::client

#endif // COMMANDER_CLIENT_SOCKET_H
