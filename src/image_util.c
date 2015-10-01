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
#include <image_util.h>
#include <image_util_private.h>
#include <stdio.h>
#include <stdlib.h>
#include <mm_error.h>

#define IMAGE_UTIL_STRING_VALID(str)	\
	((str != NULL && strlen(str) > 0) ? true : false)

#define IMAGE_UTIL_SAFE_FREE(src)      { if(src) {free(src); src = NULL;}}

static int _convert_colorspace_tbl[] = {
	MM_UTIL_IMG_FMT_YUV420, 		/* IMAGE_UTIL_COLORSPACE_YUV420 */
	MM_UTIL_IMG_FMT_YUV422, 		/* IMAGE_UTIL_COLORSPACE_YUV422 */
	MM_UTIL_IMG_FMT_I420,  			/* IMAGE_UTIL_COLORSPACE_I420 */
	MM_UTIL_IMG_FMT_NV12, 			/* IMAGE_UTIL_COLORSPACE_NV12 */
	MM_UTIL_IMG_FMT_UYVY, 			/* IMAGE_UTIL_COLORSPACE_UYVY */
	MM_UTIL_IMG_FMT_YUYV,				/* IMAGE_UTIL_COLORSPACE_YUYV */
	MM_UTIL_IMG_FMT_RGB565,			/* IMAGE_UTIL_COLORSPACE_RGB565 */
	MM_UTIL_IMG_FMT_RGB888, 		/* IMAGE_UTIL_COLORSPACE_RGB888 */
	MM_UTIL_IMG_FMT_ARGB8888, 	/* IMAGE_UTIL_COLORSPACE_ARGB8888 */
	MM_UTIL_IMG_FMT_BGRA8888, 	/* IMAGE_UTIL_COLORSPACE_BGRA8888 */
	MM_UTIL_IMG_FMT_RGBA8888, 	/* IMAGE_UTIL_COLORSPACE_RGBA8888 */
	MM_UTIL_IMG_FMT_BGRX8888, 	/* IMAGE_UTIL_COLORSPACE_BGRX8888 */
	MM_UTIL_JPEG_FMT_NV21, 		/* IMAGE_UTIL_COLORSPACE_NV12 */
	MM_UTIL_JPEG_FMT_NV16, 		/* IMAGE_UTIL_COLORSPACE_NV16 */
	MM_UTIL_JPEG_FMT_NV61, 		/* IMAGE_UTIL_COLORSPACE_NV61 */
};


