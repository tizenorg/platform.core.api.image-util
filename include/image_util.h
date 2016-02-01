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

#ifndef __TIZEN_MEDIA_IMAGE_UTIL_H__
#define __TIZEN_MEDIA_IMAGE_UTIL_H__

#include <image_util_type.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @file image_util.h
 * @brief This file contains the image util API.
 */

/**
 * @addtogroup CAPI_MEDIA_IMAGE_UTIL_MODULE
 * @{
 */

/**
* @brief Creates a handle to image util transform.
* @since_tizen 2.3
*
* @details This function creates a handle to image util transform.
*
* @remarks You must release the @a image util handle using image_util_transform_destroy().
*
* @param[out] handle A handle to image util transform
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY Out of memory
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @see image_util_transform_destroy()
*
*/
int image_util_transform_create(transformation_h *handle);

/**
* @brief Sets the image util's accurate mode.
* @since_tizen 2.3
*
* @details This function set if you use hardware accerlation or not.
*
* @remarks The value returned will be IMAGE_UTIL_ERROR_NOT_SUPPORTED, if H/W acceleration doesn't support on the device.
*
* @param[in] handle The handle to image util
* @param [in] mode Set @c true, user can use the hardware acceleration\n
*                               otherwise set @c false if user can only software image processing
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED The application does not have the hardware acceleration
*
* @pre image_util_transform_create().
*
* @see image_util_transform_create()
* @see image_util_transform_destroy()
*/
int image_util_transform_set_hardware_acceleration(transformation_h handle, bool mode);

/**
* @brief Sets the information of the converting
* @since_tizen 2.3
*
* @param[in] handle The handle to image util transform
* @param[in] colorspace The colorspace of the image buffer
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Not supported format
*
* @pre image_util_transform_create().
*
* @see image_util_transform_create()
* @see image_util_transform_run()
* @see image_util_transform_destroy()
*/
int image_util_transform_set_colorspace(transformation_h handle, image_util_colorspace_e colorspace);

/**
* @brief Sets the information of the resizing
* @since_tizen 2.3
*
* @remarks image_util_transform_set_resolution and image_util_transform_set_crop_area can't do that at the same time.
*
* @param[in] handle The handle to image util transform
* @param[in] width The width of image buffer
* @param[in] height The height of image buffer
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @pre image_util_transform_create().
*
* @see image_util_transform_create()
* @see image_util_transform_run()
* @see image_util_transform_destroy()
*/
int image_util_transform_set_resolution(transformation_h handle, unsigned int width, unsigned int height);

/**
* @brief Sets the information of the rotating
* @since_tizen 2.3
*
* @param[in] handle The handle to image util transform
* @param[in] rotation The rotation value of image buffer
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @pre image_util_transform_create().
*
* @see image_util_transform_create()
* @see image_util_transform_run()
* @see image_util_transform_destroy()
*/
int image_util_transform_set_rotation(transformation_h handle, image_util_rotation_e rotation);

/**
* @brief Sets the information of the cropping
* @since_tizen 2.3
*
* @remarks image_util_transform_set_resolution and image_util_transform_set_crop_area can't do that at the same time.
*
* @param[in] handle The handle to image util transform
* @param[in] start_x The start x position of cropped image buffer
* @param[in] start_y The start y position of cropped image buffer
* @param[in] end_x The end x position of cropped image buffer
* @param[in] end_y The end y position of cropped image buffer
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @pre image_util_transform_create().
*
* @see image_util_transform_create()
* @see image_util_transform_run()
* @see image_util_transform_destroy()
*/
int image_util_transform_set_crop_area(transformation_h handle, unsigned int start_x, unsigned int start_y, unsigned int end_x, unsigned int end_y);

/**
* @brief Gets the colorspace of the image buffer
* @since_tizen 2.3
*
* @param[in] handle The handle to image util transform
* @param[in, out] colorspace The colorspace of the image buffer
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Not supported format
*
* @pre image_util_transform_create().\n
*            image_util_transform_set_crop_area().
*
* @see image_util_transform_create()
* @see image_util_transform_destroy()
*/
int image_util_transform_get_colorspace(transformation_h handle, image_util_colorspace_e *colorspace);

/**
* @brief Gets the resolution of the image buffer
* @since_tizen 2.3
*
* @param[in] handle The handle to image util transform
* @param[in, out] width The width of source image buffer
* @param[in, out] height The height of source image buffer
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @pre image_util_transform_create().\n
*            image_util_transform_set_resolution().
*
* @see image_util_transform_create()
* @see image_util_transform_destroy()
*/
int image_util_transform_get_resolution(transformation_h handle, unsigned int *width, unsigned int *height);

/**
* @brief Gets the information of the rotating
* @since_tizen 2.3
*
* @param[in] handle The handle to image util transform
* @param[in, out] rotation The rotation value of image buffer
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @pre image_util_transform_create().\n
*            image_util_transform_set_rotation().
*
* @see image_util_transform_create()
* @see image_util_transform_destroy()
*/
int image_util_transform_get_rotation(transformation_h handle, image_util_rotation_e *rotation);

/**
* @brief Gets the information of the cropping
* @since_tizen 2.3
*
* @param[in] handle The handle to image util transform
* @param[in, out] start_x The start x position of cropped source image buffer
* @param[in, out] start_y The start y position of cropped source image buffer
* @param[in, out] end_x The end x position of cropped source image buffer
* @param[in, out] end_y The end y position of cropped source image buffer
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @pre image_util_transform_create().\n
*            image_util_transform_set_crop_area().
*
* @see image_util_transform_create()
* @see image_util_transform_destroy()
*/
int image_util_transform_get_crop_area(transformation_h handle, unsigned int *start_x, unsigned int *start_y, unsigned int *end_x, unsigned int *end_y);

