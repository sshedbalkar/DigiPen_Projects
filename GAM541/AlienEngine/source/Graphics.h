///////////////////////////////////////////////////////////////////////////////
///
///	\file Graphics.h
/// Defines grahics system.
///	
///	Authors: Benjamin Ellinger, Chris Peters
///	Copyright 2010, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////
#pragma once //Makes sure this header is only included once

#include "VMath.h"
#include "Engine.h"
#include "Sprite.h"
#include <deque>
#include <list>
#include <stack>
#include <map>
#include "debugGeometry.h"
#include "material.h"

namespace Framework
{	
//Forward Declaration of Graphics Objects
class Camera;
class ParticleEmitter;
class Shader;
class D3DManager;
class Mesh;
class Animation;
class Texture;
class Transform;
class Font;
class Text;
class RenderTargetTexture;
class Material;
class DirectionalLight;

class DebugGeometry;/// for debugging


class Graphics : public ISystem
{
private:
	/// 
	struct Reflective
	{
		float altitude;
		Mat4 mat_reflection;
		RenderTargetTexture* reflection_view;
	};
	/// for debug drawing
	struct DebugPrimitive
	{
		Vec3 v0;
		Vec3 v1;
		Vec4 color;
		int type;
	};
	///particle vertex format
	struct ParticleVertex
	{
		Vec3 position;
		Vec3 velocity;
		Vec3 v2_timer;		//extra information for particle, the meaning varies for different types
		unsigned int type;
	};
	typedef std::vector<Reflective*> ReflectiveListType;
	typedef std::list<DebugGeometry*> DebugGeometryListType;
	typedef std::map<std::string,Font*> FontMapType;
	typedef std::list<Text*> TextListType;
	typedef std::map<MaterialType,Material*> MaterialMapType;
	typedef std::map<std::string,Mesh*> MeshMapType;
	typedef std::map<std::string,Animation*> AnimationMapType;
	typedef std::map<std::string,Texture*> TextureMapType;
	typedef std::list<ParticleEmitter*> ParticleEmitterListType;

private:
	D3DManager* d3dmanager;
	ID3D10Device* device;
	ID3D10RenderTargetView* render_target_screen;
	ID3D10ShaderResourceView* srv_screen;
	ID3D10DepthStencilView* depth_stencil_view;
	IDXGISwapChain* swap_chain;
	RenderTargetTexture* render_target_raw[3];
	
	int screen_width;
	int screen_height;
	HWND hwnd;
	bool full_screen;
	/// reflections
	ReflectiveListType reflectives;
	/// static mesh shader
	Shader* mesh_shader;
	/// deferred shader
	Shader* deferred_shader;
	ID3D10EffectShaderResourceVariable* d_diffuse_ptr, *d_normal_flag_ptr, *d_view_position_ptr;
	ID3D10EffectVectorVariable* deferred_viewport_ptr;
	RenderTargetTexture* render_target_shaded;
	/// post processing shader
	Shader* post_processing_shader;
	ID3D10EffectShaderResourceVariable* p_color_ptr, *p_normal_flag_ptr, *p_view_position_ptr;
	ID3D10EffectShaderResourceVariable* previous_frame_ptr;
	ID3D10EffectVectorVariable* post_processing_viewport_ptr;
	ID3D10EffectScalarVariable* frametime_ptr;
	RenderTargetTexture* render_target_processed[2];
	int cur_pp_target;
	/// fxaa shader
	Shader* fxaa_shader;
	ID3D10EffectVectorVariable* fxaa_viewport_ptr;
	ID3D10EffectShaderResourceVariable* processed_texture_ptr;
	/// material shaders
	Shader* grass_shader;
	Shader* tile_shader;
	Shader* textured_box_shader;
	/// light source
	DirectionalLight* primary_light;
	
	///transformations
	Mat4 mat_view;
	Mat4 mat_proj;
	Mat4 mat_ortho_view;
	Mat4 mat_ortho_proj;
	///camera control
	Vec3 current_camera_position;
	std::stack<Camera*> camera_stack;

