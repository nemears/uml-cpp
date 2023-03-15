#include "uml/managers/filePersistencePolicy.h"
#include <fstream>
#include <iterator>
#include <iostream>
#include <filesystem>

namespace UML {

std::string FilePersistencePolicy::loadElementData(ID id) {
    std::string dataPath = "mount/" + id.string() + ".yml"; // TODO extension from serialization policy?
    std::ifstream file(dataPath);
    return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

void FilePersistencePolicy::saveElementData(std::string data, ID id) {
    std::string dataPath = "mount/" + id.string() + ".yml"; // TODO extension from serialization policy?
    std::ofstream file(dataPath);
    file.open(dataPath, std::ios::trunc);
    file << data;
}

void FilePersistencePolicy::eraseEl(ID id) {
    std::string dataPath = "mount/" + id.string() + ".yml"; // TODO extension from serialization policy?
    std::filesystem::remove(dataPath);
}

void FilePersistencePolicy::reindex(ID oldID, ID newID) {
    std::string oldDataPath = "mount/" + oldID.string() + ".yml"; // TODO extension from serialization policy?
    std::string newDataPath = "mount/" + newID.string() + ".yml"; // TODO extension from serialization policy?
    std::filesystem::rename(oldDataPath, newDataPath);
}

std::string FilePersistencePolicy::getProjectData(std::string path) {
    m_projectPath = path;
    return getProjectData();
}

std::string FilePersistencePolicy::getProjectData() {
    std::ifstream file(m_projectPath);
    return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

void FilePersistencePolicy::saveProjectData(std::string data, std::string path) {
    m_projectPath = path;
    saveProjectData(data);
}

void FilePersistencePolicy::saveProjectData(std::string data) {
    std::ofstream file(m_projectPath);
    file.open(m_projectPath, std::ios::trunc);
    file << data;
}

void FilePersistencePolicy::mount(std::string mountPath) {
    m_mountPath = mountPath;
    std::filesystem::create_directories(m_mountPath / std::filesystem::path("mount"));
}

FilePersistencePolicy::~FilePersistencePolicy() {
    // delete mount
    if (!m_mountPath.empty()) {
        try {
            std::filesystem::remove_all(std::filesystem::path(m_mountPath) / "mount");
        } catch (std::exception& e) {
            std::cout << "Persistence policy encountered error deleting mount: " << e.what() << std::endl;
        }
    }
}

}