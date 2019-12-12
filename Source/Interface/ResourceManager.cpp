
#include "../Header/Core/SourceCommon.h"

#include "../../Include/Interface/ResourceManager.h"
#include "../Header/Impl/ResourceManagerImpl.h"

#include <memory>



namespace vk2d {



VK2D_API ResourceManager::ResourceManager(
	_internal::RendererImpl			*	parent_renderer
)
{
	impl = std::make_unique<_internal::ResourceManagerImpl>( parent_renderer );
	if( !impl )	return;
	if( !impl->IsGood() ) {
		impl	= nullptr;
		return;
	}
	is_good		= true;
}

VK2D_API ResourceManager::~ResourceManager()
{
	
}

VK2D_API TextureResource *VK2D_APIENTRY ResourceManager::CreateTextureResource(
	uint32_t							extent_x,
	uint32_t							extent_y,
	const std::vector<vk2d::Texel>	&	texels
)
{
	if( impl ) return impl->CreateTextureResource(
		extent_x,
		extent_y,
		texels
	);
	return nullptr;
}

VK2D_API TextureResource * VK2D_APIENTRY ResourceManager::LoadTextureResource(
	std::filesystem::path		file_path
)
{
	if( impl ) return impl->LoadTextureResource( file_path );
	return nullptr;
}

VK2D_API void VK2D_APIENTRY ResourceManager::DestroyResource(
	Resource		*	resource
)
{
	if( impl ) impl->DestroyResource( resource );
}

VK2D_API bool VK2D_APIENTRY ResourceManager::IsGood()
{
	return is_good;
}



}

