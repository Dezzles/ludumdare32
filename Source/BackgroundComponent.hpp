#ifndef BackgroundComponent_HPP
#define BackgroundComponent_HPP

#include "Bubblewrap/Render.hpp"
#include "Bubblewrap/Base.hpp"

using namespace Bubblewrap;
using namespace Render;
class BackgroundComponent 
	: public Bubblewrap::Base::Component
{
public:
	BackgroundComponent();
	void Initialise( Json::Value Params );

	CREATE_REGISTER( BackgroundComponent );

	void OnAttach();

	PROTECTED_FIELD( float, BackgroundSize );
	PROTECTED_FIELD( float, Speed );
	PROTECTED_FIELD( float, GapShrink );

	virtual void Update( float dt );
private:
	Vertices* VertexList;
	std::vector<Base::Entity*> Entities_;
	void CreateMap();

};



#endif