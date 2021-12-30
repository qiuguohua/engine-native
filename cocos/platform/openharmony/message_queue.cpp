/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "message_queue.h"


void MessageQueue::EnQueue(MessageDataType data)
{
//    queueLock_.lock();
    queue_.push(data);
//    queueLock_.unlock();
}

bool MessageQueue::DeQueue(MessageDataType *data)
{
//    queueLock_.lock();
    if (queue_.empty()) {
//        queueLock_.unlock();
        return false;
    }
    *data = queue_.front();
    queue_.pop();
//    queueLock_.unlock();
    return true;
}

bool MessageQueue::IsEmpty() const
{
    return queue_.empty();
}