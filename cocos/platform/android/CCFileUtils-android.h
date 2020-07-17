/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2013-2016 Chukong Technologies Inc.
Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#ifndef __CC_FILEUTILS_ANDROID_H__
#define __CC_FILEUTILS_ANDROID_H__


#if CC_PLATFORM == CC_PLATFORM_ANDROID

#include "platform/CCFileUtils.h"
#include "base/ccMacros.h"
#include <string>
#include <vector>
#include "jni.h"
#include "android/asset_manager.h"

namespace cc {

class ZipFile;

/**
 * @addtogroup platform
 * @{
 */

//! @brief  Helper class to handle file operations
class CC_DLL FileUtilsAndroid : public FileUtils
{
    friend class FileUtils;
public:
    FileUtilsAndroid();
    /**
     * @js NA
     * @lua NA
     */
    virtual ~FileUtilsAndroid();

    static void setassetmanager(AAssetManager* a);
    static AAssetManager* getAssetManager() { return assetmanager; }
    static ZipFile* getObbFile() { return obbfile; }

    /* override functions */
    bool init() override;
    virtual FileUtils::Status getContents(const std::string& filename, ResizableBuffer* buffer) override;

    virtual std::string getWritablePath() const override;
    virtual bool isAbsolutePath(const std::string& strPath) const override;

private:
    virtual bool isFileExistInternal(const std::string& strFilePath) const override;
    virtual bool isDirectoryExistInternal(const std::string& dirPath) const override;

    static AAssetManager* assetmanager;
    static ZipFile* obbfile;
};

// end of platform group
/// @}

}

#endif // CC_PLATFORM == CC_PLATFORM_ANDROID

#endif // __CC_FILEUTILS_ANDROID_H__
