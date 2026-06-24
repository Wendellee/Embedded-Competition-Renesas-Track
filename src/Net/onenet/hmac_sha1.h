/*
 * hmac_sha1.h
 *
 *  Created on: 2026年3月12日
 *      Author: lingk
 */

#ifndef NET_ONENET_HMAC_SHA1_H_
#define NET_ONENET_HMAC_SHA1_H_

#define MAX_MESSAGE_LENGTH      1024


void hmac_sha1(
    unsigned char *key,
    int key_length,
    unsigned char *data,
    int data_length,
    unsigned char *digest
);


#endif /* NET_ONENET_HMAC_SHA1_H_ */
