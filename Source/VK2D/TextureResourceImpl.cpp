
#include "../Header/SourceCommon.h"

#include "../Header/TextureResourceImpl.h"

#include <assert.h>


namespace vk2d {

namespace _internal {





TextureResourceImpl::TextureResourceImpl( _internal::RendererImpl * renderer_parent )
{
	parent		= renderer_parent;
	assert( renderer_parent );
}

TextureResourceImpl::~TextureResourceImpl()
{
	// TODO
}

bool TextureResourceImpl::MTLoad()
{
	// TODO
	return false;
}

bool TextureResourceImpl::MTUnload()
{
	// TODO
	return false;
}

bool TextureResourceImpl::IsGood()
{
	return is_good;
}

} // _internal

} // vk2d