/**
* @brief Transform the image for given image util handle.
* @since_tizen 2.3
*
* @remarks If H/W acceleration is not set, transformation is done via S/W acceleration.
*
* @details The function execute asynchronously, which contains complete callback \n
*          If you set more than two transforming, the order of running is crop or resolution, colorspace converting, rotaion. \n
*
* @param[in] handle The handle of transform
* @param[in] src The handle to image util transform
* @param[in] callback The callback function to be invoked
* @param[in] user_data The user data to be passed to the callback function
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @see image_util_transform_create()
* @see image_util_transform_set_hardware_acceleration
* @see image_util_transform_destroy()
*/
int image_util_transform_run(transformation_h handle, media_packet_h src, image_util_transform_completed_cb callback, void *user_data);

/**
* @brief Destroys a handle to image util.
* @since_tizen 2.3
*
* @details The function frees all resources related to the image util handle. The image util
*               handle no longer can be used to perform any operation. A new image util handle
*               has to be created before the next usage.
*
* @param[in] handle The handle to image util transform
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @see image_util_transform_create()
*
*/
int image_util_transform_destroy(transformation_h handle);

/**
* @deprecated Deprecated since 3.0. Use image_util_supported_colorspace_cb() instead.
* @brief Called once for each supported JPEG encode/decode colorspace.
* @since_tizen 2.3
*
* @param[in] colorspace The colorspace
* @param[in] user_data The user data passed from the foreach function
*
* @return @c true to continue with the next iteration of the loop, \n
*         otherwise @c false to break out of the loop
*
* @pre image_util_foreach_supported_jpeg_colorspace() invokes this callback.
*
* @see image_util_foreach_supported_jpeg_colorspace()
* @see image_util_encode_jpeg()
* @see image_util_encode_jpeg_to_memory()
* @see image_util_decode_jpeg()
* @see image_util_decode_jpeg_from_memory()
*/
typedef bool(*image_util_supported_jpeg_colorspace_cb)(image_util_colorspace_e colorspace, void *user_data);

/**
* @deprecated Deprecated since 3.0. Use image_util_foreach_supported_colorspace() instead.
* @brief Retrieves all supported JPEG encoding/decoding colorspace by invoking a callback function once for each one.
* @since_tizen 2.3
*
* @param[in] callback The callback function to invoke
* @param[in] user_data The user data to be passed to the callback function
* @return 0 on success,
*         otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
* @post This function invokes image_util_supported_jpeg_colorspace_cb() repeatedly to retrieve each supported JPEG encoding/decoding colorspace.
*
* @see image_util_supported_jpeg_colorspace_cb()
* @see image_util_encode_jpeg()
* @see image_util_encode_jpeg_to_memory()
* @see image_util_decode_jpeg()
* @see image_util_decode_jpeg_from_memory()
*/
int image_util_foreach_supported_jpeg_colorspace(image_util_supported_jpeg_colorspace_cb callback, void *user_data);

/**
* @brief Calculates the size of the image buffer for the specified resolution and colorspace.
* @since_tizen 2.3
*
* @param[in] width The image width
* @param[in] height The image height
* @param[in] colorspace The image colorspace
* @param[out] size The Calculated buffer size
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @see image_util_transform_run()
* @see image_util_decode_jpeg()
* @see image_util_decode_jpeg_from_memory()
* @see image_util_encode_jpeg()
* @see image_util_encode_jpeg_to_memory()
*/
int image_util_calculate_buffer_size(int width, int height, image_util_colorspace_e colorspace , unsigned int *size);

/**
* @internal
* @brief Converts the image's colorspace.
* @since_tizen 2.3
*
* @remarks You must get the @a dest buffer size using image_util_transform_calculate_buffer_size().
*
* @param[in, out] dest The image buffer for result \n
*                     Must be allocated by the user
* @param[in] dest_colorspace The colorspace to be converted
* @param[in] src The source image buffer
* @param[in] width The width of the source image
* @param[in] height The height of the source image
* @param[in] src_colorspace The colorspace of the source image buffer
*
* @return %c 0 on success
*            otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @see image_util_transform_calculate_buffer_size()
* @see image_util_transform_create()
* @see image_util_transform_destroy()
*/
int image_util_convert_colorspace(unsigned char *dest, image_util_colorspace_e dest_colorspace, const unsigned char *src,  int width, int height, image_util_colorspace_e src_colorspace);

/**
* @internal
* @brief Resizes the image to the specified destination width and height.
* @since_tizen 2.3
*
* @remarks Because of YUV format padding, the destination image size can be adjusted.
*
* @param[in, out] dest The image buffer for result \n
*                     Must be allocated by the user.
* @param[in, out] dest_width The image width to resize, and resized width
* @param[in, out] dest_height The image height to resize, and resized height
* @param[in] src The image buffer for the original image
* @param[in] src_width The original image width
* @param[in] src_height The original image height
* @param[in] colorspace The image colorspace
*
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @see image_util_transform_calculate_buffer_size()
*/
int image_util_resize(unsigned char *dest, int *dest_width, int *dest_height, const unsigned char *src, int src_width, int src_height, image_util_colorspace_e colorspace);

