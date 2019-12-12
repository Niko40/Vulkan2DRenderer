
#include "../Header/Core/SourceCommon.h"

#include "../../Include/Interface/Mesh.h"

#include <float.h>



constexpr double PI				= 3.14159265358979323846;
constexpr double RAD			= PI * 2.0;



vk2d::AABB2d CalculateAABBFromPointList(
	const std::vector<vk2d::Vector2d>		&	points
)
{
	vk2d::AABB2d ret { points[ 0 ], points[ 0 ] };
	for( auto p : points ) {
		ret.top_left.x		= std::min( ret.top_left.x, p.x );
		ret.top_left.y		= std::min( ret.top_left.y, p.y );
		ret.bottom_right.x	= std::max( ret.bottom_right.x, p.x );
		ret.bottom_right.y	= std::max( ret.bottom_right.y, p.y );
	}
	return ret;
}

vk2d::AABB2d CalculateAABBFromVertexList(
	const std::vector<vk2d::Vertex>			&	vertices
)
{
	vk2d::AABB2d ret { vertices[ 0 ].vertex_coords, vertices[ 0 ].vertex_coords };
	for( auto p : vertices ) {
		ret.top_left.x		= std::min( ret.top_left.x, p.vertex_coords.x );
		ret.top_left.y		= std::min( ret.top_left.y, p.vertex_coords.y );
		ret.bottom_right.x	= std::max( ret.bottom_right.x, p.vertex_coords.x );
		ret.bottom_right.y	= std::max( ret.bottom_right.y, p.vertex_coords.y );
	}
	return ret;
}

void ClearVerticesToDefaultValues(
	std::vector<vk2d::Vertex>				&	vertices
)
{
	for( auto & v : vertices ) {
		v.vertex_coords			= {};
		v.uv_coords				= {};
		v.color					= { 1.0f, 1.0f, 1.0f, 1.0f };
		v.point_size			= 1.0f;
	}
}

vk2d::Vertex CreateDefaultValueVertex()
{
	vk2d::Vertex v;
	v.vertex_coords		= {};
	v.uv_coords			= {};
	v.color				= { 1.0f, 1.0f, 1.0f, 1.0f };
	v.point_size		= 1.0f;
	return v;
}



VK2D_API void VK2D_APIENTRY vk2d::Mesh::Translate(
	const vk2d::Vector2d		movement )
{
	for( auto & i : vertices ) {
		i.vertex_coords		+= movement;
	}
}

VK2D_API void VK2D_APIENTRY vk2d::Mesh::Rotate(
	float						rotation_amount_radians,
	vk2d::Vector2d				origin )
{
	auto rotation_matrix	= vk2d::CreateRotationMatrix( rotation_amount_radians );

	for( auto & i : vertices ) {
		i.vertex_coords		-= origin;
		i.vertex_coords		= rotation_matrix * i.vertex_coords;
		i.vertex_coords		+= origin;
	}
}

VK2D_API void VK2D_APIENTRY vk2d::Mesh::Scale(
	vk2d::Vector2d				scaling_amount,
	vk2d::Vector2d				origin
)
{
	for( auto & i : vertices ) {
		i.vertex_coords		-= origin;
		i.vertex_coords		*= scaling_amount;
		i.vertex_coords		+= origin;
	}
}

VK2D_API void VK2D_APIENTRY vk2d::Mesh::Scew(
	vk2d::Vector2d				scew_amount,
	vk2d::Vector2d				origin
)
{
	for( auto & i : vertices ) {
		auto c = i.vertex_coords - origin;
		i.vertex_coords.x	= c.y * scew_amount.x + c.x;
		i.vertex_coords.y	= c.x * scew_amount.y + c.y;
		i.vertex_coords		+= origin;
	}
}

VK2D_API void VK2D_APIENTRY vk2d::Mesh::Wave(
	float						rotation_amount_radians,
	float						frequency,
	float						direction_radians,
	vk2d::Vector2d				intensity,
	vk2d::Vector2d				origin
)
{
	vk2d::Vector2d	dir {
		std::cos( direction_radians ),
		std::sin( direction_radians )
	};

	for( auto & i : vertices ) {
		auto c		= i.vertex_coords - origin;
		auto d		= ( c.x * dir.x + c.y * dir.y ) * frequency;
		i.vertex_coords		= {
			std::cos( rotation_amount_radians + d ) * intensity.x + c.x,
			std::sin( rotation_amount_radians + d ) * intensity.y + c.y };
		i.vertex_coords		+= origin;
	}
}

VK2D_API void VK2D_APIENTRY vk2d::Mesh::TranslateUV(
	const vk2d::Vector2d		movement
)
{
	for( auto & i : vertices ) {
		i.uv_coords		+= movement;
	}
}

VK2D_API void VK2D_APIENTRY vk2d::Mesh::RotateUV(
	float						rotation_amount_radians,
	vk2d::Vector2d				origin
)
{
	auto rotation_matrix	= vk2d::CreateRotationMatrix( rotation_amount_radians );

	for( auto & i : vertices ) {
		i.uv_coords		-= origin;
		i.uv_coords		= rotation_matrix * i.uv_coords;
		i.uv_coords		+= origin;
	}
}

