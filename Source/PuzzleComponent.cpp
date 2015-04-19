#include "PuzzleComponent.hpp"
#include "EnemyComponent.hpp"
#include "LevelComponent.hpp"
#include "Bubblewrap/Events.hpp"
using namespace Math;

PuzzleComponent::PuzzleComponent()
{
	TileSize_ = 40;
	for ( int i = 0; i < MaxMapSize; ++i )
	{
		for ( int j = 0; j < MaxMapSize; ++j )
		{
			MapInfo_[ i ][ j ].Type_ = LocationType::Wall;
		}
	}

	/*for ( int i = 0; i < MaxMapSize; ++i )
	{
		for ( int j = 0; j < MaxMapSize; ++j )
		{
			MapInfo_[ i ][ j ].Type_ = LocationType::Empty;
		}
	}/**/

	TileData_[ LocationType::Empty ] = TileData( 0.00f, 0.25f, 0.50f, 0.75f );
	TileData_[ LocationType::Player ] = TileData( 0.25f, 0.50f, 0.25f, 0.50f );
	TileData_[ LocationType::Safe ] =	TileData( 0.75f, 1.00f, 0.00f, 0.25f );
	TileData_[ LocationType::Death ] =	TileData( 0.50f, 0.75f, 0.00f, 0.25f );
	TileData_[ LocationType::Wall ] =	TileData( 0.00f, 0.25f, 0.00f, 0.25f );
	TileData_[ LocationType::Death | LocationType::Player] = TileData( 0.50f, 0.75f, 0.25f, 0.50f );

	EnemyObject_ = nullptr;

	LevelObject_ = nullptr;
}

void PuzzleComponent::Initialise( Json::Value Params )
{
	Component::Initialise( Params );
	
	REQUIRED_LOAD( Float, TileSize, tileSize );
	REQUIRED_LOAD( Int, Height, height );
	REQUIRED_LOAD( Int, Width, width );
	REQUIRED_LOAD( Float, MovementSpeed, movementSpeed );
	REQUIRED_LOAD( Int, MaxMapDifficulty, maxMapDifficulty );
	if ( !Params[ "backgroundColour" ].isNull() )
	{
		BackgroundColour_ = Colour( Params[ "backgroundColour" ].asString() );
	}
}


void PuzzleComponent::Copy( PuzzleComponent* Target, PuzzleComponent* Base )
{
	Component::Copy( Target, Base );
	NAIVE_COPY( TileSize );
	NAIVE_COPY( Width );
	NAIVE_COPY( Height );
	NAIVE_COPY( BackgroundColour );
	NAIVE_COPY( MovementSpeed );
	NAIVE_COPY( MaxMapDifficulty );
}

void PuzzleComponent::OnAttach()
{
	Sound_ = GetParentEntity()->GetComponentsByType< Bubblewrap::Audio::Sound >()[ 0 ];
	// DoCreateMap( 4, 5);
	InputHandle_ = GetManager().GetEventManager().RegisterEvent( Events::EventTypes::Input, std::bind( &PuzzleComponent::InputFunction, this, std::placeholders::_1 ) );
}

void PuzzleComponent::DoCreateMap( int width, int height, int difficulty )
{
	Difficulty_ = difficulty;
	Width_ = width > MaxMapSize ? MaxMapSize : width;
	Height_ = height > MaxMapSize ? MaxMapSize : height;
	CreateMaze();
	GenerateMap();
	CreateMap();
}

