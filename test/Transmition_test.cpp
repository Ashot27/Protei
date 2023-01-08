#include <gtest/gtest.h>
#include <iostream>

#include "../client/Client.h"
#include "../server/Server.h"

#define IP "127.0.0.1"
#define PORT 8080

class Transmition_test : public ::testing::Test {
   protected:
    Transmition_test() {
        std::cout.rdbuf(nullptr);  // hide cout
        std::cerr.rdbuf(nullptr);  // hide cerr
        if ((server_pid = fork()) == 0) {

            Server server = Server::get_instance(IP, PORT);
            server.prepare();
            server.run();
            exit(0);
        }
    }

    ~Transmition_test() override { kill(server_pid, SIGTERM); }

    void SetUp() override {}

    void TearDown() override {}

    pid_t server_pid;
};

TEST_F(Transmition_test, tcp_client_transmition_with_server) {
    Client client(IP, PORT, true);
    Client_status _status = client.prepare();

    const string message = "20 apples, 30 bananas, 15 peaches and 1 watermelon";
    const string result_ref = "1 15 20 30\n66";

    string result;
    client.data_transmission(message, result);

    EXPECT_TRUE(result_ref == result);
}

TEST_F(Transmition_test, udp_client_transmition_with_server) {
    Client client(IP, PORT, false);
    Client_status _status = client.prepare();

    const string message = "20 apples, 30 bananas, 15 peaches and 1 watermelon";
    const string result_ref = "1 15 20 30\n66";

    string result;
    client.data_transmission(message, result);

    EXPECT_TRUE(result_ref == result);
}

TEST_F(Transmition_test, multiple_tcp_clients_transmition_with_server) {
    vector<Client*> clients;

    for (size_t i = 0; i < 10; i++) {
        Client* client = new Client(IP, PORT, true);
        client->prepare();
        clients.push_back(client);
    }

    for (auto& client : clients) {
        const string message =
            "20 apples, 30 bananas, 15 peaches and 1 watermelon";
        const string result_ref = "1 15 20 30\n66";

        string result;
        client->data_transmission(message, result);
        EXPECT_TRUE(result_ref == result);
    }

    for (auto& client : clients) {
        delete client;
    }
}