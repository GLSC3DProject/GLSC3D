#include "glsc3d_3_private.h"

#ifdef G_USE_METAL
// assigned per frame
CA::MetalDrawable *g_drawable;
MTL::CommandBuffer *g_command_buffer;
MTL::RenderCommandEncoder *g_command_encoder;

// assigned by g_scr_color
MTL::ClearColor g_clear_color;

void g_prepare_capture()
{
	G_AUTO_RELEASE blit_encoder = g_command_buffer->blitCommandEncoder();

	NS::UInteger bytesPerRow = 4 * glsc3D_width;

	blit_encoder->copyFromTexture(
		g_drawable->texture(), 0, 0, MTL::Origin(0, 0, 0), MTL::Size(glsc3D_width, glsc3D_height, 1),
		g_capture_buffer, 0, bytesPerRow, bytesPerRow * glsc3D_height);

	blit_encoder->endEncoding();
}
#endif

void g_scr_color(float r, float g, float b)
{
#ifdef G_USE_METAL
	g_clear_color = MTL::ClearColor(r, g, b, 1);
#else
	glClearColor(r, g, b, 1);
#endif
}

void g_cls()
{
#ifdef G_USE_METAL
	g_drawable = g_swapchain->nextDrawable();

	G_AUTO_RELEASE<MTL::RenderPassDescriptor> pass_clear;
	auto ca = pass_clear->colorAttachments()->object(0);
	ca->setClearColor(g_clear_color);
	ca->setLoadAction(MTL::LoadActionClear);
	ca->setTexture(g_drawable->texture());
	pass_clear->depthAttachment()->setTexture(g_depth_texture);

	g_command_buffer = g_command_queue->commandBufferWithUnretainedReferences();
	g_command_encoder = g_command_buffer->renderCommandEncoder(pass_clear);
	g_command_encoder->setDepthStencilState(g_depth_state);

	if (g_current_scale_ptr) g_current_scale_ptr->select();
	g_update_enabled_lights();
#else
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif

	g_text_buffer_clear();

#ifdef G_USE_VERTEX_BUFFERS
	g_current_2d_depth = 0;
#endif
}

void g_finish()
{
	g_triangle_buffer_flush();
	g_triangle_buffer_draw();
	g_vertex_buffer_flush();
	g_text_buffer_present();

#ifdef G_USE_METAL
	g_command_encoder->endEncoding();
	g_command_encoder->release();
	g_command_encoder = nullptr;

	if (g_capture_is_initialized) {
		g_prepare_capture();
		g_command_buffer->commit();
		g_command_buffer->waitUntilCompleted();
		g_drawable->present();
	}
	else {
		g_command_buffer->presentDrawable(g_drawable);
		g_command_buffer->commit();
	}

	g_command_buffer->release();
	g_drawable->release();
#else
	g_swap_buffers();
#endif

	g_frame_finished();
}
