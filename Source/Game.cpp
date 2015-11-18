#include "Game.hpp"
#include "Time.hpp"
#include <GLFW/glfw3.h>
#include "Texture2D.hpp"
#include "MeshLoader.hpp"


std::shared_ptr<Game> Game::_instance = nullptr;

std::shared_ptr<Texture2D> texture;

GameObject* cube, * cylinder, * sphere;

Camera* camera;

Camera* cameraSmoothFollower;
SmoothFollow* smoothFollow;

Camera* cameraTracker;
Tracker* tracker;

Camera* cameraFPS;
FPSController* fPSController;

static void APIENTRY MyGLCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam)
{
    if (source == GL_DEBUG_SOURCE_API)
    {
        std::cout << message << std::endl;
    }
}

// Creates a new game
Game::Game()
    : _window( nullptr )
    , _clearColor( 0.0f, 0.0f, 0.0f, 1.0f )
{
    // Create the window
    _window = std::make_shared<GameWindow>( 1280, 720, "Billy's 3D Billiards" );
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
#if defined( _DEBUG ) || defined( DEBUG )
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(MyGLCallback, nullptr);
#endif
	
	// Cube
	cube = AddGameObject("Cube");
	SimpleMaterial* materialCube = cube->AddComponent<SimpleMaterial>();
	MeshRenderer* meshRendererCube = cube->AddComponent<MeshRenderer>();
	meshRendererCube->SetMesh(MeshLoader::Load("Models\\Cube.obj"));
	meshRendererCube->SetMaterial(materialCube);

	// Cylinder
	cylinder = AddGameObject("Cylinder");
	SimpleMaterial* materialCylinder = cylinder->AddComponent<SimpleMaterial>();
	MeshRenderer* meshRendererCylinder = cylinder->AddComponent<MeshRenderer>();
	meshRendererCylinder->SetMesh(MeshLoader::Load("Models\\Cylinder.obj"));
	meshRendererCylinder->SetMaterial(materialCylinder);

	// Sphere
	sphere = AddGameObject("Sphere");
	SimpleMaterial* materialSphere = sphere->AddComponent<SimpleMaterial>();
	MeshRenderer* meshRendererSphere = sphere->AddComponent<MeshRenderer>();
	meshRendererSphere->SetMesh(MeshLoader::Load("Models\\Sphere.obj"));
	meshRendererSphere->SetMaterial(materialCylinder);

	// Texture
    texture = Texture2D::FromFile("Textures\\Rocks.jpg");


	// Defualt Camera
	GameObject* cameraObject = AddGameObject("CameraObject");
	camera = cameraObject->AddComponent<Camera>();
	camera->SetPosition(vec3(5, 0, 0));
	camera->LookAtPosition(vec3(0, 0, 0));

	// Smooth Follow Camera
	GameObject* cameraObjectSmoothFollower = AddGameObject("CameraObjectSmoothFollow");
	cameraSmoothFollower = cameraObjectSmoothFollower->AddComponent<Camera>();
	cameraSmoothFollower->SetPosition(vec3(-4, 4, -4));
	smoothFollow = cameraObjectSmoothFollower->AddComponent<SmoothFollow>();
	smoothFollow->SetTarget(cube->GetTransform());

	// Tracker Camera
	GameObject* cameraObjectTracker = AddGameObject("CameraObjectTracker");
	cameraTracker = cameraObjectTracker->AddComponent<Camera>();
	cameraTracker->SetPosition(vec3(4, 2, -8));
	tracker = cameraObjectTracker->AddComponent<Tracker>();
	tracker->SetTarget(sphere->GetTransform());

	// FPS Camera
	GameObject* cameraObjectFPS = AddGameObject("CameraObjectFPS");
	cameraFPS = cameraObjectFPS->AddComponent<Camera>();
	cameraFPS->SetPosition(vec3(0));
	fPSController = cameraObjectFPS->AddComponent<FPSController>();
}

// Destroys the game instance
Game::~Game()
{
    _window = nullptr;
}

// Adds a game object to the game
GameObject* Game::AddGameObject( const std::string& name )
{
    // Check if there's already a game object with the given name
    auto search = _gameObjectCache.find( name );
    if ( search != _gameObjectCache.end() )
    {
        return search->second.get();
    }

    // Create the game object
    std::shared_ptr<GameObject> go = std::make_shared<GameObject>( name );

    // Record the game object
    _gameObjects.push_back( go );
    _gameObjectCache[ name ] = go;

    return go.get();
}

// Draws the game
void Game::Draw()
{
    for (auto& object : _gameObjects)
    {
        object->Draw();
    }
}

// Get the game instance
Game* Game::GetInstance()
{
    if ( !_instance )
    {
        _instance.reset( new Game() );
    }
    return _instance.get();
}

// Get the clear color
glm::vec4 Game::GetClearColor() const
{
    return _clearColor;
}

// Runs the game
void Game::Run()
{
    Time::Start();

    // Set the window to visible then begin the game loop
    _window->SetVisible( true );
    while ( _window->IsOpen() )
    {
        // Clear the screen
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        // Update and draw the game
        Update();
        Draw();

        // Update the time values
        Time::Update();

        // Poll for events and swap the buffers
        _window->SwapBuffers();
        _window->PollEvents();
    }
}

// Set the clear color
void Game::SetClearColor( float r, float g, float b, float a )
{
    SetClearColor( glm::vec4( r, g, b, a ) );
}

// Set the clear color
void Game::SetClearColor( const glm::vec4& value )
{
    _clearColor = value;
    glClearColor( _clearColor.r,
                  _clearColor.g,
                  _clearColor.b,
                  _clearColor.a );
}

// Updates the game
void Game::Update()
{
    static int frameCount = 0;
    static float tickCount = 0.0f;

    tickCount += Time::GetElapsedTime();
    ++frameCount;

    if ( tickCount >= 1.0f )
    {
        std::string title = "Billy's 3D Billiards";

        title += "    |    " + std::to_string( frameCount ) + " FPS";
        _window->SetTitle( title );

        frameCount = 0;
        tickCount -= 1.0f;
    }



    for (auto& object : _gameObjects)
    {
        Material* material = object->GetComponentOfType<Material>();
        if ( material )
        {
            float aspectRatio = static_cast<float>( _window->GetWidth() ) / _window->GetHeight();

            material->SetTexture( "MyTexture", texture );

			material->ApplyCamera(camera);
            
        }
        object->Update();
    }

	
	vec3 cubePosition(glm::sin(Time::GetTotalTime() / 4) * 4, 0, glm::cos(Time::GetTotalTime() / 4) * 4);
	vec3 spherePosition = vec3(5.0f, abs(sin(Time::GetTotalTime())) , -5);

	cube->GetTransform()->SetPosition(cubePosition);
	sphere->GetTransform()->SetPosition(spherePosition);

    // If escape is being pressed, then we should close the window
    if ( glfwGetKey( glfwGetCurrentContext(), GLFW_KEY_ESCAPE ) )
    {
        _window->Close();
    }
}
