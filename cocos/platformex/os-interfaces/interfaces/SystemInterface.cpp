/****************************************************************************
 Copyright (c) 2017-2021 Xiamen Yaji Software Co., Ltd.

 http://www.cocos.com

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated engine source code (the "Software"), a limited,
 worldwide, royalty-free, non-assignable, revocable and non-exclusive license
 to use Cocos Creator solely to develop games on your target platforms. You shall
 not use Cocos Creator software for developing other software or tools that's
 used for developing games. You are not granted to publish, distribute,
 sublicense, and/or sell copies of Cocos Creator.

 The software or tools in this License Agreement are licensed, not sold.
 Xiamen Yaji Software Co., Ltd. reserves all rights not expressly granted to you.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
****************************************************************************/
#include "platformex/os-interfaces/interfaces/SystemInterface.h"
#include "platformex/os-interfaces/interfaces/windows/WindowsSystem.h"

namespace cc {

SystemInterface::Ptr SystemInterface::getSystemInterface() {
    return std::make_shared<WindowsSystem>();
}

std::string SystemInterface::getCurrentLanguageToString() {
    LanguageType language    = getCurrentLanguage();
    std::string  languageStr = "";
    switch (language) {
        case SystemInterface::LanguageType::ENGLISH:
            languageStr = "en";
            break;
        case SystemInterface::LanguageType::CHINESE:
            languageStr = "zh";
            break;
        case SystemInterface::LanguageType::FRENCH:
            languageStr = "fr";
            break;
        case SystemInterface::LanguageType::ITALIAN:
            languageStr = "it";
            break;
        case SystemInterface::LanguageType::GERMAN:
            languageStr = "de";
            break;
        case SystemInterface::LanguageType::SPANISH:
            languageStr = "es";
            break;
        case SystemInterface::LanguageType::DUTCH:
            languageStr = "du";
            break;
        case SystemInterface::LanguageType::RUSSIAN:
            languageStr = "ru";
            break;
        case SystemInterface::LanguageType::KOREAN:
            languageStr = "ko";
            break;
        case SystemInterface::LanguageType::JAPANESE:
            languageStr = "ja";
            break;
        case SystemInterface::LanguageType::HUNGARIAN:
            languageStr = "hu";
            break;
        case SystemInterface::LanguageType::PORTUGUESE:
            languageStr = "pt";
            break;
        case SystemInterface::LanguageType::ARABIC:
            languageStr = "ar";
            break;
        case SystemInterface::LanguageType::NORWEGIAN:
            languageStr = "no";
            break;
        case SystemInterface::LanguageType::POLISH:
            languageStr = "pl";
            break;
        case SystemInterface::LanguageType::TURKISH:
            languageStr = "tr";
            break;
        case SystemInterface::LanguageType::UKRAINIAN:
            languageStr = "uk";
            break;
        case SystemInterface::LanguageType::ROMANIAN:
            languageStr = "ro";
            break;
        case SystemInterface::LanguageType::BULGARIAN:
            languageStr = "bg";
            break;
        default:
            languageStr = "unknown";
            break;
    }
    return languageStr;
}

} // namespace cc