/**
* @internal
* @brief Rotates the image to the specified angle given in degrees.
* @since_tizen 2.3
*
* @remarks Because of YUV format padding, the destination image size can be adjusted.
* Rotations are supported only in these color spaces\n
* #IMAGE_UTIL_COLORSPACE_YV12\n
* #IMAGE_UTIL_COLORSPACE_I420\n
* #IMAGE_UTIL_COLORSPACE_NV12\n
* #IMAGE_UTIL_COLORSPACE_RGB888\n
* #IMAGE_UTIL_COLORSPACE_RGB565\n
* #IMAGE_UTIL_COLORSPACE_ARGB8888\n
* #IMAGE_UTIL_COLORSPACE_BGRA8888\n
* #IMAGE_UTIL_COLORSPACE_RGBA8888\n
* #IMAGE_UTIL_COLORSPACE_BGRX8888\n.
*
* @param[in, out] dest The image buffer for result \n
*                     Must be allocated by the user.
* @param[out] dest_width The rotated image width
* @param[out] dest_height The rotated image height
* @param[in] dest_rotation The angle to rotate
* @param[in] src The image buffer for the original image
* @param[in] src_width The original image width
* @param[in] src_height The original image height
* @param[in] colorspace The image colorspace
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @see image_util_transform_calculate_buffer_size()
*/
int image_util_rotate(unsigned char *dest, int *dest_width, int *dest_height, image_util_rotation_e dest_rotation, const unsigned char *src, int src_width, int src_height, image_util_colorspace_e colorspace);

/**
* @internal
* @brief Crops the image to the specified point and dimension.
* @since_tizen 2.3
*
* @remarks Because of YUV format padding, the destination image size can be adjusted.
* Crop is supported only in these colorspaces\n
* #IMAGE_UTIL_COLORSPACE_YV12 \n
* #IMAGE_UTIL_COLORSPACE_I420 \n
* #IMAGE_UTIL_COLORSPACE_RGB888 \n
* #IMAGE_UTIL_COLORSPACE_RGB565 \n
* #IMAGE_UTIL_COLORSPACE_ARGB8888\n
* #IMAGE_UTIL_COLORSPACE_BGRA8888\n
* #IMAGE_UTIL_COLORSPACE_RGBA8888\n
* #IMAGE_UTIL_COLORSPACE_BGRX8888\n.
*
* @param[in, out] dest The image buffer for result. Must be allocated by you
* @param[in] x The starting x-axis of crop
* @param[in] y The starting y-axis of crop
* @param[in/out] width  The image width to crop, and cropped width
* @param[in/out] height  The image height to crop, and cropped height
* @param[in] src The image buffer for original image
* @param[in] src_width The original image width
* @param[in] src_height The original image height
* @param[in] colorspace The image colorspace
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @see image_util_transform_calculate_buffer_size()
*/
int image_util_crop(unsigned char *dest, int x, int y, int *width, int *height, const unsigned char *src, int src_width, int src_height, image_util_colorspace_e colorspace);




/**
* @deprecated Deprecated since 3.0. Use image_util_decode_create() instead.
* @brief Decodes the JPEG image to the buffer.
* @since_tizen 2.3
*
* @remarks You must release @a image_buffer using free().\n
*                 http://tizen.org/privilege/mediastorage is needed if input or output path are relevant to media storage.\n
*                 http://tizen.org/privilege/externalstorage is needed if input or output path are relevant to external storage.\n
*                 If you decode the JPEG image which has odd numbered width or height to YUV colorspace, \n
*                 the width or height of decoded data will be rounded down to even numbered width or height.
*
* @param[in] path The image file path
* @param[in] colorspace The decoded image colorspace
* @param[out] image_buffer The image buffer for the decoded image
* @param[out] width The image width
* @param[out] height The image height
* @param[out] size The image buffer size
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY out of memory
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Format not supported
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @see image_util_supported_jpeg_colorspace_cb()
* @see image_util_decode_jpeg_from_memory()
* @see image_util_foreach_supported_jpeg_colorspace()
*/
int image_util_decode_jpeg(const char *path, image_util_colorspace_e colorspace, unsigned char **image_buffer, int *width, int *height, unsigned int *size);

/**
* @deprecated Deprecated since 3.0. Use image_util_decode_create() instead.
* @brief Decodes the JPEG image(in memory) to the buffer.
* @since_tizen 2.3
*
* @remarks You must release @a image_buffer using free().\n
*                  If you decode the JPEG image which has odd numbered width or height to YUV colorspace, \n
*                  the width or height of decoded data will be rounded down to even numbered width or height.
*
* @param[in] jpeg_buffer The JPEG image buffer
* @param[in] jpeg_size The JPEG image buffer size
* @param[in] colorspace The decoded image colorspace
* @param[out] image_buffer The image buffer for the decoded image
* @param[out] width The image width
* @param[out] height The image height
* @param[out] size The image buffer size
*
* @return 0 on success,
*           otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY out of memory
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Format not supported
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @see image_util_supported_jpeg_colorspace_cb()
* @see image_util_decode_jpeg()
* @see image_util_foreach_supported_jpeg_colorspace()
*/
int image_util_decode_jpeg_from_memory(const unsigned char *jpeg_buffer, int jpeg_size, image_util_colorspace_e colorspace, unsigned char **image_buffer, int *width, int *height, unsigned int *size);