	///particle systems
	ParticleEmitterListType emitters;
	ID3D10Buffer* vb_launcher;
	ID3D10ShaderResourceView* random_texture;
	Shader* advance_particles;
	ID3D10EffectVectorVariable* initial_position_bottomleft_ptr;
	ID3D10EffectVectorVariable* initial_position_topright_ptr;
	ID3D10EffectScalarVariable* particle_radius_ptr;
	ID3D10EffectVectorVariable* initial_velocity_ptr;
	ID3D10EffectVectorVariable* velocity_deviation_ptr;
	ID3D10EffectScalarVariable* life_span_ptr;
	ID3D10EffectScalarVariable* life_span_deviation_ptr;
	ID3D10EffectScalarVariable* global_time_ptr;
	ID3D10EffectScalarVariable* elapsed_time_ptr;
	ID3D10EffectScalarVariable* gravity_ptr;
	ID3D10EffectScalarVariable* deactivating_ptr;
	ID3D10EffectShaderResourceVariable* random_texture_ptr;

	Shader* particle_shader;
	ID3D10EffectScalarVariable* flag_ptr;
	ID3D10EffectShaderResourceVariable* particle_texture_ptr;
	ID3D10EffectShaderResourceVariable* particle_mask_ptr;

	/// font engine elements
	Shader* font_shader;
	FontMapType font_map;
	TextListType text_list;
	Text* test_text;
	ID3D10EffectShaderResourceVariable* font_img_ptr;

	/// materials
	MaterialMapType material_map;
	/// mesh map
	MeshMapType mesh_map;
	/// animation map
	AnimationMapType animation_map;
	/// texture map
	TextureMapType texture_map;

	/// for debugging
	Shader* debug_shader; /// debug shader
	ID3D10InputLayout* debug_input_layout;
	DebugGeometryListType debug_geometry_list; 
	ID3D10Buffer* debug_vertex_buffer;
	bool render_debug_information;
	LineListType* line_list;

	//testing
	DebugGeometry* test;

public:
	Graphics();
	virtual ~Graphics();
	virtual std::string GetName() { return "Graphics";};	

	void SetWindowProperties( HWND _hwnd, int w, int h, bool _full_screen) { hwnd = _hwnd; screen_width = w; screen_height = h; full_screen = _full_screen; }

	//// basic game functions
	virtual void Initialize();
	virtual void Update( float timeslice );
	virtual void Free();
	virtual void Load();
	virtual void Unload();

	virtual void SendMessage( Message* msg );

	//// transform
	void ScreenToWorldSpace( Vec2* out, Vec2* in );
	//// set lighting
	void SetPrimaryLight( DirectionalLight* light );
	//// camera
	void PushCamera(Camera*);
	void PopCamera();
	//// reflection
	int RegisterReflectiveSurface( float altitude );
	void GetReflection( int index, ID3D10ShaderResourceView** srv );
	////particle system
	void RegisterParticleEmitter( ParticleEmitter* emitter );
	//// debug geometry adding/deleting functions
	void RegisterDebugGeometry( DebugGeometry* geometry );
	void RemoveDebugGeometry( DebugGeometry* geometry );
	void RegisterDebug( Transform* transform,  DebugShapeType shapetype, Vec3 dim, Vec4 col, char*  name); //For Testing Debug Draw
	void RegisterLineList( LineListType* _line_list ) {line_list = _line_list;}

	//// font engine
	void RegisterFont( const char* font_name );
	void RegisterText( Text* text );

	//// surface material
	Material* GetMaterial( MaterialType id ) { if ( id == MAT_NONE ) return NULL; return material_map[id]; }
	/// get assets
	Mesh* GetMesh( std::string& id ) { return mesh_map[id]; }
	Animation* GetAnimation( std::string& id ) { return animation_map[id]; }
	Texture* GetTexture( std::string& id ) { return texture_map[id]; }

private:
	void UpdateProjectionAndOrthoMatrices();
	void UpdateViewingMatrix();
	void UpdateReflectionMatrices();

	void LoadAssets();
	////particle system
	void InitializeParticleSystem();
	void BuildRandomTexture();
	void AdvanceParticles( float timeslice );
	void RenderParticles( Mat4& p_mat_view, Mat4& p_mat_proj );
	////font engine
	void RenderText();
	//// debug drawing function
	void RenderDebug();
	void InitializeDebugDrawing();
	void RenderLineList();

};

//A global pointer to the Graphics system, used to access it anywhere.
extern Graphics* GRAPHICS;
}
