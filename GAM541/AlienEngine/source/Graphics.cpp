#include "Precompiled.h"
#include "WindowsSystem.h"
#include "Graphics.h"
#include "D3DManager.h"
#include "renderTargetTexture.h"
//graphics elements
#include "Mesh.h"
#include "animation.h"
#include "texture.h"
//shader
#include "shader.h"
//components
#include "ComponentCreator.h"
#include "Camera.h"
#include "MeshInstance.h"
#include "visualPlatform.h"
#include "particleEmitter.h"
#include "Transform.h"
#include "directionalLight.h"
//font engine
#include "font.h"
#include "text.h"
///materials
#include "grass.h"
#include "tile.h"
#include "texturedBox.h"
///xml serializer
#include "XMLSerialization.h"
///debug drawing
#include "debugGeometry.h"	


#include <iostream>

namespace Framework
{

//Our global pointer to Graphics.
Graphics* GRAPHICS = NULL;

const float FRONT = 0.1f;
const float BACK  =	1000.0f;
const int MAX_DEBUG_PRIMITIVES = 2000;
const int MAX_PARTICLES_PER_EMITTER = 1000;
const float MAX_CAMERA_MOVING_SPEED = 2.0f;
const float PI = 3.1415926535897932384626f;

//=======================================================
Graphics::Graphics()
{
	primary_light = NULL;
	line_list = NULL;
	/// global pointer
	GRAPHICS = this;
}
//=======================================================
Graphics::~Graphics()
{
	Free();
}
//=======================================================
void Graphics::Initialize()
{
	d3dmanager = new D3DManager;
	d3dmanager->Initialize( hwnd, full_screen, screen_width, screen_height );
	device = d3dmanager->GetDevice();
	swap_chain = d3dmanager->GetSwapChain();
	render_target_screen = d3dmanager->GetRenderTargetScreen();
	srv_screen = d3dmanager->GetShaderResourceScreen();
	depth_stencil_view = d3dmanager->GetDepthStencilView();

	LoadAssets();
	std::cout <<"compiling shaders...\n";
	///set up render targets
	for ( unsigned int i = 0; i < 3; ++i )
	{
		render_target_raw[i] = new RenderTargetTexture();
		render_target_raw[i]->Initialize( device, screen_width, screen_height );
	}
	///set up static mesh shaders
	mesh_shader = new Shader;
	mesh_shader->Initialize( device, "shaders\\MeshShader.fx", 
					VERTEX_ELEMENT_POSITION0|VERTEX_ELEMENT_POSITION1|VERTEX_ELEMENT_POSITION2|VERTEX_ELEMENT_POSITION3|
					VERTEX_ELEMENT_TEXCOORD0|VERTEX_ELEMENT_COLOR|VERTEX_ELEMENT_BONES|VERTEX_ELEMENT_WEIGHTS|
					VERTEX_ELEMENT_PER_INSTANCE_FLAG|
					VERTEX_ELEMENT_TRANSFORM );
	
	/// deferred shader
	deferred_shader = new Shader;
	deferred_shader->Initialize( device, "shaders\\deferredShader.fx", 0 );
	d_diffuse_ptr = deferred_shader->GetVariableByName("diffuse")->AsShaderResource();
	d_normal_flag_ptr = deferred_shader->GetVariableByName("normal_flag")->AsShaderResource();
	d_view_position_ptr = deferred_shader->GetVariableByName("view_position")->AsShaderResource();
	deferred_viewport_ptr = deferred_shader->GetVariableByName("viewport")->AsVector();
	/// post processing shader
	post_processing_shader = new Shader;
	post_processing_shader->Initialize( device, "shaders\\postProcessingShader.fx", 0 );
	p_color_ptr = post_processing_shader->GetVariableByName("color")->AsShaderResource();
	p_normal_flag_ptr = post_processing_shader->GetVariableByName("normal_flag")->AsShaderResource();
	p_view_position_ptr = post_processing_shader->GetVariableByName("view_position")->AsShaderResource();
	previous_frame_ptr = post_processing_shader->GetVariableByName("previous_frame")->AsShaderResource();
	post_processing_viewport_ptr = post_processing_shader->GetVariableByName("viewport")->AsVector();
	frametime_ptr = post_processing_shader->GetVariableByName("frametime")->AsScalar();
	/// double textured (blur)
	float clear_color[4] = {1.0f,1.0f,1.0f,1.0f};
	render_target_shaded = new RenderTargetTexture();
	render_target_shaded->Initialize( device, screen_width, screen_height );
	device->ClearRenderTargetView( render_target_shaded->GetRTV(), clear_color );
	for ( int i = 0; i < 2; ++i )
	{
		render_target_processed[i] = new RenderTargetTexture();
		render_target_processed[i]->Initialize( device, screen_width, screen_height );
		device->ClearRenderTargetView( render_target_processed[i]->GetRTV(), clear_color );
	}
	cur_pp_target = 0;
	/// fxaa shader
	fxaa_shader = new Shader;
	fxaa_shader->Initialize( device, "shaders\\FXAAShader.fx", 0 );
	processed_texture_ptr = fxaa_shader->GetVariableByName( "processed_texture" )->AsShaderResource();
	fxaa_viewport_ptr = fxaa_shader->GetVariableByName("viewport")->AsVector();
	
	///set up matrices and initialize camera
	current_camera_position = Vec3( 0.0f, 0.0f, -20.0f );
	UpdateProjectionAndOrthoMatrices();

	///set up debug shader
	InitializeDebugDrawing();
	render_debug_information = true;
	
	////set up font engine
	font_shader = new Shader;
	font_shader->Initialize( device, "shaders\\fontShader.fx", VERTEX_ELEMENT_POSITION0|VERTEX_ELEMENT_NORMAL|VERTEX_ELEMENT_COLOR|VERTEX_ELEMENT_TEXCOORD0|VERTEX_ELEMENT_TEXCOORD1 );
	RegisterFont("TimesNewRoman");
	font_img_ptr = font_shader->GetVariableByName("font_texture")->AsShaderResource();

	/// set up materials
		//grass
	grass_shader = new Shader;
	grass_shader->Initialize( device, "shaders\\grassShader.fx", VERTEX_ELEMENT_POSITION0|VERTEX_ELEMENT_NORMAL|VERTEX_ELEMENT_COLOR );
	Material* grass = new Grass;
	grass->SetShader( grass_shader );
	grass->Initialize( device );
	material_map[MAT_GRASS] = grass;
		//tile
	tile_shader = new Shader;
	tile_shader->Initialize( device, "shaders\\tileShader.fx", VERTEX_ELEMENT_POSITION0|VERTEX_ELEMENT_NORMAL|
							VERTEX_ELEMENT_COLOR|VERTEX_ELEMENT_TEXCOORD0|VERTEX_ELEMENT_TEXCOORD1 );
	Material* tile = new Tile;
	tile->SetShader( tile_shader );
	tile->Initialize( device );
	material_map[MAT_TILE] = tile;
		//textured box
	textured_box_shader = new Shader;
	textured_box_shader->Initialize( device, "shaders\\texturedBoxShader.fx", VERTEX_ELEMENT_POSITION0|VERTEX_ELEMENT_POSITION1 );
	Material* textured_box = new TexturedBox;
	textured_box->SetShader( textured_box_shader );
	textured_box->Initialize( device );
	material_map[MAT_TEXTUREDBOX] = textured_box;
	///particle systems
	InitializeParticleSystem();
	///register graphics component
	RegisterComponent(Camera);
	RegisterComponent(Sprite);
	RegisterComponent(VisualPlatform);
	RegisterComponent(MeshInstance);
	RegisterComponent(ParticleEmitter);
	//////////////////////////
	////// testing code below

	//ParticleEmitter* emitter = new ParticleEmitter;	
	//emitter->initial_velocity = Vec3( 2.0f, 1.0f, 0.0f );

	//RegisterParticleEmitter( emitter );

	//emitter = new ParticleEmitter;
	//emitter->initial_velocity = Vec3( -2.0f, -1.0f, 0.0f );
	//RegisterParticleEmitter( emitter );

	test_text = new Text;
	RegisterText( test_text );
	test_text->SetPosition( 10.0f, 450.0f, 200.0f );

}
//=======================================================
void Graphics::Update( float timeslice )
{
	char frametime[10];
	sprintf_s(frametime, "%0.3f", timeslice );
	test_text->UpdateText( frametime, "TimesNewRoman", 20 );

	/* environment set up */
	float viewport[2] = { 1/(float)screen_width, 1/(float)screen_height };
	UpdateViewingMatrix();
	AdvanceParticles( timeslice );
//	current_camera_position.z = -20.0f;
	d3dmanager->SetRenderWireFrame( false );
	ID3D10RenderTargetView* render_targets[3];
	float clear_color[3][4] = { { 1.0f, 1.0f, 1.0f, 1.0f },		//default color
								{0.0f,1.0f,0.0f,0.0f},			//default normal/flag
								{0.0f,0.0f,BACK,0.0f} };		//default view position/shininess
	for ( int i = 0; i < 3; ++i )
	{
		render_targets[i] = render_target_raw[i]->GetRTV();
		device->ClearRenderTargetView( render_targets[i], clear_color[i] );
	}
	/// clear reflectives
	d3dmanager->SetDepthEnable( true );
	/**********************************
	*******  3d object rendering ******
	**********************************/
	/* draw reflection */
	UpdateReflectionMatrices();
	for ( unsigned int i = 0; i < reflectives.size(); ++i )
	{
		ID3D10RenderTargetView* rtv = reflectives[i]->reflection_view->GetRTV();
		device->ClearDepthStencilView( depth_stencil_view, D3D10_CLEAR_DEPTH, 1.0f, 0 );
		device->ClearRenderTargetView( rtv, clear_color[0] );
		device->OMSetRenderTargets( 1, &rtv, depth_stencil_view );
		MeshMapType::iterator sit;
		device->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
		mesh_shader->SetTransformationConstants( device, reflectives[i]->mat_reflection, mat_proj );
		for ( sit = mesh_map.begin(); sit != mesh_map.end(); ++sit )
		{
			mesh_shader->Apply( device );
			sit->second->Draw( device, mesh_shader );
		}
		MaterialMapType::iterator mit;
		for ( mit = material_map.begin(); mit != material_map.end(); ++mit )
		{
			mit->second->Draw( device, reflectives[i]->mat_reflection, mat_proj, &reflectives[i]->altitude );
		}
		RenderParticles( reflectives[i]->mat_reflection, mat_proj );
	}
	/* meshes */
	device->ClearDepthStencilView( depth_stencil_view, D3D10_CLEAR_DEPTH, 1.0f, 0 );
	device->OMSetRenderTargets( 3, render_targets, depth_stencil_view );
	mesh_shader->SetTransformationConstants( device, mat_view, mat_proj );
	device->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	MeshMapType::iterator sit;
	mesh_shader->Apply( device );
	for ( sit = mesh_map.begin(); sit != mesh_map.end(); ++sit )
	{
		sit->second->Draw( device, mesh_shader );
	}
	/* terrain */
	MaterialMapType::iterator mit;
	for ( mit = material_map.begin(); mit != material_map.end(); ++mit )
	{
		mit->second->Update( timeslice );
		mit->second->Draw( device, mat_view, mat_proj );
	}
	RenderParticles( mat_view, mat_proj );
	/*********************************
	*******  deferred shading*********
	*********************************/
	d3dmanager->SetDepthEnable( false );
	ID3D10RenderTargetView* rtv_shaded = render_target_shaded->GetRTV();
	device->OMSetRenderTargets( 1, &rtv_shaded, NULL );
	device->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_POINTLIST );
	deferred_shader->SetTransformationConstants( device, mat_ortho_view, mat_ortho_proj );
	deferred_viewport_ptr->SetFloatVector( viewport );
	d_diffuse_ptr->SetResource( render_target_raw[0]->GetSRV() );
	d_normal_flag_ptr->SetResource( render_target_raw[1]->GetSRV() );
	d_view_position_ptr->SetResource( render_target_raw[2]->GetSRV() );
	deferred_shader->Apply(device);
	device->Draw(1, 0);
	/*********************************
	*******  post processing *********
	*********************************/
	//set up render target
	ID3D10RenderTargetView* rtv_processed = render_target_processed[cur_pp_target]->GetRTV();
	device->OMSetRenderTargets( 1, &rtv_processed, NULL );
	//apply filters
	device->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_POINTLIST );
	post_processing_shader->SetTransformationConstants( device, mat_ortho_view, mat_ortho_proj );
	p_color_ptr->SetResource( render_target_shaded->GetSRV() );
	p_normal_flag_ptr->SetResource( render_target_raw[1]->GetSRV() );
	p_view_position_ptr->SetResource( render_target_raw[2]->GetSRV() );
	previous_frame_ptr->SetResource( render_target_processed[1-cur_pp_target]->GetSRV() );
	post_processing_viewport_ptr->SetFloatVector( viewport );
	frametime_ptr->SetFloat( 1000.0f * timeslice );
	post_processing_shader->Apply( device );
	device->Draw( 1, 0 );
	cur_pp_target = 1 - cur_pp_target;	//flip target
	//fxaa
	device->OMSetRenderTargets( 1, &render_target_screen, NULL );
	device->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_POINTLIST );
	fxaa_shader->SetTransformationConstants( device, mat_ortho_view, mat_ortho_proj );
	processed_texture_ptr->SetResource( render_target_processed[cur_pp_target]->GetSRV() );
	fxaa_viewport_ptr->SetFloatVector( viewport );
	fxaa_shader->Apply( device );
	device->Draw( 1, 0 );	

	/*********************************
	*******		HUD / text   *********
	*********************************/
	RenderText();
	/*********************************
	*******  debug drawing   *********
	*********************************/
	RenderLineList();
	if ( render_debug_information ) RenderDebug();
	swap_chain->Present( 0, 0 );
}
//=======================================================
void Graphics::Free()
{
	swap_chain = NULL;
	device = NULL;
	render_target_screen = NULL;
	srv_screen = NULL;
	depth_stencil_view = NULL;
	d3dmanager->Shutdown();

	for ( int i = 0; i < 3; ++i ) { S_DELETE(render_target_raw[i]); }
	for ( unsigned int i = 0; i < reflectives.size(); ++i ) 
	{ S_DELETE( reflectives[i]->reflection_view ); S_DELETE( reflectives[i] );}

	S_DELETE(grass_shader);
	S_DELETE(tile_shader);
	S_DELETE(mesh_shader);

	S_RELEASE(random_texture);
	S_RELEASE(vb_launcher);
	S_DELETE(particle_shader);
	S_DELETE(advance_particles);

	S_DELETE(deferred_shader);
	S_DELETE(render_target_shaded);
	S_DELETE(post_processing_shader);
	for ( int i = 0; i < 2; ++i ) { S_DELETE(render_target_processed[i]); }
	S_DELETE(fxaa_shader);

	MeshMapType::iterator sit;
	for ( sit = mesh_map.begin(); sit != mesh_map.end(); ++sit )
	{
		S_DELETE(sit->second);
	}
	
	TextureMapType::iterator tit;
	for ( tit = texture_map.begin(); tit != texture_map.end(); ++tit )
	{
		S_DELETE(tit->second);
	}

	MaterialMapType::iterator mit;
	for ( mit = material_map.begin(); mit != material_map.end(); ++mit )
	{
		S_DELETE(mit->second);
	}
}
//=======================================================
void Graphics::Load()
{
}
//=======================================================
void Graphics::Unload()
{
	MaterialMapType::iterator mit;
	for ( mit = material_map.begin(); mit != material_map.end(); ++mit )
	{
		mit->second->Free();
	}
	debug_geometry_list.clear();
}
//=======================================================
void Graphics::LoadAssets()
{
	std::string id;
	std::string path;
	std::string extra;
	std::vector<ISerializer*> entries;

	std::cout <<"loading assets...\n";

	XMLReader* reader = new XMLReader();
	reader->open( "Assets\\GraphicsAssets.xml" );
	ISerializer* stream = reader->data().clone();
	//read texture data
	stream->readProperty( "textures", *stream );
	stream->readProperty( "texture", entries );
	for ( unsigned int i = 0; i < entries.size(); ++i )
	{
		Texture* texture = new Texture;
		entries[i]->readProperty("id", id );
		if ( entries[i]->readValue("diffuse", path ) )
			texture->LoadDiffuse( device, path.c_str() );
		if ( entries[i]->readValue( "mask", path ) )
			texture->LoadMask( device, path.c_str() );
		if ( entries[i]->readValue( "normal", path ) )
			texture->LoadNormal( device, path.c_str() );
		texture_map[id] = texture;
	}
	for ( unsigned int i = 0; i < entries.size(); ++i ) delete entries[i];
	entries.clear();
	stream->goToParent();
	//read mesh data
	stream->readProperty( "meshes", *stream );
	stream->readProperty( "mesh", entries );
	for ( unsigned int i = 0; i < entries.size(); ++i )
	{
		entries[i]->readProperty("id", id );
		entries[i]->readProperty("path", path );
		entries[i]->readProperty("animated", extra );
		bool animated = (extra == "true");
		Mesh* mesh = new Mesh;
		mesh->BuildVertexBuffer( device, path.c_str(), animated );
		mesh_map[id] = mesh;
	}
	for ( unsigned int i = 0; i < entries.size(); ++i ) delete entries[i];
	entries.clear();
	stream->goToParent();

	//read animation data
	stream->readProperty( "animations", *stream );
	stream->readProperty( "animation", entries );
	for ( unsigned int i = 0; i < entries.size(); ++i )
	{
//		entries[i]->readProperty("id", str );
//		std::cout <<str<<std::endl;
	}
	for ( unsigned int i = 0; i < entries.size(); ++i ) delete entries[i];
	entries.clear();
	stream->goToParent();

	

	delete stream;
	delete reader;
}
//=======================================================
void Graphics::UpdateProjectionAndOrthoMatrices()
{
	float fov, screen_aspect;

	fov = PI / 8.0f;
	screen_aspect = (float)screen_width / (float) screen_height;
	D3DXMatrixPerspectiveFovLH( &mat_proj, fov, screen_aspect, FRONT, BACK );
// set transforms for 2d
	D3DXVECTOR3 eye( (float)screen_width / 2.0f, (float)screen_height / 2.0f, -4.0f );
	D3DXVECTOR3 at( (float)screen_width / 2.0f, (float)screen_height / 2.0f, 0.0f );
	D3DXVECTOR3 up( 0.0f, 1.0f, 0.0f );
	D3DXMatrixLookAtLH( &mat_ortho_view, &eye, &at, &up );
	D3DXMatrixOrthoLH( &mat_ortho_proj, (float)screen_width, (float)screen_height, FRONT, BACK );

}
//=======================================================
void Graphics::UpdateViewingMatrix()
{
	Camera* camera = NULL;
	if ( !camera_stack.empty() ) camera = camera_stack.top();
	if ( camera )
	{
		Vec3 distance = camera->transform->Position - current_camera_position;
		float sq = D3DXVec3LengthSq( &distance );
//		std::cout <<sq <<std::endl;
		if ( sq > MAX_CAMERA_MOVING_SPEED*MAX_CAMERA_MOVING_SPEED )
		{
			distance /= sqrt(sq);
			distance *= MAX_CAMERA_MOVING_SPEED;
		}
		current_camera_position += distance;
	}
	Vec3 at( current_camera_position.x, current_camera_position.y, 0.0f );
	Vec3 up( 0.0f, 1.0f, 0.0f );
	D3DXMatrixLookAtLH( &mat_view, &current_camera_position, &at, &up );
}
//=======================================================
void Graphics::UpdateReflectionMatrices()
{
	Vec3 pos( current_camera_position.x, current_camera_position.y, current_camera_position.z );
	Vec3 up( 0.0f, -1.0f, 0.0f );
	Vec3 at( current_camera_position.x, current_camera_position.y, 0.0f );
	for ( unsigned int i = 0; i < reflectives.size() ; ++i )
	{
		pos.y = current_camera_position.y;
		pos.y -= 2.0f * ( current_camera_position.y - reflectives[i]->altitude );
		at.y = pos.y;
		D3DXMatrixLookAtLH( &reflectives[i]->mat_reflection, &pos, &at, &up );
	}
}
//=======================================================
int Graphics::RegisterReflectiveSurface( float altitude )
{
	unsigned int i;
	bool matched = false;
	for ( i = 0; i < reflectives.size(); ++i )
	{
		if ( abs(reflectives[i]->altitude - altitude) < 0.5f ) { matched = true; break; }
	}
	if ( matched ) return i;
	//else
	Reflective* reflective = new Reflective;
	reflective->altitude = altitude;
	reflective->reflection_view = new RenderTargetTexture;
	reflective->reflection_view->Initialize( device, screen_width, screen_height );
	reflectives.push_back( reflective );
	return reflectives.size() - 1;
}
//=======================================================
void Graphics::GetReflection( int index, ID3D10ShaderResourceView** srv )
{
	*srv = reflectives[index]->reflection_view->GetSRV();
}
//=======================================================
void Graphics::SetPrimaryLight( DirectionalLight* light )
{
	primary_light = light;
	Vec3 eye( 20.0f, 0.0f, -20.0f );
	Vec3 up( 0.0f, 1.0f, 0.0f );
	Vec3 at( 0.0f, 0.0f, 0.0f );
	D3DXMatrixLookAtLH( &primary_light->mat_shadow_view, &eye, &at, &up );

	D3DXMatrixMultiply( &primary_light->mat_shadow_view, &primary_light->mat_shadow_view, &mat_proj );
//	D3DXMatrixMultiply( &primary_light->mat_shadow_view, &mat_view, &mat_proj );
}
//=======================================================
void Graphics::PushCamera( Camera* camera )
{
	if ( camera ) camera_stack.push( camera );
}
//=======================================================
void Graphics::PopCamera()
{
	if ( !camera_stack.empty() ) camera_stack.pop();
}
/********************
*	particle system *
********************/
//=======================================================
void Graphics::InitializeParticleSystem()
{
	BuildRandomTexture();
	D3D10_BUFFER_DESC vb_desc;
//	D3D10_SUBRESOURCE_DATA vb_data;

	advance_particles = new Shader;
	advance_particles->Initialize( device, "shaders\\advanceParticles.fx", VERTEX_ELEMENT_POSITION0|VERTEX_ELEMENT_POSITION1|VERTEX_ELEMENT_POSITION2|VERTEX_ELEMENT_PER_VERTEX_FLAG );
	initial_position_bottomleft_ptr = advance_particles->GetVariableByName("initial_position_bottomleft")->AsVector();
	initial_position_topright_ptr = advance_particles->GetVariableByName("initial_position_topright")->AsVector();
	particle_radius_ptr = advance_particles->GetVariableByName("particle_radius")->AsScalar();
	initial_velocity_ptr = advance_particles->GetVariableByName("initial_velocity")->AsVector();
	velocity_deviation_ptr = advance_particles->GetVariableByName("velocity_deviation")->AsVector();
	life_span_ptr = advance_particles->GetVariableByName("life_span")->AsScalar();
	life_span_deviation_ptr = advance_particles->GetVariableByName("life_span_deviation")->AsScalar();
	global_time_ptr = advance_particles->GetVariableByName("global_time")->AsScalar();
	elapsed_time_ptr = advance_particles->GetVariableByName("elapsed_time")->AsScalar();
	gravity_ptr = advance_particles->GetVariableByName("gravity")->AsScalar();
	deactivating_ptr = advance_particles->GetVariableByName("deactivating")->AsScalar();
	random_texture_ptr = advance_particles->GetVariableByName("random_texture")->AsShaderResource();

	particle_shader = new Shader;
	particle_shader->Initialize( device, "shaders\\particleShader.fx", VERTEX_ELEMENT_POSITION0|VERTEX_ELEMENT_POSITION1|VERTEX_ELEMENT_POSITION2|VERTEX_ELEMENT_PER_VERTEX_FLAG );
	particle_texture_ptr = particle_shader->GetVariableByName("diffuse")->AsShaderResource();
	particle_mask_ptr = particle_shader->GetVariableByName("mask")->AsShaderResource();
	flag_ptr = particle_shader->GetVariableByName("flag")->AsScalar();

	ZeroMemory( &vb_desc, sizeof(vb_desc) );
	vb_desc.Usage = D3D10_USAGE_DYNAMIC;
	vb_desc.ByteWidth = sizeof( ParticleVertex );
	vb_desc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vb_desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	vb_desc.MiscFlags = 0;

	device->CreateBuffer( &vb_desc, NULL, &vb_launcher );
}
//=======================================================
void Graphics::RegisterParticleEmitter( ParticleEmitter* emitter )
{
	HRESULT hr;
	D3D10_BUFFER_DESC vb_desc;

	int max_vertices = emitter->max_particles;

	ZeroMemory( &vb_desc, sizeof( vb_desc ) );
	vb_desc.Usage = D3D10_USAGE_DEFAULT;
	vb_desc.ByteWidth = sizeof( ParticleVertex ) * max_vertices;
	vb_desc.BindFlags = D3D10_BIND_VERTEX_BUFFER | D3D10_BIND_STREAM_OUTPUT;
	vb_desc.CPUAccessFlags = 0;
	vb_desc.MiscFlags = 0;

	hr = device->CreateBuffer( &vb_desc, NULL, &emitter->vb[0] );
	ErrorIf( FAILED(hr), "failed building vertex buffer for particles" );

	hr = device->CreateBuffer( &vb_desc, NULL, &emitter->vb[1] );
	ErrorIf( FAILED(hr), "failed building vertex buffer for particles" );
	emitters.push_back( emitter );

	emitter->cur_target_buffer = 0;
}
//=======================================================
void Graphics::AdvanceParticles( float timeslice )
{
	device->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_POINTLIST );
	unsigned int stride = sizeof( ParticleVertex );
	unsigned int offset = 0;
	ID3D10Buffer* target;
	ID3D10Buffer* source;

	/// set per frame constants
	global_time_ptr->SetInt((unsigned int)timeGetTime());
	elapsed_time_ptr->SetFloat( timeslice );
	gravity_ptr->SetFloat( -3.0f );
	random_texture_ptr->SetResource( random_texture );

	/// loop through all emitters
	ParticleEmitterListType::iterator it;
	for ( it = emitters.begin(); it != emitters.end(); ++it )
	{
		/// emitter deactivating 
		if ( (*it)->deactivating )
		{
			(*it)->timer -= timeslice;
			if ( (*it)->timer < 0.0f ) (*it)->activated = false;
		}
		/// emitter inactivated
		if ( !(*it)->activated ) continue;

		/// set per emitter constants
		const Vec3& pos = (*it)->transform->Position;
		initial_position_bottomleft_ptr->SetFloatVector( pos + (*it)->offset_bottomleft );
		initial_position_topright_ptr->SetFloatVector( pos + (*it)->offset_topright );
		particle_radius_ptr->SetFloat( (*it)->radius );
		life_span_ptr->SetFloat( (*it)->life_span );
		life_span_deviation_ptr->SetFloat( (*it)->life_span_deviation );
		initial_velocity_ptr->SetFloatVector( (*it)->initial_velocity );
		velocity_deviation_ptr->SetFloatVector( (*it)->velocity_deviation );
		deactivating_ptr->SetBool( (*it)->deactivating );

		/// advance one frame
		/// if the emitter is new, set up the launcher
		advance_particles->Apply( device );	
		if ( (*it)->initializing )
		{
			ParticleVertex* launcher;
			vb_launcher->Map( D3D10_MAP_WRITE_DISCARD, 0, (void**)&launcher );
			launcher[0].type = PT_LAUNCHER | (*it)->type;
			launcher[0].v2_timer.x = (*it)->interval;	/// first element of v2_timer will store the interval
			launcher[0].v2_timer.z = 0.0f;				/// third element is the timer
			launcher[0].velocity.x = (*it)->particles_per_millisecond;
			vb_launcher->Unmap();
			target = (*it)->vb[(*it)->cur_target_buffer];
			device->IASetVertexBuffers( 0, 1, &vb_launcher, &stride, &offset );
			device->SOSetTargets( 1, &target, &offset );
			device->Draw( 1, 0 );
			(*it)->initializing = false;
		}
		else
		{
			source = (*it)->vb[(*it)->cur_target_buffer];
			target = (*it)->vb[1 - (*it)->cur_target_buffer];
			device->IASetVertexBuffers( 0, 1, &source, &stride, &offset );
			device->SOSetTargets( 1, &target, &offset );
//	advance_particles->Apply( device );	
			device->DrawAuto();
			(*it)->cur_target_buffer = 1 - (*it)->cur_target_buffer;
		}
	}//end for
	target = NULL;
	device->SOSetTargets( 1, &target, &offset );
}
//=======================================================
void Graphics::RenderParticles( Mat4& p_mat_view, Mat4& p_mat_proj )
{
	particle_shader->SetTransformationConstants( device, p_mat_view, p_mat_proj );

	ParticleEmitterListType::iterator it;
	for ( it = emitters.begin(); it != emitters.end(); ++it )
	{
		if ( !(*it)->activated ) continue;
		d3dmanager->SetDepthEnable( false );
		if ( (*it)->blend_method == BLEND_ADD_DIFFUSE )
			d3dmanager->SetBlendAddDiffuse();
		else if ( (*it)->blend_method == BLEND_SUB_DIFFUSE )
			d3dmanager->SetBlendSubDiffuse();
		else 
		{
			d3dmanager->SetBlendDisable();
			d3dmanager->SetDepthEnable( true );
		}

		if ( (*it)->texture )
		{
			particle_texture_ptr->SetResource( (*it)->texture->GetDiffuse() );
			particle_mask_ptr->SetResource( (*it)->texture->GetMask() );
		}
		flag_ptr->SetInt( (*it)->flag );
		particle_shader->Apply( device );
		uint stride = sizeof(ParticleVertex);
		uint offset = 0;
		device->IASetVertexBuffers( 0, 1, &(*it)->vb[(*it)->cur_target_buffer], &stride, &offset );
		device->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_POINTLIST );
		device->DrawAuto();//(MAX_PARTICLES_PER_EMITTER, 0 );
	}
	d3dmanager->SetDepthEnable( true );
}
//=======================================================
void Graphics::BuildRandomTexture()
{
	int num_random_values = 2048;

    D3D10_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = new float[num_random_values * 4];
    InitData.SysMemPitch = num_random_values * 4 * sizeof( float );
    InitData.SysMemSlicePitch = num_random_values * 4 * sizeof( float );
    for( int i = 0; i < num_random_values * 4; i++ )
    {
        ( ( float* )InitData.pSysMem )[i] = Rand( 0.0f, 1.0f );
    }

	ID3D10Texture1D* texture;
	D3D10_TEXTURE1D_DESC dstex;
	dstex.Width = num_random_values;
	dstex.MipLevels = 1;
	dstex.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	dstex.Usage = D3D10_USAGE_DEFAULT;
	dstex.BindFlags = D3D10_BIND_SHADER_RESOURCE;
	dstex.CPUAccessFlags = 0;
	dstex.MiscFlags = 0;
	dstex.ArraySize = 1;
	device->CreateTexture1D( &dstex, &InitData, &texture );
	delete [] InitData.pSysMem;

	// Create the resource view
	D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory( &SRVDesc, sizeof( SRVDesc ) );
	SRVDesc.Format = dstex.Format;
	SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE1D;
	SRVDesc.Texture2D.MipLevels = dstex.MipLevels;
	device->CreateShaderResourceView( texture, &SRVDesc, &random_texture );

	S_RELEASE( texture );
}
/********************
*	debug rendering *
********************/
//=======================================================
void Graphics::RenderLineList()
{
	if ( line_list == NULL ) return;
	DebugPrimitive* mapped_data;
	
	debug_shader->SetTransformationConstants( device, mat_view, mat_proj );
	debug_vertex_buffer->Map( D3D10_MAP_WRITE_DISCARD, NULL, (void**)&mapped_data );
	int line_count = 0;
	
	LineListType::iterator it;
	for ( it = line_list->begin(); it != line_list->end(); ++it )
	{
		mapped_data[line_count].v0 = Vec3( (*it).start.x, (*it).start.y, 0.0f );
		mapped_data[line_count].v1 = Vec3( (*it).end.x - (*it).start.x, (*it).end.y - (*it).start.y, 0.0f );
		mapped_data[line_count].type = DEBUG_SHAPE_LINESEGMENT;
		mapped_data[line_count].color = Vec4( 0.0f,0.0f,0.0f,1.0f );
		++line_count;
	}

	debug_vertex_buffer->Unmap();
	unsigned int stride = sizeof( DebugPrimitive );
	unsigned int offset = 0;
	device->IASetVertexBuffers( 0, 1, &debug_vertex_buffer, &stride, &offset );
	device->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_POINTLIST );

	debug_shader->Apply( device );
	device->Draw( line_count, 0 );

}
//=======================================================
void Graphics::InitializeDebugDrawing()
{
	D3D10_BUFFER_DESC vb_desc;
	HRESULT hr;
	ZeroMemory( &vb_desc, sizeof( vb_desc ) );
	vb_desc.Usage = D3D10_USAGE_DYNAMIC;
	vb_desc.ByteWidth = sizeof( DebugPrimitive ) * MAX_DEBUG_PRIMITIVES;
	vb_desc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vb_desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	vb_desc.MiscFlags = 0;
	hr = device->CreateBuffer( &vb_desc, NULL, &debug_vertex_buffer );
	if ( FAILED(hr) ) exit(0);/////ErrorIf
	
	debug_shader = new Shader;
	debug_shader->Initialize( device, "Shaders\\debugShader.fx", 
					VERTEX_ELEMENT_POSITION0|VERTEX_ELEMENT_NORMAL|VERTEX_ELEMENT_COLOR|VERTEX_ELEMENT_PER_VERTEX_FLAG );
}
//=======================================================
void Graphics::RenderDebug()
{
	DebugPrimitive* mapped_data;
	
	debug_shader->SetTransformationConstants( device, mat_view, mat_proj );
	debug_vertex_buffer->Map( D3D10_MAP_WRITE_DISCARD, NULL, (void**)&mapped_data );
	int primitive_count = 0;
	DebugGeometryListType::iterator g_it;
	for ( g_it = debug_geometry_list.begin(); g_it != debug_geometry_list.end(); ++g_it )
	{
		DebugGeometry::DebugShapeListType::iterator primitive_it;
		DebugGeometry::DebugShapeListType& primitive_list = (*g_it)->debug_shapes;
		for ( primitive_it = primitive_list.begin(); primitive_it != primitive_list.end(); ++primitive_it )
		{
			mapped_data[primitive_count].type = (int)(*primitive_it).second->type;
			mapped_data[primitive_count].v0 = (*g_it)->transform->Position;
			mapped_data[primitive_count].v1 = (*primitive_it).second->param;
			mapped_data[primitive_count].color = (*primitive_it).second->color;
			++primitive_count;
		}
	}
	debug_vertex_buffer->Unmap();
	unsigned int stride = sizeof( DebugPrimitive );
	unsigned int offset = 0;
	device->IASetVertexBuffers( 0, 1, &debug_vertex_buffer, &stride, &offset );
	device->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_POINTLIST );

	debug_shader->Apply( device );
	device->Draw( primitive_count, 0 );
}
//=======================================================
void Graphics::RegisterDebugGeometry( DebugGeometry* geometry )
{
	debug_geometry_list.push_back( geometry );
}
//=======================================================
void Graphics::RemoveDebugGeometry( DebugGeometry* geometry )
{
	DebugGeometryListType::iterator g_it;
	for ( g_it = debug_geometry_list.begin(); g_it != debug_geometry_list.end(); ++g_it )
	{
		if ( (*g_it) == geometry ) debug_geometry_list.erase(g_it);
	}
	
}
//=======================================================
void Graphics::ScreenToWorldSpace( Vec2* out, Vec2* in )
{
	Vec3 temp( in->x, in->y, 0.0f );
	Mat4 i;
	D3DXMatrixIdentity( &i );
	D3DXVec3Unproject( &temp, &temp, d3dmanager->GetViewport(), &mat_proj, &mat_view, &i );
	
	float scale = -(current_camera_position.z )/ FRONT;
	temp.x -= current_camera_position.x;
	temp.y -= current_camera_position.y;
	temp.x *= scale;
	temp.y *= scale;
	temp.z = 0.0f;
	temp.x += current_camera_position.x;
	temp.y += current_camera_position.y;

	out->x = temp.x;
	out->y = temp.y;
}
/********************
*	font engine		*
********************/
//=======================================================
void Graphics::RegisterFont( const char* font_name )
{
	Font* font = new Font;
	std::string name = "Assets\\fonts\\";
	name += font_name;
	std::string img_file,dat_file;
	img_file = name; img_file += ".bmp";
	dat_file = name; dat_file += ".dat";
	font->Initialize( device, img_file.c_str(), dat_file.c_str() );
	font_map[font_name] = font;
}
//=======================================================
void Graphics::RegisterText( Text* text )
{
	Font::BuildVertexBuffer( device, &text->vertex_buffer );
	text_list.push_back( text );
}
//=======================================================
void Graphics::RenderText()
{
	font_shader->SetTransformationConstants( device, mat_ortho_view, mat_ortho_proj );
	device->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_POINTLIST );

	font_shader->Apply( device );
	TextListType::iterator it;
	for ( it = text_list.begin(); it != text_list.end(); ++it )
	{
		Font* font = font_map[(*it)->font];
		if ( (*it)->changed ) 
		{
			if (!font) continue;
			font->UpdateVertexBuffer( (*it ) );
		}
		unsigned int stride = font->GetVertexTypeSize();
		unsigned int offset = 0;
		device->IASetVertexBuffers( 0, 1, &(*it)->vertex_buffer, &stride, &offset );
		font_img_ptr->SetResource( font->GetFontTexture() );
		
		device->Draw( (*it)->text.size(), 0 );
	}
}
/*************************************
*	message processing(debug purpose)*
**************************************/
//======================================
void Graphics::SendMessage( Message* msg )
{
	if ( msg->MessageId == Mid::CharacterKey )
	{	
		MessageCharacterKey* m = (MessageCharacterKey*)msg;
		if ( m->character == 'v' ) 
		{
			ParticleEmitterListType::iterator it;
			static bool k = true;
			for ( it = emitters.begin(); it != emitters.end(); ++it )
			{
				(*it)->Activate( k );
			}
			k = !k;
		}
		else
		{
			Camera* camera = NULL;
			if ( !camera_stack.empty() ) camera = camera_stack.top();
			if ( camera )
			{
				Vec3* pos = &(camera->transform->Position);
				if ( m->character == 'a' ) pos->x -= 0.1f;
				if ( m->character == 's' ) pos->y -= 0.1f;
				if ( m->character == 'd' ) pos->x += 0.1f;
				if ( m->character == 'w' ) pos->y += 0.1f;
			}
		}
	}
//	printf("xx\n");
}

void Graphics::RegisterDebug( Transform* transform,  DebugShapeType shape, Vec3 dim,Vec4 col,char*  name) 
{
	test = new DebugGeometry();
	test->SetTransform( transform );
	test->AddDebugShape( shape, dim, col, name );
	//test->AddDebugShape( DEBUG_SHAPE_ARROW, Vec3(2,0,-1), Vec4(0,0,0,1), "arrow" );
}
}//end namespace
