
ffi = require("ffi")
ffi.cdef[[

typedef void *cc_atlas_t;
typedef void *cc_texture_t;
typedef void *cc_block_t;
typedef void *cc_chunk_t;
typedef void *cc_world_t;

double noise1(double arg);
float noise2(float vec[2]);
float noise3(float vec[3]);

cc_atlas_t *cc_load_atlas(const char *path);
void cc_atlas_upload(cc_atlas_t *atlas);
cc_texture_t *cc_atlas_texture(cc_atlas_t *atlas, int left, int top, int right, int bottom);

void cc_texture_draw(cc_texture_t *image, float x, float y);
void cc_draw_rect(float x, float y, float width, float height);

void cc_chunk_set_block(cc_chunk_t *chunk, char block, int x, int y, int z);

void cc_world_add_block(cc_world_t *world, cc_block_t *block);
cc_chunk_t *cc_world_add_chunk(cc_world_t *world);

cc_block_t *cc_new_block();
void cc_block_set_name(cc_block_t *block, const char *name);
void cc_block_set_texture(cc_block_t *block, int side, cc_texture_t *texture);
void cc_block_set_color(cc_block_t *block, int side, float r, float g, float b);


]]

noise1 = ffi.C.noise1

function noise2(vec)
	return ffi.C.noise2( ffi.new("float[2]", vec) )
end

cc = {}
cc.loadAtlas       = ffi.C.cc_load_atlas
cc.atlasTexture    = ffi.C.cc_atlas_texture
cc.worldAddChunk   = ffi.C.cc_world_add_chunk
cc.worldAddBlock   = ffi.C.cc_world_add_block
cc.chunkSetBlock   = ffi.C.cc_chunk_set_block
cc.newBlock        = ffi.C.cc_new_block
cc.blockSetName    = ffi.C.cc_block_set_name
cc.blockSetTexture = ffi.C.cc_block_set_texture
cc.blockSetColor   = ffi.C.cc_block_set_color