VK2D_API void VK2D_APIENTRY vk2d::Mesh::ScaleUV(
	vk2d::Vector2d				scaling_amount,
	vk2d::Vector2d				origin
)
{
	for( auto & i : vertices ) {
		i.uv_coords		-= origin;
		i.uv_coords		*= scaling_amount;
		i.uv_coords		+= origin;
	}
}

VK2D_API void VK2D_APIENTRY vk2d::Mesh::ScewUV(
	vk2d::Vector2d				scew_amount,
	vk2d::Vector2d				origin
)
{
	for( auto & i : vertices ) {
		auto c = i.uv_coords - origin;
		i.uv_coords.x	= c.y * scew_amount.x + c.x;
		i.uv_coords.y	= c.x * scew_amount.y + c.y;
		i.uv_coords		+= origin;
	}
}

VK2D_API void VK2D_APIENTRY vk2d::Mesh::WaveUV(
	float						rotation_amount_radians,
	float						frequency,
	float						direction_radians,
	vk2d::Vector2d				intensity,
	vk2d::Vector2d				origin
)
{
	vk2d::Vector2d	dir {
		std::cos( direction_radians ),
		std::sin( direction_radians )
	};

	for( auto & i : vertices ) {
		auto c		= i.uv_coords - origin;
		auto d		= ( c.x * dir.x + c.y * dir.y ) * frequency;
		i.uv_coords		= {
			std::cos( rotation_amount_radians + d ) * intensity.x + c.x,
			std::sin( rotation_amount_radians + d ) * intensity.y + c.y };
		i.uv_coords		+= origin;
	}
}

VK2D_API void VK2D_APIENTRY vk2d::Mesh::SetVertexColor(
	vk2d::Color					new_color )
{
	for( auto & v : vertices ) {
		v.color		= new_color;
	}
}

VK2D_API void VK2D_APIENTRY vk2d::Mesh::SetVertexColorGradient(
	vk2d::Color					color_1,
	vk2d::Color					color_2,
	vk2d::Vector2d				coord_1,
	vk2d::Vector2d				coord_2
)
{
	vk2d::Vector2d				coord_vector	= coord_2 - coord_1;
	vk2d::Vector2d				coord_dir		= {};
	float						coord_lenght	= std::sqrt( coord_vector.x * coord_vector.x + coord_vector.y * coord_vector.y );
	if( coord_lenght > 0.0f ) {
		coord_dir					= coord_vector / coord_lenght;
	} else {
		coord_lenght				= 0.0001f;
		coord_dir					= { 1.0f, 0.0f };
	}

	auto forward_rotation_matrix = vk2d::Matrix2(
		+coord_dir.x, -coord_dir.y,
		+coord_dir.y, +coord_dir.x
	);
	auto backward_rotation_matrix = vk2d::Matrix2(
		+coord_dir.x, +coord_dir.y,
		-coord_dir.y, +coord_dir.x
	);
	auto coord_linearilized = backward_rotation_matrix * ( coord_dir * coord_lenght );

	for( auto & v : vertices ) {
		auto c = v.vertex_coords - coord_1;

		c = backward_rotation_matrix * c;
		auto cx = c.x / coord_linearilized.x;

		if( cx < 0.0f )			v.color = color_1;
		else if( cx > 1.0f )	v.color = color_2;
		else {
			auto g = vk2d::Color(
				color_1.r * ( 1.0f - cx ) + color_2.r * cx,
				color_1.g * ( 1.0f - cx ) + color_2.g * cx,
				color_1.b * ( 1.0f - cx ) + color_2.b * cx,
				color_1.a * ( 1.0f - cx ) + color_2.a * cx
			);
			v.color		= g;
			//v.color = { 0, 1, 0, 1 };
		}
		c = forward_rotation_matrix * c;

		v.vertex_coords = c + coord_1;
	}
}

VK2D_API void VK2D_APIENTRY vk2d::Mesh::ConfineUVToBoundingBox()
{
	auto aabb = CalculateAABBFromVertexList( vertices );
	auto size = aabb.bottom_right - aabb.top_left;
	for( auto & v : vertices ) {
		auto vp = v.vertex_coords - aabb.top_left;
		v.uv_coords		= vp / size;
	}
}

VK2D_API void VK2D_APIENTRY vk2d::Mesh::SetTexture(
	vk2d::TextureResource	*	texture_resource_pointer
)
{
	texture = texture_resource_pointer;
}

VK2D_API void VK2D_APIENTRY vk2d::Mesh::SetPointSize(
	float						point_size
)
{
	for( auto & v : vertices ) {
		v.point_size = point_size;
	}
}

VK2D_API void VK2D_APIENTRY vk2d::Mesh::SetLineSize(
	float line_width
)
{
	this->line_width = line_width;
}

