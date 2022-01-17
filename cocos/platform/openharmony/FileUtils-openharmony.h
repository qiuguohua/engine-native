#pragma once
//clang-format off
#include <cstdint>
//clang-format on
#include <rawfile/raw_dir.h>
#include <rawfile/raw_file.h>
#include <rawfile/resource_manager.h>
#include "base/Macros.h"
#include "cocos/platform/FileUtils.h"

namespace cc {

class CC_DLL FileUtilsOpenHarmony : public FileUtils {
public:
    //        FileUtilsOpenHarmony();
    //        virtual ~FileUtilsOpenHarmony();
    static bool initResourceManager(ResourceManager *mgr, const std::string &assetPath, const std::string &moduleName);

    static void setRawfilePrefix(const std::string &prefix);

    bool init() override;

    bool isAbsolutePath(const std::string &strPath) const override;

    std::string getWritablePath() const override;

    std::string expandPath(const std::string &input, bool *isRawFile) const;

    std::pair<int, std::function<void()>> getFd(const std::string &path) const;
    
    long getFileSize(const std::string &filepath) override;

    std::string getSuitableFOpen(const std::string &filenameUtf8) const override;
private:
    bool isFileExistInternal(const std::string &strFilePath) const override;

    bool isDirectoryExistInternal(const std::string &dirPath) const override;

    friend class FileUtils;
};

} // namespace cc
