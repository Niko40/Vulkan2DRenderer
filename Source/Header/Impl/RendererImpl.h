#pragma once

#include "../Header/Core/SourceCommon.h"
#include "../../../Include/Interface/Renderer.h"

#include "../../Header/Core/VulkanMemoryManagement.h"
#include "../Core/QueueResolver.h"

#include "../Impl/WindowImpl.h"
#include "../Core/DescriptorSet.h"

#include <list>
#include <vector>
#include <array>
#include <memory>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>




namespace vk2d {

class Window;
class Monitor;
class Renderer;
class ResourceManager;

namespace _internal {

class ThreadPool;
class DescriptorSetLayout;
class MonitorImpl;



struct WindowCoordinateScaling {
	alignas( 8 )	vk2d::Vector2f		multiplier		= {};
	alignas( 8 )	vk2d::Vector2f		offset			= {};
};



class RendererImpl {
	friend class Renderer;

public:
	RendererImpl(
		const RendererCreateInfo						&	renderer_create_info );

	~RendererImpl();

	std::vector<vk2d::Monitor*>								GetMonitors();

	vk2d::Monitor										*	GetPrimaryMonitor();

	void													SetMonitorUpdateCallback(
		vk2d::MonitorUpdateCallbackFun						monitor_update_callback_funtion );

	vk2d::GamepadEventCallbackFun							GetGamepadEventCallback();

	void													SetGamepadEventCallback(
		vk2d::GamepadEventCallbackFun						gamepad_event_callback_function );

	bool													IsGamepadPresent(
		vk2d::Gamepad										gamepad );

	std::string												GetGamepadName(
		vk2d::Gamepad										gamepad );

	vk2d::GamepadState										QueryGamepadState(
		vk2d::Gamepad										gamepad );

	// TODO: gamepad mapping
	void													SetGamepadMapping();

	Window												*	CreateOutputWindow(
		WindowCreateInfo								&	window_create_info );
	void													CloseOutputWindow(
		Window											*	window );

	vk2d::_internal::DescriptorAutoPool					*	GetDescriptorPool();

	vk2d::Sampler										*	CreateSampler(
		const vk2d::SamplerCreateInfo					&	sampler_create_info );

	void													DestroySampler(
		vk2d::Sampler									*	sampler );

	vk2d::Multisamples										GetMaximumSupportedMultisampling();
	vk2d::Multisamples										GetAllSupportedMultisampling();

	PFN_VK2D_ReportFunction									GetReportFunction() const;

	ThreadPool											*	GetThreadPool() const;
	const std::vector<uint32_t>							&	GetLoaderThreads() const;
	const std::vector<uint32_t>							&	GetGeneralThreads() const;
	ResourceManager										*	GetResourceManager() const;

	VkInstance												GetVulkanInstance() const;
	VkPhysicalDevice										GetVulkanPhysicalDevice() const;
	VkDevice												GetVulkanDevice() const;

	ResolvedQueue											GetPrimaryRenderQueue() const;
	ResolvedQueue											GetSecondaryRenderQueue() const;
	ResolvedQueue											GetPrimaryComputeQueue() const;
	ResolvedQueue											GetPrimaryTransferQueue() const;

	const VkPhysicalDeviceProperties					&	GetPhysicalDeviceProperties() const;
	const VkPhysicalDeviceMemoryProperties				&	GetPhysicalDeviceMemoryProperties() const;
	const VkPhysicalDeviceFeatures						&	GetPhysicalDeviceFeatures() const;

	VkShaderModule											GetVertexShaderModule() const;
	VkShaderModule											GetFragmentShaderModule() const;

	VkPipelineCache											GetPipelineCache() const;
	VkPipelineLayout										GetPipelineLayout() const;
	const DescriptorSetLayout							&	GetSamplerDescriptorSetLayout() const;
	const DescriptorSetLayout							&	GetTextureDescriptorSetLayout() const;

	VkDescriptorSet											GetDefaultTextureDescriptorSet() const;
	vk2d::Sampler										*	GetDefaultSampler() const;