void PuzzleComponent::CreateMap()
{
	Vertices* vertices = GetParentEntity()->GetComponentsByType<Vertices>( "BorderVertices" )[ 0 ];
	float height = TileSize_ * Height_;
	float width = TileSize_ * Width_;

	RenderWidth_ = width;
	RenderHeight_ = height;
	for ( int i = 0; i < Width_ + 1; ++i )
	{
		Vertex v;
		v.Position_ = Math::Vector3f( i * TileSize_, 0, 0 );
		v.Colour_ = Colour( ( unsigned char ) 255, 255, 255 );
		vertices->AddVertex( v );

		v.Position_ = Math::Vector3f( i * TileSize_, height, 0 );
		vertices->AddVertex( v );
	}
	for ( int i = 0; i < Height_ + 1; ++i )
	{
		Vertex v;
		v.Position_ = Math::Vector3f( 0, i * TileSize_, 0 );
		v.Colour_ = Colour( ( unsigned char ) 255, 255, 255 );
		vertices->AddVertex( v );

		v.Position_ = Math::Vector3f( width, i * TileSize_, 0 );
		vertices->AddVertex( v );
	}
	vertices->Refresh();

	vertices = GetParentEntity()->GetComponentsByType<Vertices>( "FillVertices" )[ 0 ];
	{
		Vertex v;
		v.Position_ = Math::Vector3f( 0, 0, 0 );
		v.Colour_ = BackgroundColour_;
		vertices->AddVertex( v );

		v.Position_ = Math::Vector3f( 0, height, 0 );
		v.Colour_ = BackgroundColour_;
		vertices->AddVertex( v );

		v.Position_ = Math::Vector3f( width, 0, 0 );
		v.Colour_ = BackgroundColour_;
		vertices->AddVertex( v );

		v.Position_ = Math::Vector3f( width, height, 0 );
		v.Colour_ = BackgroundColour_;
		vertices->AddVertex( v );
	}
	vertices = GetParentEntity()->GetComponentsByType<Vertices>( "TileVertices" )[ 0 ];
	for ( int i = 0; i < Width_; ++i )
	{
		for ( int j = 0; j < Height_; ++j )
		{
			Vertex v1, v2;
			v1.Colour_ = Colour( 1.0f, 1.0f, 1.0f );
			v2.Colour_ = Colour( 1.0f, 1.0f, 1.0f );
			v1.Position_ = Math::Vector3f( i * TileSize_, j * TileSize_, 0.0f );
			MapInfo_[ i ][ j ].StartPoint_ = vertices->AddVertex( v1 );
			v1.Position_ = Math::Vector3f( ( i + 1 ) * TileSize_, ( j + 0 ) * TileSize_, 0.0f );
			v2.Position_ = Math::Vector3f( ( i + 0 ) * TileSize_, ( j + 1 ) * TileSize_, 0.0f );
			vertices->AddVertex( v1 );
			vertices->AddVertex( v2 );
			vertices->AddVertex( v1 );
			vertices->AddVertex( v2 );
			v2.Position_ = Math::Vector3f( ( i + 1 ) * TileSize_, ( j + 1 ) * TileSize_, 0.0f );
			vertices->AddVertex( v2 );
		}
	}
	UpdateTexCoords();
	vertices->Refresh();
}

void PuzzleComponent::UpdateTexCoords()
{
	Vertices* vertices = GetParentEntity()->GetComponentsByType<Vertices>( "TileVertices" )[ 0 ];
	for ( int i = 0; i < Width_; ++i )
	{
		for ( int j = 0; j < Height_; ++j )
		{
			TileData data = TileData_[ MapInfo_[ i ][ j ].Type_ ];
			vertices->EditVertex( MapInfo_[ i ][ j ].StartPoint_ + 0 ).TexCoords_ = Math::Vector2f( data.Left_, data.Top_ );
			vertices->EditVertex( MapInfo_[ i ][ j ].StartPoint_ + 1 ).TexCoords_ = Math::Vector2f( data.Right_, data.Top_ );
			vertices->EditVertex( MapInfo_[ i ][ j ].StartPoint_ + 2 ).TexCoords_ = Math::Vector2f( data.Left_, data.Bottom_ );
			vertices->EditVertex( MapInfo_[ i ][ j ].StartPoint_ + 3 ).TexCoords_ = Math::Vector2f( data.Right_, data.Top_ );
			vertices->EditVertex( MapInfo_[ i ][ j ].StartPoint_ + 4 ).TexCoords_ = Math::Vector2f( data.Left_, data.Bottom_);
			vertices->EditVertex( MapInfo_[ i ][ j ].StartPoint_ + 5 ).TexCoords_ = Math::Vector2f( data.Right_, data.Bottom_);
		}
	}
	vertices->Refresh();
}

PuzzleComponent::PositionInfo PuzzleComponent::GetPositionInfo( int X, int Y )
{
	if ( ( X < 0 ) || ( X >= Width_ ) || ( Y < 0 ) || ( Height_ <= Y ) )
	{
		PositionInfo ret;
		ret.Type_ = LocationType::Wall;
		return ret;
	}
	return MapInfo_[ X ][ Y ];
}

