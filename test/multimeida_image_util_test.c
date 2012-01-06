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

#include <Elementary.h>
#include <glib.h>
#include <Ecore.h>
#include <Ecore_X.h>
#include <stdbool.h>
#include <stdio.h>
#include <image_util.h>

struct raw_image{
	unsigned char *buffer;
	int size;
	int w;
	int h;
};

Evas_Object* img;

char *colorspace_str_tbl[] = 	{
	"IMAGE_UTIL_COLORSPACE_YUV420", 			/**< YUV420 */
	"IMAGE_UTIL_COLORSPACE_YUV422", 			/**< YUV422 */
	"IMAGE_UTIL_COLORSPACE_I420", 				/**< I420 */
	"IMAGE_UTIL_COLORSPACE_NV12", 				/**< NV12 */

	"IMAGE_UTIL_COLORSPACE_UYVY", 				/**< UYVY */
	"IMAGE_UTIL_COLORSPACE_YUYV", 				/**< YUYV */

	"IMAGE_UTIL_COLORSPACE_RGB565", 			/**< RGB565, high-byte is Blue */
	"IMAGE_UTIL_COLORSPACE_RGB888", 			/**< RGB888, high-byte is Blue */
	"IMAGE_UTIL_COLORSPACE_ARGB8888", 	/**< ARGB8888, high-byte is Blue */
	
	"IMAGE_UTIL_COLORSPACE_BGRA8888", 	/**< BGRA8888, high-byte is Alpha */
	"IMAGE_UTIL_COLORSPACE_RGBA8888", 	/**< RGBA8888, high-byte is Alpha */
	"IMAGE_UTIL_COLORSPACE_BGRX8888" 		/**< BGRX8888, high-byte is X */
};



bool decode_jpeg_cb( image_util_colorspace_e colorspace , void * user_data){
		int ret;
		unsigned char * buffer = NULL;
		int w, h;
		int size;

		const char *path = (const char*)user_data;
		
		printf("jpeg(%s) decode to [%s]\n", path, colorspace_str_tbl[colorspace] );
		

		w = h = 0;
		ret = image_util_decode_jpeg( path ,colorspace,  &buffer, &w, &h , &size );
		printf("image (%dx%d) - %dbyte , ret = %d(%x)\n", w,h, size, ret, ret);
		if( ret != 0 )
			return true;
			
		
		evas_object_image_size_set(img, w, h);
		unsigned char *bgra8888_image = evas_object_image_data_get(img, true);	
		ret = image_util_convert_colorspace(bgra8888_image , IMAGE_UTIL_COLORSPACE_BGRA8888, buffer , w, h, colorspace );
		printf("convert %d\n", ret);
		free(buffer);
		
		evas_object_image_data_set(img, bgra8888_image);
		
		evas_object_hide(img);		
		evas_object_resize(img, w,h);	
		evas_object_image_fill_set(img, 0, 0,  w, h);	
		evas_object_image_data_update_add(img,0,0, w, h);
		evas_object_show(img);	

		
		sleep(1);
		

		return true;
	
}
void jpeg_decode_test( const char *path ){

	printf("****************\n");
	printf("JPEG DECODE TEST - start\n");
	printf("****************\n");	
	image_util_foreach_supported_jpeg_colorspace( decode_jpeg_cb , (void*)path);
	printf("****************\n");
	printf("JPEG DECODE TEST - end\n");
	printf("****************\n");	
		
}

bool encode_jpeg_cb( image_util_colorspace_e colorspace , void * user_data){
	struct raw_image *img_data = (struct raw_image *)user_data;
	unsigned char *buffer;
	unsigned int size;
	int ret;
	char filename[255];

	printf("jpeg encode from [%s]\n", colorspace_str_tbl[colorspace] );

	
	image_util_calculate_buffer_size(img_data->w,img_data->h, colorspace, &size);
	buffer = malloc(size);
	ret = image_util_convert_colorspace(buffer, colorspace , img_data->buffer , img_data->w, img_data->h , IMAGE_UTIL_COLORSPACE_BGRA8888);

	sprintf(filename, "test_%s.jpg", colorspace_str_tbl[colorspace]);	
	ret = image_util_encode_jpeg(buffer, img_data->w , img_data->h , colorspace, 100, filename);
	printf("%s encode jpg %d\n",  colorspace_str_tbl[colorspace] , ret);	

	return true;
	
}

void jpeg_encode_test( const char *path ){
	printf("****************\n");
	printf("JPEG ENCODE TEST - start\n");
	printf("****************\n");	
	
	int w,h;
	struct raw_image img_data;
	evas_object_image_file_set(img, path , NULL);
	unsigned char* origin_buffer = evas_object_image_data_get(img, true);	
	evas_object_image_size_get(img, &w, &h);
	evas_object_resize(img, w,h);	
	img_data.buffer = origin_buffer;
	img_data.h = h;
	img_data.w = w;	
	image_util_foreach_supported_jpeg_colorspace( encode_jpeg_cb , (void*)&img_data);
	evas_object_image_data_set(img, origin_buffer);	

	printf("****************\n");
	printf("JPEG ENCODE TEST - end\n");
	printf("****************\n");	
	
}