	DeviceMemoryPool									*	GetDeviceMemoryPool() const;

	bool													IsGood() const;


	bool													CreateInstance();
	bool													PickPhysicalDevice();
	bool													CreateDeviceAndQueues();
	bool													CreateDescriptorPool();
	bool													CreateDefaultSampler();
	bool													CreatePipelineCache();
	bool													CreateShaderModules();
	bool													CreateDescriptorSetLayouts();
	bool													CreatePipelineLayout();
	bool													CreateDeviceMemoryPool();
	bool													CreateThreadPool();
	bool													CreateResourceManager();
	bool													CreateDefaultTexture();

	void													DestroyInstance();
	void													DestroyDevice();
	void													DestroyDescriptorPool();
	void													DestroyDefaultSampler();
	void													DestroyPipelineCache();
	void													DestroyShaderModules();
	void													DestroyDescriptorSetLayouts();
	void													DestroyPipelineLayout();
	void													DestroyDeviceMemoryPool();
	void													DestroyThreadPool();
	void													DestroyResourceManager();
	void													DestroyDefaultTexture();

	std::vector<VkPhysicalDevice>							EnumeratePhysicalDevices();
	VkPhysicalDevice										PickBestPhysicalDevice();

	vk2d::MonitorUpdateCallbackFun							monitor_update_callback				= nullptr;

private:
	static uint64_t											renderer_count;		// used to keep track of Renderer instances

	vk2d::RendererCreateInfo								create_info_copy					= {};

	std::vector<const char*>								instance_layers						= {};
	std::vector<const char*>								instance_extensions					= {};
	std::vector<const char*>								device_extensions					= {};

	PFN_VK2D_ReportFunction									report_function						= {};

	std::unique_ptr<vk2d::ResourceManager>					resource_manager					= {};
	std::unique_ptr<vk2d::_internal::ThreadPool>			thread_pool							= {};
	std::vector<uint32_t>									loader_threads						= {};
	std::vector<uint32_t>									general_threads						= {};

	VkDebugUtilsMessengerEXT								debug_utils_messenger				= {};

	VkInstance												instance							= {};
	VkPhysicalDevice										physical_device						= {};
	VkDevice												device								= {};

	VkPhysicalDeviceProperties								physical_device_properties			= {};
	VkPhysicalDeviceMemoryProperties						physical_device_memory_properties	= {};
	VkPhysicalDeviceFeatures								physical_device_features			= {};

	VkShaderModule											vertex_shader_module				= {};
	VkShaderModule											fragment_shader_module				= {};
	VkPipelineCache											pipeline_cache						= {};
	VkPipelineLayout										pipeline_layout						= {};

	std::unique_ptr<DescriptorSetLayout>					sampler_descriptor_set_layout		= {};
	std::unique_ptr<DescriptorSetLayout>					sampler_data_descriptor_set_layout	= {};
	std::unique_ptr<DescriptorSetLayout>					texture_descriptor_set_layout		= {};

	ResolvedQueue											primary_render_queue				= {};
	ResolvedQueue											secondary_render_queue				= {};
	ResolvedQueue											primary_compute_queue				= {};
	ResolvedQueue											primary_transfer_queue				= {};

	std::unique_ptr<DeviceMemoryPool>						device_memory_pool					= {};

	std::unique_ptr<vk2d::_internal::DescriptorAutoPool>	descriptor_pool						= {};

	std::unique_ptr<vk2d::Sampler>							default_sampler						= {};

	vk2d::_internal::CompleteImageResource					default_texture						= {};
	vk2d::_internal::PoolDescriptorSet						default_texture_descriptor_set		= {};

	std::vector<std::unique_ptr<Window>>					windows								= {};
	std::vector<std::unique_ptr<vk2d::Sampler>>				samplers							= {};

	vk2d::GamepadEventCallbackFun							joystick_event_callback				= {};

	bool													is_good								= {};
};



} // _internal



} // vk2d