static int _convert_encode_colorspace_tbl[] = {
	MM_UTIL_JPEG_FMT_YUV420						,	/* IMAGE_UTIL_COLORSPACE_YUV420 */
	-1											,	/* IMAGE_UTIL_COLORSPACE_YUV422 */
	MM_UTIL_JPEG_FMT_YUV420						,	/* IMAGE_UTIL_COLORSPACE_I420 */
	MM_UTIL_JPEG_FMT_NV12						,	/* IMAGE_UTIL_COLORSPACE_NV12 */
	-1											,	/* IMAGE_UTIL_COLORSPACE_UYVY */
	-1											,	/* IMAGE_UTIL_COLORSPACE_YUYV */
	-1											,	/* IMAGE_UTIL_COLORSPACE_RGB565 */
	MM_UTIL_JPEG_FMT_RGB888						,	/* IMAGE_UTIL_COLORSPACE_RGB888 */
	MM_UTIL_JPEG_FMT_ARGB8888					,	/* IMAGE_UTIL_COLORSPACE_ARGB8888 */
	MM_UTIL_JPEG_FMT_BGRA8888					,	/* IMAGE_UTIL_COLORSPACE_BGRA8888 */
	MM_UTIL_JPEG_FMT_RGBA8888					,	/* IMAGE_UTIL_COLORSPACE_RGBA8888 */
	-1											,	/* IMAGE_UTIL_COLORSPACE_BGRX8888 */
	-1											,	/* IMAGE_UTIL_COLORSPACE_NV21 */
	-1											,	/* IMAGE_UTIL_COLORSPACE_NV16 */
	-1											,	/* IMAGE_UTIL_COLORSPACE_NV61 */
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

static void _image_util_transform_completed_cb(media_packet_h *dst, int error, void *user_data)
{
	int error_value = IMAGE_UTIL_ERROR_NONE;
	image_util_cb_s *_util_cb = (image_util_cb_s *)user_data;

	if ((_util_cb != NULL) && (_util_cb->image_processing_completed_cb != NULL)) {
		error_value = _image_util_error_convert(error);
		_util_cb->image_processing_completed_cb(dst, error_value, _util_cb->user_data);
	}

	return;
}

static int _image_util_create_transform_handle(transformation_s *handle)
{
	int err = MM_UTIL_ERROR_NONE;
	MMHandleType image_h;

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

int image_util_transform_create(transformation_h *handle)
{
	int err = MM_UTIL_ERROR_NONE;

	image_util_debug("image_util_create");

	image_util_retvm_if((handle == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid Handle");

	transformation_s *_handle = (transformation_s *)calloc(1, sizeof(transformation_s));
	image_util_retvm_if((_handle == NULL), IMAGE_UTIL_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY(0x%08x)", IMAGE_UTIL_ERROR_OUT_OF_MEMORY);

	_handle->colorspace = -1;
	_handle->_util_cb = NULL;
	_handle->image_h = 0;
	_handle->hardware_acceleration = false;
	_handle->set_convert = false;
	_handle->set_resize  = false;
	_handle->set_rotate = false;
	_handle->set_crop  = false;

	err = _image_util_create_transform_handle(_handle);
	if (err != MM_UTIL_ERROR_NONE) {
		image_util_error("Error - create transform handle");
		IMAGE_UTIL_SAFE_FREE(_handle);
		return _convert_image_util_error_code(__func__, err);
	}

	*handle = (transformation_h)_handle;

	return _convert_image_util_error_code(__func__, err);
}

int  image_util_transform_set_hardware_acceleration(transformation_h handle, bool mode)
{
	int err = MM_UTIL_ERROR_NONE;
	transformation_s *_handle = (transformation_s *)handle;

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
	transformation_s *_handle = (transformation_s *)handle;

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
	transformation_s *_handle = (transformation_s *)handle;

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
	_handle->set_resize  = true;

	return _convert_image_util_error_code(__func__, err);
}

int image_util_transform_set_rotation(transformation_h handle, image_util_rotation_e rotation)
{
	int err = MM_UTIL_ERROR_NONE;
	transformation_s *_handle = (transformation_s *)handle;

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
	transformation_s *_handle = (transformation_s *)handle;
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
	_handle->set_crop  = true;

	return _convert_image_util_error_code(__func__, err);
}

int image_util_transform_get_colorspace(transformation_h handle, image_util_colorspace_e *colorspace)
{
	int ret = IMAGE_UTIL_ERROR_NONE;
	transformation_s *_handle = (transformation_s *)handle;

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
	transformation_s *_handle = (transformation_s *)handle;

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

int image_util_transform_get_rotation(transformation_h handle, image_util_rotation_e *rotation)
{
	int ret = IMAGE_UTIL_ERROR_NONE;
	transformation_s *_handle = (transformation_s *)handle;

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
	transformation_s *_handle = (transformation_s *)handle;

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
	transformation_s *_handle = (transformation_s *)handle;

	image_util_debug("image_util_transform");

	image_util_retvm_if((_handle == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid Handle");
	image_util_retvm_if((completed_cb == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid callback");
	image_util_retvm_if((src == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid source");
	image_util_retvm_if((!_handle->set_convert && !_handle->set_resize && !_handle->set_rotate && !_handle->set_crop), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid transform");

	if (_handle->_util_cb != NULL) {
		IMAGE_UTIL_SAFE_FREE(_handle->_util_cb);
		_handle->_util_cb = NULL;
	}
	_handle->_util_cb = (image_util_cb_s *)calloc(1, sizeof(image_util_cb_s));
	image_util_retvm_if((_handle->_util_cb == NULL), IMAGE_UTIL_ERROR_OUT_OF_MEMORY, "Out of memory");

	_handle->_util_cb->user_data = user_data;
	_handle->_util_cb->image_processing_completed_cb = completed_cb;

	if (_handle->_util_cb) {
		err = mm_util_transform(_handle->image_h, src, (mm_util_completed_callback)_image_util_transform_completed_cb, (void *)_handle->_util_cb);
	}
	return _convert_image_util_error_code(__func__, err);
}

int image_util_transform_destroy(transformation_h handle)
{
	int err = MM_UTIL_ERROR_NONE;
	transformation_s *_handle = (transformation_s *)handle;

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

int image_util_convert_colorspace(unsigned char *dest, image_util_colorspace_e dest_colorspace, const unsigned char *src,  int width, int height, image_util_colorspace_e src_colorspace)
{
	int err = MM_UTIL_ERROR_NONE;

	image_util_retvm_if((dest == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "dest is null");
	image_util_retvm_if((src == NULL), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "src is null");
	image_util_retvm_if((dest_colorspace < 0 || dest_colorspace >= sizeof(_convert_colorspace_tbl) / sizeof(int)), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid dst_colorspace");
	image_util_retvm_if((src_colorspace < 0 || src_colorspace >= sizeof(_convert_colorspace_tbl) / sizeof(int)), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid src_colorspace");

	err = mm_util_convert_colorspace(src, width, height, _convert_colorspace_tbl[src_colorspace], dest, _convert_colorspace_tbl[dest_colorspace]);

	return _convert_image_util_error_code(__func__, err);
}


int image_util_calculate_buffer_size(int width, int height, image_util_colorspace_e colorspace , unsigned int *size)
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
	image_util_retvm_if((src_width <= x  || src_height <= y || src_width < x + *width || src_height < y + *height), IMAGE_UTIL_ERROR_INVALID_PARAMETER, "Invalid crop area");

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

int image_util_encode_jpeg(const unsigned char *buffer, int width, int height, image_util_colorspace_e colorspace,  int quality, const char *path)
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

int image_util_encode_jpeg_to_memory(const unsigned char *image_buffer, int width, int height, image_util_colorspace_e colorspace, int quality,  unsigned char **jpeg_buffer, unsigned int *jpeg_size)
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
