//
// Created by pairs on 7/11/18.
//

#include <zros/zros.h>
#include <zros/node_handle.h>
#include "test_service.pb.h"

int main() {
    // 1. init & run service discovery
    // for service discovery
    const std::string master_address = "localhost:23333";
    zros::init(master_address);

    // 2. create node
    const std::string node_name = "example_service_client";
    auto node_handle = std::make_shared<zros::NodeHandle>(node_name);

    // 3. create service client
    const std::string service_name = "test_service";
    auto client = node_handle->serviceClient<zros_example::TestServiceRequest, zros_example::TestServiceResponse>(
            service_name
            );

    auto client2 = node_handle->serviceClient<zros_example::TestServiceRequest, zros_example::TestServiceResponse>(
            "test_service2"
    );

    std::thread call_thread = std::thread([client]() {
        while (true) {
            zros_example::TestServiceRequest request;
            zros_example::TestServiceResponse response;
            request.set_detail("hello world");
            while(client->get_ready()) {
                auto status = client->call(&request, &response);
                if (status.code() == status.OK) {
                    SSPD_LOG_INFO << "call success";
//                    SSPD_LOG_INFO << "the response is " << response;
                } else {
                    SSPD_LOG_WARNING << "call failed " << status.details();
                }
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
    call_thread.join();
}