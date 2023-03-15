#pragma once
#include "uml/id.h"

namespace UML {
    class FilePersistencePolicy {
        private:
            std::string m_projectPath = "project.yml";
            std::string m_mountPath = ".";
        protected:
            std::string loadElementData(ID id);
            void saveElementData(std::string data, ID id);
            std::string getProjectData(std::string path);
            std::string getProjectData();
            void saveProjectData(std::string data, std::string path);
            void saveProjectData(std::string data);
            void eraseEl(ID id);
            void reindex(ID oldID, ID newID);
        public:
            void mount(std::string mountPath);
            virtual ~FilePersistencePolicy();
    };
}