VK2D_API void VK2D_APIENTRY vk2d::Mesh::SetMeshType(
	vk2d::MeshType				type
)
{
	switch( mesh_type ) {
	case vk2d::MeshType::TRIANGLE_FILLED:
		switch( type ) {
		case vk2d::MeshType::TRIANGLE_FILLED:
			mesh_type	= type;
			break;
		case vk2d::MeshType::TRIANGLE_WIREFRAME:
			mesh_type	= type;
			break;
		case vk2d::MeshType::LINE:
//			mesh_type	= type;
			break;
		case vk2d::MeshType::POINT:
			mesh_type	= type;
			break;
		default:
			break;
		}
		break;
	case vk2d::MeshType::TRIANGLE_WIREFRAME:
		switch( type ) {
		case vk2d::MeshType::TRIANGLE_FILLED:
			mesh_type	= type;
			break;
		case vk2d::MeshType::TRIANGLE_WIREFRAME:
			mesh_type	= type;
			break;
		case vk2d::MeshType::LINE:
//			mesh_type	= type;
			break;
		case vk2d::MeshType::POINT:
			mesh_type	= type;
			break;
		default:
			break;
		}
		break;
	case vk2d::MeshType::LINE:
		switch( type ) {
		case vk2d::MeshType::TRIANGLE_FILLED:
//			mesh_type	= type;
			break;
		case vk2d::MeshType::TRIANGLE_WIREFRAME:
//			mesh_type	= type;
			break;
		case vk2d::MeshType::LINE:
			mesh_type	= type;
			break;
		case vk2d::MeshType::POINT:
			mesh_type	= type;
			break;
		default:
			break;
		}
		break;
	case vk2d::MeshType::POINT:
		// Point cannot be anything else.
		break;
	default:
		break;
	}
}







VK2D_API vk2d::Mesh VK2D_APIENTRY vk2d::GeneratePointMeshFromList(
	const std::vector<vk2d::Vector2d>		&	points
)
{
	auto aabb			= CalculateAABBFromPointList( points );
	auto aabb_origin	= aabb.top_left;
	auto aabb_size		= aabb.bottom_right - aabb_origin;

	Mesh mesh;
	mesh.vertices.resize( points.size() );
	ClearVerticesToDefaultValues( mesh.vertices );
	for( size_t i = 0; i < points.size(); ++i ) {
		mesh.vertices[ i ].vertex_coords	= points[ i ];
		mesh.vertices[ i ].uv_coords		= ( points[ i ] - aabb_origin ) / aabb_size;
	}

	mesh.generated				= true;
	mesh.generated_mesh_type	= vk2d::MeshType::POINT;
	mesh.SetMeshType( mesh.generated_mesh_type );
	return mesh;
}

VK2D_API vk2d::Mesh VK2D_APIENTRY vk2d::GenerateLineMeshFromList(
	const std::vector<vk2d::Vector2d>		&	points,
	const std::vector<vk2d::VertexIndex_2>	&	indices
)
{
	auto mesh = GeneratePointMeshFromList( points );
	mesh.indices.resize( indices.size() * 2 );
	for( size_t i = 0, d = 0; i < indices.size(); ++i, d += 2 ) {
		mesh.indices[ d + 0 ] = indices[ i ].indices[ 0 ];
		mesh.indices[ d + 1 ] = indices[ i ].indices[ 1 ];
	}

	mesh.generated				= true;
	mesh.generated_mesh_type	= vk2d::MeshType::POINT;
	mesh.SetMeshType( mesh.generated_mesh_type );
	return mesh;
}

VK2D_API vk2d::Mesh VK2D_APIENTRY vk2d::GenerateTriangleMeshFromList(
	const std::vector<vk2d::Vector2d>		&	points,
	const std::vector<vk2d::VertexIndex_3>	&	indices,
	bool										filled
)
{
	auto mesh = GeneratePointMeshFromList( points );
	mesh.indices.resize( indices.size() * 3 );
	for( size_t i = 0, d = 0; i < indices.size(); ++i, d += 3 ) {
		mesh.indices[ d + 0 ] = indices[ i ].indices[ 0 ];
		mesh.indices[ d + 1 ] = indices[ i ].indices[ 1 ];
		mesh.indices[ d + 2 ] = indices[ i ].indices[ 2 ];
	}

	if( filled ) {
		mesh.generated_mesh_type	= vk2d::MeshType::TRIANGLE_FILLED;
	} else {
		mesh.generated_mesh_type	= vk2d::MeshType::TRIANGLE_WIREFRAME;
	}
	mesh.generated				= true;
	mesh.SetMeshType( mesh.generated_mesh_type );
	return mesh;
}

