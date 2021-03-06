#ifndef ScoreComponent_HPP
#define ScoreComponent_HPP

#include "Bubblewrap/Render.hpp"
#include "Bubblewrap/Base.hpp"

using namespace Bubblewrap;
using namespace Render;
class ScoreComponent 
	: public Bubblewrap::Base::Component
{
public:
	ScoreComponent();
	void Initialise( Json::Value Params );

	CREATE_REGISTER( ScoreComponent );

	void OnAttach();
	void OnDetach();

	PROTECTED_FIELD( float, Score );

	void InputFunction( Bubblewrap::Events::Event* Event );
private:
	Text* ScoreText_;
	Events::EventHandle InputHandle_;
};



#endif