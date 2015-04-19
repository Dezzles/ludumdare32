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

	PROTECTED_FIELD( float, Score );

private:
	Text* ScoreText_;
};



#endif