VK2D_API vk2d::Mesh VK2D_APIENTRY vk2d::GenerateBoxMesh(
	vk2d::Vector2d				top_left,
	vk2d::Vector2d				bottom_right,
	bool						filled
)
{
	vk2d::Mesh ret;
	ret.vertices.resize( 4 );
	ClearVerticesToDefaultValues( ret.vertices );

	// 0. Top left
	ret.vertices[ 0 ].vertex_coords		= { top_left.x,			top_left.y };
	ret.vertices[ 0 ].uv_coords			= { 0.0f, 0.0f };

	// 1. Top right
	ret.vertices[ 1 ].vertex_coords		= { bottom_right.x,		top_left.y };
	ret.vertices[ 1 ].uv_coords			= { 1.0f, 0.0f };

	// 2. Bottom left
	ret.vertices[ 2 ].vertex_coords		= { top_left.x,			bottom_right.y };
	ret.vertices[ 2 ].uv_coords			= { 0.0f, 1.0f };

	// 3. Bottom right
	ret.vertices[ 3 ].vertex_coords		= { bottom_right.x,		bottom_right.y };
	ret.vertices[ 3 ].uv_coords			= { 1.0f, 1.0f };

	if( filled ) {
		// Draw filled polygons
		ret.indices.resize( 2 * 3 );
		ret.indices[ 0 ]	= 0;
		ret.indices[ 1 ]	= 2;
		ret.indices[ 2 ]	= 1;
		ret.indices[ 3 ]	= 1;
		ret.indices[ 4 ]	= 2;
		ret.indices[ 5 ]	= 3;
		ret.generated				= true;
		ret.generated_mesh_type		= vk2d::MeshType::TRIANGLE_FILLED;
		ret.SetMeshType( ret.generated_mesh_type );
	} else {
		// Draw lines
		ret.indices.resize( 4 * 2 );
		ret.indices[ 0 ]	= 0;
		ret.indices[ 1 ]	= 2;
		ret.indices[ 2 ]	= 2;
		ret.indices[ 3 ]	= 3;
		ret.indices[ 4 ]	= 3;
		ret.indices[ 5 ]	= 1;
		ret.indices[ 6 ]	= 1;
		ret.indices[ 7 ]	= 0;
		ret.generated				= true;
		ret.generated_mesh_type		= vk2d::MeshType::LINE;
		ret.SetMeshType( ret.generated_mesh_type );
	}

	return ret;
}

VK2D_API vk2d::Mesh VK2D_APIENTRY vk2d::GenerateCircleMesh(
	vk2d::Vector2d			top_left,
	vk2d::Vector2d			bottom_right,
	bool					filled,
	float					edge_count
)
{
	if( edge_count < 3.0f ) edge_count = 3.0f;

	Vector2d center_point					= {
		( top_left.x + bottom_right.x ) / 2.0f,
		( top_left.y + bottom_right.y ) / 2.0f
	};

	float center_to_edge_x		= bottom_right.x - center_point.x;
	float center_to_edge_y		= bottom_right.y - center_point.y;

	float rotation_step			= 0.0f;
	float rotation_step_size	= float( RAD / edge_count );

	uint32_t edge_count_integer	= uint32_t( std::ceil( edge_count ) );

	vk2d::Mesh ret;

	ret.vertices.resize( edge_count_integer );
	ClearVerticesToDefaultValues( ret.vertices );

	for( uint32_t i = 0; i < edge_count_integer; ++i ) {
		ret.vertices[ i ].vertex_coords		= {
			std::cos( rotation_step ) * center_to_edge_x + center_point.x,
			std::sin( rotation_step ) * center_to_edge_y + center_point.y
		};
		ret.vertices[ i ].uv_coords			= {
			std::cos( rotation_step ) * 0.5f + 0.5f,
			std::sin( rotation_step ) * 0.5f + 0.5f
		};
		rotation_step					+= rotation_step_size;
	}

	if( filled ) {
		// Draw filled polygons
		ret.indices.resize( size_t( edge_count_integer - 2 ) * 3 );
		{
			for( uint32_t i = 2, a = 0; i < edge_count_integer; ++i, a += 3 ) {
				assert( i < edge_count_integer );

				ret.indices[ size_t( a ) + 0 ]	= 0;
				ret.indices[ size_t( a ) + 1 ]	= i - 1;
				ret.indices[ size_t( a ) + 2 ]	= i;
			}
		}
		ret.generated				= true;
		ret.generated_mesh_type		= vk2d::MeshType::TRIANGLE_FILLED;
		ret.SetMeshType( ret.generated_mesh_type );
	} else {
		// Draw lines
		ret.indices.resize( size_t( edge_count_integer ) * 2 );
		{
			for( uint32_t i = 1, a = 0; i < edge_count_integer; ++i, a += 2 ) {
				assert( i < edge_count_integer );

				ret.indices[ size_t( a ) + 0 ]	= i - 1;
				ret.indices[ size_t( a ) + 1 ]	= i;
			}
			ret.indices[ size_t( edge_count_integer ) * 2LL - 2 ]	= edge_count_integer - 1;
			ret.indices[ size_t( edge_count_integer ) * 2LL - 1 ]	= 0;
		}
		ret.generated				= true;
		ret.generated_mesh_type		= vk2d::MeshType::LINE;
		ret.SetMeshType( ret.generated_mesh_type );
	}
	return ret;
}

