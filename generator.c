/*
SPDX-License-Identifier: ISC

Copyright (c) 2026 Mustafa Al-Ameri

Permission to use, copy, modify, and/or distribute this software for any purpose
with or without fee is hereby granted, provided that the above copyright notice
and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED “AS IS” AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
THIS SOFTWARE.
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct StrVec {
	char** pData;
	size_t capacity;
	size_t itemCount;
} StrVec;

void str_vec_init(StrVec* pVector, size_t initialCapacity) {
	pVector->pData = malloc(initialCapacity * sizeof(char*));
	if (pVector->pData == NULL) {
		exit(EXIT_FAILURE);
	}
	pVector->capacity = initialCapacity;
	pVector->itemCount = 0;
}

void str_vec_push(StrVec* pVector, char* strValue) {
	if (pVector->itemCount >= pVector->capacity) {
		pVector->capacity *= 2;
		pVector->pData =
			realloc(pVector->pData, pVector->capacity * sizeof(char*));
		if (pVector->pData == NULL) {
			exit(EXIT_FAILURE);
		}
	}
	pVector->pData[pVector->itemCount++] = strValue;
}

void str_vec_free(StrVec* pVector) {
	for (size_t i = 0; i < pVector->itemCount; ++i) {
		free(pVector->pData[i]);
	}
	free(pVector->pData);
}

int main(void) {
	FILE* pTemplate = fopen("template0.h", "rb");
	if (pTemplate != NULL) {
		int ch;
		while ((ch = fgetc(pTemplate)) != EOF) {
			fputc(ch, stdout);
		}
		fclose(pTemplate);
	}

	FILE* pGLHeaderFile = fopen("GL/glcorearb.h", "rb");
	if (pGLHeaderFile == NULL) {
		return EXIT_FAILURE;
	}

	StrVec vecNames, vecUpperNames;
	str_vec_init(&vecNames, 256);
	str_vec_init(&vecUpperNames, 256);

	char pLineBuffer[2048];
	while (fgets(pLineBuffer, sizeof(pLineBuffer), pGLHeaderFile) != NULL) {
		if (strncmp(pLineBuffer, "GLAPI ", 6) != 0) continue;

		char* pApiEntry = strstr(pLineBuffer, "APIENTRY ");
		if (pApiEntry == NULL) continue;

		char* strFunctionName =
			strtok(pApiEntry + strlen("APIENTRY "), " (\t\r\n;");
		if (strFunctionName == NULL) continue;

		char strUppercaseFunctionName[2048];
		size_t j = 0;
		for (; strFunctionName[j] != '\0'; ++j) {
			strUppercaseFunctionName[j] =
				(char)toupper((unsigned char)strFunctionName[j]);
		}
		strUppercaseFunctionName[j] = '\0';

		printf_s("PFN%sPROC %s;\n", strUppercaseFunctionName, strFunctionName);
		str_vec_push(&vecNames, strdup(strFunctionName));
		str_vec_push(&vecUpperNames, strdup(strUppercaseFunctionName));
	}
	fclose(pGLHeaderFile);

	pTemplate = fopen("template1.h", "rb");
	if (pTemplate != NULL) {
		int ch;
		while ((ch = fgetc(pTemplate)) != EOF) {
			fputc(ch, stdout);
		}
		fclose(pTemplate);
	}

	for (size_t i = 0; i < vecNames.itemCount; ++i) {
		printf_s(
			"\t%s = (PFN%sPROC) glfl_get_func(\"%s\");\n",
			vecNames.pData[i],
			vecUpperNames.pData[i],
			vecNames.pData[i]);
	}

	pTemplate = fopen("template2.h", "rb");
	if (pTemplate != NULL) {
		int ch;
		while ((ch = fgetc(pTemplate)) != EOF) {
			fputc(ch, stdout);
		}
		fclose(pTemplate);
	}

	str_vec_free(&vecNames);
	str_vec_free(&vecUpperNames);

	return EXIT_SUCCESS;
}