/**
* @deprecated Deprecated since 3.0. Use image_util_decode_create() instead.
* @brief Decodes the JPEG image to the buffer with downscale decoding option.
* @since_tizen 2.4
*
* @remarks You must release @a image_buffer using free().\n
*                 http://tizen.org/privilege/mediastorage is needed if input or output path are relevant to media storage.\n
*                 http://tizen.org/privilege/externalstorage is needed if input or output path are relevant to external storage.\n
*                 If you decode the JPEG image which has odd numbered width or height to YUV colorspace, \n
*                 the width or height of decoded data will be rounded down to even numbered width or height.
*
* @param[in] path The image file path
* @param[in] colorspace The decoded image colorspace
* @param[in] downscale The downscale value
* @param[out] image_buffer The image buffer for the decoded image
* @param[out] width The image width
* @param[out] height The image height
* @param[out] size The image buffer size
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY out of memory
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Format not supported
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @see image_util_supported_jpeg_colorspace_cb()
* @see image_util_decode_jpeg_from_memory()
* @see image_util_foreach_supported_jpeg_colorspace()
*/
int image_util_decode_jpeg_with_downscale(const char *path, image_util_colorspace_e colorspace, image_util_scale_e downscale, unsigned char **image_buffer, int *width, int *height, unsigned int *size);

/**
* @deprecated Deprecated since 3.0. Use image_util_decode_create() instead.
* @brief Decodes the JPEG image(in memory) to the buffer with downscale decoding option.
* @since_tizen 2.4
*
* @remarks You must release @a image_buffer using free().\n
*                   If you decode the JPEG image which has odd numbered width or height to YUV colorspace, \n
*                   the width or height of decoded data will be rounded down to even numbered width or height.
*
* @param[in] jpeg_buffer The JPEG image buffer
* @param[in] jpeg_size The JPEG image buffer size
* @param[in] colorspace The decoded image colorspace
* @param[in] downscale The downscale value
* @param[out] image_buffer The image buffer for the decoded image
* @param[out] width The image width
* @param[out] height The image height
* @param[out] size The image buffer size
*
* @return @c 0 on success,
*           otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY out of memory
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Format not supported
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @see image_util_supported_jpeg_colorspace_cb()
* @see image_util_decode_jpeg()
* @see image_util_foreach_supported_jpeg_colorspace()
*/
int image_util_decode_jpeg_from_memory_with_downscale(const unsigned char *jpeg_buffer, int jpeg_size, image_util_colorspace_e colorspace, image_util_scale_e downscale, unsigned char **image_buffer, int *width, int *height, unsigned int *size);

/**
* @deprecated Deprecated since 3.0. Use image_util_encode_create() instead.
* @brief Encodes the image to the JPEG image.
* @since_tizen 2.3
*
* @remarks http://tizen.org/privilege/mediastorage is needed if input or output path are relevant to media storage.\n
*                 http://tizen.org/privilege/externalstorage is needed if input or output path are relevant to external storage.
*
* @param[in] buffer The original image buffer
* @param[in] width The original image width
* @param[in] height The original image height
* @param[in] colorspace The original image colorspace
* @param[in] quality The quality for JPEG image encoding(1 ~ 100)
* @param[in] path The file path to be created
*
* @return 0 on success,
*           otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_NO_SUCH_FILE No such file
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Format not supported
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @see image_util_supported_jpeg_colorspace_cb()
* @see image_util_foreach_supported_jpeg_colorspace()
* @see image_util_encode_jpeg_to_memory()
*/
int image_util_encode_jpeg(const unsigned char *buffer, int width, int height, image_util_colorspace_e colorspace,  int quality, const char *path);

/**
* @deprecated Deprecated since 3.0. Use image_util_encode_create() instead.
* @brief Encodes the image to the JPEG image
* @since_tizen 2.3
*
* @remarks You must release @a jpeg_buffer using free().
*
* @param[in] image_buffer The original image buffer
* @param[in] width The image width
* @param[in] height The image height
* @param[in] colorspace The original image colorspace
* @param[in] quality The quality for JPEG image encoding(1 ~ 100)
* @param[out] jpeg_buffer The created JPEG image buffer \n
*                         The buffer is created by frameworks
* @param[out] jpeg_size The created jpeg image buffer size
*
* @return 0 on success,
*           otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY out of memory
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Format not supported
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @see image_util_supported_jpeg_colorspace_cb()
* @see image_util_foreach_supported_jpeg_colorspace()
* @see image_util_encode_jpeg()
*/
int image_util_encode_jpeg_to_memory(const unsigned char *image_buffer, int width, int height, image_util_colorspace_e colorspace, int quality,  unsigned char **jpeg_buffer, unsigned int *jpeg_size);

/**
* @brief Extracts representative color from an image buffer
* @since_tizen 3.0
* @remarks @a image_buffer should be RGB888 colorspace.
*
* @param[in] image_buffer The original image buffer
* @param[in] width The image width
* @param[in] height The image height
* @param[out] rgb_r The red color in RGB color space
* @param[out] rgb_g The green color in RGB color space
* @param[out] rgb_b The blue color in RGB color space
*
* @return 0 on success,
*           otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY out of memory
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
*
*/
int image_util_extract_color_from_memory(const unsigned char *image_buffer, int width, int height, unsigned char *rgb_r, unsigned char *rgb_g, unsigned char *rgb_b);

/**
* @brief Called once for each supported image encode/decode colorspace.
* @since_tizen 3.0
*
* @param[in] colorspace The colorspace
* @param[in] user_data The user data passed from the foreach function
*
* @return @c true to continue with the next iteration of the loop, \n
*         otherwise @c false to break out of the loop
*
* @pre image_util_foreach_supported_colorspace() invokes this callback.
*
* @see image_util_foreach_supported_colorspace()
* @see image_util_encode_create()
* @see image_util_decode_create()
*/
typedef bool(*image_util_supported_colorspace_cb)(image_util_colorspace_e colorspace, void *user_data);

