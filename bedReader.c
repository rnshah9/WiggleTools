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

#include <stdlib.h>
#include <string.h> 

#include "wiggleIterators.h"

typedef struct bedReaderData_st {
	char  *filename;
	FILE * file;
	int stop;
} BedReaderData;

void BedReaderPop(WiggleIterator * wi) {
	BedReaderData * data = (BedReaderData *) wi->data;
	char line[5000];
	char chrom[1000];

	while (fgets(line, 5000, data->file)) {
		int start, finish;
		sscanf(line, "%s\t%i\t%i", chrom, &start, &finish);
		// I like my finishes to be non inclusive...
		finish++;

		if (wi->chrom[0] == '\0' || strcmp(wi->chrom, chrom) < 0) {
			wi->chrom = (char *) calloc(strlen(chrom), sizeof(char));
			strcpy(wi->chrom, chrom);
			wi->finish = -1;
		}

		if (finish <= wi->finish)
			continue;
		else if (start < wi->finish)
			wi->start = wi->finish;
		else
			wi->start = start;
		wi->finish = finish;

		if (wi->start > data->stop)
			wi->done = true;

		return;
	}

	wi->done = true;
}

void BedReaderSeek(WiggleIterator * wi, const char * chrom, int start, int finish) {
	BedReaderData * data = (BedReaderData*) wi->data;
	if (strcmp(chrom, wi->chrom) < 0 || start < wi->start) {
		fclose(data->file);
		data->file = openOrFail(data->filename, "input file", "r");
	}

	while (wi->finish < finish || strcmp(chrom, wi->chrom) < 0)
		BedReaderPop(wi);

	data->stop = finish;
}


WiggleIterator * BedReader(char * filename) {
	BedReaderData * data = (BedReaderData *) calloc(1, sizeof(BedReaderData));
	data->filename = filename;
	data->stop = -1;
	data->file = openOrFail(filename, "Bed file", "r");
	WiggleIterator * new = newWiggleIterator(data, &BedReaderPop, &BedReaderSeek);
	new->value = 1;
	return new;
}
