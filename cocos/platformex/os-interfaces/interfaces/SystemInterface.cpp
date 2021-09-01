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