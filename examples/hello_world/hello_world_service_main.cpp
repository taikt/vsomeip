// Copyright (C) 2015-2017 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
#ifndef VSOMEIP_ENABLE_SIGNAL_HANDLING
#include <csignal>
#endif
#include <vsomeip/vsomeip.hpp>
#include "hello_world_service.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <iostream>

static std::condition_variable_any sighandler_condition;
static std::recursive_mutex sighandler_mutex;

//#ifndef VSOMEIP_ENABLE_SIGNAL_HANDLING
hello_world_service *hw_srv_ptr(nullptr);
    void handle_signal(int _signal) {
        if (hw_srv_ptr != nullptr &&
                (_signal == SIGINT || _signal == SIGTERM)) {
                    LOG_INF("receive stop signal");
                     //hw_srv_ptr->terminate();
                     sighandler_condition.notify_one();
                }
    }
           
//#endif
// hello_world_service hw_srv;

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    hello_world_service hw_srv;
//#ifndef VSOMEIP_ENABLE_SIGNAL_HANDLING
    hw_srv_ptr = &hw_srv;
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
//#endif

    std::thread sighandler_thread([&]() {
        while (1) {
            std::unique_lock<std::recursive_mutex> its_lock(sighandler_mutex);
            sighandler_condition.wait(its_lock);
            LOG_INF("receive stop signal");
            //hw_srv.stop();
            hw_srv.terminate();
            return;
            
        }
    });
    

    if (hw_srv.init()) {
        hw_srv.start();
        //return 0;
    } else {
        //return 1;
    }
    sighandler_thread.join();
    
}