void PuzzleComponent::GenerateMap()
{
	int DangerSpots = 1 + Difficulty_;
	if ( Difficulty_ > MaxMapDifficulty_ )
	{
		Difficulty_ = MaxMapDifficulty_;
	}
	int posx = rand() % Width_;
	int posy = rand() % Height_;
	while ( MapInfo_[ posx ][ posy ].Type_ != LocationType::Empty )
	{
		posx = rand() % Width_;
		posy = rand() % Height_;
	}
	MapInfo_[ posx ][ posy ].Type_ = LocationType::Player;

	posx = rand() % Width_;
	posy = rand() % Height_;
	while ( MapInfo_[ posx ][ posy ].Type_ != LocationType::Empty )
	{
		posx = rand() % Width_;
		posy = rand() % Height_;
	}
	MapInfo_[ posx ][ posy ].Type_ = LocationType::Safe;

	struct Pos { int x; int y; };

	std::vector<Pos> walls;
	for ( int Idx1 = 0; Idx1 < Width_; ++Idx1 )
	{
		for ( int Idx2 = 0; Idx2 < Height_; ++Idx2 )
		{
			if ( MapInfo_[ Idx1 ][ Idx2 ].Type_ == LocationType::Wall )
			{
				Pos p;
				p.x = Idx1;
				p.y = Idx2;
				walls.push_back( p );
			}
		}
	}

	while ( walls.size() > Difficulty_ )
	{
		int r = rand() % walls.size();
		MapInfo_[ walls[ r ].x ][ walls[ r ].y ].Type_ = LocationType::Empty;
		walls.erase( walls.begin() + r );
	}
	walls.clear();

	for ( int Idx1 = 0; Idx1 < Width_; ++Idx1 )
	{
		for ( int Idx2 = 0; Idx2 < Height_; ++Idx2 )
		{
			if ( MapInfo_[ Idx1 ][ Idx2 ].Type_ == LocationType::Empty )
			{
				Pos p;
				p.x = Idx1;
				p.y = Idx2;
				walls.push_back( p );
			}
		}
	}
	int targetPieces = Difficulty_;
	while ( ( targetPieces > 0 ) && ( walls.size() > 0 ) )
	{
		int u = rand() % walls.size();
		MapInfo_[ walls[ u ].x ][ walls[ u ].y ].Type_ = LocationType::Death;
		walls.erase( walls.begin() + u );
		--targetPieces;
	}
	/*posx = rand() % Width_;
	posy = rand() % Height_;
	for ( int Idx = 0; Idx < Difficulty_; ++Idx )
	{
		while ( MapInfo_[ posx ][ posy ].Type_ != LocationType::Empty )
		{
			posx = rand() % Width_;
			posy = rand() % Height_;
		}
		MapInfo_[ posx ][ posy ].Type_ = LocationType::Death;
	}/**/
}

void PuzzleComponent::InputFunction( Bubblewrap::Events::Event* Event )
{
	Bubblewrap::Events::InputData data = *( Bubblewrap::Events::InputData* )Event->GetData();
	int xMod = 0;
	int yMod = 0;
	if (data.InputType_ == Events::InputData::InputType::KeyDown )
	switch ( data.Key_ )
	{
	case Events::Key::W:
		yMod = -1;
		break;
	case Events::Key::S:
		yMod = 1;
		break;
	case Events::Key::A:
		xMod = -1;
		break;
	case Events::Key::D:
		xMod = 1;
		break;
	}

	int playerX = 0;
	int playerY = 0;
	bool playerFound = false;
	for ( playerX = 0; playerX < Width_; ++playerX )
	{
		for ( playerY = 0; playerY < Height_; ++playerY )
		{
			if ( MapInfo_[ playerX ][ playerY ].Type_ & LocationType::Player )
			{
				playerFound = true;
				break;
			}
		}
		if ( playerFound )
			break;
	}

	if ( GetPositionInfo( playerX + xMod, playerY + yMod ).Type_ != LocationType::Wall )
	{
		MapInfo_[ playerX ][ playerY ].Type_ = (LocationType)( MapInfo_[ playerX ][ playerY ].Type_ - ( LocationType::Player ));
		MapInfo_[ playerX + xMod ][ playerY + yMod ].Type_ = (LocationType)(LocationType::Player | MapInfo_[ playerX + xMod ][ playerY + yMod ].Type_);
		EnemyObject_->SetGoFast( false );
		if ( MapInfo_[ playerX + xMod ][ playerY + yMod ].Type_ == ( LocationType ) ( LocationType::Player | LocationType::Death ) )
		{
			EnemyObject_->SetGoFast( true );
			if ( Sound_ != nullptr )
				Sound_->Play();
		}
		if ( MapInfo_[ playerX + xMod ][ playerY + yMod ].Type_ == ( LocationType ) ( LocationType::Player | LocationType::Safe ) )
		{
			Base::Entity* explosion = dynamic_cast<Base::Entity*>( GetRegister().LoadObject( "basics:Explosion", GetParentEntity()->GetParentEntity() ) );
			explosion->SetLocalPosition( EnemyObject_->GetParentEntity()->LocalPosition() );
			EnemyObject_->GetParentEntity()->Destroy();
			GetParentEntity()->Destroy();
			LevelObject_->RemovePuzzle( GetParentEntity() );
		}

	}
	UpdateTexCoords();
	
}