/**
* @brief Retrieves all supported image encoding/decoding colorspace by invoking a callback function once for each one.
* @since_tizen 3.0
*
* @param[in] callback The callback function to invoke
* @param[in] user_data The user data to be passed to the callback function
* @return 0 on success,
*         otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
*
* @post This function invokes image_util_supported_colorspace_cb() repeatedly to retrieve each supported image encoding/decoding colorspace.
*
* @see image_util_supported_colorspace_cb()
* @see image_util_encode_create()
* @see image_util_decode_create()
*/
int image_util_foreach_supported_colorspace(image_util_type_e image_type, image_util_supported_colorspace_cb callback, void *user_data);

/**
* @brief Creates a handle to image util decoding.
* @since_tizen 3.0
*
* @details This function creates a handle to image util decoding.
*
* @remarks You must release the @a image util handle using image_util_decode_destroy().
*
* @param[out] handle A handle to image util decoding
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY Out of memory
*
* @see image_util_decode_destroy()
*
*/
int image_util_decode_create(image_util_decode_h *handle);

/**
* @brief Sets the input file path from which to decode.
* @since_tizen 3.0
*
* @remarks One of image_util_decode_set_input_path() or image_util_decode_set_input_buffer() should be set.\n
*          If both are set then the latest input set, is considered.\n
*          http://tizen.org/privilege/mediastorage is needed if input or output path are relevant to media storage.\n
*          http://tizen.org/privilege/externalstorage is needed if input or output path are relevant to external storage.\n
*          Finds out image type by reading the header of the image provided in input path.
*
* @param[in] handle The handle to image util decoding
* @param[in] path The path to input image
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_NO_SUCH_FILE No such file
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY Out of memory
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Format not supported
*
* @pre image_util_decode_create()
*
* @post image_util_decode_run() / image_util_decode_run_async()
* @post image_util_decode_destroy()
*
* @see image_util_decode_create()
* @see image_util_decode_set_output_buffer()
* @see image_util_decode_run()
* @see image_util_decode_run_async()
* @see image_util_decode_destroy()
*/
int image_util_decode_set_input_path(image_util_decode_h handle, const char *path);

/**
* @brief Sets the input buffer from which to decode.
* @since_tizen 3.0
*
* @remarks One of image_util_decode_set_input_path() or image_util_decode_set_input_buffer() should be set.\n
*          If both are set then the latest input set, is considered.\n
*          Finds out image type by reading the header of the image provided in input buffer.
*
* @param[in] handle The handle to image util decoding
* @param[in] src_buffer The input image buffer
* @param[in] src_size The input image buffer size
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY Out of memory
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Format not supported
*
* @pre image_util_decode_create()
*
* @post image_util_decode_run() / image_util_decode_run_async()
* @post image_util_decode_destroy()
*
* @see image_util_decode_create()
* @see image_util_decode_set_output_buffer()
* @see image_util_decode_run()
* @see image_util_decode_run_async()
* @see image_util_decode_destroy()
*/
int image_util_decode_set_input_buffer(image_util_decode_h handle, const unsigned char *src_buffer, unsigned long long src_size);

/**
* @brief Sets the output buffer to which the decoded buffer will be written to.
* @since_tizen 3.0
*
* @remarks Either image_util_decode_set_input_path() or image_util_decode_set_input_buffer() should be set.\n
*          By default the decoded output buffer colorspace will be IMAGE_UTIL_COLORSPACE_RGBA8888.
*          Use image_util_decode_set_colorspace to change the colorspace.
*
* @param[in] handle The handle to image util decoding
* @param[in] dst_buffer The decoded output buffer
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
*
* @pre image_util_decode_create()
*
* @post image_util_decode_run() / image_util_decode_run_async()
* @post image_util_decode_destroy()
*
* @see image_util_decode_create()
* @see image_util_decode_set_input_path()
* @see image_util_decode_set_input_buffer()
* @see image_util_decode_run()
* @see image_util_decode_run_async()
* @see image_util_decode_destroy()
*/
int image_util_decode_set_output_buffer(image_util_decode_h handle, unsigned char **dst_buffer);

/**
* @brief Sets the decoded image colorspace format.
* @since_tizen 3.0
*
* @remarks The default colorspace is IMAGE_UTIL_COLORSPACE_RGBA8888.\n
*          Use image_util_foreach_supported_colorspace to get supported colorspaces for each image format.\n
*          Errors would be returned if not supported.
*
* @param[in] handle The handle to image util decoding
* @param[in] colorspace The decoded image colorspace
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Format not supported
*
* @pre image_util_decode_create()
*
* @post image_util_decode_run() / image_util_decode_run_async()
* @post image_util_decode_destroy()
*
* @see image_util_supported_colorspace_cb()
* @see image_util_foreach_supported_colorspace()
* @see image_util_decode_create()
* @see image_util_decode_set_input_path()
* @see image_util_decode_set_input_buffer()
* @see image_util_decode_set_output_buffer()
* @see image_util_decode_run()
* @see image_util_decode_run_async()
* @see image_util_decode_destroy()
*/
int image_util_decode_set_colorspace(image_util_encode_h handle, image_util_colorspace_e colorspace);

