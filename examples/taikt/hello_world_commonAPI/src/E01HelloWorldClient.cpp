// Copyright (C) 2014-2019 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <iostream>
#include <string>

#ifndef _WIN32
#include <unistd.h>
#endif

//#include <CommonAPI-3.2/CommonAPI/CommonAPI.hpp>
#include <CommonAPI/CommonAPI.hpp>
#include <v0/commonapi/examples/E01HelloWorldProxy.hpp>

using namespace v0::commonapi::examples;
#include<thread>

#include <csignal>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <iostream>


std::shared_ptr<E01HelloWorldProxy<>> myProxy;
static std::condition_variable_any sighandler_condition;
static std::recursive_mutex sighandler_mutex;

int sendRequest() {
    const std::string name = "World";
    CommonAPI::CallStatus callStatus;
    std::string returnMessage;

    CommonAPI::CallInfo info(1000);
    info.sender_ = 1234;

    while (true) {
        myProxy->sayHello(name, callStatus, returnMessage, &info);
        if (callStatus != CommonAPI::CallStatus::SUCCESS) {
            std::cerr << "Remote call failed!\n";
            return -1;
        }
        info.timeout_ = info.timeout_ + 1000;

        std::cout << "Got message: '" << returnMessage << "'\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}


void handle_signal(int _signal) {
     std::cout << "receive stop signal\n";   
    if ((_signal == SIGINT || _signal == SIGTERM)) {
        std::cout << "receive stop signal\n";   
        sighandler_condition.notify_one();
    }        
}

int main() {
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    
    CommonAPI::Runtime::setProperty("LogContext", "E01C");
    CommonAPI::Runtime::setProperty("LogApplication", "E01C");
    CommonAPI::Runtime::setProperty("LibraryBase", "E01HelloWorld");

    std::thread sighandler_thread([&]() {
        while (1) {
            std::unique_lock<std::recursive_mutex> its_lock(sighandler_mutex);
            sighandler_condition.wait(its_lock);
            std::cout << "stop client\n";                      
            return;            
        }
    });

    std::shared_ptr < CommonAPI::Runtime > runtime = CommonAPI::Runtime::get();

    std::string domain = "local";
    std::string instance = "commonapi.examples.HelloWorld";
    std::string connection = "client-sample";

    //std::shared_ptr<E01HelloWorldProxy<>> myProxy = runtime->buildProxy<E01HelloWorldProxy>(domain,instance, connection);
    myProxy = runtime->buildProxy<E01HelloWorldProxy>(domain,instance, connection);

    std::cout << "Checking availability!" << std::endl;
#if 0
    while (!myProxy->isAvailable())
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    std::cout << "Available..." << std::endl;
#endif


    CommonAPI::ProxyStatusEvent::Subscription mProxyStatus;
    mProxyStatus = myProxy->getProxyStatusEvent().subscribe([&](const CommonAPI::AvailabilityStatus& availabilityStatus)
    {              
        if ((availabilityStatus == CommonAPI::AvailabilityStatus::UNKNOWN) ||
            (availabilityStatus == CommonAPI::AvailabilityStatus::NOT_AVAILABLE))
        {
            // provider is crashed or shutdown
            // It will be notified here, need to reconnect provider later
            std::cout << "service is unavailable..." << std::endl;
            //disconnectService(); // example code           
        }
        else if (availabilityStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
        {
            // notify when provider is available
            std::cout << "service is Available..." << std::endl;
            // can send request when proxy is ready
            sendRequest(); // example code 
        }
        else
        {
            std::cout << "unknown availability status..." << std::endl;
        }
    });



    sighandler_thread.join();

    return 0;
}

