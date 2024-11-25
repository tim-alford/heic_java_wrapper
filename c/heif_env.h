// vim: ts=2
#include <heif.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

typedef struct {
				const char * (*heif_get_version)(void);
				void (*heif_init)(void);
				struct heif_error (*heif_load_plugin)(const char *, const struct heif_plugin_info **);
				struct heif_context* (*heif_context_alloc)(void);
				struct heif_error (*heif_context_read_from_file)(struct heif_context *, const char *, const struct heif_reading_options *);
				struct heif_error (*heif_context_get_primary_image_handle)(struct heif_context *, struct heif_image_handle **);
				struct heif_error (*heif_decode_image)(struct heif_image_handle *, 
						struct heif_image **, enum heif_colorspace, 
						enum heif_chroma, const struct heif_decoding_options *);
				const uint8_t* (*heif_image_get_plane_readonly)(const struct heif_image*, enum heif_channel, int *);
				int (*heif_image_get_width)(const struct heif_image *, enum heif_channel);
				int (*heif_image_get_height)(const struct heif_image *, enum heif_channel);
				int (*heif_image_get_bits_per_pixel_range)(const struct heif_image*, enum heif_channel);
				void * handle;
} HeifEnv;

int heif_env_init(const char * library, HeifEnv* heif);

void heif_env_destroy(HeifEnv* heif);