void PuzzleComponent::OnDetach()
{
	InputHandle_.Destroy();
}

void PuzzleComponent::Update( float dt )
{
	auto pos = GetParentEntity()->LocalPosition();
	float x = pos.X();
	x += dt * MovementSpeed_;
	if ( x > TargetXCoord_ )
		x = TargetXCoord_;
	if ( x > MaxPosition_ )
		x = pos.X();
	pos.SetX( x );
	GetParentEntity()->SetLocalPosition( pos );
}

void PuzzleComponent::CreateMaze()
{
	struct Pos { int x; int y; };

	Pos start;
	start.x = rand() % Width_;
	start.y = rand() % Height_;

	bool visited[ MaxMapSize ][ MaxMapSize ];
	for ( int Idx1 = 0; Idx1 < Width_; ++Idx1 )
	{
		for ( int Idx2 = 0; Idx2 < Width_; ++Idx2 )
		{
			visited[ Idx1 ][ Idx2 ] = false;
		}
	}

	std::vector<Pos> openList;
	openList.push_back( start );
	visited[ start.x ][ start.y ] = true;

	while ( openList.size() > 0 )
	{
		int r = rand() % openList.size();
		Pos cell = openList[ r ];
		openList.erase( openList.begin() + r);
		visited[ cell.x ][ cell.y ] = true;
		int count = 0;
		if ( ( cell.x > 0 ) && ( MapInfo_[ cell.x - 1 ][ cell.y ].Type_ == LocationType::Empty ) )
			++count;
		if ( ( cell.x < Width_ - 1 ) && ( MapInfo_[ cell.x + 1 ][ cell.y ].Type_ == LocationType::Empty ) )
			++count;
		if ( ( cell.y > 0 ) && ( MapInfo_[ cell.x ][ cell.y - 1 ].Type_ == LocationType::Empty ) )
			++count;
		if ( ( cell.y < Height_ - 1 ) && ( MapInfo_[ cell.x ][ cell.y + 1 ].Type_ == LocationType::Empty ) )
			++count;
		if ( count < 2 )
		{
			MapInfo_[ cell.x ][ cell.y ].Type_ = LocationType::Empty;
		}
		else
		{
			continue;
		}

		if ( cell.x > 0 )
		{
			if ( !visited[ cell.x - 1 ][ cell.y ] )
			{
				visited[ cell.x - 1 ][ cell.y ] = true;
				Pos n;
				n.x = cell.x - 1;
				n.y = cell.y;
				openList.push_back( n );
			}
		}
		if ( cell.x < Width_ - 1 )
		{
			if ( !visited[ cell.x + 1 ][ cell.y ] )
			{
				visited[ cell.x + 1 ][ cell.y ] = true;
				Pos n;
				n.x = cell.x + 1;
				n.y = cell.y;
				openList.push_back( n );
			}
		}
		if ( cell.y > 0 )
		{
			if ( !visited[ cell.x ][ cell.y - 1 ] )
			{
				visited[ cell.x ][ cell.y - 1 ] = true;
				Pos n;
				n.x = cell.x;
				n.y = cell.y - 1;
				openList.push_back( n );
			}
		}
		if ( cell.y < Height_ - 1 )
		{
			if ( !visited[ cell.x ][ cell.y + 1 ] )
			{
				visited[ cell.x ][ cell.y + 1 ] = true;
				Pos n;
				n.x = cell.x;
				n.y = cell.y + 1;
				openList.push_back( n );
			}
		}
	}




}