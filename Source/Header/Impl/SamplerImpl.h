#pragma once

#include "../Core/SourceCommon.h"

#include "../Core/DescriptorSet.h"
#include "../Core/VulkanMemoryManagement.h"

namespace vk2d {

namespace _internal {



class SamplerImpl {
public:
	SamplerImpl(
		vk2d::Sampler						*	sampler,
		vk2d::_internal::RendererImpl		*	renderer,
		const vk2d::SamplerCreateInfo		&	create_info );

	~SamplerImpl();

	VkSampler									GetVulkanSampler();
	VkBuffer									GetVulkanBufferForSamplerData();
	vk2d::Vector2u								GetBorderColorEnable();
	bool										IsAnyBorderColorEnabled();

	bool										IsGood();



	struct BufferData {
		alignas( 16 )	vk2d::Colorf			borderColor			= {};	// Border color
		alignas( 8 )	vk2d::Vector2u			borderColorEnable	= {};	// Border color enable
	};

private:
	vk2d::Sampler							*	sampler_parent		= {};
	vk2d::_internal::RendererImpl			*	renderer_parent		= {};
	VkDevice									vk_device			= {};

	VkSampler									sampler				= {};
	vk2d::_internal::CompleteBufferResource		sampler_data		= {};

	vk2d::Vector2u								border_color_enable	= {};

	bool										is_good				= {};
};



} // _internal

} // vk2d