/**
* @brief Sets the downscale value at which JPEG image should be decoded.
* @since_tizen 3.0
*
* @remarks This is API is supported only for JPEG decoding.
*
* @param[in] handle The handle to image util decoding
* @param[in] down_scale The downscale at which image is to be decoded
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Format not supported
*
* @pre image_util_decode_create()
*
* @post image_util_decode_run() / image_util_decode_run_async()
* @post image_util_decode_destroy()
*
* @see image_util_decode_create()
* @see image_util_decode_set_input_path()
* @see image_util_decode_set_input_buffer()
* @see image_util_decode_set_output_buffer()
* @see image_util_decode_run()
* @see image_util_decode_run_async()
* @see image_util_decode_destroy()
*/
int image_util_decode_set_jpeg_downscale(image_util_encode_h handle, image_util_scale_e down_scale);

/**
* @brief Starts decoding of the image and fills the output buffer set using image_util_decode_set_output_buffer().
* @since_tizen 3.0
*
* @remarks The output will be stored in the pointer set using image_util_decode_set_output_buffer().\n
*		The function executes synchronously.\n
*		When any of the @pre functions are not called, IMAGE_UTIL_ERROR_INVALID_PARAMETER is returned.
*
* @param[in] handle The handle to image util decoding
* @param[out] width Width of the decoded image
* @param[out] height Height of the decoded image
* @param[out] size Size of the decoded image
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
*
* @pre image_util_decode_create()
* @pre image_util_decode_set_input_buffer() / image_util_decode_set_input_path().
* @pre image_util_decode_set_output_buffer()
*
* @post image_util_decode_destroy()
*
* @see image_util_decode_create()
* @see image_util_decode_set_input_path()
* @see image_util_decode_set_input_buffer()
* @see image_util_decode_set_output_buffer()
* @see image_util_decode_destroy()
*/
int image_util_decode_run(image_util_decode_h handle, unsigned long *width, unsigned long *height, unsigned long long *size);

/**
* @brief Starts decoding of the image and fills the output buffer set using image_util_decode_set_output_buffer().
* @since_tizen 3.0
*
* @remarks The output will be stored in the pointer set using image_util_decode_set_output_buffer().\n
*		The function executes asynchronously, which contains complete callback.\n
*		When any of the @pre functions are not called, IMAGE_UTIL_ERROR_INVALID_PARAMETER is returned.
*
* @param[in] handle The handle to image util decoding
* @param[in] callback The callback function to be invoked
* @param[in] user_data The user data to be passed to the callback function
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
*
* @pre image_util_decode_create()
* @pre image_util_decode_set_input_buffer() / image_util_decode_set_input_path().
* @pre image_util_decode_set_output_buffer()
*
* @post image_util_decode_destroy()
*
* @see image_util_decode_create()
* @see image_util_decode_set_input_path()
* @see image_util_decode_set_input_buffer()
* @see image_util_decode_set_output_buffer()
* @see image_util_decode_destroy()
*/
int image_util_decode_run_async(image_util_decode_h handle, image_util_decode_completed_cb callback, void *user_data);

/**
* @brief Destroys the image decoding handle.
* @since_tizen 3.0
*
* @remarks Any image handle created should be destroyed.
*
* @param[in] handle The handle to image util decoding
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
*
* @pre image_util_decode_create()
*
* @see image_util_decode_create()
*/
int image_util_decode_destroy(image_util_decode_h handle);

/**
* @brief Creates a handle to image util encoding.
* @since_tizen 3.0
*
* @details This function creates a handle to image util encoding.
*
* @remarks You must release the @a image util handle using image_util_encode_destroy().
*
* @param[in] image_type The type of output image for which to create encode handle.
* @param[out] handle A handle to image util encoding
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY Out of memory
*
* @see image_util_encode_destroy()
*
*/
int image_util_encode_create(image_util_type_e image_type, image_util_encode_h *handle);

/**
* @brief Sets the resolution of the encoded image.
* @since_tizen 3.0
*
* @remarks This should be called before calling image_util_encode_run().\n
*          While encoding animated gif image, image_util_encode_set_gif_frame_count() should be set\n
*          and image_util_encode_set_input_buffer() should be called after this.\n
*          Resolution should be set for each frame in the animated gif image.
*
* @param[in] handle The handle to image util encoding
* @param[in] width Width of the original image
* @param[in] height Height of the original image
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
*
* @pre image_util_encode_create()
*
* @post image_util_encode_run() / image_util_encode_run_async()
* @post image_util_encode_destroy()
*
* @see image_util_encode_create()
* @see image_util_encode_set_input_buffer()
* @see image_util_encode_set_output_path()
* @see image_util_encode_set_output_buffer()
* @see image_util_encode_run()
* @see image_util_encode_run_async()
* @see image_util_encode_destroy()
*/
int image_util_encode_set_resolution(image_util_encode_h handle, unsigned long width, unsigned long height);

/**
* @brief Sets the colorspace format for image encoding.
* @since_tizen 3.0
*
* @remarks The default colorspace is IMAGE_UTIL_COLORSPACE_RGBA8888.\n
*          Use image_util_foreach_supported_colorspace to get supported colorspaces for each image format.\n
*          Errors would be returned if not supported.
*
* @param[in] handle The handle to image util encoding
* @param[in] colorspace The encoded image colorspace
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Format not supported
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
*
* @pre image_util_encode_create()
*
* @post image_util_encode_run() / image_util_encode_run_async()
* @post image_util_encode_destroy()
*
* @see image_util_supported_colorspace_cb()
* @see image_util_foreach_supported_colorspace()
* @see image_util_encode_create()
* @see image_util_encode_set_resolution()
* @see image_util_encode_set_input_buffer()
* @see image_util_encode_set_output_path()
* @see image_util_encode_set_output_buffer()
* @see image_util_encode_run()
* @see image_util_encode_run_async()
* @see image_util_encode_destroy()
*/
int image_util_encode_set_colorspace(image_util_encode_h handle, image_util_colorspace_e colorspace);

