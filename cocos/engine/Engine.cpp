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

#include "engine/Engine.h"
#include "base/AutoreleasePool.h"
#include "base/Macros.h"
#include "platformex/AbstractPlatform.h"

#include "cocos/bindings/jswrapper/SeApi.h"
#include "cocos/renderer/GFXDeviceManager.h"
#include "pipeline/RenderPipeline.h"

#if USE_AUDIO
    #include "cocos/audio/include/AudioEngine.h"
#endif

#if USE_SOCKET
    #include "cocos/network/WebSocket.h"
#endif

#include <memory>
#include <sstream>
#include "base/Scheduler.h"
#include "cocos/network/HttpClient.h"
#include "engine/EngineManager.h"
#include "platformex/os-interfaces/interfaces/SystemWindowUI.h"
#include "sdl2/SDL_events.h"

namespace {

bool setCanvasCallback(se::Object* global) {
    std::stringstream   ss;
    se::ScriptEngine*   se              = se::ScriptEngine::getInstance();
    cc::SystemWindowUI* window          = CURRENT_ENGINE()->GetOSInterface<cc::SystemWindowUI>();
    auto                handler         = window->getWindowHandler();
    auto                viewSize        = window->getViewSize();
    char                commandBuf[200] = {0};
    ss << "window.innerWidth = " << viewSize[0] << "; "
       << "window.innerHeight = " << viewSize[1] << "; "
       << "window.windowHandler = "
       << reinterpret_cast<intptr_t>(handler)
       << ";";

    se->evalString(ss.str().c_str());
    return true;
}

template <typename T>
std::enable_if_t<std::is_base_of<cc::OSEvent, T>::value, const T&>
EventCast(const cc::OSEvent& ev) {
    const T& ev_detail = dynamic_cast<const T&>(ev);
    //CC_ASSERT(ev_detail);
    return std::move(ev_detail);
}

} // namespace

namespace cc {

Engine::Engine() {
}

Engine::~Engine() {
#if USE_AUDIO
    AudioEngine::end();
#endif

    pipeline::RenderPipeline::getInstance()->destroy();

    EventDispatcher::destroy();
    se::ScriptEngine::destroyInstance();

    gfx::DeviceManager::destroy();
}

int Engine::init() {
    _scheduler = std::make_shared<Scheduler>();
    FileUtils::getInstance()->addSearchPath("Resources", true);

    EventDispatcher::init();

    EngineManager::getInstance()->setCurrentEngine(shared_from_this());

    AbstratctPlatform* platform = AbstratctPlatform::GetPlatform();
    platform->setEventHandleCallback(
        std::bind(&Engine::EventHandle,
                  this, std::placeholders::_1, std::placeholders::_2));

    se::ScriptEngine::getInstance()->addPermanentRegisterCallback(setCanvasCallback);
    return 0;
}

int Engine::run() {
    SystemWindowUI* pevent = GetOSInterface<SystemWindowUI>();
    while (!_quit) {
        tick();
        pevent->pollEvent();
    }
    return 0;
}

void Engine::pause() {
    cc::EventDispatcher::dispatchEnterBackgroundEvent();
}

void Engine::resume() {
    cc::EventDispatcher::dispatchEnterForegroundEvent();
}

int Engine::restart() {
    _needRestart = true;
    return 0;
}

void Engine::close() { // NOLINT
    if (cc::EventDispatcher::initialized()) {
        cc::EventDispatcher::dispatchCloseEvent();
    }

    auto* scriptEngine = se::ScriptEngine::getInstance();

    cc::PoolManager::getInstance()->getCurrentPool()->clear();
#if USE_AUDIO
    cc::AudioEngine::stopAll();
#endif
#if USE_SOCKET
    cc::network::WebSocket::closeAllConnections();
#endif
    cc::network::HttpClient::destroyInstance();

    _scheduler->removeAllFunctionsToBePerformedInCocosThread();
    _scheduler->unscheduleAll();

    scriptEngine->cleanup();
    cc::EventDispatcher::destroy();

    // exit
    exit(0);
}

uint Engine::getTotalFrames() const {
    return _totalFrames;
}

void Engine::setPreferredFramesPerSecond(int fps) {
    if (fps == 0) {
        return;
    }

    _fps                            = fps;
    _prefererredNanosecondsPerFrame = static_cast<long>(1.0 / _fps * NANOSECONDS_PER_SECOND); //NOLINT(google-runtime-int)
}

EngineScheduler::Ptr Engine::getEngineScheduler() {
    return _scheduler;
}

void Engine::addEvent(OSEventType evtype, EventCb cb) {
    _eventCallbacks.insert(std::make_pair(evtype, cb));
}

void Engine::removeEvent(OSEventType evtype) {
    auto it = _eventCallbacks.find(evtype);
    if (it != _eventCallbacks.end()) {
        _eventCallbacks.erase(it);
    }
    // debug;
    CC_ASSERT(false);
}

void Engine::restartVM() {
    cc::EventDispatcher::dispatchRestartVM();

    pipeline::RenderPipeline::getInstance()->destroy();

    auto* scriptEngine = se::ScriptEngine::getInstance();

    cc::PoolManager::getInstance()->getCurrentPool()->clear();
#if USE_AUDIO
    cc::AudioEngine::stopAll();
#endif
#if USE_SOCKET
    cc::network::WebSocket::closeAllConnections();
#endif
    cc::network::HttpClient::destroyInstance();

    _scheduler->removeAllFunctionsToBePerformedInCocosThread();
    _scheduler->unscheduleAll();

    scriptEngine->cleanup();
    cc::EventDispatcher::destroy();

    // start

    cc::EventDispatcher::init();
    init();
    cc::gfx::DeviceManager::addCustomEvent();
}

void Engine::tick() {
    if (_needRestart) {
        restartVM();
        _needRestart = false;
    }

    static std::chrono::steady_clock::time_point prevTime;
    static std::chrono::steady_clock::time_point now;
    static float                                 dt   = 0.F;
    static double                                dtNS = NANOSECONDS_60FPS;

    ++_totalFrames;

    // iOS/macOS use its own fps limitation algorithm.
#if (CC_PLATFORM == CC_PLATFORM_ANDROID || CC_PLATFORM == CC_PLATFORM_WINDOWS || CC_PLATFORM == CC_PLATFORM_OHOS)
    if (dtNS < static_cast<double>(_prefererredNanosecondsPerFrame)) {
        std::this_thread::sleep_for(
            std::chrono::nanoseconds(_prefererredNanosecondsPerFrame - static_cast<int64_t>(dtNS)));
        dtNS = static_cast<double>(_prefererredNanosecondsPerFrame);
    }
#endif

    prevTime = std::chrono::steady_clock::now();

    _scheduler->update(dt);
    cc::EventDispatcher::dispatchTickEvent(dt);

    LegacyAutoreleasePool* currentPool = PoolManager::getInstance()->getCurrentPool();
    if (currentPool) {
        currentPool->clear();
    }

    now  = std::chrono::steady_clock::now();
    dtNS = dtNS * 0.1 + 0.9 * static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(now - prevTime).count());
    dt   = static_cast<float>(dtNS) / NANOSECONDS_PER_SECOND;
}

