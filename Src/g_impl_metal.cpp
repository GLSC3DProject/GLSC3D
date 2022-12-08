#include "glsc3d_3_private.h"

#ifdef G_USE_METAL
MTL::CommandQueue *g_command_queue;
MTL::DepthStencilState *g_depth_state;
MTL::Texture *g_depth_texture;

G_PROGRAM_TYPE g_constant_program, g_lighting_program;
G_PROGRAM_TYPE g_line_program;
G_PROGRAM_TYPE g_marker_programs[G_NUM_MARKER_SIZE_TYPES][G_NUM_MARKER_TYPES];
G_PROGRAM_TYPE g_text_program;

void g_general_init()
{
	g_command_queue = g_device->newCommandQueue();

	G_AUTO_RELEASE<MTL::DepthStencilDescriptor> depth_desc;
	depth_desc->setDepthCompareFunction(MTL::CompareFunctionLessEqual);
	depth_desc->setDepthWriteEnabled(true);
	g_depth_state = g_device->newDepthStencilState(depth_desc);
}

void g_update_depth_buffer_size()
{
	if (g_depth_texture) g_depth_texture->release();

	G_AUTO_RELEASE<MTL::TextureDescriptor> depth_tex_desc;
	depth_tex_desc->setPixelFormat(MTL::PixelFormatDepth32Float);
	depth_tex_desc->setWidth(glsc3D_width);
	depth_tex_desc->setHeight(glsc3D_height);
	depth_tex_desc->setStorageMode(MTL::StorageModePrivate);
	depth_tex_desc->setUsage(MTL::TextureUsageRenderTarget);
	g_depth_texture = g_device->newTexture(depth_tex_desc);
}

G_PROGRAM_TYPE g_make_pipeline(MTL::RenderPipelineDescriptor *desc, MTL::Function *vs, MTL::Function *fs)
{
	desc->setVertexFunction(vs);
	desc->setFragmentFunction(fs);

	NS::Error *err;
	if (auto state = g_device->newRenderPipelineState(desc, &err)) return state;

	std::cerr << "Failed to create pipeline:" << std::endl;
	std::cerr << err->localizedDescription()->utf8String() << std::endl;
	g_quit();
}

void g_shader_program_init()
{
	G_AUTO_RELEASE<MTL::RenderPipelineDescriptor> desc;

	auto ca = desc->colorAttachments()->object(0);
	ca->setPixelFormat(g_swapchain->pixelFormat());

	ca->setBlendingEnabled(true);
	ca->setRgbBlendOperation(MTL::BlendOperationAdd);
	ca->setAlphaBlendOperation(MTL::BlendOperationAdd);

	ca->setSourceRGBBlendFactor(MTL::BlendFactorSourceAlpha);
	ca->setDestinationRGBBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);

	ca->setSourceAlphaBlendFactor(MTL::BlendFactorSourceAlpha);
	ca->setDestinationAlphaBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);

	desc->setDepthAttachmentPixelFormat(MTL::PixelFormatDepth32Float);

	G_AUTO_RELEASE library = g_device->newLibrary(
		NS::MakeConstantString("../Shaders/shaders.metallib"), nullptr);

	G_AUTO_RELEASE constant_vs = library->newFunction(NS::MakeConstantString("constant_vs"));
	G_AUTO_RELEASE constant_fs = library->newFunction(NS::MakeConstantString("constant_fs"));

	G_AUTO_RELEASE lighting_vs = library->newFunction(NS::MakeConstantString("lighting_vs"));
	G_AUTO_RELEASE lighting_fs = library->newFunction(NS::MakeConstantString("lighting_fs"));

	G_AUTO_RELEASE line_vs = library->newFunction(NS::MakeConstantString("line_vs"));
	G_AUTO_RELEASE line_fs = library->newFunction(NS::MakeConstantString("line_fs"));

	G_AUTO_RELEASE text_vs = library->newFunction(NS::MakeConstantString("text_vs"));
	G_AUTO_RELEASE text_fs = library->newFunction(NS::MakeConstantString("text_fs"));

	G_AUTO_RELEASE<MTL::Function> marker_vert_shaders[] = {
		library->newFunction(NS::MakeConstantString("marker_standard_vs")),
		library->newFunction(NS::MakeConstantString("marker_virtual_vs")),
	};

	G_AUTO_RELEASE<MTL::Function> marker_frag_shaders[] = {
		library->newFunction(NS::MakeConstantString("marker_square_fs")),
		library->newFunction(NS::MakeConstantString("marker_circle_fs")),
		library->newFunction(NS::MakeConstantString("marker_sphere_fs")),
	};

	g_constant_program = g_make_pipeline(desc, constant_vs, constant_fs);
	g_lighting_program = g_make_pipeline(desc, lighting_vs, lighting_fs);
	g_line_program = g_make_pipeline(desc, line_vs, line_fs);
	g_text_program = g_make_pipeline(desc, text_vs, text_fs);

	for (int i = 0; i < G_NUM_MARKER_SIZE_TYPES; i++) {
		for (int j = 0; j < G_NUM_MARKER_TYPES; j++) {
			g_marker_programs[i][j] = g_make_pipeline(desc, marker_vert_shaders[i], marker_frag_shaders[j]);
		}
	}
}

void g_set_viewport(int x, int y, int width, int height)
{
	g_command_encoder->setViewport({(double)x, (double)y, (double)width, (double)height, 0.0, 1.0});
}

void g_use_program(G_PROGRAM_TYPE program)
{
	g_command_encoder->setRenderPipelineState(program);
}

#endif
