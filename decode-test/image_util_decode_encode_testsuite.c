/*
 * image-utility
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Vineeth T M <vineeth.tm@samsung.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <image_util.h>
#include <glib.h>

#define DECODE_RESULT_PATH "/media/decode_test."
#define BUFFER_SIZE 128

unsigned long image_width = 0, image_height = 0;
unsigned long long image_size = 0;

GCond g_thread_cond;
GMutex g_thread_mutex;

void _wait()
{
	g_mutex_lock(&g_thread_mutex);
	fprintf(stderr, "waiting... until finishing \n");
	g_cond_wait(&g_thread_cond, &g_thread_mutex);
	fprintf(stderr, "<=== get signal from callback \n");
	g_mutex_unlock(&g_thread_mutex);
}

void _signal()
{
	g_mutex_lock(&g_thread_mutex);
	g_cond_signal(&g_thread_cond);
	fprintf(stderr, "===> send signal to test proc \n");
	g_mutex_unlock(&g_thread_mutex);
}

static inline void flush_stdin()
{
	int ch;
	while ((ch = getchar()) != EOF && ch != '\n') ;
}

static int _read_file(char *file_name, void **data, unsigned long long *data_size)
{
	FILE *fp = NULL;
	long file_size = 0;

	if (!file_name || !data || !data_size) {
		fprintf(stderr, "\tNULL pointer\n");
		return FALSE;
	}

	fprintf(stderr, "\tTry to open %s to read\n", file_name);

	fp = fopen(file_name, "r");
	if (fp == NULL) {
		fprintf(stderr, "\tfile open failed %d\n", errno);
		return FALSE;
	}

	fseek(fp, 0, SEEK_END);
	file_size = ftell(fp);
	if (file_size > -1L) {
		rewind(fp);
		*data = (void *)malloc(file_size);
		if (*data == NULL) {
			fprintf(stderr, "\tmalloc failed %d\n", errno);
			fclose(fp);
			fp = NULL;
			return FALSE;
		} else {
			if (fread(*data, 1, file_size, fp)) {
				fprintf(stderr, "#Success# fread\n");
			} else {
				fprintf(stderr, "#Error# fread\n");
				fclose(fp);
				fp = NULL;
				return FALSE;
			}
		}
		fclose(fp);
		fp = NULL;

		if (*data) {
			*data_size = (unsigned long long)file_size;
			return TRUE;
		} else {
			*data_size = 0;
			return FALSE;
		}
	} else {
		fprintf(stderr, "#Error# ftell\n");
		fclose(fp);
		fp = NULL;
		return FALSE;
	}
}

static int _write_file(const char *file_name, void *data, unsigned long long data_size)
{
	FILE *fp = NULL;

	if (!file_name || !data || data_size <= 0) {
		fprintf(stderr, "\tinvalid data %s %p size:%lld\n", file_name, data, data_size);
		return FALSE;
	}

	fprintf(stderr, "\tTry to open %s to write\n", file_name);

	fp = fopen(file_name, "w");
	if (fp == NULL) {
		fprintf(stderr, "\tfile open failed %d\n", errno);
		return FALSE;
	}

	fwrite(data, 1, data_size, fp);
	fclose(fp);
	fp = NULL;

	fprintf(stderr, "\tfile [%s] write DONE\n", file_name);

	return TRUE;
}

bool decode_completed_cb(int error, void *user_param, unsigned long width, unsigned long height, unsigned long long size)
{
	image_width = width;
	image_height = height;
	_signal();

	return TRUE;
}

bool encode_completed_cb(int error, void *user_param, unsigned long long size)
{
	image_size = size;
	_signal();

	return TRUE;
}

int main(int argc, char *argv[])
{
	int ret = 0;
	image_util_decode_h decoded = NULL;
	image_util_encode_h encoded = NULL;
	void *src = NULL;
	unsigned char *data = NULL;
	unsigned char **animated_data = NULL;
	unsigned char *dst = NULL;
	unsigned long long src_size = 0;
	int encode_image_type = -1;

	if (argc < 3) {
		fprintf(stderr, "\t[usage]\n");
		fprintf(stderr, "\t\t1. decode/encode : capi-media-image-util-decode-test decode/decode-mem/decode-async filepath encode_image_type\n");
		fprintf(stderr, "\t\t2. encode gif : capi-media-image-util-decode-test encode-gif/encode-gif-mem 'folderpath containing png images named \
					with number prefix according to the animation order'\n");
		return 0;
	}

	if (argv[3])
		encode_image_type = atoi(argv[3]);

	if (!strcmp("encode-gif", argv[1]) || !strcmp("encode-gif-mem", argv[1])) {
		struct dirent *dp;
		DIR *fd;
		int number_files = 0, i = 0;
		char gif_filename[BUFFER_SIZE] = { 0, }, temp_filename[BUFFER_SIZE] = {
		0,}, temp[BUFFER_SIZE] = {
		0,};
		unsigned long gif_image_width[100] = { 0, }, gif_image_height[100] = {
		0,};
		memset(gif_filename, 0, BUFFER_SIZE);
		{
			snprintf(gif_filename, BUFFER_SIZE, "%s%s", DECODE_RESULT_PATH, "gif");
		}

		if ((fd = opendir(argv[2])) == NULL) {
			fprintf(stderr, "listdir: can't open %s\n", argv[2]);
			return 0;
		}

		while ((dp = readdir(fd)) != NULL) {
			if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
				continue;		/* skip self and parent */

			strncpy(temp_filename, dp->d_name, strlen(dp->d_name) - 6);
			number_files++;
		}
		closedir(fd);
		if (!number_files) {
			fprintf(stderr, "\t\tCannot open directory\n");
			return 0;
		}

		for (i = 0; i < number_files; i++) {
			animated_data = (unsigned char **)realloc(animated_data, (i + 1) * sizeof(unsigned char *));
			ret = image_util_decode_create(&decoded);
			if (ret != IMAGE_UTIL_ERROR_NONE)
				return 0;

			snprintf(temp, BUFFER_SIZE, "%s%s%d.png", argv[2], temp_filename, i);

			ret = image_util_decode_set_input_path(decoded, temp);
			if (ret != IMAGE_UTIL_ERROR_NONE)
				return 0;

			ret = image_util_decode_set_output_buffer(decoded, &animated_data[i]);
			if (ret != IMAGE_UTIL_ERROR_NONE)
				return 0;

			ret = image_util_decode_run(decoded, &gif_image_width[i], &gif_image_height[i], NULL);
			if (ret != IMAGE_UTIL_ERROR_NONE)
				return 0;

			ret = image_util_decode_destroy(decoded);
			if (ret != IMAGE_UTIL_ERROR_NONE)
				return 0;
		}

		ret = image_util_encode_create(IMAGE_UTIL_GIF, &encoded);
		if (ret != IMAGE_UTIL_ERROR_NONE)
			return 0;

		for (i = 0; i < number_files; i++) {
			ret = image_util_encode_set_resolution(encoded, gif_image_width[i], gif_image_height[i]);
			if (ret != IMAGE_UTIL_ERROR_NONE)
				return 0;

			ret = image_util_encode_set_input_buffer(encoded, animated_data[i]);
			if (ret != IMAGE_UTIL_ERROR_NONE)
				return 0;
		}

		if (!strcmp("encode-gif-mem", argv[1])) {
			ret = image_util_encode_set_output_buffer(encoded, &dst);
			if (ret != IMAGE_UTIL_ERROR_NONE)
				return 0;
		} else {
			ret = image_util_encode_set_output_path(encoded, gif_filename);
			if (ret != IMAGE_UTIL_ERROR_NONE)
				return 0;
		}

		ret = image_util_encode_run(encoded, &image_size);
		if (ret != IMAGE_UTIL_ERROR_NONE)
			return 0;

		if (!strcmp("encode-gif-mem", argv[1])) {
			_write_file(gif_filename, (void *)dst, image_size);
			free(dst);
		}

		ret = image_util_encode_destroy(encoded);
		if (ret != IMAGE_UTIL_ERROR_NONE)
			return 0;

		for (i = 0; i < number_files; i++)
			free(animated_data[i]);
		free(animated_data);

		return 0;
	}

	if (!strcmp("decode", argv[1]) || !strcmp("decode-mem", argv[1]) || !strcmp("decode-async", argv[1])) {
		ret = image_util_decode_create(&decoded);
		if (ret != IMAGE_UTIL_ERROR_NONE)
			return 0;

		if (!strcmp("decode", argv[1])) {
			ret = image_util_decode_set_input_path(decoded, argv[2]);
			if (ret != IMAGE_UTIL_ERROR_NONE)
				return 0;
		} else {
			if (_read_file(argv[2], &src, &src_size)) {
				ret = image_util_decode_set_input_buffer(decoded, (unsigned char *)src, src_size);
				if (ret != IMAGE_UTIL_ERROR_NONE)
					return 0;
			} else
				return 0;
		}
		ret = image_util_decode_set_output_buffer(decoded, &data);
		if (ret != IMAGE_UTIL_ERROR_NONE)
			return 0;

		if (!strcmp("decode-async", argv[1])) {
			ret = image_util_decode_run_async(decoded, (image_util_decode_completed_cb) decode_completed_cb, NULL);
			_wait();
		} else
			ret = image_util_decode_run(decoded, &image_width, &image_height, NULL);
		if (ret != IMAGE_UTIL_ERROR_NONE)
			return 0;

		free(src);
		ret = image_util_decode_destroy(decoded);
		if (ret != IMAGE_UTIL_ERROR_NONE)
			return 0;
	} else {
		fprintf(stderr, "\tunknown command [%s]\n", argv[1]);
		return 0;
	}

	fprintf(stderr, "\tIMAGE OPERATION SUCCESS\n");
	if (data) {
		fprintf(stderr, "\t##Decoded data##: %p\t width: %lu\t height:%lu\n", data, (long unsigned int)image_width, (long unsigned int)image_height);
		char filename[BUFFER_SIZE] = { 0, }, type[4] = {
		0,};
		memset(filename, 0, BUFFER_SIZE);

		switch (encode_image_type) {
		case IMAGE_UTIL_JPEG:
			snprintf(type, 4, "%s", "jpg");
			break;
		case IMAGE_UTIL_PNG:
			snprintf(type, 4, "%s", "png");
			break;
		case IMAGE_UTIL_GIF:
			snprintf(type, 4, "%s", "gif");
			break;
		case IMAGE_UTIL_BMP:
			snprintf(type, 4, "%s", "bmp");
			break;
		default:
			break;
		}
		snprintf(filename, BUFFER_SIZE, "%s%s", DECODE_RESULT_PATH, type);

		ret = image_util_encode_create(encode_image_type, &encoded);
		if (ret != IMAGE_UTIL_ERROR_NONE)
			return 0;

		ret = image_util_encode_set_resolution(encoded, image_width, image_height);
		if (ret != IMAGE_UTIL_ERROR_NONE)
			return 0;

		if (encode_image_type == IMAGE_UTIL_JPEG) {
			ret = image_util_encode_set_quality(encoded, 100);
			if (ret != IMAGE_UTIL_ERROR_NONE)
				return 0;
		}

		ret = image_util_encode_set_input_buffer(encoded, data);
		if (ret != IMAGE_UTIL_ERROR_NONE)
			return 0;

		if (!strcmp("decode-mem", argv[1])) {
			if (encode_image_type == IMAGE_UTIL_BMP)
				ret = image_util_encode_set_output_path(encoded, filename);
			else
				ret = image_util_encode_set_output_buffer(encoded, &dst);
			if (ret != IMAGE_UTIL_ERROR_NONE)
				return 0;
		} else {
			ret = image_util_encode_set_output_path(encoded, filename);
			if (ret != IMAGE_UTIL_ERROR_NONE)
				return 0;
		}

		if (!strcmp("decode-async", argv[1])) {
			ret = image_util_encode_run_async(encoded, (image_util_encode_completed_cb) encode_completed_cb, NULL);
			_wait();
		} else
			ret = image_util_encode_run(encoded, &image_size);
		if (ret != IMAGE_UTIL_ERROR_NONE)
			return 0;

		if (!strcmp("decode-mem", argv[1]) && (encode_image_type != IMAGE_UTIL_BMP)) {
			_write_file(filename, (void *)dst, image_size);
			free(dst);
		}

		ret = image_util_encode_destroy(encoded);
		if (ret != IMAGE_UTIL_ERROR_NONE)
			return 0;

		free(data);
	} else {
		fprintf(stderr, "\tDECODED data is NULL\n");
	}

	return 0;
}