/**
* @brief Sets the quality for image encoding.
* @since_tizen 3.0
*
* @remarks If application does not set this, then by default quality of 75 is set.\n
*             Quality is supported for JPEG format. IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT will be returned for other formats.
*
* @param[in] handle The handle to image util encoding
* @param[in] quality Encoding quality from 1~100
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Format not supported
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
*
* @pre image_util_encode_create()
*
* @post image_util_encode_run() / image_util_encode_run_async()
* @post image_util_encode_destroy()
*
* @see image_util_encode_create()
* @see image_util_encode_set_resolution()
* @see image_util_encode_set_input_buffer()
* @see image_util_encode_set_output_path()
* @see image_util_encode_set_output_buffer()
* @see image_util_encode_run()
* @see image_util_encode_run_async()
* @see image_util_encode_destroy()
*/
int image_util_encode_set_quality(image_util_encode_h handle, int quality);

/**
* @brief Sets the compression value of png image encoding(0~9).
* @since_tizen 3.0
*
* @remarks If application does not set this, then default compression of 6 is set.
*
* @param[in] handle The handle to image util encoding
* @param[in] compression The compression value valid from 0~9
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Format not supported
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
*
* @pre image_util_encode_create()
*
* @post image_util_encode_run() / image_util_encode_run_async()
* @post image_util_encode_destroy()
*
* @see image_util_encode_create()
* @see image_util_encode_set_resolution()
* @see image_util_encode_set_input_buffer()
* @see image_util_encode_set_output_path()
* @see image_util_encode_set_output_buffer()
* @see image_util_encode_run()
* @see image_util_encode_run_async()
* @see image_util_encode_destroy()
*/
int image_util_encode_set_png_compression(image_util_encode_h handle, image_util_png_compression_e compression);

/**
* @brief Sets the number of frames to be encoded to an animated gif image.
* @since_tizen 3.0
*
* @remarks This should be set only for animated gif image. If this is not set then only first frame will be encoded.\n
*          image_util_encode_set_resolution() ,image_util_encode_set_input_buffer() and\n
*          image_util_encode_set_gif_frame_delay_time() should be called after this.
*
* @param[in] handle The handle to image util encoding
* @param[in] frame_count Frame count
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Format not supported
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
*
* @pre image_util_encode_create()
*
* @post image_util_encode_set_resolution()
* @post image_util_encode_set_input_buffer()
* @post image_util_encode_set_output_path() / image_util_encode_set_output_buffer()
* @post image_util_encode_run() / image_util_encode_run_async()
* @post image_util_encode_destroy()
*
* @see image_util_encode_create()
* @see image_util_encode_set_resolution()
* @see image_util_encode_set_gif_frame_delay_time()
* @see image_util_encode_set_input_buffer()
* @see image_util_encode_set_output_path()
* @see image_util_encode_set_output_buffer()
* @see image_util_encode_run()
* @see image_util_encode_run_async()
* @see image_util_encode_destroy()
*/
int image_util_encode_set_gif_frame_count(image_util_encode_h handle, unsigned int frame_count);

/**
* @brief Sets the time delay between each frame in the encoded animated gif image.
* @since_tizen 3.0
*
* @remarks This will work only for animated gif image. If this is not set then there will be no delay between each frame.\n
*          This should be set for each frame in the animated gif image.\n
*          This can be set a different value for each frame, which results in different delay time between different frames.\n
*          If this is not set for all frames, then the delay value of the last set frame will be considered for the other frames.\n
*          image_util_encode_set_gif_frame_count() should be set before calling this.\n
*          image_util_encode_set_input_buffer() should be called after this.
*
* @param[in] handle The handle to image util encoding
* @param[in] delay_time Time delay between each frame in the encoded image, in 0.01sec units.
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Format not supported
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
*
* @pre image_util_encode_create()
* @pre image_util_encode_set_gif_frame_count()
*
* @post image_util_encode_set_input_buffer()
* @post image_util_encode_run() / image_util_encode_run_async()
* @post image_util_encode_destroy()
*
* @see image_util_encode_create()
* @see image_util_encode_set_resolution()
* @see image_util_encode_set_gif_frame_count()
* @see image_util_encode_set_input_buffer()
* @see image_util_encode_set_output_path()
* @see image_util_encode_set_output_buffer()
* @see image_util_encode_run()
* @see image_util_encode_run_async()
* @see image_util_encode_destroy()
*/
int image_util_encode_set_gif_frame_delay_time(image_util_encode_h handle, unsigned long long delay_time);

