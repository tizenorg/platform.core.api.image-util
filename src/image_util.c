/*
* Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
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
*/

#include <dlog.h>

#include <mm_util_imgp.h>
#include <mm_util_jpeg.h>
#include <mm_util_imgcv.h>
#include <mm_util_png.h>
#include <mm_util_gif.h>
#include <mm_util_bmp.h>
#include <image_util.h>
#include <image_util_private.h>
#include <stdio.h>
#include <stdlib.h>

#define IMAGE_UTIL_STRING_VALID(str)	\
	((str != NULL && strlen(str) > 0) ? true : false)

#define IMAGE_UTIL_SAFE_FREE(src)	{ if (src) {free(src); src = NULL; } }

static int _convert_colorspace_tbl[] = {
	MM_UTIL_IMG_FMT_YUV420,		/* IMAGE_UTIL_COLORSPACE_YUV420 */
	MM_UTIL_IMG_FMT_YUV422,		/* IMAGE_UTIL_COLORSPACE_YUV422 */
	MM_UTIL_IMG_FMT_I420,		/* IMAGE_UTIL_COLORSPACE_I420 */
	MM_UTIL_IMG_FMT_NV12,		/* IMAGE_UTIL_COLORSPACE_NV12 */
	MM_UTIL_IMG_FMT_UYVY,		/* IMAGE_UTIL_COLORSPACE_UYVY */
	MM_UTIL_IMG_FMT_YUYV,		/* IMAGE_UTIL_COLORSPACE_YUYV */
	MM_UTIL_IMG_FMT_RGB565,		/* IMAGE_UTIL_COLORSPACE_RGB565 */
	MM_UTIL_IMG_FMT_RGB888,		/* IMAGE_UTIL_COLORSPACE_RGB888 */
	MM_UTIL_IMG_FMT_ARGB8888,	/* IMAGE_UTIL_COLORSPACE_ARGB8888 */
	MM_UTIL_IMG_FMT_BGRA8888,	/* IMAGE_UTIL_COLORSPACE_BGRA8888 */
	MM_UTIL_IMG_FMT_RGBA8888,	/* IMAGE_UTIL_COLORSPACE_RGBA8888 */
	MM_UTIL_IMG_FMT_BGRX8888,	/* IMAGE_UTIL_COLORSPACE_BGRX8888 */
	MM_UTIL_JPEG_FMT_NV21,		/* IMAGE_UTIL_COLORSPACE_NV12 */
	MM_UTIL_JPEG_FMT_NV16,		/* IMAGE_UTIL_COLORSPACE_NV16 */
	MM_UTIL_JPEG_FMT_NV61,		/* IMAGE_UTIL_COLORSPACE_NV61 */
};

static int _convert_encode_colorspace_tbl[] = {
	MM_UTIL_JPEG_FMT_YUV420,	/* IMAGE_UTIL_COLORSPACE_YUV420 */
	-1,							/* IMAGE_UTIL_COLORSPACE_YUV422 */
	MM_UTIL_JPEG_FMT_YUV420,	/* IMAGE_UTIL_COLORSPACE_I420 */
	MM_UTIL_JPEG_FMT_NV12,		/* IMAGE_UTIL_COLORSPACE_NV12 */
	-1,							/* IMAGE_UTIL_COLORSPACE_UYVY */
	-1,							/* IMAGE_UTIL_COLORSPACE_YUYV */
	-1,							/* IMAGE_UTIL_COLORSPACE_RGB565 */
	MM_UTIL_JPEG_FMT_RGB888,	/* IMAGE_UTIL_COLORSPACE_RGB888 */
	MM_UTIL_JPEG_FMT_ARGB8888,	/* IMAGE_UTIL_COLORSPACE_ARGB8888 */
	MM_UTIL_JPEG_FMT_BGRA8888,	/* IMAGE_UTIL_COLORSPACE_BGRA8888 */
	MM_UTIL_JPEG_FMT_RGBA8888,	/* IMAGE_UTIL_COLORSPACE_RGBA8888 */
	-1,							/* IMAGE_UTIL_COLORSPACE_BGRX8888 */
	-1,							/* IMAGE_UTIL_COLORSPACE_NV21 */
	-1,							/* IMAGE_UTIL_COLORSPACE_NV16 */
	-1,							/* IMAGE_UTIL_COLORSPACE_NV61 */
};

static int _convert_jpeg_colorspace_tbl[] = {
	MM_UTIL_JPEG_FMT_YUV420,	/* IMAGE_UTIL_COLORSPACE_YUV420 */
	-1,							/* IMAGE_UTIL_COLORSPACE_YUV422 */
	MM_UTIL_JPEG_FMT_YUV420,	/* IMAGE_UTIL_COLORSPACE_I420 */
	MM_UTIL_JPEG_FMT_NV12,		/* IMAGE_UTIL_COLORSPACE_NV12 */
	-1,							/* IMAGE_UTIL_COLORSPACE_UYVY */
	-1,							/* IMAGE_UTIL_COLORSPACE_YUYV */
	-1,							/* IMAGE_UTIL_COLORSPACE_RGB565 */
	MM_UTIL_JPEG_FMT_RGB888,	/* IMAGE_UTIL_COLORSPACE_RGB888 */
	MM_UTIL_JPEG_FMT_ARGB8888,	/* IMAGE_UTIL_COLORSPACE_ARGB8888 */
	MM_UTIL_JPEG_FMT_BGRA8888,	/* IMAGE_UTIL_COLORSPACE_BGRA8888 */
	MM_UTIL_JPEG_FMT_RGBA8888,	/* IMAGE_UTIL_COLORSPACE_RGBA8888 */
	-1,							/* IMAGE_UTIL_COLORSPACE_BGRX8888 */
	-1,							/* IMAGE_UTIL_COLORSPACE_NV21 */
	-1,							/* IMAGE_UTIL_COLORSPACE_NV16 */
	-1,							/* IMAGE_UTIL_COLORSPACE_NV61 */
};

static int _convert_png_colorspace_tbl[] = {
	-1,							/* IMAGE_UTIL_COLORSPACE_YUV420 */
	-1,							/* IMAGE_UTIL_COLORSPACE_YUV422 */
	-1,							/* IMAGE_UTIL_COLORSPACE_I420 */
	-1,							/* IMAGE_UTIL_COLORSPACE_NV12 */
	-1,							/* IMAGE_UTIL_COLORSPACE_UYVY */
	-1,							/* IMAGE_UTIL_COLORSPACE_YUYV */
	-1,							/* IMAGE_UTIL_COLORSPACE_RGB565 */
	-1,							/* IMAGE_UTIL_COLORSPACE_RGB888 */
	-1,							/* IMAGE_UTIL_COLORSPACE_ARGB8888 */
	-1,							/* IMAGE_UTIL_COLORSPACE_BGRA8888 */
	MM_UTIL_PNG_COLOR_TYPE_RGB_ALPHA,	/* IMAGE_UTIL_COLORSPACE_RGBA8888 */
	-1,							/* IMAGE_UTIL_COLORSPACE_BGRX8888 */
	-1,							/* IMAGE_UTIL_COLORSPACE_NV21 */
	-1,							/* IMAGE_UTIL_COLORSPACE_NV16 */
	-1,							/* IMAGE_UTIL_COLORSPACE_NV61 */
};

static int _convert_gif_colorspace_tbl[] = {
	-1,							/* IMAGE_UTIL_COLORSPACE_YUV420 */
	-1,							/* IMAGE_UTIL_COLORSPACE_YUV422 */
	-1,							/* IMAGE_UTIL_COLORSPACE_I420 */
	-1,							/* IMAGE_UTIL_COLORSPACE_NV12 */
	-1,							/* IMAGE_UTIL_COLORSPACE_UYVY */
	-1,							/* IMAGE_UTIL_COLORSPACE_YUYV */
	-1,							/* IMAGE_UTIL_COLORSPACE_RGB565 */
	-1,							/* IMAGE_UTIL_COLORSPACE_RGB888 */
	-1,							/* IMAGE_UTIL_COLORSPACE_ARGB8888 */
	-1,							/* IMAGE_UTIL_COLORSPACE_BGRA8888 */
	MM_UTIL_GIF_FMT_RGBA8888,	/* IMAGE_UTIL_COLORSPACE_RGBA8888 */
	-1,							/* IMAGE_UTIL_COLORSPACE_BGRX8888 */
	-1,							/* IMAGE_UTIL_COLORSPACE_NV21 */
	-1,							/* IMAGE_UTIL_COLORSPACE_NV16 */
	-1,							/* IMAGE_UTIL_COLORSPACE_NV61 */
};

static int _convert_bmp_colorspace_tbl[] = {
	-1,							/* IMAGE_UTIL_COLORSPACE_YUV420 */
	-1,							/* IMAGE_UTIL_COLORSPACE_YUV422 */
	-1,							/* IMAGE_UTIL_COLORSPACE_I420 */
	-1,							/* IMAGE_UTIL_COLORSPACE_NV12 */
	-1,							/* IMAGE_UTIL_COLORSPACE_UYVY */
	-1,							/* IMAGE_UTIL_COLORSPACE_YUYV */
	-1,							/* IMAGE_UTIL_COLORSPACE_RGB565 */
	-1,							/* IMAGE_UTIL_COLORSPACE_RGB888 */
	-1,							/* IMAGE_UTIL_COLORSPACE_ARGB8888 */
	-1,							/* IMAGE_UTIL_COLORSPACE_BGRA8888 */
	MM_UTIL_BMP_FMT_RGBA8888,	/* IMAGE_UTIL_COLORSPACE_RGBA8888 */
	-1,							/* IMAGE_UTIL_COLORSPACE_BGRX8888 */
	-1,							/* IMAGE_UTIL_COLORSPACE_NV21 */
	-1,							/* IMAGE_UTIL_COLORSPACE_NV16 */
	-1,							/* IMAGE_UTIL_COLORSPACE_NV61 */
};

static int _convert_decode_scale_tbl[] = {
	MM_UTIL_JPEG_DECODE_DOWNSCALE_1_1,
	MM_UTIL_JPEG_DECODE_DOWNSCALE_1_2,
	MM_UTIL_JPEG_DECODE_DOWNSCALE_1_4,
	MM_UTIL_JPEG_DECODE_DOWNSCALE_1_8,
};

static int _convert_image_util_error_code(const char *func, int code)
{
	int ret = IMAGE_UTIL_ERROR_INVALID_OPERATION;
	char *errorstr = NULL;
	switch (code) {
	case MM_UTIL_ERROR_NONE:
		ret = IMAGE_UTIL_ERROR_NONE;
		errorstr = strdup("ERROR_NONE");
		break;
	case MM_UTIL_ERROR_NO_SUCH_FILE:
		ret = IMAGE_UTIL_ERROR_NO_SUCH_FILE;
		errorstr = strdup("NO_SUCH_FILE");
		break;
	case MM_UTIL_ERROR_INVALID_PARAMETER:
		ret = IMAGE_UTIL_ERROR_INVALID_PARAMETER;
		errorstr = strdup("INVALID_PARAMETER");
		break;
	case MM_UTIL_ERROR_NOT_SUPPORTED_FORMAT:
		ret = IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT;
		errorstr = strdup("NOT_SUPPORTED_FORMAT");
		break;
	case MM_UTIL_ERROR_OUT_OF_MEMORY:
		ret = IMAGE_UTIL_ERROR_OUT_OF_MEMORY;
		errorstr = strdup("OUT_OF_MEMORY");
		break;
	case MM_UTIL_ERROR_INVALID_OPERATION:
	default:
		ret = IMAGE_UTIL_ERROR_INVALID_OPERATION;
		errorstr = strdup("INVALID_OPERATION");

	}

	image_util_debug("[%s] %s(0x%08x)", func, errorstr, ret);
	IMAGE_UTIL_SAFE_FREE(errorstr);
	return ret;
}

