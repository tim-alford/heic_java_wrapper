// vim: ts=2
#include "heif_env.h"

int heif_env_init(const char * library, HeifEnv* heif){
	// load library
	void * handle = dlopen(library, RTLD_LAZY);
	if(handle == NULL){
		printf("Failed to load library, handle was NULL\n");
		return EXIT_FAILURE;
	}
	// load functions
	void * tmp = dlsym(handle, "heif_get_version");
	void * tmp2 = dlsym(handle, "heif_init");
	void * tmp3 = dlsym(handle, "heif_context_alloc");
	void * tmp4 = dlsym(handle, "heif_context_read_from_file");
	void * tmp5 = dlsym(handle, "heif_context_get_primary_image_handle");
	void * tmp6 = dlsym(handle, "heif_decode_image");
	void * tmp7 = dlsym(handle, "heif_load_plugin");
	void * tmp8 = dlsym(handle, "heif_image_get_plane_readonly");
	void * tmp9 = dlsym(handle, "heif_image_get_width");
	void * tmp10 = dlsym(handle, "heif_image_get_height");
	void * tmp11 = dlsym(handle, "heif_image_get_bits_per_pixel_range");
	// make sure that everything exists
	if(tmp == NULL || tmp2 == NULL || tmp3 == NULL || 
			tmp4 == NULL || tmp5 == NULL || tmp6 == NULL || 
			tmp7 == NULL || tmp8 == NULL || tmp9 == NULL || 
			tmp10 == NULL){
		printf("Failed to find one more many functions, please check export table.");
		return EXIT_FAILURE;
	}
	// assign to members of our env struct
	*(void **)(&heif->heif_get_version) = tmp;
	*(void **)(&heif->heif_init) = tmp2;
	*(void **)(&heif->heif_context_alloc) = tmp3;
	*(void **)(&heif->heif_context_read_from_file) = tmp4;
	*(void **)(&heif->heif_context_get_primary_image_handle) = tmp5;
	*(void **)(&heif->heif_decode_image) = tmp6;
	*(void **)(&heif->heif_load_plugin) = tmp7;
	*(void **)(&heif->heif_image_get_plane_readonly) = tmp8;
	*(void **)(&heif->heif_image_get_width) = tmp9;
	*(void **)(&heif->heif_image_get_height) = tmp10;
	*(void **)(&heif->heif_image_get_bits_per_pixel_range) = tmp11;
	// save handle
	heif->handle = handle;
	return EXIT_SUCCESS;
}

void heif_env_destroy(HeifEnv* heif){
	dlclose(heif->handle);
}