bool Engine::EventHandle(OSEventType type, const OSEvent& ev) {
    if (type == OSEventType::KEYBOARD_OSEVENT) {
        cc::EventDispatcher::dispatchKeyboardEvent(EventCast<KeyboardEvent>(ev));
        return true;
    } else if (type == OSEventType::KEYBOARD_OSEVENT) {
        cc::EventDispatcher::dispatchCustomEvent(EventCast<CustomEvent>(ev));
        return true;
    } else if (type == OSEventType::MOUSE_OSEVENT) {
        cc::EventDispatcher::dispatchMouseEvent(EventCast<MouseEvent>(ev));
        return true;
    } else if (type == OSEventType::TOUCH_OSEVENT) {
        cc::EventDispatcher::dispatchTouchEvent(EventCast<TouchEvent>(ev));
        return true;
    } else if (type == OSEventType::WINDOW_OSEVENT) {
        const WindowEven& wev = EventCast<WindowEven>(ev);
        if (wev.type == WindowEven::Type::WINDOWEVENT_SHOW ||
            wev.type == WindowEven::Type::WINDOWEVENT_RESTORED) {
            resume();
        } else if (wev.type == WindowEven::Type::WINDOWEVENT_SIZE_CHANGED ||
                   wev.type == WindowEven::Type::WINDOWEVENT_RESIZED) {
            cc::EventDispatcher::dispatchResizeEvent(wev.width, wev.height);
        } else if (wev.type == WindowEven::Type::WINDOWEVENT_HIDDEN ||
                   wev.type == WindowEven::Type::WINDOWEVENT_MINIMIZED) {
            pause();
        } else if (wev.type == WindowEven::Type::WINDOWEVENT_CLOSE) {
            cc::EventDispatcher::dispatchCloseEvent();
        } else if (wev.type == WindowEven::Type::WINDOWEVENT_QUIT) {
            _quit = true;
        }
        return true;
    }
    for (auto it : _eventCallbacks) {
        if (type == it.first) {
            it.second(ev);
            break;
        }
    }
    return false;
}
} // namespace cc