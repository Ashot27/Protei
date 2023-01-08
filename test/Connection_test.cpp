#include <gtest/gtest.h>
#include "../client/Client.h"
#include "../server/Server.h"

class Connection_test : public ::testing::Test {
   protected:
    Connection_test() {
        std::cout.rdbuf(nullptr);  // hide cout
        std::cerr.rdbuf(nullptr);  // hide cerr
    }

    ~Connection_test() override {}

    void SetUp() override {}

    void TearDown() override {}
};

#define IP "127.0.0.1"
#define PORT 8080

TEST_F(Connection_test, udp_client_prepare) {
    Client client(IP, PORT, false);
    Client_status _status = client.prepare();
    EXPECT_TRUE(client.is_status_ok());
}

TEST_F(Connection_test, tcp_client_prepare_without_server) {
    Client client(IP, PORT, true);
    Client_status _status = client.prepare();
    EXPECT_FALSE(client.is_status_ok());
}

TEST_F(Connection_test, tcp_client_prepare_with_server) {
    Server server = Server::get_instance(IP, PORT);
    Server_status s_status = server.prepare();
    EXPECT_TRUE(s_status == Server_status::up);
    Client client(IP, PORT, true);
    Client_status c_status = client.prepare();
    EXPECT_TRUE(client.is_status_ok());
}
