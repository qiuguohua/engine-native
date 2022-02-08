#include "cocos/platform/openharmony/FileUtils-OpenHarmony.h"
#include <hilog/log.h>
#include <sys/stat.h>
#include <cstdio>
#include <regex>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#include "cocos/base/Log.h"
#include "cocos/platform/java/jni/JniHelper.h"

#include <sys/syscall.h>
#include <unistd.h>
#include "platform/openharmony/common/PluginCommon.h"

#define ASSETS_FOLDER_NAME "/sdcard/cocos/xcsample/"
//#define ASSETS_FOLDER_NAME "/data/data/ohos.example.xcomponent1/files/"
#define ASSETS_FOLDER_WRITEABLE_PATH "/data/data/ohos.example.xcomponent1/files/writeable_path"
//#define ASSETS_FOLDER_WRITEABLE_PATH "/sdcard/cocos/writeable_path"

#ifndef JCLS_HELPER
    #define JCLS_HELPER "com/cocos/lib/CocosHelper"
#endif

namespace cc {

namespace {

std::string rawfilePrefix = "entry/resources/rawfile/";

void printRawfiles(ResourceManager *mgr, const std::string &path) {
//    auto *file = OpenRawFile(mgr, path.c_str());
//    if (file) {
//        HILOG_DEBUG(LOG_APP, "PrintRawfile %{public}s", path.c_str());
//        return;
//    }
//
//    RawDir *dir = OpenRawDir(mgr, path.c_str());
//    if (dir) {
//        auto fileCnt = GetRawFileCount(dir);
//        for (auto i = 0; i < fileCnt; i++) {
//            std::string subFile  = GetRawFileName(dir, i);
//            auto        newPath  = path + "/" + subFile; // NOLINT
//            auto        debugPtr = newPath.c_str();
//            HILOG_ERROR(LOG_APP, " find path %{public}s", newPath.c_str());
//            printRawfiles(mgr, newPath);
//        }
//    } else {
//        HILOG_ERROR(LOG_APP, "Invalidate path %{public}s", path.c_str());
//    }
}
} // namespace

bool FileUtilsOpenHarmony::initResourceManager(ResourceManager *mgr, const std::string &assetPath, const std::string &moduleName) {
//    CCASSERT(mgr, "ResourceManager should not be empty!");
//    ohosResourceMgr = mgr;
//    if (!assetPath.empty() && assetPath[assetPath.length() - 1] != '/') {
//        ohosAssetPath = assetPath + "/";
//    } else {
//        ohosAssetPath = assetPath;
//    }
//    if (!moduleName.empty()) {
//        setRawfilePrefix(moduleName + "/resources/rawfile/");
//    }
    return true;
}

void FileUtilsOpenHarmony::setRawfilePrefix(const std::string &prefix) {
    //rawfilePrefix = prefix;
}


FileUtils *FileUtils::getInstance() {
    if (FileUtils::sharedFileUtils == nullptr) {
        FileUtils::sharedFileUtils = new FileUtilsOpenHarmony();
        if (!FileUtils::sharedFileUtils->init()) {
            delete FileUtils::sharedFileUtils;
            FileUtils::sharedFileUtils = nullptr;
            CC_LOG_DEBUG("ERROR: Could not init CCFileUtilsAndroid");
        }
    }
    return FileUtils::sharedFileUtils;
}

bool FileUtilsOpenHarmony::init() {
    _defaultResRootPath = ASSETS_FOLDER_NAME;
    return FileUtils::init();
}

bool FileUtilsOpenHarmony::isAbsolutePath(const std::string &strPath) const {
    return !strPath.empty() && (strPath[0] == '/' || strPath.find(ASSETS_FOLDER_NAME) == 0);
}

std::string FileUtilsOpenHarmony::getSuitableFOpen(const std::string &filenameUtf8) const {
    return filenameUtf8;
}

long FileUtilsOpenHarmony::getFileSize(const std::string &filepath) {
    if (filepath.empty()) {
        return 0;
    }

    auto *fs = FileUtils::getInstance();

    std::string fullPath = fs->fullPathForFilename(filepath);
    if (fullPath.empty()) {
        return 0;
    }

    FILE *fp = fopen(getSuitableFOpen(fullPath).c_str(), "rb");
    if (!fp) {
        return 0;
    }
    struct stat statBuf;
    auto descriptor = fileno(fp);
    if (fstat(descriptor, &statBuf) == -1) {
        fclose(fp);
        return 0;
    }
    fclose(fp);
    return statBuf.st_size;
}

std::string FileUtilsOpenHarmony::getWritablePath() const {
    return ASSETS_FOLDER_WRITEABLE_PATH;
}

bool FileUtilsOpenHarmony::isFileExistInternal(const std::string &strFilePath) const {
    if (strFilePath.empty()) {
        return false;
    }
    std::string strPath = strFilePath;
    if (!isAbsolutePath(strPath)) { // Not absolute path, add the default root path at the beginning.
        strPath.insert(0, _defaultResRootPath);
    }
    FILE *fp = fopen(getSuitableFOpen(strPath).c_str(), "r");
    LOGE("qgh cocos isFileExistInternal1 %s", getSuitableFOpen(strPath).c_str());
    if (!fp) {
        LOGE("qgh cocos isFileExistInternal2 %s", getSuitableFOpen(strPath).c_str());
        return false;
    }
    LOGE("qgh cocos isFileExistInternal3 %s", getSuitableFOpen(strPath).c_str());
    fclose(fp);
    return true;
}

bool FileUtilsOpenHarmony::isDirectoryExistInternal(const std::string &dirPath) const {
//    if (dirPath.empty()) return false;
//    std::string dirPathMf = dirPath[dirPath.length() - 1] == '/' ? dirPath.substr(0, dirPath.length() - 1) : dirPath;
//
//    if (dirPathMf[0] == '/') {
//        struct stat st;
//        return stat(dirPathMf.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
//    }
//
//    if (dirPathMf.find(_defaultResRootPath) == 0) {
//        dirPathMf = rawfilePrefix + dirPathMf.substr(_defaultResRootPath.length(), dirPathMf.length());
//    }
//    assert(ohosResourceMgr);
//    auto dir = OpenRawDir(ohosResourceMgr, dirPathMf.c_str());
//    if (dir != nullptr) {
//        CloseRawDir(dir);
//        return true;
//    }
    return false;
}

std::string FileUtilsOpenHarmony::expandPath(const std::string &input, bool *isRawFile) const {
//    if (!input.empty() && input[0] == '/') {
//        if (isRawFile) *isRawFile = false;
//        return input;
//    }
//    const auto fullpath = fullPathForFilename(input);
//
//    if (fullpath.find(_defaultResRootPath) == 0) {
//        if (isRawFile) *isRawFile = true;
//        return rawfilePrefix + fullpath.substr(_defaultResRootPath.length(), fullpath.length());
//    }
//
//    if (isRawFile) *isRawFile = false;
//
//    return fullpath;
    return "";
}

std::pair<int, std::function<void()>> FileUtilsOpenHarmony::getFd(const std::string &path) const {
//    bool       isRawFile = false;
//    const auto fullpath  = expandPath(path, &isRawFile);
//    if (isRawFile) {
//        RawFile *rf = OpenRawFile(ohosResourceMgr, fullpath.c_str());
//        // FIXME: try reuse file
//        const auto bufSize   = GetRawFileSize(rf);
//        auto       fileCache = std::vector<char>(bufSize);
//        auto *     buf       = fileCache.data();
//        // Fill buffer
//        const auto readBytes = ReadRawFile(rf, buf, bufSize);
//        assert(readBytes == bufSize); // read failure ?
//        auto fd = syscall(__NR_memfd_create, fullpath.c_str(), 0);
//        {
//            auto writeBytes = ::write(fd, buf, bufSize); // Write can fail?
//            assert(writeBytes == bufSize);
//            ::lseek(fd, 0, SEEK_SET);
//        }
//        if (errno != 0) {
//            const auto *errMsg = strerror(errno);
//            CC_LOG_ERROR("failed to open buffer fd %s", errMsg);
//        }
//        return std::make_pair(fd, [fd]() {
//            close(fd);
//        });
//    }
//
//    FILE *fp = fopen(fullpath.c_str(), "rb");
//    return std::make_pair(fileno(fp), [fp]() {
//        fclose(fp);
//    });
    return std::make_pair(0, []() {});
}

} // namespace cc