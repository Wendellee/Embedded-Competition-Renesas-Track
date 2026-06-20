/*
 * buding.c
 *
 *  Created on: 2026年6月20日
 *      Author: Little Prince
 */
// 补丁：手动定义堆内存边界符号，解决 bsp_sbrk.c 的链接报错
void *__HeapBase = 0;
void *__HeapLimit = 0;

