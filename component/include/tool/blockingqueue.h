/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-15 15:55:56
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-15 15:59:35
 * @FilePath: \EOS\component\include\tool\blockingqueue.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_BLOCKINGQUEUE_H
#define EOS_BLOCKINGQUEUE_H

#include "qqueue.h"
#include "qwaitcondition.h"
#include "qmutex.h"

template <typename T>
class COMPONENT_IMEXPORT BlockingQueue
{
public:
    explicit  BlockingQueue() {}
    void enqueue(const T& value)
    {
        QMutexLocker locker(&mutex);
        queue.enqueue(value);
        condition.wakeOne();   //唤醒在deque这里wait的线程
    }
    T dequeue()
    {
        QMutexLocker locker(&mutex);
        while (queue.isEmpty()) {
            condition.wait(&mutex);
        }
        return queue.dequeue();
    }
    bool isEmpty() const
    {
        QMutexLocker locker(&mutex);
        return queue.isEmpty();
    }
    int size() const
    {
        QMutexLocker locker(&mutex);
        return queue.size();
    }

private:
    QQueue<T> queue;
    mutable QMutex mutex;
    QWaitCondition condition;
};

#endif //EOS_BLOCKINGQUEUE_H