static image_util_error_e _image_util_error_convert(int error)
{
	switch (error) {
	case MM_UTIL_ERROR_NONE:
		image_util_debug("Error None");
		return IMAGE_UTIL_ERROR_NONE;
	case MM_UTIL_ERROR_INVALID_PARAMETER:
		image_util_error("INVALID_PARAMETER(0x%08x)", IMAGE_UTIL_ERROR_INVALID_PARAMETER);
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	case MM_UTIL_ERROR_NOT_SUPPORTED_FORMAT:
		image_util_error("NOT_SUPPORTED_FORMAT(0x%08x)", IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT);
		return IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT;
	case MM_UTIL_ERROR_OUT_OF_MEMORY:
		image_util_error("OUT_OF_MEMORY(0x%08x)", IMAGE_UTIL_ERROR_OUT_OF_MEMORY);
		return IMAGE_UTIL_ERROR_OUT_OF_MEMORY;
		break;
	case MM_UTIL_ERROR_INVALID_OPERATION:
	default:
		image_util_error("INVALID_OPERATION(0x%08x)", error);
		return IMAGE_UTIL_ERROR_INVALID_OPERATION;
	}
}

static void _image_util_transform_completed_cb(media_packet_h * dst, int error, void *user_data)
{
	int error_value = IMAGE_UTIL_ERROR_NONE;
	image_util_cb_s *_util_cb = (image_util_cb_s *) user_data;

	if ((_util_cb != NULL) && (_util_cb->image_processing_completed_cb != NULL)) {
		error_value = _image_util_error_convert(error);
		_util_cb->image_processing_completed_cb(dst, error_value, _util_cb->user_data);
	}

	return;
}

static int _image_util_create_transform_handle(transformation_s * handle)
{
	int err = MM_UTIL_ERROR_NONE;
	mm_util_imgp_h image_h;

	err = mm_util_create(&image_h);
	if (err != MM_UTIL_ERROR_NONE) {
		image_util_error("Error - mm_util_create");
		return _convert_image_util_error_code(__func__, err);
	}

	handle->image_h = image_h;

	return _convert_image_util_error_code(__func__, err);
}

static bool _image_util_check_resolution(int width, int height)
{
	if (width <= 0) {
		image_util_error("invalid width [%d]", width);
		return false;
	}

	if (height <= 0) {
		image_util_error("invalid height [%d]", height);
		return false;
	}

	return true;
}

