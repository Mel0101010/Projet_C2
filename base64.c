//
// Created by melo on 4/8/25.
//

#include "base64.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

void encoder_base64(const char *input, char **output) {
	BIO *bmem = BIO_new(BIO_s_mem());
	BIO *b64 = BIO_new(BIO_f_base64());
	b64 = BIO_push(b64, bmem);

	BIO_write(b64, input, strlen(input));
	BIO_flush(b64);

	BUF_MEM *bptr;
	BIO_get_mem_ptr(b64, &bptr);

	*output = (char *)malloc(bptr->length + 1);
	memcpy(*output, bptr->data, bptr->length);
	(*output)[bptr->length] = '\0';

	BIO_free_all(b64);
}

void decoder_base64(const char *input, char **output) {
	BIO *b64 = BIO_new(BIO_f_base64());
	BIO *bmem = BIO_new_mem_buf(input, strlen(input));
	bmem = BIO_push(b64, bmem);

	int length = BIO_read(bmem, *output, strlen(input));
	(*output)[length] = '\0';

	BIO_free_all(bmem);
}