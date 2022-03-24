#ifndef COMMANDER_INTERACTIVE_CONNECTOR_H
#define COMMANDER_INTERACTIVE_CONNECTOR_H

namespace commander
{

    struct InMemoryConnector : jsonrpccxx::IClientConnector {

        explicit InMemoryConnector(jsonrpccxx::JsonRpcServer &server) :
            server(server)
        {}

        std::string Send(const std::string &request) override {
            return server.HandleRequest(request);
        }

        jsonrpccxx::JsonRpcServer &server;

    };

} // namespace commander

#endif //COMMANDER_INTERACTIVE_CONNECTOR_H