VK2D_API vk2d::Mesh VK2D_APIENTRY vk2d::GeneratePieMesh(
	vk2d::Vector2d		top_left,
	vk2d::Vector2d		bottom_right,
	float				begin_angle_radians,
	float				coverage,
	bool				filled,
	float				edge_count
)
{
	vk2d::Mesh ret;

	if( edge_count < 3.0f )			edge_count		= 3.0f;
	if( coverage > 1.0f )			coverage		= 1.0f;
	if( coverage <= 0.0f )			return ret;		// Nothing to draw

	Vector2d center_point					= {
		( top_left.x + bottom_right.x ) / 2.0f,
		( top_left.y + bottom_right.y ) / 2.0f
	};

	float center_to_edge_x		= bottom_right.x - center_point.x;
	float center_to_edge_y		= bottom_right.y - center_point.y;

	float rotation_step_size	= float( RAD / edge_count );

	uint32_t edge_count_integer	= uint32_t( std::ceil( edge_count ) );

	{
		auto angle_clamp		= float( std::floor( begin_angle_radians / RAD ) );
		begin_angle_radians		= begin_angle_radians - float( angle_clamp * RAD );
		assert( begin_angle_radians <= RAD );
	}
	auto end_angle_radians		= float( RAD ) * coverage;
	end_angle_radians			+= begin_angle_radians;
	auto area_angle				= end_angle_radians - begin_angle_radians;

	ret.vertices.reserve( size_t( edge_count_integer ) + 2 );
	// Center vertex.
	{
		auto v = CreateDefaultValueVertex();
		v.vertex_coords		= center_point;
		v.uv_coords			= { 0.5f, 0.5f };
		ret.vertices.push_back( v );
	}

	auto intermediate_point_begin	= uint32_t( begin_angle_radians / rotation_step_size );
	auto intermediate_point_end		= uint32_t( end_angle_radians / rotation_step_size );
	auto intermediate_point_count	= intermediate_point_end - intermediate_point_begin;

	// Start vertex.
	{
		auto v = CreateDefaultValueVertex();
		v.vertex_coords		= {
			std::cos( begin_angle_radians ) * center_to_edge_x + center_point.x,
			std::sin( begin_angle_radians ) * center_to_edge_y + center_point.y
		};
		v.uv_coords			= {
			std::cos( begin_angle_radians ) * 0.5f + 0.5f,
			std::sin( begin_angle_radians ) * 0.5f + 0.5f
		};
		ret.vertices.push_back( v );
	}

	// Intermediate vertices.
	{
		// First half, from begin angle to 0 angle
		for( uint32_t i = intermediate_point_begin + 1; i < intermediate_point_end + 1; ++i ) {
			if( double( rotation_step_size ) * i > RAD ) break;

			auto v = CreateDefaultValueVertex();
			v.vertex_coords		= {
				std::cos( rotation_step_size * i ) * center_to_edge_x + center_point.x,
				std::sin( rotation_step_size * i ) * center_to_edge_y + center_point.y
			};
			v.uv_coords			= {
				std::cos( rotation_step_size * i ) * 0.5f + 0.5f,
				std::sin( rotation_step_size * i ) * 0.5f + 0.5f
			};
			ret.vertices.push_back( v );
		}

		// Second half, from 0 angle to end angle
		float rotation_step		= 0.0f;
		while( rotation_step	< end_angle_radians - RAD ) {

			auto v = CreateDefaultValueVertex();
			v.vertex_coords		= {
				std::cos( rotation_step ) * center_to_edge_x + center_point.x,
				std::sin( rotation_step ) * center_to_edge_y + center_point.y
			};
			v.uv_coords			= {
				std::cos( rotation_step ) * 0.5f + 0.5f,
				std::sin( rotation_step ) * 0.5f + 0.5f
			};
			ret.vertices.push_back( v );

			rotation_step		+= rotation_step_size;
		}
	}

	// End vertex.
	{
		auto v = CreateDefaultValueVertex();
		v.vertex_coords		= {
			std::cos( end_angle_radians ) * center_to_edge_x + center_point.x,
			std::sin( end_angle_radians ) * center_to_edge_y + center_point.y
		};
		v.uv_coords			= {
			std::cos( end_angle_radians ) * 0.5f + 0.5f,
			std::sin( end_angle_radians ) * 0.5f + 0.5f
		};
		ret.vertices.push_back( v );
	}



	if( filled ) {
		// Draw filled polygons
		ret.indices.resize( ( ret.vertices.size() - 2 ) * 3 );
		{
			for( size_t i = 2, a = 0; i < ret.vertices.size(); ++i, a += 3 ) 				{
				ret.indices[ a + 0 ]	= 0;
				ret.indices[ a + 1 ]	= uint32_t( i ) - 1;
				ret.indices[ a + 2 ]	= uint32_t( i );
			}
		}
		ret.generated				= true;
		ret.generated_mesh_type		= vk2d::MeshType::TRIANGLE_FILLED;
		ret.SetMeshType( ret.generated_mesh_type );
	} else {
		// Draw lines
		ret.indices.resize( ret.vertices.size() * 2 );
		for( size_t i = 1, a = 0; i < ret.vertices.size(); ++i, a += 2 ) {
			ret.indices[ a + 0 ]	= uint32_t( i ) - 1;
			ret.indices[ a + 1 ]	= uint32_t( i );
		}
		ret.indices[ ret.indices.size() - 2 ]	= uint32_t( ret.vertices.size() - 1 );
		ret.indices[ ret.indices.size() - 1 ]	= 0;

		ret.generated				= true;
		ret.generated_mesh_type		= vk2d::MeshType::LINE;
		ret.SetMeshType( ret.generated_mesh_type );
	}

	return ret;
}

