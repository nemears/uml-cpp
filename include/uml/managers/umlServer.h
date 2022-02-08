#ifndef _UML_UML_SERVER_H_
#define _UML_UML_SERVER_H_

#include "uml/umlManager.h"
#include <atomic>
#include <iostream>
#include <mutex>
#include <condition_variable>

#define UML_PORT 8652
#define UML_SERVER_MSG_SIZE 200
#define UML_SERVER_NUM_ELS 200

namespace std {
    class thread;
}

namespace UML {

    class UmlServer : public UmlManager {
        private:
            struct ClientInfo {
                int socket;
                std::thread* thread;
                std::thread* handler;
                std::mutex handlerMtx;
                std::condition_variable handlerCv;
                std::list<std::string> threadQueue;
            };

            int m_port = UML_PORT;

            //data
            const ID m_shutdownID = ID::randomID();
            int m_socketD = 0;
            std::unordered_map<ID, ClientInfo> m_clients;
            std::unordered_map<std::string, ID> m_urls;
            std::list<ID> m_releaseQueue;
            int m_numEls = 0;
            int m_maxEls = UML_SERVER_NUM_ELS;

            // threading
            static void acceptNewClients(UmlServer* me);
            static void receiveFromClient(UmlServer* me, ID id);
            static void clientSubThreadHandler(UmlServer* me, ID id);
            static void garbageCollector(UmlServer* me);
            void handleMessage(ID id, std::string buff);
            std::thread* m_acceptThread;
            std::thread* m_garbageCollectionThread;
            std::unordered_map<ID, std::mutex> m_locks;
            std::atomic<bool> m_running = false;
            std::mutex m_runMtx;
            std::condition_variable m_runCv;
            std::mutex m_logMtx;
            std::mutex m_acceptMtx;
            std::mutex m_msgMtx;
            std::condition_variable m_msgCv;
            bool m_msgV = false;
            std::mutex m_shutdownMtx;
            std::condition_variable m_shutdownCv;
            bool m_shutdownV = false;
            std::mutex m_garbageMtx;
            std::condition_variable m_garbageCv;

            // helper methods
            void createNode(Element* el) override;
        public:
            UmlServer();
            UmlServer(int port);
            virtual ~UmlServer();
            int numClients();
            void log(std::string msg);
            bool loaded(ID id) override;
            void reset();
            void shutdown();
            void setMaxEls(int maxEls);
            int getMaxEls();
            int getNumElsInMemory();
            int waitTillShutDown(int ms);
            int waitTillShutDown();
            int waitForProcessing();
    };
}

#endif