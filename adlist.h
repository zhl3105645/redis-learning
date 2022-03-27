/* adlist.h - A generic doubly linked list implementation
 *
 * Copyright (c) 2006-2012, Salvatore Sanfilippo <antirez at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __ADLIST_H__
#define __ADLIST_H__

/* Node, List, and Iterator are the only data structures used currently. */

// 双端链表节点
typedef struct listNode {
    struct listNode *prev;
    struct listNode *next;
    void *value;
} listNode;

// 链表 Iterator
typedef struct listIter {
    listNode *next;
    int direction; // 方向：正向 or 反向
} listIter;

// 链表 
typedef struct list {
    listNode *head; // 头节点
    listNode *tail; // 尾节点
    void *(*dup)(void *ptr); // 自定义节点值复制函数
    void (*free)(void *ptr); // 自定义节点释放函数
    int (*match)(void *ptr, void *key); // 自定义节点值匹配函数
    unsigned long len; // 链表长度
} list;

/* Functions implemented as macros */
#define listLength(l) ((l)->len)
#define listFirst(l) ((l)->head)
#define listLast(l) ((l)->tail)
#define listPrevNode(n) ((n)->prev)
#define listNextNode(n) ((n)->next)
#define listNodeValue(n) ((n)->value)

#define listSetDupMethod(l,m) ((l)->dup = (m))
#define listSetFreeMethod(l,m) ((l)->free = (m))
#define listSetMatchMethod(l,m) ((l)->match = (m))

#define listGetDupMethod(l) ((l)->dup)
#define listGetFreeMethod(l) ((l)->free)
#define listGetMatchMethod(l) ((l)->match)

/* Prototypes */
// 链表创建
list *listCreate(void);
// 链表释放
void listRelease(list *list);
// 清空链表
void listEmpty(list *list);
// 头插
list *listAddNodeHead(list *list, void *value);
// 尾插
list *listAddNodeTail(list *list, void *value);
// 插入节点 after or before old_node 
list *listInsertNode(list *list, listNode *old_node, void *value, int after);
// 删除节点
void listDelNode(list *list, listNode *node);
// 链表迭代器
listIter *listGetIterator(list *list, int direction);
// 迭代器下一个节点
listNode *listNext(listIter *iter);
// 释放迭代器
void listReleaseIterator(listIter *iter);
// 复制链表
list *listDup(list *orig);
// 搜索 key 对应的 listNode
listNode *listSearchKey(list *list, void *key);
// 获取 list index 的 listNode
listNode *listIndex(list *list, long index);
// 初始化 迭代器：从头到尾
void listRewind(list *list, listIter *li);
// 初始化 迭代器：从尾到头
void listRewindTail(list *list, listIter *li);
// 将尾节点移至头部
void listRotateTailToHead(list *list);
// 将头节点移至尾部
void listRotateHeadToTail(list *list);
// 将 链表 o 加到 l 后面
void listJoin(list *l, list *o);

/* Directions for iterators */
#define AL_START_HEAD 0 // 从头到尾
#define AL_START_TAIL 1 // 从尾到头

#endif /* __ADLIST_H__ */
