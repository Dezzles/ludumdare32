#ifndef MainMenuComponent_HPP
#define MainMenuComponent_HPP

#include "Bubblewrap/Render.hpp"
#include "Bubblewrap/Base.hpp"

using namespace Bubblewrap;
using namespace Render;
class MainMenuComponent 
	: public Bubblewrap::Base::Component
{
public:
	MainMenuComponent();
	void Initialise( Json::Value Params );

	CREATE_REGISTER( MainMenuComponent );

	void OnAttach();
	void OnDetach();

	void InputFunction( Bubblewrap::Events::Event* Event );
private:
 	Events::EventHandle InputHandle_;
};



#endif