int image_util_foreach_supported_jpeg_colorspace(image_util_supported_jpeg_colorspace_cb callback, void *user_data)
{
	int i = 0;

	image_util_retvm_if((callback == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "callback is null");

	/* RGB has higher precedence than YUV */
	for (i = sizeof(_convert_encode_colorspace_tbl) / sizeof(int) - 1; i >= 0; i--) {
		if (_convert_encode_colorspace_tbl[i] != -1)
			if (false == callback(i, user_data))
				return IMAGE_UTIL_ERROR_NONE;

	}

	return IMAGE_UTIL_ERROR_NONE;
}

int image_util_transform_create(transformation_h * handle)
{
	int err = MM_UTIL_ERROR_NONE;

	image_util_debug("image_util_create");

	image_util_retvm_if((handle == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid Handle");

	transformation_s *_handle = (transformation_s *) calloc(1, sizeof(transformation_s));
	image_util_retvm_if((_handle == NULL), IMAGE_UTIL_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY(0x%08x)", IMAGE_UTIL_ERROR_OUT_OF_MEMORY);

	_handle->colorspace = -1;
	_handle->_util_cb = NULL;
	_handle->image_h = 0;
	_handle->hardware_acceleration = false;
	_handle->set_convert = false;
	_handle->set_resize = false;
	_handle->set_rotate = false;
	_handle->set_crop = false;

	err = _image_util_create_transform_handle(_handle);
	if (err != MM_UTIL_ERROR_NONE) {
		image_util_error("Error - create transform handle");
		IMAGE_UTIL_SAFE_FREE(_handle);
		return _convert_image_util_error_code(__func__, err);
	}

	*handle = (transformation_h) _handle;

	return _convert_image_util_error_code(__func__, err);
}

int image_util_transform_set_hardware_acceleration(transformation_h handle, bool mode)
{
	int err = MM_UTIL_ERROR_NONE;
	transformation_s *_handle = (transformation_s *) handle;

	image_util_debug("Set hardware_acceleration %d", mode);

	image_util_retvm_if((_handle == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid Handle");
#ifndef ENABLE_HW_ACCELERATION
	image_util_retvm_if((mode == true), IMAGE_UTIL_ERROR_NOT_SUPPORTED, "hardware acceleration is not supported");
#endif

	err = mm_util_set_hardware_acceleration(_handle->image_h, mode);
	if (err != MM_UTIL_ERROR_NONE) {
		image_util_error("Error - Set hardware_acceleration");
		return _convert_image_util_error_code(__func__, err);
	}

	image_util_debug("Set hardware_acceleration %d", mode);
	_handle->hardware_acceleration = mode;

	return _convert_image_util_error_code(__func__, err);
}

int image_util_transform_set_colorspace(transformation_h handle, image_util_colorspace_e colorspace)
{
	int err = MM_UTIL_ERROR_NONE;
	transformation_s *_handle = (transformation_s *) handle;

	image_util_debug("Set colorspace_convert_info [%d]", colorspace);

	image_util_retvm_if((_handle == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid Handle");

	err = mm_util_set_colorspace_convert(_handle->image_h, colorspace);
	if (err != MM_UTIL_ERROR_NONE) {
		image_util_error("Error - Set colorspace convert");
		return _convert_image_util_error_code(__func__, err);
	}

	_handle->colorspace = colorspace;
	_handle->set_convert = true;

	return _convert_image_util_error_code(__func__, err);
}

int image_util_transform_set_resolution(transformation_h handle, unsigned int width, unsigned int height)
{
	int err = MM_UTIL_ERROR_NONE;
	transformation_s *_handle = (transformation_s *) handle;

	image_util_debug("Set resize_info w[%d] h[%d]", width, height);

	image_util_retvm_if((_handle == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid Handle");
	image_util_retvm_if((_handle->set_crop), IMAGE_UTIL_ERROR_INVALID_OPERATION, "Crop and Resize can't do at the same time");
	image_util_retvm_if((_image_util_check_resolution(width, height) == false), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid resolution");

	err = mm_util_set_resolution(_handle->image_h, width, height);
	if (err != MM_UTIL_ERROR_NONE) {
		image_util_error("Error - Set resolution");
		return _convert_image_util_error_code(__func__, err);
	}
	_handle->width = width;
	_handle->height = height;
	_handle->set_resize = true;

	return _convert_image_util_error_code(__func__, err);
}

int image_util_transform_set_rotation(transformation_h handle, image_util_rotation_e rotation)
{
	int err = MM_UTIL_ERROR_NONE;
	transformation_s *_handle = (transformation_s *) handle;

	image_util_debug("Set rotate_info [%d]", rotation);

	image_util_retvm_if((_handle == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid Handle");

	err = mm_util_set_rotation(_handle->image_h, rotation);
	if (err != MM_UTIL_ERROR_NONE) {
		image_util_error("Error - Set rotation");
		return _convert_image_util_error_code(__func__, err);
	}
	_handle->rotation = rotation;
	_handle->set_rotate = true;

	return _convert_image_util_error_code(__func__, err);
}

int image_util_transform_set_crop_area(transformation_h handle, unsigned int start_x, unsigned int start_y, unsigned int end_x, unsigned int end_y)
{
	int err = MM_UTIL_ERROR_NONE;
	transformation_s *_handle = (transformation_s *) handle;
	int dest_width;
	int dest_height;

	image_util_retvm_if((_handle == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid Handle");
	image_util_retvm_if((_handle->set_resize), IMAGE_UTIL_ERROR_INVALID_OPERATION, "Crop and Resize can't do at the same time");

	dest_width = end_x - start_x;
	dest_height = end_y - start_y;

	image_util_debug("Set crop_info x[%d] y[%d] w[%d] h[%d]", start_x, start_y, dest_width, dest_height);

	image_util_retvm_if((_image_util_check_resolution(dest_width, dest_height) == false), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid dest resolution");
	err = mm_util_set_crop_area(_handle->image_h, start_x, start_y, end_x, end_y);
	if (err != MM_UTIL_ERROR_NONE) {
		image_util_error("Error - Set crop area");
		return _convert_image_util_error_code(__func__, err);
	}
	_handle->start_x = start_x;
	_handle->start_y = start_y;
	_handle->end_x = end_x;
	_handle->end_y = end_y;
	_handle->set_crop = true;

	return _convert_image_util_error_code(__func__, err);
}

int image_util_transform_get_colorspace(transformation_h handle, image_util_colorspace_e * colorspace)
{
	int ret = IMAGE_UTIL_ERROR_NONE;
	transformation_s *_handle = (transformation_s *) handle;

	image_util_debug("Get colorspace_convert_info [%d]", colorspace);

	if (_handle == NULL) {
		image_util_error("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	if (!colorspace) {
		image_util_error("colorspace area parameter error");
		return IMAGE_UTIL_ERROR_INVALID_OPERATION;
	}

	if (!_handle->set_convert) {
		image_util_error("Did not set colorspace before");
		return IMAGE_UTIL_ERROR_INVALID_OPERATION;
	}

	*colorspace = _handle->colorspace;
	return ret;
}

int image_util_transform_get_resolution(transformation_h handle, unsigned int *width, unsigned int *height)
{
	int ret = IMAGE_UTIL_ERROR_NONE;
	transformation_s *_handle = (transformation_s *) handle;

	image_util_debug("Set resize_info w[%d] h[%d]", width, height);

	if (_handle == NULL) {
		image_util_error("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	if (!width || !height) {
		image_util_error("resolution area parameter error");
		return IMAGE_UTIL_ERROR_INVALID_OPERATION;
	}

	if (!_handle->set_resize) {
		image_util_error("Did not set resolution before");
		return IMAGE_UTIL_ERROR_INVALID_OPERATION;
	}

	*width = _handle->width;
	*height = _handle->height;

	return ret;
}

int image_util_transform_get_rotation(transformation_h handle, image_util_rotation_e * rotation)
{
	int ret = IMAGE_UTIL_ERROR_NONE;
	transformation_s *_handle = (transformation_s *) handle;

	image_util_debug("Set rotate_info [%d]", rotation);

	if (_handle == NULL) {
		image_util_error("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	if (!rotation) {
		image_util_error("rotation area parameter error");
		return IMAGE_UTIL_ERROR_INVALID_OPERATION;
	}

	if (!_handle->set_rotate) {
		image_util_error("Did not set rotation before");
		return IMAGE_UTIL_ERROR_INVALID_OPERATION;
	}

	*rotation = _handle->rotation;

	return ret;
}

int image_util_transform_get_crop_area(transformation_h handle, unsigned int *start_x, unsigned int *start_y, unsigned int *end_x, unsigned int *end_y)
{
	int ret = IMAGE_UTIL_ERROR_NONE;
	transformation_s *_handle = (transformation_s *) handle;

	if (_handle == NULL) {
		image_util_error("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	if (!start_x || !start_y || !end_x || !end_y) {
		image_util_error("crop area parameter error");
		return IMAGE_UTIL_ERROR_INVALID_OPERATION;
	}

	if (!_handle->set_crop) {
		image_util_error("Did not set crop area before");
		return IMAGE_UTIL_ERROR_INVALID_OPERATION;
	}

	*start_x = _handle->start_x;
	*start_y = _handle->start_y;
	*end_x = _handle->end_x;
	*end_y = _handle->end_y;

	return ret;
}

int image_util_transform_run(transformation_h handle, media_packet_h src, image_util_transform_completed_cb completed_cb, void *user_data)
{
	int err = MM_UTIL_ERROR_NONE;
	transformation_s *_handle = (transformation_s *) handle;

	image_util_debug("image_util_transform");

	image_util_retvm_if((_handle == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid Handle");
	image_util_retvm_if((completed_cb == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid callback");
	image_util_retvm_if((src == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid source");
	image_util_retvm_if((!_handle->set_convert && !_handle->set_resize && !_handle->set_rotate && !_handle->set_crop), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid transform");

	if (_handle->_util_cb != NULL) {
		IMAGE_UTIL_SAFE_FREE(_handle->_util_cb);
		_handle->_util_cb = NULL;
	}
	_handle->_util_cb = (image_util_cb_s *) calloc(1, sizeof(image_util_cb_s));
	image_util_retvm_if((_handle->_util_cb == NULL), IMAGE_UTIL_ERROR_OUT_OF_MEMORY, "Out of memory");

	_handle->_util_cb->user_data = user_data;
	_handle->_util_cb->image_processing_completed_cb = completed_cb;

	if (_handle->_util_cb)
		err = mm_util_transform(_handle->image_h, src, (mm_util_completed_callback) _image_util_transform_completed_cb, (void *)_handle->_util_cb);

	return _convert_image_util_error_code(__func__, err);
}

int image_util_transform_destroy(transformation_h handle)
{
	int err = MM_UTIL_ERROR_NONE;
	transformation_s *_handle = (transformation_s *) handle;

	image_util_debug("image_util_destroy");

	if (_handle == NULL) {
		image_util_error("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	err = mm_util_destroy(_handle->image_h);

	IMAGE_UTIL_SAFE_FREE(_handle->_util_cb);
	IMAGE_UTIL_SAFE_FREE(_handle);

	return _convert_image_util_error_code(__func__, err);
}

int image_util_convert_colorspace(unsigned char *dest, image_util_colorspace_e dest_colorspace, const unsigned char *src, int width, int height, image_util_colorspace_e src_colorspace)
{
	int err = MM_UTIL_ERROR_NONE;

	image_util_retvm_if((dest == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "dest is null");
	image_util_retvm_if((src == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "src is null");
	image_util_retvm_if((dest_colorspace < 0 || dest_colorspace >= sizeof(_convert_colorspace_tbl) / sizeof(int)), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid dst_colorspace");
	image_util_retvm_if((src_colorspace < 0 || src_colorspace >= sizeof(_convert_colorspace_tbl) / sizeof(int)), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid src_colorspace");

	err = mm_util_convert_colorspace(src, width, height, _convert_colorspace_tbl[src_colorspace], dest, _convert_colorspace_tbl[dest_colorspace]);

	return _convert_image_util_error_code(__func__, err);
}

int image_util_calculate_buffer_size(int width, int height, image_util_colorspace_e colorspace, unsigned int *size)
{
	int err = MM_UTIL_ERROR_NONE;

	image_util_retvm_if((colorspace < 0 || colorspace >= sizeof(_convert_colorspace_tbl) / sizeof(int)), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid colorspace");
	image_util_retvm_if((width <= 0 || height <= 0), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid width or Invalid height");
	image_util_retvm_if((size == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "size is null");

	err = mm_util_get_image_size(_convert_colorspace_tbl[colorspace], width, height, size);
	return _convert_image_util_error_code(__func__, err);
}

int image_util_resize(unsigned char *dest, int *dest_width, int *dest_height, const unsigned char *src, int src_width, int src_height, image_util_colorspace_e colorspace)
{
	int err = MM_UTIL_ERROR_NONE;

	image_util_retvm_if((dest == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "dest is null");
	image_util_retvm_if((src == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "src is null");
	image_util_retvm_if((colorspace < 0 || colorspace >= sizeof(_convert_colorspace_tbl) / sizeof(int)), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid colorspace");
	image_util_retvm_if((dest_width == NULL || dest_height == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "dest_width or dest_height is null");
	image_util_retvm_if((*dest_width <= 0 || *dest_height <= 0), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid dest_width or Invalid dest_height");

	unsigned int dest_w, dest_h;
	dest_w = *dest_width;
	dest_h = *dest_height;
	err = mm_util_resize_image(src, src_width, src_height, _convert_colorspace_tbl[colorspace], dest, &dest_w, &dest_h);
	if (err == MM_UTIL_ERROR_NONE) {
		*dest_width = (int)dest_w;
		*dest_height = (int)dest_h;
	}

	return _convert_image_util_error_code(__func__, err);
}

int image_util_rotate(unsigned char *dest, int *dest_width, int *dest_height, image_util_rotation_e dest_rotation, const unsigned char *src, int src_width, int src_height, image_util_colorspace_e colorspace)
{
	int err = MM_UTIL_ERROR_NONE;

	image_util_retvm_if((dest == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "dest is null");
	image_util_retvm_if((src == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "src is null");
	image_util_retvm_if((colorspace < 0 || colorspace >= sizeof(_convert_colorspace_tbl) / sizeof(int)), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid colorspace");
	image_util_retvm_if((dest_rotation < 0 || dest_rotation > IMAGE_UTIL_ROTATION_FLIP_VERT), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid rotation");
	image_util_retvm_if((dest_width == NULL || dest_height == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "dest_width or dest_height is null");

	unsigned int dest_w, dest_h;
	err = mm_util_rotate_image(src, src_width, src_height, _convert_colorspace_tbl[colorspace], dest, &dest_w, &dest_h, dest_rotation);
	if (err == MM_UTIL_ERROR_NONE) {
		*dest_width = (int)dest_w;
		*dest_height = (int)dest_h;
	}
	return _convert_image_util_error_code(__func__, err);
}

int image_util_crop(unsigned char *dest, int x, int y, int *width, int *height, const unsigned char *src, int src_width, int src_height, image_util_colorspace_e colorspace)
{
	int err = MM_UTIL_ERROR_NONE;

	image_util_retvm_if((dest == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "dest is null");
	image_util_retvm_if((src == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "src is null");
	image_util_retvm_if((colorspace < 0 || colorspace >= sizeof(_convert_colorspace_tbl) / sizeof(int)), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid colorspace");
	image_util_retvm_if((width == NULL || height == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "width or height is null");
	image_util_retvm_if((src_width <= x || src_height <= y || src_width < x + *width || src_height < y + *height), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid crop area");

	unsigned int dest_w, dest_h;
	dest_w = *width;
	dest_h = *height;
	err = mm_util_crop_image(src, src_width, src_height, _convert_colorspace_tbl[colorspace], x, y, &dest_w, &dest_h, dest);
	if (err == MM_UTIL_ERROR_NONE) {
		*width = (int)dest_w;
		*height = (int)dest_h;
	}

	return _convert_image_util_error_code(__func__, err);
}

int image_util_decode_jpeg(const char *path, image_util_colorspace_e colorspace, unsigned char **image_buffer, int *width, int *height, unsigned int *size)
{
	int err = MM_UTIL_ERROR_NONE;

	image_util_retvm_if((path == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "path is null");
	image_util_retvm_if((image_buffer == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "image_buffer is null");
	image_util_retvm_if((size == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "size is null");
	image_util_retvm_if((strlen(path) == 0), IMAGE_UTIL_ERROR_NO_SUCH_FILE, "Invalid path");
	image_util_retvm_if((colorspace < 0 || colorspace >= sizeof(_convert_colorspace_tbl) / sizeof(int)), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid colorspace");
	image_util_retvm_if((_convert_encode_colorspace_tbl[colorspace] == -1), IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT, "not supported format");

	mm_util_jpeg_yuv_data decoded;
	memset(&decoded, 0, sizeof(mm_util_jpeg_yuv_data));

	err = mm_util_decode_from_jpeg_file(&decoded, path, _convert_encode_colorspace_tbl[colorspace]);
	if (err == MM_UTIL_ERROR_NONE) {
		*image_buffer = decoded.data;
		if (width)
			*width = decoded.width;
		if (height)
			*height = decoded.height;
		if (size)
			*size = (unsigned int)decoded.size;
	}
	return _convert_image_util_error_code(__func__, err);
}

int image_util_decode_jpeg_from_memory(const unsigned char *jpeg_buffer, int jpeg_size, image_util_colorspace_e colorspace, unsigned char **image_buffer, int *width, int *height, unsigned int *size)
{
	int err = MM_UTIL_ERROR_NONE;

	image_util_retvm_if((jpeg_buffer == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "jpeg_buffer is null");
	image_util_retvm_if((image_buffer == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "image_buffer is null");
	image_util_retvm_if((size == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "size is null");
	image_util_retvm_if((colorspace < 0 || colorspace >= sizeof(_convert_colorspace_tbl) / sizeof(int)), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid colorspace");
	image_util_retvm_if((_convert_encode_colorspace_tbl[colorspace] == -1), IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT, "not supported format");

	mm_util_jpeg_yuv_data decoded;
	memset(&decoded, 0, sizeof(mm_util_jpeg_yuv_data));

	err = mm_util_decode_from_jpeg_memory(&decoded, (void *)jpeg_buffer, jpeg_size, _convert_encode_colorspace_tbl[colorspace]);

	if (err == MM_UTIL_ERROR_NONE) {
		*image_buffer = decoded.data;
		if (width)
			*width = decoded.width;
		if (height)
			*height = decoded.height;
		if (size)
			*size = (unsigned int)decoded.size;
	}

	return _convert_image_util_error_code(__func__, err);
}

int image_util_decode_jpeg_with_downscale(const char *path, image_util_colorspace_e colorspace, image_util_scale_e downscale, unsigned char **image_buffer, int *width, int *height, unsigned int *size)
{
	int err = MM_UTIL_ERROR_NONE;

	image_util_retvm_if((path == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "path is null");
	image_util_retvm_if((image_buffer == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "image_buffer is null");
	image_util_retvm_if((size == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "size is null");
	image_util_retvm_if((strlen(path) == 0), IMAGE_UTIL_ERROR_NO_SUCH_FILE, "Invalid path");
	image_util_retvm_if((colorspace < 0 || colorspace >= sizeof(_convert_colorspace_tbl) / sizeof(int)), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid colorspace");
	image_util_retvm_if((_convert_encode_colorspace_tbl[colorspace] == -1), IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT, "not supported format");
	image_util_retvm_if((downscale < 0 || downscale >= sizeof(_convert_decode_scale_tbl) / sizeof(int)), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "downsacle is invalid");

	mm_util_jpeg_yuv_data decoded;
	memset(&decoded, 0, sizeof(mm_util_jpeg_yuv_data));

	err = mm_util_decode_from_jpeg_file_with_downscale(&decoded, path, _convert_encode_colorspace_tbl[colorspace], _convert_decode_scale_tbl[downscale]);
	if (err == MM_UTIL_ERROR_NONE) {
		*image_buffer = decoded.data;
		if (width)
			*width = decoded.width;
		if (height)
			*height = decoded.height;
		if (size)
			*size = (unsigned int)decoded.size;
	}
	return _convert_image_util_error_code(__func__, err);
}

int image_util_decode_jpeg_from_memory_with_downscale(const unsigned char *jpeg_buffer, int jpeg_size, image_util_colorspace_e colorspace, image_util_scale_e downscale, unsigned char **image_buffer, int *width, int *height, unsigned int *size)
{
	int err = MM_UTIL_ERROR_NONE;

	image_util_retvm_if((jpeg_buffer == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "jpeg_buffer is null");
	image_util_retvm_if((image_buffer == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "image_buffer is null");
	image_util_retvm_if((size == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "size is null");
	image_util_retvm_if((colorspace < 0 || colorspace >= sizeof(_convert_colorspace_tbl) / sizeof(int)), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid colorspace");
	image_util_retvm_if((_convert_encode_colorspace_tbl[colorspace] == -1), IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT, "not supported format");
	image_util_retvm_if((downscale < 0 || downscale >= sizeof(_convert_decode_scale_tbl) / sizeof(int)), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "downsacle is invalid");

	mm_util_jpeg_yuv_data decoded;
	memset(&decoded, 0, sizeof(mm_util_jpeg_yuv_data));

	err = mm_util_decode_from_jpeg_memory_with_downscale(&decoded, (void *)jpeg_buffer, jpeg_size, _convert_encode_colorspace_tbl[colorspace], _convert_decode_scale_tbl[downscale]);

	if (err == MM_UTIL_ERROR_NONE) {
		*image_buffer = decoded.data;
		if (width)
			*width = decoded.width;
		if (height)
			*height = decoded.height;
		if (size)
			*size = (unsigned int)decoded.size;
	}

	return _convert_image_util_error_code(__func__, err);
}

int image_util_encode_jpeg(const unsigned char *buffer, int width, int height, image_util_colorspace_e colorspace, int quality, const char *path)
{
	int err = MM_UTIL_ERROR_NONE;

	image_util_retvm_if((path == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "path is null");
	image_util_retvm_if((buffer == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "buffer is null");
	image_util_retvm_if((strlen(path) == 0), IMAGE_UTIL_ERROR_NO_SUCH_FILE, "Invalid path");
	image_util_retvm_if((colorspace < 0 || colorspace >= sizeof(_convert_colorspace_tbl) / sizeof(int)), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid colorspace");
	image_util_retvm_if((_convert_encode_colorspace_tbl[colorspace] == -1), IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT, "not supported format");

	err = mm_util_jpeg_encode_to_file(path, (void *)buffer, width, height, _convert_encode_colorspace_tbl[colorspace], quality);
	return _convert_image_util_error_code(__func__, err);
}

int image_util_encode_jpeg_to_memory(const unsigned char *image_buffer, int width, int height, image_util_colorspace_e colorspace, int quality, unsigned char **jpeg_buffer, unsigned int *jpeg_size)
{
	int err = MM_UTIL_ERROR_NONE;
	int isize = 0;

	image_util_retvm_if((jpeg_buffer == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "jpeg_buffer is null");
	image_util_retvm_if((image_buffer == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "image_buffer is null");
	image_util_retvm_if((jpeg_size == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "jpeg_size is null");
	image_util_retvm_if((colorspace < 0 || colorspace >= sizeof(_convert_colorspace_tbl) / sizeof(int)), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid colorspace");
	image_util_retvm_if((_convert_encode_colorspace_tbl[colorspace] == -1), IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT, "not supported format");

	err = mm_util_jpeg_encode_to_memory((void **)jpeg_buffer, &isize, (void *)image_buffer, width, height, _convert_encode_colorspace_tbl[colorspace], quality);
	if (err == MM_UTIL_ERROR_NONE)
		*jpeg_size = (unsigned int)isize;

	return _convert_image_util_error_code(__func__, err);
}

int image_util_extract_color_from_memory(const unsigned char *image_buffer, int width, int height, unsigned char *rgb_r, unsigned char *rgb_g, unsigned char *rgb_b)
{
	int ret = MM_UTIL_ERROR_NONE;

	image_util_retvm_if((image_buffer == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "image_buffer     is null");

	unsigned char r_color, g_color, b_color;
	ret = mm_util_cv_extract_representative_color((void *)image_buffer, width, height, &r_color, &g_color, &b_color);

	*rgb_r = r_color;
	*rgb_g = g_color;
	*rgb_b = b_color;

	return _convert_image_util_error_code(__func__, ret);
}

int image_util_foreach_supported_colorspace(image_util_type_e image_type, image_util_supported_colorspace_cb callback, void *user_data)
{
	int i = 0;

	image_util_retvm_if((callback == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "callback is null");

	switch (image_type) {
	case IMAGE_UTIL_JPEG:
		/* RGB has higher precedence than YUV */
		for (i = sizeof(_convert_jpeg_colorspace_tbl) / sizeof(int) - 1; i >= 0; i--) {
			if (_convert_jpeg_colorspace_tbl[i] != -1)
				if (false == callback(i, user_data))
					return IMAGE_UTIL_ERROR_NONE;

		}
		break;
	case IMAGE_UTIL_PNG:
		for (i = sizeof(_convert_png_colorspace_tbl) / sizeof(int) - 1; i >= 0; i--) {
			if (_convert_png_colorspace_tbl[i] != -1)
				if (false == callback(i, user_data))
					return IMAGE_UTIL_ERROR_NONE;

		}
		break;
	case IMAGE_UTIL_GIF:
		for (i = sizeof(_convert_gif_colorspace_tbl) / sizeof(int) - 1; i >= 0; i--) {
			if (_convert_gif_colorspace_tbl[i] != -1)
				if (false == callback(i, user_data))
					return IMAGE_UTIL_ERROR_NONE;

		}
		break;
	case IMAGE_UTIL_BMP:
		for (i = sizeof(_convert_bmp_colorspace_tbl) / sizeof(int) - 1; i >= 0; i--) {
			if (_convert_bmp_colorspace_tbl[i] != -1)
				if (false == callback(i, user_data))
					return IMAGE_UTIL_ERROR_NONE;

		}
		break;
	default:
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	return IMAGE_UTIL_ERROR_NONE;
}

static int _image_util_decode_create_jpeg_handle(decode_encode_s * handle)
{
	int err = MM_UTIL_ERROR_NONE;

	image_util_retvm_if((handle == NULL), MM_UTIL_ERROR_INVALID_PARAMETER, "Invalid Handle");

	mm_util_jpeg_yuv_data *_handle = (mm_util_jpeg_yuv_data *) calloc(1, sizeof(mm_util_jpeg_yuv_data));
	image_util_retvm_if((_handle == NULL), MM_UTIL_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY(0x%08x)", MM_UTIL_ERROR_OUT_OF_MEMORY);

	handle->image_h = (mm_util_imgp_h) _handle;
	handle->colorspace = IMAGE_UTIL_COLORSPACE_RGBA8888;
	handle->down_scale = sizeof(image_util_scale_e);

	return err;
}

static int _image_util_decode_create_png_handle(decode_encode_s * handle)
{
	int err = MM_UTIL_ERROR_NONE;

	image_util_retvm_if((handle == NULL), MM_UTIL_ERROR_INVALID_PARAMETER, "Invalid Handle");

	mm_util_png_data *_handle = (mm_util_png_data *) calloc(1, sizeof(mm_util_png_data));
	image_util_retvm_if((_handle == NULL), MM_UTIL_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY(0x%08x)", MM_UTIL_ERROR_OUT_OF_MEMORY);

	mm_util_init_decode_png(_handle);

	handle->image_h = (mm_util_imgp_h) _handle;

	return err;
}

static int _image_util_decode_create_gif_handle(decode_encode_s * handle)
{
	int err = MM_UTIL_ERROR_NONE;

	image_util_retvm_if((handle == NULL), MM_UTIL_ERROR_INVALID_PARAMETER, "Invalid Handle");

	mm_util_gif_data *_handle = (mm_util_gif_data *) calloc(1, sizeof(mm_util_gif_data));
	image_util_retvm_if((_handle == NULL), MM_UTIL_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY(0x%08x)", MM_UTIL_ERROR_OUT_OF_MEMORY);

	handle->image_h = (mm_util_imgp_h) _handle;

	return err;
}

static int _image_util_decode_create_bmp_handle(decode_encode_s * handle)
{
	int err = MM_UTIL_ERROR_NONE;

	image_util_retvm_if((handle == NULL), MM_UTIL_ERROR_INVALID_PARAMETER, "Invalid Handle");

	mm_util_bmp_data *_handle = (mm_util_bmp_data *) calloc(1, sizeof(mm_util_bmp_data));
	image_util_retvm_if((_handle == NULL), MM_UTIL_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY(0x%08x)", MM_UTIL_ERROR_OUT_OF_MEMORY);

	handle->image_h = (mm_util_imgp_h) _handle;

	return err;
}

int image_util_decode_create(image_util_decode_h * handle)
{
	int err = MM_UTIL_ERROR_NONE;

	image_util_debug("image_util_decode_create");

	image_util_retvm_if((handle == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid Handle");

	decode_encode_s *_handle = (decode_encode_s *) calloc(1, sizeof(decode_encode_s));
	image_util_retvm_if((_handle == NULL), IMAGE_UTIL_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY(0x%08x)", IMAGE_UTIL_ERROR_OUT_OF_MEMORY);

	_handle->src_buffer = NULL;
	_handle->dst_buffer = NULL;
	_handle->path = NULL;
	_handle->image_h = 0;
	_handle->is_decode = TRUE;
	_handle->image_type = -1;
	_handle->image_count = 0;

	*handle = (image_util_decode_h) _handle;

	return _convert_image_util_error_code(__func__, err);
}

static char _JPEG_HEADER[] = { 0xFF, 0xD8 };
static char _PNG_HEADER[] = { 0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a };
static char _GIF_HEADER[] = { 'G', 'I', 'F' };
static char _BMP_HEADER[] = { 'B', 'M' };

static int _image_util_decode_create_image_handle(image_util_decode_h handle, const unsigned char *src_buffer)
{
	image_util_type_e image_type = -1;
	static struct {
		char *header;
		int size;
		image_util_type_e image_type;
	} image_header[] = {
		{
		_JPEG_HEADER, sizeof(_JPEG_HEADER), IMAGE_UTIL_JPEG}
		, {
		_PNG_HEADER, sizeof(_PNG_HEADER), IMAGE_UTIL_PNG}
		, {
		_GIF_HEADER, sizeof(_GIF_HEADER), IMAGE_UTIL_GIF}
		, {
		_BMP_HEADER, sizeof(_BMP_HEADER), IMAGE_UTIL_BMP}
	,};
	unsigned int i = 0;
	int err = MM_UTIL_ERROR_NONE;
	decode_encode_s *_handle = (decode_encode_s *) handle;

	if (_handle == NULL || _handle->is_decode == FALSE) {
		image_util_error("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}
	if (src_buffer == NULL) {
		image_util_error("Invalid input buffer");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	for (i = 0; i < sizeof(image_header) / sizeof(image_header[0]); i++) {
		if (strncmp((const char *)src_buffer, image_header[i].header, image_header[i].size) == 0) {
			image_type = image_header[i].image_type;
			break;
		}
	}

	_handle->image_type = image_type;

	switch (image_type) {
	case IMAGE_UTIL_JPEG:
		err = _image_util_decode_create_jpeg_handle(_handle);
		break;
	case IMAGE_UTIL_PNG:
		err = _image_util_decode_create_png_handle(_handle);
		break;
	case IMAGE_UTIL_GIF:
		err = _image_util_decode_create_gif_handle(_handle);
		break;
	case IMAGE_UTIL_BMP:
		err = _image_util_decode_create_bmp_handle(_handle);
		break;
	default:
		err = MM_UTIL_ERROR_NOT_SUPPORTED_FORMAT;
		break;
	}

	if (err != MM_UTIL_ERROR_NONE) {
		image_util_error("Error - create image handle");
		IMAGE_UTIL_SAFE_FREE(_handle);
		return _convert_image_util_error_code(__func__, err);
	}

	return _convert_image_util_error_code(__func__, err);
}

int image_util_decode_set_input_path(image_util_decode_h handle, const char *path)
{
	int err = IMAGE_UTIL_ERROR_NONE;
	decode_encode_s *_handle = (decode_encode_s *) handle;
	FILE *fp = NULL;
	unsigned char *src_buffer = NULL;

	if (_handle == NULL || _handle->is_decode == FALSE) {
		image_util_error("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}
	image_util_retvm_if((path == NULL || strlen(path) == 0), IMAGE_UTIL_ERROR_NO_SUCH_FILE, "Invalid path");

	if (_handle->src_buffer)
		_handle->src_buffer = NULL;

	fp = fopen(path, "r");
	if (fp == NULL) {
		image_util_error("File open failed");
		return IMAGE_UTIL_ERROR_NO_SUCH_FILE;
	}
	src_buffer = (void *)malloc(sizeof(_PNG_HEADER));
	if (!fread(src_buffer, 1, sizeof(_PNG_HEADER), fp)) {
		image_util_error("File read failed");
		fclose(fp);
		fp = NULL;
		free(src_buffer);
		return IMAGE_UTIL_ERROR_INVALID_OPERATION;
	}

	err = _image_util_decode_create_image_handle(_handle, src_buffer);

	fclose(fp);
	fp = NULL;
	free(src_buffer);

	_handle->path = path;

	return err;
}

int image_util_decode_set_input_buffer(image_util_decode_h handle, const unsigned char *src_buffer, unsigned long long src_size)
{
	int err = IMAGE_UTIL_ERROR_NONE;
	decode_encode_s *_handle = (decode_encode_s *) handle;

	if (_handle == NULL || _handle->is_decode == FALSE) {
		image_util_error("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}
	if (src_buffer == NULL || src_size == 0) {
		image_util_error("Invalid input buffer");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	if (_handle->path)
		_handle->path = NULL;

	err = _image_util_decode_create_image_handle(_handle, src_buffer);

	if (!_handle->src_buffer)
		_handle->src_buffer = (void *)calloc(1, sizeof(void *));
	_handle->src_buffer[0] = (void *)src_buffer;
	_handle->src_size = src_size;

	return err;
}

int image_util_decode_set_output_buffer(image_util_decode_h handle, unsigned char **dst_buffer)
{
	int err = IMAGE_UTIL_ERROR_NONE;
	decode_encode_s *_handle = (decode_encode_s *) handle;

	if (_handle == NULL || _handle->is_decode == FALSE) {
		image_util_error("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}
	if (dst_buffer == NULL) {
		image_util_error("Invalid output buffer");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	_handle->dst_buffer = (void **)dst_buffer;

	return err;
}

int image_util_decode_set_colorspace(image_util_encode_h handle, image_util_colorspace_e colorspace)
{
	int err = IMAGE_UTIL_ERROR_NONE;
	decode_encode_s *_handle = (decode_encode_s *) handle;

	if (_handle == NULL || _handle->is_decode == FALSE) {
		image_util_error("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	image_util_retvm_if((colorspace < 0 || colorspace >= sizeof(_convert_jpeg_colorspace_tbl) / sizeof(int)), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid colorspace");
	switch (_handle->image_type) {
	case IMAGE_UTIL_JPEG:
		image_util_retvm_if((_convert_jpeg_colorspace_tbl[colorspace] == -1), IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT, "not supported format");
		break;
	case IMAGE_UTIL_PNG:
		image_util_retvm_if((_convert_png_colorspace_tbl[colorspace] == -1), IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT, "not supported format");
		break;
	case IMAGE_UTIL_GIF:
		image_util_retvm_if((_convert_gif_colorspace_tbl[colorspace] == -1), IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT, "not supported format");
		break;
	case IMAGE_UTIL_BMP:
		image_util_retvm_if((_convert_bmp_colorspace_tbl[colorspace] == -1), IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT, "not supported format");
		break;
	default:
		image_util_error("Invalid image type");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	_handle->colorspace = colorspace;

	return err;
}

int image_util_decode_set_jpeg_downscale(image_util_encode_h handle, image_util_scale_e down_scale)
{
	int err = IMAGE_UTIL_ERROR_NONE;
	decode_encode_s *_handle = (decode_encode_s *) handle;

	if (_handle == NULL || _handle->is_decode == FALSE) {
		image_util_error("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}
	if (_handle->image_type != IMAGE_UTIL_JPEG) {
		image_util_error("Wrong image format");
		return IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT;
	}
	image_util_retvm_if((down_scale < 0 || down_scale >= sizeof(image_util_scale_e)), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "downscale is invalid");

	_handle->down_scale = down_scale;

	return err;
}

static int _image_util_decode_internal(decode_encode_s * _handle)
{
	int err = MM_UTIL_ERROR_NONE;

	switch (_handle->image_type) {
	case IMAGE_UTIL_JPEG:
		{
			mm_util_jpeg_yuv_data *jpeg_data;

			jpeg_data = (mm_util_jpeg_yuv_data *) _handle->image_h;
			if (jpeg_data == NULL) {
				image_util_error("Invalid jpeg data");
				return MM_UTIL_ERROR_INVALID_PARAMETER;
			}

			if (_handle->path) {
				if (_handle->down_scale < sizeof(image_util_scale_e))
					err = mm_util_decode_from_jpeg_file_with_downscale(jpeg_data, _handle->path, _convert_jpeg_colorspace_tbl[_handle->colorspace], _convert_decode_scale_tbl[_handle->down_scale]);
				else
					err = mm_util_decode_from_jpeg_file(jpeg_data, _handle->path, _convert_jpeg_colorspace_tbl[_handle->colorspace]);
			} else {
				if (_handle->down_scale < sizeof(image_util_scale_e))
					err = mm_util_decode_from_jpeg_memory_with_downscale(jpeg_data, _handle->src_buffer[0], _handle->src_size, _convert_jpeg_colorspace_tbl[_handle->colorspace], _convert_decode_scale_tbl[_handle->down_scale]);
				else
					err = mm_util_decode_from_jpeg_memory(jpeg_data, _handle->src_buffer[0], _handle->src_size, _convert_jpeg_colorspace_tbl[_handle->colorspace]);
			}

			if (err == MM_UTIL_ERROR_NONE) {
				*(_handle->dst_buffer) = jpeg_data->data;
				_handle->dst_size = (unsigned long long)jpeg_data->size;
				_handle->width = jpeg_data->width;
				_handle->height = jpeg_data->height;
			}
		}
		break;
	case IMAGE_UTIL_PNG:
		{
			mm_util_png_data *png_data;

			png_data = (mm_util_png_data *) _handle->image_h;
			if (png_data == NULL) {
				image_util_error("Invalid png data");
				return MM_UTIL_ERROR_INVALID_PARAMETER;
			}

			if (_handle->path)
				err = mm_util_decode_from_png_file(png_data, _handle->path);
			else
				err = mm_util_decode_from_png_memory(png_data, &_handle->src_buffer[0], _handle->src_size);

			if (err == MM_UTIL_ERROR_NONE) {
				*(_handle->dst_buffer) = png_data->data;
				_handle->dst_size = png_data->size;
				_handle->width = png_data->width;
				_handle->height = png_data->height;
			}
		}
		break;
	case IMAGE_UTIL_GIF:
		{
			mm_util_gif_data *gif_data;

			gif_data = (mm_util_gif_data *) _handle->image_h;
			if (gif_data == NULL) {
				image_util_error("Invalid gif data");
				return MM_UTIL_ERROR_INVALID_PARAMETER;
			}

			if (_handle->path)
				err = mm_util_decode_from_gif_file(gif_data, _handle->path);
			else
				err = mm_util_decode_from_gif_memory(gif_data, &_handle->src_buffer[0]);

			if (err == MM_UTIL_ERROR_NONE) {
				*(_handle->dst_buffer) = gif_data->frames[0]->data;
				_handle->dst_size = gif_data->size;
				_handle->width = gif_data->width;
				_handle->height = gif_data->height;
			}
		}
		break;
	case IMAGE_UTIL_BMP:
		{
			mm_util_bmp_data *bmp_data;

			bmp_data = (mm_util_bmp_data *) _handle->image_h;
			if (bmp_data == NULL) {
				image_util_error("Invalid bmp data");
				return MM_UTIL_ERROR_INVALID_PARAMETER;
			}

			if (_handle->path)
				err = mm_util_decode_from_bmp_file(bmp_data, _handle->path);
			else
				err = mm_util_decode_from_bmp_memory(bmp_data, &_handle->src_buffer[0], _handle->src_size);

			if (err == MM_UTIL_ERROR_NONE) {
				*(_handle->dst_buffer) = bmp_data->data;
				_handle->dst_size = bmp_data->size;
				_handle->width = bmp_data->width;
				_handle->height = bmp_data->height;
			}
		}
		break;
	default:
		err = MM_UTIL_ERROR_INVALID_PARAMETER;
		break;
	}

	return err;
}

int image_util_decode_run(image_util_decode_h handle, unsigned long *width, unsigned long *height, unsigned long long *size)
{
	int err = MM_UTIL_ERROR_NONE;
	decode_encode_s *_handle = (decode_encode_s *) handle;

	if (_handle == NULL || _handle->is_decode == FALSE) {
		image_util_error("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}
	if ((_handle->path == NULL && _handle->src_buffer == NULL) || _handle->dst_buffer == NULL) {
		image_util_error("Invalid input/output");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	err = _image_util_decode_internal(_handle);

	if (err != MM_UTIL_ERROR_NONE) {
		image_util_error("Error - decode run");
		return _convert_image_util_error_code(__func__, err);
	}

	if (width)
		*width = _handle->width;
	if (height)
		*height = _handle->height;
	if (size)
		*size = _handle->dst_size;

	return _convert_image_util_error_code(__func__, err);
}

gpointer _image_util_decode_thread(gpointer data)
{
	decode_encode_s *_handle = (decode_encode_s *) data;
	int err = MM_UTIL_ERROR_NONE;
	gint64 end_time = 0;

	if (!_handle) {
		image_util_error("[ERROR] - handle");
		return NULL;
	}

	while (!_handle->is_finish) {
		end_time = g_get_monotonic_time() + 1 * G_TIME_SPAN_SECOND;
		image_util_debug("waiting...");
		g_mutex_lock(&(_handle->thread_mutex));
		g_cond_wait_until(&(_handle->thread_cond), &(_handle->thread_mutex), end_time);
		image_util_debug("<=== get run decode thread signal");
		g_mutex_unlock(&(_handle->thread_mutex));

		if (_handle->is_finish) {
			image_util_debug("exit loop");
			break;
		}

		err = _image_util_decode_internal(_handle);
		if (err == MM_UTIL_ERROR_NONE)
			image_util_debug("Success - decode_internal");
		else
			image_util_error("Error - decode_internal");

		if (_handle->_decode_cb) {
			image_util_debug("completed_cb");
			_handle->is_finish = TRUE;
			_handle->_decode_cb->image_decode_completed_cb(_convert_image_util_error_code(__func__, err), _handle->_decode_cb->user_data, _handle->width, _handle->height, _handle->dst_size);
		}
	}

	image_util_debug("exit thread");

	return NULL;
}

static int _image_util_decode_create_thread(decode_encode_s * handle)
{
	int err = MM_UTIL_ERROR_NONE;

	image_util_retvm_if((handle == NULL), MM_UTIL_ERROR_INVALID_PARAMETER, "Invalid Handle");

	g_mutex_init(&(handle->thread_mutex));

	g_cond_init(&(handle->thread_cond));

	/*create threads */
	handle->thread = g_thread_new("decode_thread", (GThreadFunc) _image_util_decode_thread, (gpointer) handle);
	if (!handle->thread) {
		image_util_error("ERROR - create thread");
		g_mutex_clear(&(handle->thread_mutex));

		g_cond_clear(&(handle->thread_cond));
		return MM_UTIL_ERROR_INVALID_OPERATION;
	}

	return err;
}

int image_util_decode_run_async(image_util_decode_h handle, image_util_decode_completed_cb completed_cb, void *user_data)
{
	int err = MM_UTIL_ERROR_NONE;
	decode_encode_s *_handle = (decode_encode_s *) handle;

	if (_handle == NULL || _handle->is_decode == FALSE) {
		image_util_error("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}
	if ((_handle->path == NULL && _handle->src_buffer == NULL) || _handle->dst_buffer == NULL) {
		image_util_error("Invalid input/output");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}
	image_util_retvm_if((completed_cb == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid callback");

	if (_handle->_decode_cb != NULL) {
		IMAGE_UTIL_SAFE_FREE(_handle->_decode_cb);
		_handle->_decode_cb = NULL;
	}
	_handle->_decode_cb = (decode_cb_s *) calloc(1, sizeof(decode_cb_s));
	image_util_retvm_if((_handle->_decode_cb == NULL), IMAGE_UTIL_ERROR_OUT_OF_MEMORY, "Out of memory");

	_handle->_decode_cb->user_data = user_data;
	_handle->_decode_cb->image_decode_completed_cb = completed_cb;

	err = _image_util_decode_create_thread(_handle);

	return _convert_image_util_error_code(__func__, err);
}

int image_util_decode_destroy(image_util_decode_h handle)
{
	int err = IMAGE_UTIL_ERROR_NONE;
	decode_encode_s *_handle = (decode_encode_s *) handle;

	image_util_debug("image_util_decode_destroy");

	if (_handle == NULL || _handle->is_decode == FALSE) {
		image_util_error("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	switch (_handle->image_type) {
	case IMAGE_UTIL_JPEG:
		{
			mm_util_jpeg_yuv_data *jpeg_data;

			jpeg_data = (mm_util_jpeg_yuv_data *) _handle->image_h;
			if (jpeg_data == NULL) {
				image_util_error("Invalid jpeg data");
				return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
			}
			IMAGE_UTIL_SAFE_FREE(jpeg_data);
		}
		break;
	case IMAGE_UTIL_PNG:
		{
			mm_util_png_data *png_data;

			png_data = (mm_util_png_data *) _handle->image_h;
			if (png_data == NULL) {
				image_util_error("Invalid png data");
				return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
			}
			IMAGE_UTIL_SAFE_FREE(png_data);
		}
		break;
	case IMAGE_UTIL_GIF:
		{
			mm_util_gif_data *gif_data;

			gif_data = (mm_util_gif_data *) _handle->image_h;
			if (gif_data == NULL) {
				image_util_error("Invalid gif data");
				return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
			}
			IMAGE_UTIL_SAFE_FREE(gif_data);
		}
		break;
	case IMAGE_UTIL_BMP:
		{
			mm_util_bmp_data *bmp_data;

			bmp_data = (mm_util_bmp_data *) _handle->image_h;
			if (bmp_data == NULL) {
				image_util_error("Invalid bmp data");
				return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
			}
			IMAGE_UTIL_SAFE_FREE(bmp_data);
		}
		break;
	default:
		break;
	}

	/* g_thread_exit(handle->thread); */
	if (_handle->thread) {
		_handle->is_finish = TRUE;
		g_mutex_lock(&(_handle->thread_mutex));
		g_cond_signal(&(_handle->thread_cond));
		image_util_debug("===> send signal(finish) to decode_thread");
		g_mutex_unlock(&(_handle->thread_mutex));

		g_thread_join(_handle->thread);

		g_mutex_clear(&(_handle->thread_mutex));

		g_cond_clear(&(_handle->thread_cond));
	}
	IMAGE_UTIL_SAFE_FREE(_handle->src_buffer);
	IMAGE_UTIL_SAFE_FREE(_handle);

	return err;
}

static int _image_util_encode_create_jpeg_handle(decode_encode_s * handle)
{
	int err = MM_UTIL_ERROR_NONE;

	image_util_retvm_if((handle == NULL), MM_UTIL_ERROR_INVALID_PARAMETER, "Invalid Handle");

	mm_util_jpeg_yuv_data *_handle = (mm_util_jpeg_yuv_data *) calloc(1, sizeof(mm_util_jpeg_yuv_data));
	image_util_retvm_if((_handle == NULL), MM_UTIL_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY(0x%08x)", MM_UTIL_ERROR_OUT_OF_MEMORY);

	handle->image_h = (mm_util_imgp_h) _handle;
	handle->colorspace = IMAGE_UTIL_COLORSPACE_RGBA8888;
	handle->quality = 75;

	return err;
}

static int _image_util_encode_create_png_handle(decode_encode_s * handle)
{
	int err = MM_UTIL_ERROR_NONE;

	image_util_retvm_if((handle == NULL), MM_UTIL_ERROR_INVALID_PARAMETER, "Invalid Handle");

	mm_util_png_data *_handle = (mm_util_png_data *) calloc(1, sizeof(mm_util_png_data));
	image_util_retvm_if((_handle == NULL), MM_UTIL_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY(0x%08x)", MM_UTIL_ERROR_OUT_OF_MEMORY);

	mm_util_init_encode_png(_handle);

	handle->image_h = (mm_util_imgp_h) _handle;

	return err;
}

static int _image_util_encode_create_gif_handle(decode_encode_s * handle)
{
	int err = MM_UTIL_ERROR_NONE;

	image_util_retvm_if((handle == NULL), MM_UTIL_ERROR_INVALID_PARAMETER, "Invalid Handle");

	mm_util_gif_data *_handle = (mm_util_gif_data *) calloc(1, sizeof(mm_util_gif_data));
	image_util_retvm_if((_handle == NULL), MM_UTIL_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY(0x%08x)", MM_UTIL_ERROR_OUT_OF_MEMORY);

	_handle->frames = (mm_util_gif_frame_data **) calloc(1, sizeof(mm_util_gif_frame_data *));
	if (_handle->frames == NULL) {
		image_util_error("Error - OUT_OF_MEMORY");
		IMAGE_UTIL_SAFE_FREE(_handle);
		return MM_UTIL_ERROR_OUT_OF_MEMORY;
	}
	_handle->frames[0] = (mm_util_gif_frame_data *) calloc(1, sizeof(mm_util_gif_frame_data));
	if (_handle->frames[0] == NULL) {
		image_util_error("Error - OUT_OF_MEMORY");
		IMAGE_UTIL_SAFE_FREE(_handle);
		return MM_UTIL_ERROR_OUT_OF_MEMORY;
	}

	handle->image_h = (mm_util_imgp_h) _handle;

	return err;
}

static int _image_util_encode_create_bmp_handle(decode_encode_s * handle)
{
	int err = MM_UTIL_ERROR_NONE;

	image_util_retvm_if((handle == NULL), MM_UTIL_ERROR_INVALID_PARAMETER, "Invalid Handle");

	mm_util_bmp_data *_handle = (mm_util_bmp_data *) calloc(1, sizeof(mm_util_bmp_data));
	image_util_retvm_if((_handle == NULL), MM_UTIL_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY(0x%08x)", MM_UTIL_ERROR_OUT_OF_MEMORY);

	handle->image_h = (mm_util_imgp_h) _handle;

	return err;
}

int image_util_encode_create(image_util_type_e image_type, image_util_encode_h * handle)
{
	int err = MM_UTIL_ERROR_NONE;

	image_util_debug("image_util_encode_create");

	image_util_retvm_if((handle == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid Handle");

	decode_encode_s *_handle = (decode_encode_s *) calloc(1, sizeof(decode_encode_s));
	image_util_retvm_if((_handle == NULL), IMAGE_UTIL_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY(0x%08x)", IMAGE_UTIL_ERROR_OUT_OF_MEMORY);

	_handle->image_type = image_type;
	_handle->src_buffer = NULL;
	_handle->dst_buffer = NULL;
	_handle->path = NULL;
	_handle->image_h = 0;
	_handle->is_decode = FALSE;
	_handle->image_count = 1;
	_handle->current_image_count = 0;

	switch (image_type) {
	case IMAGE_UTIL_JPEG:
		err = _image_util_encode_create_jpeg_handle(_handle);
		break;
	case IMAGE_UTIL_PNG:
		err = _image_util_encode_create_png_handle(_handle);
		break;
	case IMAGE_UTIL_GIF:
		err = _image_util_encode_create_gif_handle(_handle);
		break;
	case IMAGE_UTIL_BMP:
		err = _image_util_encode_create_bmp_handle(_handle);
		break;
	default:
		err = MM_UTIL_ERROR_INVALID_PARAMETER;
		break;
	}

	if (err != MM_UTIL_ERROR_NONE) {
		image_util_error("Error - create image handle");
		IMAGE_UTIL_SAFE_FREE(_handle);
		return _convert_image_util_error_code(__func__, err);
	}

	*handle = (image_util_encode_h) _handle;

	return _convert_image_util_error_code(__func__, err);
}

int image_util_encode_set_resolution(image_util_encode_h handle, unsigned long width, unsigned long height)
{
	int err = IMAGE_UTIL_ERROR_NONE;
	decode_encode_s *_handle = (decode_encode_s *) handle;

	if (_handle == NULL || _handle->is_decode == TRUE) {
		image_util_error("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}
	image_util_retvm_if((_image_util_check_resolution(width, height) == false), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid resolution");

	switch (_handle->image_type) {
	case IMAGE_UTIL_JPEG:
		{
			mm_util_jpeg_yuv_data *jpeg_data;

			jpeg_data = (mm_util_jpeg_yuv_data *) _handle->image_h;
			if (jpeg_data == NULL) {
				image_util_error("Invalid jpeg data");
				return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
			}
			jpeg_data->width = width;
			jpeg_data->height = height;
		}
		break;
	case IMAGE_UTIL_PNG:
		{
			mm_util_png_data *png_data;

			png_data = (mm_util_png_data *) _handle->image_h;
			if (png_data == NULL) {
				image_util_error("Invalid png data");
				return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
			}
			mm_util_png_encode_set_width(png_data, width);
			mm_util_png_encode_set_height(png_data, height);
		}
		break;
	case IMAGE_UTIL_GIF:
		{
			mm_util_gif_data *gif_data;

			gif_data = (mm_util_gif_data *) _handle->image_h;
			if (gif_data == NULL) {
				image_util_error("Invalid gif data");
				return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
			}
			if (_handle->current_image_count >= _handle->image_count) {
				image_util_error("Present frame count exceeds total number of frames.");
				return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
			}

			if (!_handle->current_image_count) {
				mm_util_gif_encode_set_width(gif_data, width);
				mm_util_gif_encode_set_height(gif_data, height);
				_handle->width = width;
				_handle->height = height;
			} else if ((width > gif_data->frames[0]->width) || (height > gif_data->frames[0]->height)) {
				image_util_error("Image resolution cannot be more than the resolution of the first image");
				return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
			}

			gif_data->frames[_handle->current_image_count]->width = width;
			gif_data->frames[_handle->current_image_count]->height = height;

			return err;
		}
		break;
	case IMAGE_UTIL_BMP:
		{
			mm_util_bmp_data *bmp_data;

			bmp_data = (mm_util_bmp_data *) _handle->image_h;
			if (bmp_data == NULL) {
				image_util_error("Invalid bmp data");
				return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
			}
			mm_util_bmp_encode_set_width(bmp_data, width);
			mm_util_bmp_encode_set_height(bmp_data, height);
		}
		break;
	default:
		err = IMAGE_UTIL_ERROR_INVALID_PARAMETER;
		break;
	}

	_handle->width = width;
	_handle->height = height;

	return err;
}

int image_util_encode_set_colorspace(image_util_encode_h handle, image_util_colorspace_e colorspace)
{
	int err = IMAGE_UTIL_ERROR_NONE;
	decode_encode_s *_handle = (decode_encode_s *) handle;

	if (_handle == NULL || _handle->is_decode == TRUE) {
		image_util_error("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	image_util_retvm_if((colorspace < 0 || colorspace >= sizeof(_convert_jpeg_colorspace_tbl) / sizeof(int)), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid colorspace");
	switch (_handle->image_type) {
	case IMAGE_UTIL_JPEG:
		image_util_retvm_if((_convert_jpeg_colorspace_tbl[colorspace] == -1), IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT, "not supported format");
		break;
	case IMAGE_UTIL_PNG:
		image_util_retvm_if((_convert_png_colorspace_tbl[colorspace] == -1), IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT, "not supported format");
		break;
	case IMAGE_UTIL_GIF:
		image_util_retvm_if((_convert_gif_colorspace_tbl[colorspace] == -1), IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT, "not supported format");
		break;
	case IMAGE_UTIL_BMP:
		image_util_retvm_if((_convert_bmp_colorspace_tbl[colorspace] == -1), IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT, "not supported format");
		break;
	default:
		image_util_error("Invalid image type");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	_handle->colorspace = colorspace;

	return err;
}

int image_util_encode_set_quality(image_util_encode_h handle, int quality)
{
	int err = IMAGE_UTIL_ERROR_NONE;
	decode_encode_s *_handle = (decode_encode_s *) handle;

	if (_handle == NULL || _handle->is_decode == TRUE) {
		image_util_error("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}
	if (_handle->image_type != IMAGE_UTIL_JPEG) {
		image_util_error("Wrong image format");
		return IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT;
	}
	image_util_retvm_if((quality <= 0 || quality > 100), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid quality");

	_handle->quality = quality;

	return err;
}

int image_util_encode_set_png_compression(image_util_encode_h handle, image_util_png_compression_e compression)
{
	int err = IMAGE_UTIL_ERROR_NONE;
	decode_encode_s *_handle = (decode_encode_s *) handle;
	mm_util_png_data *png_data;

	if (_handle == NULL || _handle->is_decode == TRUE) {
		image_util_error("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}
	if (_handle->image_type != IMAGE_UTIL_PNG) {
		image_util_error("Wrong image format");
		return IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT;
	}
	image_util_retvm_if((compression < IMAGE_UTIL_PNG_COMPRESSION_0 || compression > IMAGE_UTIL_PNG_COMPRESSION_9), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid compression value");

	png_data = (mm_util_png_data *) _handle->image_h;
	if (png_data == NULL) {
		image_util_error("Invalid png data");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	mm_util_png_encode_set_compression_level(png_data, compression);

	return err;
}

int image_util_encode_animated_gif_set_frame_count(image_util_encode_h handle, unsigned int frame_count)
{
	int err = IMAGE_UTIL_ERROR_NONE;
	decode_encode_s *_handle = (decode_encode_s *) handle;
	mm_util_gif_data *gif_data;
	unsigned int i = 0;

	if (_handle == NULL || _handle->is_decode == TRUE) {
		image_util_error("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}
	if (frame_count == 0) {
		image_util_error("Frame count cannot be set as 0");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}
	gif_data = (mm_util_gif_data *) _handle->image_h;
	if (gif_data == NULL) {
		image_util_error("Invalid gif data");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	_handle->image_count = frame_count;

	gif_data->frames = (mm_util_gif_frame_data **) realloc(gif_data->frames, (_handle->image_count) * sizeof(mm_util_gif_frame_data *));
	if (gif_data->frames == NULL) {
		image_util_error("Error - OUT_OF_MEMORY");
		IMAGE_UTIL_SAFE_FREE(_handle);
		return IMAGE_UTIL_ERROR_OUT_OF_MEMORY;
	}

	for(i = 1; i < _handle->image_count; i++) {
		gif_data->frames[i] = (mm_util_gif_frame_data *) calloc(1, sizeof(mm_util_gif_frame_data));
		if (gif_data->frames[i] == NULL) {
			image_util_error("Error - OUT_OF_MEMORY");
			IMAGE_UTIL_SAFE_FREE(_handle);
			return IMAGE_UTIL_ERROR_OUT_OF_MEMORY;
		}
	}

	return err;
}

int image_util_encode_animated_gif_set_frame_delay_time(image_util_encode_h handle, unsigned long long delay_time)
{
	int err = IMAGE_UTIL_ERROR_NONE;
	decode_encode_s *_handle = (decode_encode_s *) handle;
	mm_util_gif_data *gif_data;

	if (_handle == NULL || _handle->is_decode == TRUE) {
		image_util_error("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	gif_data = (mm_util_gif_data *) _handle->image_h;
	if (gif_data == NULL) {
		image_util_error("Invalid gif data");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}
	if (_handle->current_image_count >= _handle->image_count) {
		image_util_error("Present frame count exceeds total number of frames.");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}
	if (!_handle->image_count || gif_data->frames == NULL) {
		image_util_error("Error allocating gif frames.");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	mm_util_gif_encode_set_frame_delay_time(gif_data->frames[_handle->current_image_count], delay_time);

	return err;
}

int image_util_encode_set_input_buffer(image_util_encode_h handle, const unsigned char *src_buffer)
{
	int err = IMAGE_UTIL_ERROR_NONE;
	decode_encode_s *_handle = (decode_encode_s *) handle;

	if (_handle == NULL || _handle->is_decode == TRUE) {
		image_util_error("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}
	if (src_buffer == NULL) {
		image_util_error("Invalid input buffer");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	_handle->src_buffer = (void *)realloc(_handle->src_buffer, (_handle->current_image_count + 1) * sizeof(void *));
	_handle->src_buffer[_handle->current_image_count] = (void *)src_buffer;

	if (_handle->image_type == IMAGE_UTIL_GIF) {
		mm_util_gif_data *gif_data;

		gif_data = (mm_util_gif_data *) _handle->image_h;

		if (gif_data->frames == NULL) {
			image_util_error("Error allocating gif frames.");
			return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
		}

		gif_data->frames[_handle->current_image_count]->data = _handle->src_buffer[_handle->current_image_count];
		if(_handle->image_count > 1)
			_handle->current_image_count++;
	}

	return err;
}

int image_util_encode_set_output_path(image_util_encode_h handle, const char *path)
{
	int err = IMAGE_UTIL_ERROR_NONE;
	decode_encode_s *_handle = (decode_encode_s *) handle;

	if (_handle == NULL || _handle->is_decode == TRUE) {
		image_util_error("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}
	image_util_retvm_if((path == NULL || strlen(path) == 0), IMAGE_UTIL_ERROR_NO_SUCH_FILE, "Invalid path");

	if (_handle->dst_buffer)
		_handle->dst_buffer = NULL;

	_handle->path = path;

	return err;
}

int image_util_encode_set_output_buffer(image_util_encode_h handle, unsigned char **dst_buffer)
{
	int err = IMAGE_UTIL_ERROR_NONE;
	decode_encode_s *_handle = (decode_encode_s *) handle;

	if (_handle == NULL || _handle->is_decode == TRUE) {
		image_util_error("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}
	if (_handle->image_type == IMAGE_UTIL_BMP) {
		image_util_error("BMP library does not support encoding to memory");
		return IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT;
	}
	if (dst_buffer == NULL) {
		image_util_error("Invalid output buffer");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	if (_handle->path)
		_handle->path = NULL;

	_handle->dst_buffer = (void **)dst_buffer;

	return err;
}

static int _image_util_encode_internal(decode_encode_s * _handle)
{
	int err = MM_UTIL_ERROR_NONE;

	switch (_handle->image_type) {
	case IMAGE_UTIL_JPEG:
		{
			if (_handle->path)
				err = mm_util_jpeg_encode_to_file(_handle->path, _handle->src_buffer[0], _handle->width, _handle->height, _convert_jpeg_colorspace_tbl[_handle->colorspace], _handle->quality);
			else
				err = mm_util_jpeg_encode_to_memory(_handle->dst_buffer, (int *)&(_handle->dst_size), _handle->src_buffer[0], _handle->width, _handle->height, _convert_jpeg_colorspace_tbl[_handle->colorspace], _handle->quality);
		}
		break;
	case IMAGE_UTIL_PNG:
		{
			mm_util_png_data *png_data;

			png_data = (mm_util_png_data *) _handle->image_h;
			if (png_data == NULL) {
				image_util_error("Invalid png data");
				return MM_UTIL_ERROR_INVALID_PARAMETER;
			}

			if (_handle->path)
				err = mm_util_encode_to_png_file(&(_handle->src_buffer[0]), png_data, _handle->path);
			else
				err = mm_util_encode_to_png_memory(&(_handle->src_buffer[0]), png_data);

			if (err == MM_UTIL_ERROR_NONE) {
				if (_handle->dst_buffer)
					*(_handle->dst_buffer) = png_data->data;
				_handle->dst_size = png_data->size;
				_handle->width = png_data->width;
				_handle->height = png_data->height;
			}
		}
		break;
	case IMAGE_UTIL_GIF:
		{
			mm_util_gif_data *gif_data;
			void *dst_buffer = NULL;

			gif_data = (mm_util_gif_data *) _handle->image_h;
			if (gif_data == NULL) {
				image_util_error("Invalid gif data");
				return MM_UTIL_ERROR_INVALID_PARAMETER;
			}
			if((_handle->image_count > 1) && (_handle->image_count != _handle->current_image_count)) {
				image_util_error("Total frame count does not match with the data set, for animated gif encoding");
				return MM_UTIL_ERROR_INVALID_OPERATION;
			}

			mm_util_gif_encode_set_image_count(gif_data, _handle->image_count);
			if (_handle->path)
				err = mm_util_encode_gif_to_file(gif_data, _handle->path);
			else
				err = mm_util_encode_gif_to_memory(gif_data, &dst_buffer);

			if (err == MM_UTIL_ERROR_NONE) {
				if (_handle->dst_buffer)
					*(_handle->dst_buffer) = (unsigned char *)dst_buffer;
				_handle->dst_size = gif_data->size;
				_handle->width = gif_data->width;
				_handle->height = gif_data->height;
			}
		}
		break;
	case IMAGE_UTIL_BMP:
		{
			mm_util_bmp_data *bmp_data;

			bmp_data = (mm_util_bmp_data *) _handle->image_h;
			if (bmp_data == NULL) {
				image_util_error("Invalid bmp data");
				return MM_UTIL_ERROR_INVALID_PARAMETER;
			}

			bmp_data->data = _handle->src_buffer[0];
			if (_handle->path)
				err = mm_util_encode_bmp_to_file(bmp_data, _handle->path);
			else {
				image_util_error("Not yet implemented");
				return MM_UTIL_ERROR_INVALID_PARAMETER;
			}

			if (err == MM_UTIL_ERROR_NONE) {
				if (_handle->dst_buffer)
					*(_handle->dst_buffer) = bmp_data->data;
				_handle->dst_size = bmp_data->size;
				_handle->width = bmp_data->width;
				_handle->height = bmp_data->height;
			}
		}
		break;
	default:
		err = MM_UTIL_ERROR_INVALID_PARAMETER;
		break;
	}

	return err;
}

int image_util_encode_run(image_util_encode_h handle, unsigned long long *size)
{
	int err = MM_UTIL_ERROR_NONE;
	decode_encode_s *_handle = (decode_encode_s *) handle;

	if (_handle == NULL || _handle->is_decode == TRUE) {
		image_util_error("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}
	if ((_handle->path == NULL && _handle->dst_buffer == NULL) || _handle->src_buffer == NULL) {
		image_util_error("Invalid input/output");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}
	image_util_retvm_if((_image_util_check_resolution(_handle->width, _handle->height) == false), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid resolution");

	err = _image_util_encode_internal(_handle);

	if (err != MM_UTIL_ERROR_NONE) {
		image_util_error("Error - encode run");
		return _convert_image_util_error_code(__func__, err);
	}

	if (size)
		*size = _handle->dst_size;

	return _convert_image_util_error_code(__func__, err);
}

gpointer _image_util_encode_thread(gpointer data)
{
	decode_encode_s *_handle = (decode_encode_s *) data;
	int err = MM_UTIL_ERROR_NONE;
	gint64 end_time = 0;

	if (!_handle) {
		image_util_error("[ERROR] - handle");
		return NULL;
	}

	while (!_handle->is_finish) {
		end_time = g_get_monotonic_time() + 1 * G_TIME_SPAN_SECOND;
		image_util_debug("waiting...");
		g_mutex_lock(&(_handle->thread_mutex));
		g_cond_wait_until(&(_handle->thread_cond), &(_handle->thread_mutex), end_time);
		image_util_debug("<=== get run encode thread signal");
		g_mutex_unlock(&(_handle->thread_mutex));

		if (_handle->is_finish) {
			image_util_debug("exit loop");
			break;
		}

		err = _image_util_encode_internal(_handle);
		if (err == MM_UTIL_ERROR_NONE)
			image_util_debug("Success - encode_internal");
		else
			image_util_error("Error - encode_internal");

		if (_handle->_encode_cb) {
			image_util_debug("completed_cb");
			_handle->is_finish = TRUE;
			_handle->_encode_cb->image_encode_completed_cb(_convert_image_util_error_code(__func__, err), _handle->_encode_cb->user_data, _handle->dst_size);
		}
	}

	image_util_debug("exit thread");

	return NULL;
}

static int _image_util_encode_create_thread(decode_encode_s * handle)
{
	int ret = MM_UTIL_ERROR_NONE;

	image_util_retvm_if((handle == NULL), MM_UTIL_ERROR_INVALID_PARAMETER, "Invalid Handle");

	g_mutex_init(&(handle->thread_mutex));

	g_cond_init(&(handle->thread_cond));

	/*create threads */
	handle->thread = g_thread_new("encode_thread", (GThreadFunc) _image_util_encode_thread, (gpointer) handle);
	if (!handle->thread) {
		image_util_error("ERROR - create thread");
		g_mutex_clear(&(handle->thread_mutex));

		g_cond_clear(&(handle->thread_cond));
		return MM_UTIL_ERROR_INVALID_OPERATION;
	}

	return ret;
}

int image_util_encode_run_async(image_util_encode_h handle, image_util_encode_completed_cb completed_cb, void *user_data)
{
	int err = MM_UTIL_ERROR_NONE;
	decode_encode_s *_handle = (decode_encode_s *) handle;

	if (_handle == NULL || _handle->is_decode == TRUE) {
		image_util_error("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}
	if ((_handle->path == NULL && _handle->dst_buffer == NULL) || _handle->src_buffer == NULL) {
		image_util_error("Invalid input/output");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}
	image_util_retvm_if((_image_util_check_resolution(_handle->width, _handle->height) == false), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid resolution");

	image_util_retvm_if((completed_cb == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid callback");

	if (_handle->_encode_cb != NULL) {
		IMAGE_UTIL_SAFE_FREE(_handle->_encode_cb);
		_handle->_encode_cb = NULL;
	}
	_handle->_encode_cb = (encode_cb_s *) calloc(1, sizeof(encode_cb_s));
	image_util_retvm_if((_handle->_encode_cb == NULL), IMAGE_UTIL_ERROR_OUT_OF_MEMORY, "Out of memory");

	_handle->_encode_cb->user_data = user_data;
	_handle->_encode_cb->image_encode_completed_cb = completed_cb;

	err = _image_util_encode_create_thread(_handle);

	return _convert_image_util_error_code(__func__, err);
}

int image_util_encode_destroy(image_util_encode_h handle)
{
	int err = IMAGE_UTIL_ERROR_NONE;
	decode_encode_s *_handle = (decode_encode_s *) handle;

	image_util_debug("image_util_encode_destroy");

	if (_handle == NULL || _handle->is_decode == TRUE) {
		image_util_error("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	switch (_handle->image_type) {
	case IMAGE_UTIL_JPEG:
		{
			mm_util_jpeg_yuv_data *jpeg_data;

			jpeg_data = (mm_util_jpeg_yuv_data *) _handle->image_h;
			if (jpeg_data == NULL) {
				image_util_error("Invalid jpeg data");
				return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
			}
			IMAGE_UTIL_SAFE_FREE(jpeg_data);
		}
		break;
	case IMAGE_UTIL_PNG:
		{
			mm_util_png_data *png_data;

			png_data = (mm_util_png_data *) _handle->image_h;
			if (png_data == NULL) {
				image_util_error("Invalid png data");
				return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
			}
			IMAGE_UTIL_SAFE_FREE(png_data);
		}
		break;
	case IMAGE_UTIL_GIF:
		{
			mm_util_gif_data *gif_data;
			unsigned int i = 0;

			gif_data = (mm_util_gif_data *) _handle->image_h;
			if (gif_data == NULL) {
				image_util_error("Invalid gif data");
				return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
			}
			for (i = 1; i < _handle->image_count; i++)
				IMAGE_UTIL_SAFE_FREE(gif_data->frames[i]);
			IMAGE_UTIL_SAFE_FREE(gif_data->frames[0]);
			IMAGE_UTIL_SAFE_FREE(gif_data->frames);
			IMAGE_UTIL_SAFE_FREE(gif_data);
		}
		break;
	case IMAGE_UTIL_BMP:
		{
			mm_util_bmp_data *bmp_data;

			bmp_data = (mm_util_bmp_data *) _handle->image_h;
			if (bmp_data == NULL) {
				image_util_error("Invalid bmp data");
				return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
			}
			IMAGE_UTIL_SAFE_FREE(bmp_data);
		}
		break;
	default:
		err = IMAGE_UTIL_ERROR_INVALID_PARAMETER;
		break;
	}

	/* g_thread_exit(handle->thread); */
	if (_handle->thread) {
		_handle->is_finish = TRUE;
		g_mutex_lock(&(_handle->thread_mutex));
		g_cond_signal(&(_handle->thread_cond));
		image_util_debug("===> send signal(finish) to decode_thread");
		g_mutex_unlock(&(_handle->thread_mutex));

		g_thread_join(_handle->thread);

		g_mutex_clear(&(_handle->thread_mutex));

		g_cond_clear(&(_handle->thread_cond));
	}

	IMAGE_UTIL_SAFE_FREE(_handle->src_buffer);
	IMAGE_UTIL_SAFE_FREE(_handle);

	return err;
}
