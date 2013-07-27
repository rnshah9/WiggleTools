// Copyright (c) 2013, Daniel Zerbino
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// 
// (1) Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer. 
// 
// (2) Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in
// the documentation and/or other materials provided with the
// distribution.  
// 
// (3)The name of the author may not be used to
// endorse or promote products derived from this software without
// specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
// IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <zlib.h>
#include <pthread.h>

// Local header
#include "wiggleTools.h"
#include "wiggleIterators.h"

void pop(WiggleIterator * wi) {
	// Safety check
	if (wi->done)
		return;
	// Update
	wi->pop(wi);
}

WiggleIterator * newWiggleIterator(void * data, void (*popFunction)(WiggleIterator *), void (*seek)(WiggleIterator *, const char *, int, int)) {
	WiggleIterator * new = (WiggleIterator *) calloc(1, sizeof(WiggleIterator));
	new->data = data;
	new->pop = popFunction;
	new->seek = seek;
	new->chrom = calloc(1000,1);
	new->chrom = calloc(1000,1);
	new->value = 1; // Default value for non-valued bed tracks;
	pop(new);
	return new;
}

void destroyWiggleIterator(WiggleIterator * wi) {
	free(wi->data);
	free(wi->chrom);
	free(wi);
}

void seek(WiggleIterator * wi, const char * chrom, int start, int finish) {
	(*(wi->seek))(wi, chrom, start, finish);
}

FILE * openOrFail(char * filename, char * description, char * mode) {;
	FILE * file;
	if (!(file = fopen(filename, mode))) {
		printf("Could not open %s %s, exiting...\n", (char *) description, (char *) filename);
		exit(1);
	}
	return file;
}
