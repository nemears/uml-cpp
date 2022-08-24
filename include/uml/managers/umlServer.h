#ifndef _UML_UML_SERVER_H_
#define _UML_UML_SERVER_H_

// #include "uml/umlManager.h"
#include "threadSafeManager.h"
#include <atomic>
#include <iostream>
#include <mutex>
#include <condition_variable>
#ifdef WIN32
#include "winsock2.h"
#include <ws2tcpip.h>
#include <stdio.h>
typedef SOCKET socketType;
#else
typedef int socketType;
#endif

#define UML_PORT 8652
#define UML_SERVER_MSG_SIZE 200
#define UML_SERVER_NUM_ELS 200

namespace std {
    class thread;
}

namespace UML {

    class UmlServer : public ThreadSafeManager {

        private:
            struct ClientInfo {
                socketType socket;
                std::thread* thread;
                std::thread* handler;
                std::mutex handlerMtx;
                std::condition_variable handlerCv;
                std::list<std::string> threadQueue;
            };

            int m_port = UML_PORT;

            //data
            const ID m_shutdownID = ID::randomID();
            socketType m_socketD = 
            #ifndef WIN32
            0;
            #else
            INVALID_SOCKET;
            WSADATA m_wsaData;
            #endif
            std::unordered_map<ID, ClientInfo> m_clients;
            std::unordered_map<std::string, ID> m_urls;
            std::list<ID> m_releaseQueue;
            long unsigned int m_numEls = 0;
            long unsigned int m_maxEls = UML_SERVER_NUM_ELS;

            // threading
            static void acceptNewClients(UmlServer* me);
            static void receiveFromClient(UmlServer* me, ID id);
            static void clientSubThreadHandler(UmlServer* me, ID id);
            static void garbageCollector(UmlServer* me);
            static void zombieKiller(UmlServer* me);
            void handleMessage(ID id, std::string buff);
            std::thread* m_acceptThread = 0;
            std::thread* m_garbageCollectionThread = 0;
            std::thread* m_zombieKillerThread = 0;
            std::unordered_map<ID, std::mutex> m_locks;
            std::mutex m_locksMtx;
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
            std::list<ID> m_zombies;
            std::mutex m_zombieMtx;
            std::condition_variable m_zombieCv;
            std::mutex m_graphMtx;

            // helper methods
        protected:
            // void eraseNode(ManagerNode* node, ID id);// override;
            // void createNode(Element* el);// override;
            void closeClientConnections(ClientInfo& client);
            std::vector<std::unique_lock<std::mutex>> lockReferences(Element& el);
        public:
            UmlServer();
            UmlServer(int port);
            UmlServer(int port, bool deferStart);
            UmlServer(bool deferStart);
            virtual ~UmlServer();
            void start();
            int numClients();
            void log(std::string msg);
            // bool loaded(ID id);// override;
            size_t count(ID id);// override;
            void reset();
            // void reindex(ID oldID, ID newID) override;
            // void forceRestore(ElementPtr ref, Parsers::ParserMetaData& data);// override;
            void shutdownServer();
            void setMaxEls(int maxEls);
            int getMaxEls();
            int getNumElsInMemory();
            int waitTillShutDown(int ms);
            int waitTillShutDown();
            int waitForProcessing();
            void setRoot(Element* el); // override;
            void setRoot(Element& el);
    };
}

#endif