/**
* @brief Sets the input buffer from which to encode.
* @since_tizen 3.0
*
* @remarks Either image_util_encode_set_output_path() or image_util_encode_set_output_buffer() should be set.\n
*          By default the input buffer colorspace will be considered as IMAGE_UTIL_COLORSPACE_RGBA8888.\n
*          Use image_util_encode_set_colorspace to change the colorspace.\n
*          While encoding animated gif image, image_util_encode_set_gif_frame_count(),\n
*          image_util_encode_set_resolution() and image_util_encode_set_gif_frame_delay_time()\n
*          should be called before this.\n
*          Input buffer should be set for each frame in the animated gif image.
*
* @param[in] handle The handle to image util decoding
* @param[in] src_buffer The input image buffer
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
*
* @pre image_util_encode_create()
* @pre image_util_encode_set_resolution()
*
* @post image_util_encode_run() / image_util_encode_run_async()
* @post image_util_encode_destroy()
*
* @see image_util_encode_create()
* @see image_util_encode_set_resolution()
* @see image_util_encode_set_input_buffer()
* @see image_util_encode_set_output_path()
* @see image_util_encode_set_output_buffer()
* @see image_util_encode_run()
* @see image_util_encode_run_async()
* @see image_util_encode_destroy()
*/
int image_util_encode_set_input_buffer(image_util_encode_h handle, const unsigned char *src_buffer);

/**
* @brief Sets the output path to which to encoded buffer will be written to.
* @since_tizen 3.0
*
* @remarks One of image_util_encode_set_output_path() or image_util_encode_set_output_buffer() should be set.\n
*          If both are set then the latest output set is considered.\n
*          http://tizen.org/privilege/mediastorage is needed if input or output path are relevant to media storage.\n
*          http://tizen.org/privilege/externalstorage is needed if input or output path are relevant to external storage.
*
* @param[in] handle The handle to image util encoding
* @param[in] path The output file path
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_NO_SUCH_FILE No such file
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @pre image_util_encode_create()
*
* @post image_util_encode_run() / image_util_encode_run_async()
* @post image_util_encode_destroy()
*
* @see image_util_encode_create()
* @see image_util_encode_set_resolution()
* @see image_util_encode_set_input_buffer()
* @see image_util_encode_run()
* @see image_util_encode_run_async()
* @see image_util_encode_destroy()
*/
int image_util_encode_set_output_path(image_util_encode_h handle, const char *path);

/**
* @brief Sets the output buffer to which to encoded buffer will be written to.
* @since_tizen 3.0
*
* @remarks One of image_util_encode_set_output_path() or image_util_encode_set_output_buffer() should be set.\n
*          If both are set then the latest output set is considered.\n
*          Output buffer setting is not supported for bmp. IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT will be returned for bmp.
*
* @param[in] handle The handle to image util encoding
* @param[in] dst_buffer The output image buffer
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Format not supported
*
* @pre image_util_encode_create()
*
* @post image_util_encode_run() / image_util_encode_run_async()
* @post image_util_encode_destroy()
*
* @see image_util_encode_create()
* @see image_util_encode_set_resolution()
* @see image_util_encode_set_input_buffer()
* @see image_util_encode_run()
* @see image_util_encode_run_async()
* @see image_util_encode_destroy()
*/
int image_util_encode_set_output_buffer(image_util_encode_h handle, unsigned char **dst_buffer);

/**
* @brief Starts encoding of the image and fills the output buffer, set using image_util_encode_set_output_buffer() or image_util_encode_set_output_path().
* @since_tizen 3.0
*
* @remarks The output will be stored in the pointer set to image_util_encode_set_output_buffer() or image_util_encode_set_output_path().\n
*		The function executes synchronously.\n
*		When any of the @pre functions are not called, IMAGE_UTIL_ERROR_INVALID_PARAMETER is returned.
*
* @param[in] handle The handle to image util encoding
* @param[out] size Size of the encoded image
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
*
* @pre image_util_encode_create()
* @pre image_util_encode_set_resolution()
* @pre image_util_encode_set_input_buffer()
* @pre image_util_encode_set_output_buffer() / image_util_encode_set_output_path()
*
* @post image_util_encode_destroy()
*
* @see image_util_encode_create()
* @see image_util_encode_set_resolution()
* @see image_util_encode_set_input_buffer()
* @see image_util_encode_set_output_path()
* @see image_util_encode_set_output_buffer()
* @see image_util_encode_destroy()
*/
int image_util_encode_run(image_util_encode_h handle, unsigned long long *size);

/**
* @brief Starts encoding of the image and fills the output buffer, set using image_util_encode_set_output_buffer() or image_util_encode_set_output_path().
* @since_tizen 3.0
*
* @remarks The output will be stored in the pointer set to image_util_encode_set_output_buffer() or image_util_encode_set_output_path().\n
*		The function executes asynchronously, which contains complete callback.\n
*		When any of the @pre functions are not called, IMAGE_UTIL_ERROR_INVALID_PARAMETER is returned.
*
* @param[in] handle The handle to image util encoding
* @param[in] callback The callback function to be invoked
* @param[in] user_data The user data to be passed to the callback function
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
*
* @pre image_util_encode_create()
* @pre image_util_encode_set_resolution()
* @pre image_util_encode_set_input_buffer()
* @pre image_util_encode_set_output_buffer() / image_util_encode_set_output_path()
*
* @post image_util_encode_destroy()
*
* @see image_util_encode_create()
* @see image_util_encode_set_resolution()
* @see image_util_encode_set_input_buffer()
* @see image_util_encode_set_output_path()
* @see image_util_encode_set_output_buffer()
* @see image_util_encode_destroy()
*/
int image_util_encode_run_async(image_util_encode_h handle, image_util_encode_completed_cb callback, void *user_data);

/**
* @brief Destroys the image encoding handle.
* @since_tizen 3.0
*
* @remarks Any image handle created should be destroyed.
*
* @param[in] handle The handle to image util encoding
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
*
* @pre image_util_encode_create()
*
* @see image_util_encode_create()
*/
int image_util_encode_destroy(image_util_encode_h handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_MEDIA_IMAGE_UTIL_H__ */
