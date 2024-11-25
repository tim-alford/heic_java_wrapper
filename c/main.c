// vim: ts=2
#include "heif_env.h"
static HeifEnv e;
static const char * HEIF_LIB = "/usr/local/lib/libheif.so";
static const char * INPUT = "/home/talford/IMG_8123.HEIC";
int main(const char ** argv, int argc){
	heif_env_init(HEIF_LIB, &e);
	const char * version = (*e.heif_get_version)();
	(*e.heif_init)();
	const struct heif_plugin_info* info;
	struct heif_error error = (*e.heif_load_plugin)("/usr/local/lib/heif/libheif-libde265.so", &info);
	if(error.code != 0){
		printf("PLUGIN = %s\n", error.message);
		return 1;
	}
	struct heif_context* ctx = (*e.heif_context_alloc)();
	error = (*e.heif_context_read_from_file)(ctx, INPUT, NULL);
	if(error.code != 0){
		printf("%s\n", error.message);
		return 1;
	}
	struct heif_image_handle* image;
	error = (*e.heif_context_get_primary_image_handle)(ctx, &image);
	if(error.code != 0){
		printf("%s\n", error.message);
		return 1;
	}
	struct heif_image * i;
	error = (*e.heif_decode_image)(image, &i, heif_colorspace_RGB, heif_chroma_interleaved_RGBA, NULL);
	if(error.code != 0){
		printf("%s\n", error.message);
		return 1;
	}
	int stride;
	const uint8_t* bytes = (*e.heif_image_get_plane_readonly)(i, heif_channel_interleaved, &stride);
	int width = (*e.heif_image_get_width)(i, heif_channel_interleaved);
	int height = (*e.heif_image_get_height)(i, heif_channel_interleaved);
	int bpp = (*e.heif_image_get_bits_per_pixel_range)(i, heif_channel_interleaved);
	if(bytes == NULL){
		printf("Failed to read bytes from image\n");
		return 1;
	}
	printf("W = %d, H = %d\n", width, height);
	dlclose(e.handle);
	return EXIT_SUCCESS;
}