void colorspace_convert_test( const char *path ){
	printf("****************\n");
	printf("colorspace_convert_test TEST - start\n");
	printf("****************\n");	

	
	int w,h;
	evas_object_image_file_set(img, path, NULL);
	unsigned char* origin_buffer = evas_object_image_data_get(img, true);	
	evas_object_image_size_get(img, &w, &h);
	evas_object_resize(img, w,h);	

	int i;
	int j;
	int ret;

	for( i = IMAGE_UTIL_COLORSPACE_YUV420 ; i <= IMAGE_UTIL_COLORSPACE_BGRX8888 ; i++ ){
		unsigned char *buffer;
		unsigned int size;

		if( i == IMAGE_UTIL_COLORSPACE_BGRA8888 )
			continue;
		
		image_util_calculate_buffer_size(w, h, i,&size);
		buffer = malloc(size);
		ret = image_util_convert_colorspace(buffer, i , origin_buffer, w, h, IMAGE_UTIL_COLORSPACE_BGRA8888);
		printf("[%d] convert %s -> %s\n", ret , colorspace_str_tbl[IMAGE_UTIL_COLORSPACE_BGRA8888], colorspace_str_tbl[i]);
		
		for( j = IMAGE_UTIL_COLORSPACE_YUV420 ; j <= IMAGE_UTIL_COLORSPACE_BGRX8888 ; j++){
			if( i == j )
				continue;
			unsigned char *buffer2;
			unsigned int size2;
			image_util_calculate_buffer_size(w, h, j,&size2);
			buffer2 = malloc(size2);
			ret = image_util_convert_colorspace(buffer2, j , buffer, w, h, i);
			printf("[%d] convert %s -> %s\n", ret , colorspace_str_tbl[i], colorspace_str_tbl[j]);
			free(buffer2);
		}
		
		free(buffer);		
			
	}



	evas_object_image_data_set(img, origin_buffer);	

	
	printf("****************\n");
	printf("colorspace_convert_test - end\n");
	printf("****************\n");	

}



void transform_test( const char *path ){
	printf("****************\n");
	printf("transform_test TEST - start\n");
	printf("****************\n");	

	
	int w,h;
	evas_object_image_file_set(img, path, NULL);
	unsigned char* origin_buffer = evas_object_image_data_get(img, false);	
	evas_object_image_size_get(img, &w, &h);
	evas_object_resize(img, w,h);	

	int ret;


	unsigned char *buffer;
	unsigned int size;
	int dw, dh;

	dh = 800;
	dw = 200;

	image_util_calculate_buffer_size(dw, dh, IMAGE_UTIL_COLORSPACE_BGRA8888 , &size);
	buffer = malloc(size);

	printf("%d, %d\n", dw, dh);
	ret = image_util_transform(buffer , &dw, &dh , IMAGE_UTIL_ROTATION_NONE , origin_buffer, w, h, IMAGE_UTIL_COLORSPACE_BGRA8888);
	printf("[%d] image_util_transform\n", ret);

	printf("%d, %d\n", dw, dh);

	evas_object_hide(img);		
	evas_object_resize(img, dw,dh);	
	evas_object_image_size_set(img, dw, dh);
	evas_object_image_data_set(img, buffer);	
	
	evas_object_image_fill_set(img, 0, 0,  dw, dh);	
	evas_object_image_data_update_add(img,0,0, dw, dh);
	evas_object_show(img);	

	
	printf("****************\n");
	printf("transform_test TEST - end\n");
	printf("****************\n");	
	
}

void* test_main(void *arg){
	//jpeg_decode_test("sample.jpg");
	//jpeg_encode_test("sample.jpg");
	//colorspace_convert_test("sample.jpg");
	transform_test("sample.jpg");

	
	return NULL;
}



 Eina_Bool _window_update(void *data){
 	return 1;
}


int main(int argc, char ** argv)
{
	int w,h;
	elm_init(argc, argv);
	Evas_Object* win;


	win = elm_win_add(NULL, "VIDEO OVERLAY", ELM_WIN_BASIC);
	elm_win_title_set(win, "video overlay window");
	elm_win_borderless_set(win, EINA_TRUE);
	ecore_x_window_size_get(ecore_x_window_root_first_get(),	&w, &h);
	evas_object_resize(win, w, h);
	elm_win_indicator_state_set(win, EINA_TRUE);

	evas_object_show(win);	

	img = evas_object_image_add(evas_object_evas_get(win));
	printf("%d,%d\n", w,h);


	
	evas_object_resize(img, w,h);	
	evas_object_image_fill_set(img, 0, 0, w, h);	
	evas_object_show(img);	


	ecore_timer_add(0.1, _window_update, NULL);
	

	
	pthread_t gloop_thread;
	pthread_create(&gloop_thread, NULL, test_main,  NULL);

	elm_run();
	elm_shutdown();
	

	return 0;
}


