#pragma once
#include <ctime>


//NameSpace que almacena IDs relacionados con los sistemas y los componentes
namespace ecs {
	using CmpIdType = std::size_t;
	using GrpIdType = std::size_t;
	using HdlrIdType = std::size_t;
	using SysIdType = std::size_t;

	enum CmpId : CmpIdType {
		Transform = 0,
		ImageComponent,
		AnimatedImageComponent,
		GameState,
		//
		// don't touch the rest of this enum, it is to identify the max
		// number of components
		_LastCmptId_
	};

	enum SysId : SysIdType {
		_sys_GameCtrl,
		_sys_Tron,
		_sys_Render,
		_sys_Audio,
		_sys_Socket,
		//
		// don't touch the rest of this enum, it is to identify the max
		// number of systems
		_LastSysId_
	};

	constexpr std::size_t maxComponents = _LastCmptId_;
	constexpr std::size_t maxSystems = _LastSysId_;

	// these two should be used to get a component via the field
	// entity_
#define GETCMP2_(id,type)  GETCMP3(entity_,id,type)
#define GETCMP1_(type)     GETCMP3(entity_,ecs::type,type)

// these two should be used to get a component via an
// entity e provided as a parameter
#define GETCMP2(e,type) GETCMP3(e,ecs::type,type)
#define GETCMP3(e,id,type) e->getComponent<type>(id)
}
