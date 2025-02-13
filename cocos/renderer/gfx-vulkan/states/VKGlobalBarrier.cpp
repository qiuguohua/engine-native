/****************************************************************************
 Copyright (c) 2020-2021 Xiamen Yaji Software Co., Ltd.

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

#include "VKGlobalBarrier.h"
#include "../VKCommands.h"

namespace cc {
namespace gfx {

CCVKGlobalBarrier::CCVKGlobalBarrier(const GlobalBarrierInfo &info, uint32_t hash) : GlobalBarrier(info, hash) {
    _typedID = generateObjectID<decltype(this)>();

    _gpuBarrier = CC_NEW(CCVKGPUGlobalBarrier);
    _gpuBarrier->accessTypes.resize(info.prevAccesses.size() + info.nextAccesses.size());

    uint32_t index = 0U;
    for (AccessType type : info.prevAccesses) {
        _gpuBarrier->accessTypes[index++] = THSVS_ACCESS_TYPES[static_cast<uint32_t>(type)];
    }
    for (AccessType type : info.nextAccesses) {
        _gpuBarrier->accessTypes[index++] = THSVS_ACCESS_TYPES[static_cast<uint32_t>(type)];
    }

    _gpuBarrier->barrier.prevAccessCount = utils::toUint(info.prevAccesses.size());
    _gpuBarrier->barrier.pPrevAccesses   = _gpuBarrier->accessTypes.data();
    _gpuBarrier->barrier.nextAccessCount = utils::toUint(info.nextAccesses.size());
    _gpuBarrier->barrier.pNextAccesses   = _gpuBarrier->accessTypes.data() + info.prevAccesses.size();

    thsvsGetVulkanMemoryBarrier(_gpuBarrier->barrier, &_gpuBarrier->srcStageMask, &_gpuBarrier->dstStageMask, &_gpuBarrier->vkBarrier);
}

CCVKGlobalBarrier::~CCVKGlobalBarrier() {
    CC_SAFE_DELETE(_gpuBarrier);
}

} // namespace gfx
} // namespace cc