VK2D_API vk2d::Mesh VK2D_APIENTRY vk2d::GeneratePieBoxMesh(
	vk2d::Vector2d		top_left,
	vk2d::Vector2d		bottom_right,
	float				begin_angle_radians,
	float				coverage,
	bool				filled
)
{
	vk2d::Mesh ret;

	if( coverage >= 1.0f ) {
		return vk2d::GenerateBoxMesh( top_left, bottom_right, filled );
	}
	if( coverage <= 0.0f ) {
		return ret;		// Nothing to draw
	}

	Vector2d center_point					= {
		( top_left.x + bottom_right.x ) / 2.0f,
		( top_left.y + bottom_right.y ) / 2.0f
	};

	float center_to_edge_x		= bottom_right.x - center_point.x;
	float center_to_edge_y		= bottom_right.y - center_point.y;

	float width					= std::abs( bottom_right.x - top_left.x );
	float height				= std::abs( bottom_right.y - top_left.y );

	auto end_angle_radians		= float( RAD ) * coverage;
	end_angle_radians			+= begin_angle_radians;
	auto area_angle				= end_angle_radians - begin_angle_radians;

	enum AllPointsIndex : uint32_t {
		BOX_CORNER_0,
		BOX_CORNER_1,
		BOX_CORNER_2,
		BOX_CORNER_3,
		PIE_BEGIN,
		PIE_END,
	};
	std::array<Vector2d, 6>	unordered_outer_points { {
		{ top_left.x,		top_left.y		},
		{ bottom_right.x,	top_left.y		},
		{ bottom_right.x,	bottom_right.y	},
		{ top_left.x,		bottom_right.y	}
	} };

	// Get pie begin and end coordinates
	{
		// Specialized version of Ray to AABB intersecion test that
		// always intersects and returns the exit coordinates only.
		auto RayExitIntersection	=[](
			Vector2d box_coords_1,
			Vector2d box_coords_2,
			Vector2d box_center,		// ray origin, and box center are both at the same coordinates
			Vector2d ray_end
			) -> Vector2d
		{
			Vector2d	ray_begin			= box_center;
			float		clipped_exit		= 1.0;

			// Clipping on x and y axis
			float dim_low_x		= ( box_coords_1.x - ray_begin.x ) / ( ray_end.x - ray_begin.x );
			float dim_low_y		= ( box_coords_1.y - ray_begin.y ) / ( ray_end.y - ray_begin.y );
			float dim_high_x	= ( box_coords_2.x - ray_begin.x ) / ( ray_end.x - ray_begin.x );
			float dim_high_y	= ( box_coords_2.y - ray_begin.y ) / ( ray_end.y - ray_begin.y );

			dim_high_x			= std::max( dim_low_x, dim_high_x );
			dim_high_y			= std::max( dim_low_y, dim_high_y );
			clipped_exit		= std::min( dim_high_y, dim_high_x );

			Vector2d collider_localized				= ray_end - ray_begin;
			Vector2d localized_intersection_point	= collider_localized * clipped_exit;
			Vector2d globalized_intersection_point	= localized_intersection_point + box_center;
			return globalized_intersection_point;
		};

		float ray_lenght			= center_to_edge_x + center_to_edge_y;
		Vector2d ray_angle_1_end	= { std::cos( begin_angle_radians ) * ray_lenght, std::sin( begin_angle_radians ) * ray_lenght };
		Vector2d ray_angle_2_end	= { std::cos( end_angle_radians ) * ray_lenght, std::sin( end_angle_radians ) * ray_lenght };
		ray_angle_1_end				+= center_point;
		ray_angle_2_end				+= center_point;

		unordered_outer_points[ PIE_BEGIN ]	= RayExitIntersection(
			top_left,
			bottom_right,
			center_point,
			ray_angle_1_end
		);
		unordered_outer_points[ PIE_END ]		= RayExitIntersection(
			top_left,
			bottom_right,
			center_point,
			ray_angle_2_end
		);
	}

	auto IsOnSameXAxis	= [](
		Vector2d			point_1,
		Vector2d			point_2
		) -> bool
	{
		if( point_1.y < point_2.y + 0.0001f &&
			point_1.y > point_2.y - 0.0001f ) {
			return true;
		}
		return false;
	};

	auto IsOnSameYAxis	= [](
		Vector2d			point_1,
		Vector2d			point_2
		) -> bool
	{
		if( point_1.x < point_2.x + 0.0001f &&
			point_1.x > point_2.x - 0.0001f ) {
			return true;
		}
		return false;
	};

	// Generate an ordered point list
	std::vector<Vector2d> outer_point_list;
	{
		// We'll linearize / unwrap the box so that a single number represents x/y coordinates
		//
		// 0---1
		// |   |	->	0---1---2---3---0
		// 3---2

		struct LinearPoint {
			uint32_t		original_linear_point_index		= {};
			float			linear_coords					= {};
			Vector2d		actual_coords					= {};
		};
		float distance_counter = 0.0f;
		std::array<LinearPoint, 6>	linear_points { {
			{ BOX_CORNER_0, 0.0f,							unordered_outer_points[ BOX_CORNER_0 ] },
			{ BOX_CORNER_1, distance_counter += width,		unordered_outer_points[ BOX_CORNER_1 ] },
			{ BOX_CORNER_2, distance_counter += height,		unordered_outer_points[ BOX_CORNER_2 ] },
			{ BOX_CORNER_3, distance_counter += width,		unordered_outer_points[ BOX_CORNER_3 ] }
		} };

		auto ResolveLinearPoint	=[
			width,
				height,
				IsOnSameXAxis,
				IsOnSameYAxis,
				&unordered_outer_points,
				&linear_points
		](
			uint32_t		index
			)
		{
			// Find which side of the box the point is located at
			Vector2d actual_coords	= unordered_outer_points[ index ];

			if( IsOnSameXAxis( actual_coords, unordered_outer_points[ 0 ] ) ) {
				// Top
				float linear		= ( actual_coords.x - linear_points[ 0 ].actual_coords.x ) + linear_points[ 0 ].linear_coords;
				linear_points[ index ]	= { index, linear, actual_coords };
			} else if( IsOnSameYAxis( actual_coords, unordered_outer_points[ 1 ] ) ) {
				// Right
				float linear		= ( actual_coords.y - linear_points[ 1 ].actual_coords.y ) + linear_points[ 1 ].linear_coords;
				linear_points[ index ]	= { index, linear, actual_coords };
			} else if( IsOnSameXAxis( actual_coords, unordered_outer_points[ 3 ] ) ) {
				// Bottom
				float linear		= ( linear_points[ 2 ].actual_coords.x - actual_coords.x ) + linear_points[ 2 ].linear_coords;
				linear_points[ index ]	= { index, linear, actual_coords };
			} else if( IsOnSameYAxis( actual_coords, unordered_outer_points[ 0 ] ) ) {
				// Left
				float linear		= ( linear_points[ 3 ].actual_coords.y - actual_coords.y ) + linear_points[ 3 ].linear_coords;
				linear_points[ index ]	= { index, linear, actual_coords };
			} else {
				assert( 0 );
			}
		};

			ResolveLinearPoint( PIE_BEGIN );
			ResolveLinearPoint( PIE_END );

			std::sort(
				linear_points.begin(),
				linear_points.end(),
				[]( LinearPoint & p1, LinearPoint & p2 )
				{
					return p1.linear_coords < p2.linear_coords;
				} );

			// We have a list of points in a winding order.
			// Now we need to figure out which points are
			// within and collect a list from those.

			// Find current index for pie begin and end in linear_points array
			uint32_t	linear_resolved_pie_begin_point		= UINT32_MAX;
			uint32_t	linear_resolved_pie_end_point		= UINT32_MAX;
			for( size_t i = 0; i < linear_points.size(); ++i ) {
				if( linear_points[ i ].original_linear_point_index == PIE_BEGIN )	linear_resolved_pie_begin_point	= uint32_t( i );
				if( linear_points[ i ].original_linear_point_index == PIE_END )		linear_resolved_pie_end_point	= uint32_t( i );
			}
			assert( linear_resolved_pie_begin_point != UINT32_MAX );
			assert( linear_resolved_pie_end_point != UINT32_MAX );

			// Sort() organized everything for us, now we just need to
			// collect all in-between coordinates from the indices
			outer_point_list.reserve( 6 );
			uint32_t current_linear_point_index		= linear_resolved_pie_begin_point;
			while( current_linear_point_index != linear_resolved_pie_end_point ) {
				outer_point_list.push_back( linear_points[ current_linear_point_index ].actual_coords );
				if( ( ++current_linear_point_index ) == 6 ) current_linear_point_index = 0;
			}
			outer_point_list.push_back( linear_points[ linear_resolved_pie_end_point ].actual_coords );
	}

	// Create vertices
	{
		ret.vertices.reserve( 7 );

		auto v = CreateDefaultValueVertex();
		v.vertex_coords		= center_point;
		v.uv_coords			= { 0.5f, 0.5f };
		ret.vertices.push_back( v );

		for( auto opl : outer_point_list ) {
			v.vertex_coords		= opl;
			v.uv_coords			= ( opl - unordered_outer_points[ 0 ] ) / Vector2d( width, height );
			ret.vertices.push_back( v );
		}
	}

	if( filled ) {
		ret.indices.reserve( ret.vertices.size() * 3 );
		for( size_t i = 2; i < ret.vertices.size(); ++i ) {
			ret.indices.push_back( 0 );
			ret.indices.push_back( uint32_t( i ) - 1 );
			ret.indices.push_back( uint32_t( i ) );
		}

		ret.generated				= true;
		ret.generated_mesh_type		= vk2d::MeshType::TRIANGLE_FILLED;
		ret.SetMeshType( ret.generated_mesh_type );

	} else {
		ret.indices.reserve( ret.vertices.size() * 2 + 2 );
		for( size_t i = 1; i < ret.vertices.size(); ++i ) {
			ret.indices.push_back( uint32_t( i ) - 1 );
			ret.indices.push_back( uint32_t( i ) );
		}
		ret.indices.push_back( uint32_t( ret.vertices.size() - 1 ) );
		ret.indices.push_back( 0 );

		ret.generated				= true;
		ret.generated_mesh_type		= vk2d::MeshType::LINE;
		ret.SetMeshType( ret.generated_mesh_type );
	}

	return ret;
}

VK2D_API vk2d::Mesh VK2D_APIENTRY vk2d::GenerateLatticeMesh(
	vk2d::Vector2d			top_left,
	vk2d::Vector2d			bottom_right,
	vk2d::Vector2d			subdivisions,
	bool					filled )
{
	vk2d::Mesh ret;

	uint32_t vertex_count_x			= uint32_t( std::ceil( subdivisions.x ) ) + 2;
	uint32_t vertex_count_y			= uint32_t( std::ceil( subdivisions.y ) ) + 2;
	uint32_t total_vertex_count		= vertex_count_x * vertex_count_y;

	vk2d::Vector2d mesh_size		= bottom_right - top_left;
	vk2d::Vector2d vertex_spacing	= mesh_size / ( subdivisions + vk2d::Vector2d( 1.0f, 1.0f ) );
	vk2d::Vector2d uv_spacing		= vk2d::Vector2d( 1.0f, 1.0f ) / ( subdivisions + vk2d::Vector2d( 1.0f, 1.0f ) );

	ret.vertices.resize( total_vertex_count );
	ClearVerticesToDefaultValues( ret.vertices );

	for( size_t y = 0; y < vertex_count_y - 1; ++y ) {
		for( size_t x = 0; x < vertex_count_x - 1; ++x ) {
			auto & v		= ret.vertices[ y * vertex_count_x + x ];
			v.vertex_coords	= { vertex_spacing.x * x + top_left.x, vertex_spacing.y * y + top_left.y };
			v.uv_coords		= { uv_spacing.x * x, uv_spacing.y * y };
		}
		auto & v			= ret.vertices[ y * vertex_count_x + vertex_count_x - 1 ];
		v.vertex_coords		= { bottom_right.x, vertex_spacing.y * y + top_left.y };
		v.uv_coords			= { 1.0f, uv_spacing.y * y };
	}
	for( size_t x = 0; x < vertex_count_x - 1; ++x ) {
		auto & v		= ret.vertices[ size_t( vertex_count_y - 1 ) * vertex_count_x + x ];
		v.vertex_coords	= { vertex_spacing.x * x + top_left.x, bottom_right.y };
		v.uv_coords		= { uv_spacing.x * x, 1.0f };
	}
	auto & v			= ret.vertices[ size_t( vertex_count_y - 1 ) * vertex_count_x + vertex_count_x - 1 ];
	v.vertex_coords		= { bottom_right.x, bottom_right.y };
	v.uv_coords			= { 1.0f, 1.0f };

	if( filled ) {
		// Draw solid mesh with polygons.

		auto reserved_size = ( vertex_count_x - 1 ) * ( vertex_count_y - 1 ) * 2 * 3;
		ret.indices.reserve( reserved_size );
		for( size_t y = 1; y < vertex_count_y; ++y ) {
			for( size_t x = 1; x < vertex_count_x; ++x ) {
				auto tl		= uint32_t( ( y - 1 ) * vertex_count_x + x - 1	);	// top left vertex.
				auto tr		= uint32_t( ( y - 1 ) * vertex_count_x + x );		// top right vertex.
				auto bl		= uint32_t( ( y - 0 ) * vertex_count_x + x - 1 );	// bottom left vertex.
				auto br		= uint32_t( ( y - 0 ) * vertex_count_x + x );		// bottom right vertex.
				ret.indices.push_back( tl );	// First triangle.
				ret.indices.push_back( bl );
				ret.indices.push_back( tr );
				ret.indices.push_back( tr );	// Second triangle.
				ret.indices.push_back( bl );
				ret.indices.push_back( br );
			}
		}

		ret.generated				= true;
		ret.generated_mesh_type		= vk2d::MeshType::TRIANGLE_FILLED;
		ret.SetMeshType( ret.generated_mesh_type );

	} else {
		// Draw lattice lines.

		auto reserved_size = ( vertex_count_x - 1 ) * ( vertex_count_y - 1 ) * 4 * 2;
		ret.indices.reserve( reserved_size );
		for( size_t y = 1; y < vertex_count_y; ++y ) {
			for( size_t x = 1; x < vertex_count_x; ++x ) {
				auto tl		= uint32_t( ( y - 1 ) * vertex_count_x + x - 1 );	// top left vertex.
				auto tr		= uint32_t( ( y - 1 ) * vertex_count_x + x );		// top right vertex.
				auto bl		= uint32_t( ( y - 0 ) * vertex_count_x + x - 1 );	// bottom left vertex.
				auto br		= uint32_t( ( y - 0 ) * vertex_count_x + x );		// bottom right vertex.
				ret.indices.push_back( tl );	// First line.
				ret.indices.push_back( bl );
				ret.indices.push_back( bl );	// Second line.
				ret.indices.push_back( br );
				ret.indices.push_back( br );	// Third line.
				ret.indices.push_back( tr );
				ret.indices.push_back( tr );	// Fourth line.
				ret.indices.push_back( tl );
			}
		}

		ret.generated				= true;
		ret.generated_mesh_type		= vk2d::MeshType::LINE;
		ret.SetMeshType( ret.generated_mesh_type );
	}

	return ret;
}
