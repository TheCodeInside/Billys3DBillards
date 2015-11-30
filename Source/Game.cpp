#include "Game.hpp"
#include "Time.hpp"
#include <GLFW/glfw3.h>
#include "Texture2D.hpp"
#include "MeshLoader.hpp"
#include "RigidBody.h"
#include "Physics.hpp"

std::shared_ptr<Game> Game::_instance = nullptr;

std::shared_ptr<Texture2D> texture;

CameraManager* cameraManager;

GameObject* cube, *cylinder, *sphere, *otherSphere, *testSphere, *otherTestSphere;

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
    if ( glDebugMessageCallback )
    {
        glDebugMessageCallback( MyGLCallback, nullptr );
    }
#endif
    
    // Cube
    {
        cube = AddGameObject( "Cube" );
        SimpleMaterial* materialCube = cube->AddComponent<SimpleMaterial>();
        MeshRenderer* meshRendererCube = cube->AddComponent<MeshRenderer>();
        RigidBody* rigidCube = cube->AddComponent<RigidBody>();
        Collider* cubeCollider = cube->AddComponent<SphereCollider>();
        meshRendererCube->SetMesh( MeshLoader::Load( "Models\\Cube.obj" ) );
        meshRendererCube->SetMaterial( materialCube );
    }

    // Cylinder
    {
        cylinder = AddGameObject( "Cylinder" );
        SimpleMaterial* materialCylinder = cylinder->AddComponent<SimpleMaterial>();
        MeshRenderer* meshRendererCylinder = cylinder->AddComponent<MeshRenderer>();
        RigidBody* rigidCylinder = cylinder->AddComponent<RigidBody>();
        Collider* cylinderColider = cylinder->AddComponent<SphereCollider>();
        meshRendererCylinder->SetMesh( MeshLoader::Load( "Models\\Cylinder.obj" ) );
        meshRendererCylinder->SetMaterial( materialCylinder );
    }

    // Sphere
    {
        sphere = AddGameObject( "Sphere" );
        SimpleMaterial* materialSphere = sphere->AddComponent<SimpleMaterial>();
        MeshRenderer* meshRendererSphere = sphere->AddComponent<MeshRenderer>();
        RigidBody* rigidSphere = sphere->AddComponent<RigidBody>();
        //rigidSphere->SetPosition(vec3(10.0f, 10.0f, 10.0f));
        Collider* sphereCollider = sphere->AddComponent<SphereCollider>();
        meshRendererSphere->SetMesh( MeshLoader::Load( "Models\\Sphere.obj" ) );
        meshRendererSphere->SetMaterial( materialSphere );
    }

    // Other Sphere
    {
        otherSphere = AddGameObject( "OtherSphere" );
        SimpleMaterial* materialOtherSphere = otherSphere->AddComponent<SimpleMaterial>();
        MeshRenderer* meshRendererOtherSphere = otherSphere->AddComponent<MeshRenderer>();
        RigidBody* rigidOtherSphere = otherSphere->AddComponent<RigidBody>();
        Collider* othersphereCollider = otherSphere->AddComponent<SphereCollider>();
        meshRendererOtherSphere->SetMesh( MeshLoader::Load( "Models\\Sphere.obj" ) );
        meshRendererOtherSphere->SetMaterial( materialOtherSphere );
    }

    // Test Sphere
    {
        testSphere = AddGameObject( "TestSphere" );
        SimpleMaterial* materialSphere = testSphere->AddComponent<SimpleMaterial>();
        MeshRenderer* meshRendererTestSphere = testSphere->AddComponent<MeshRenderer>();
        RigidBody* rigidTestSphere = testSphere->AddComponent<RigidBody>();
        //rigidSphere->SetPosition(vec3(10.0f, 10.0f, 10.0f));
        Collider* sphereTestCollider = testSphere->AddComponent<SphereCollider>();
        meshRendererTestSphere->SetMesh( MeshLoader::Load( "Models\\Sphere.obj" ) );
        meshRendererTestSphere->SetMaterial( materialSphere );
    }

    // Other Test Sphere
    {
        otherTestSphere = AddGameObject( "otherTestSphere" );
        SimpleMaterial* materialOtherSphere = otherTestSphere->AddComponent<SimpleMaterial>();
        MeshRenderer* meshRendererOtherTestSphere = otherTestSphere->AddComponent<MeshRenderer>();
        RigidBody* rigidOtherTestSphere = otherTestSphere->AddComponent<RigidBody>();
        Collider* otherTestsphereCollider = otherTestSphere->AddComponent<SphereCollider>();
        meshRendererOtherTestSphere->SetMesh( MeshLoader::Load( "Models\\Sphere.obj" ) );
        meshRendererOtherTestSphere->SetMaterial( materialOtherSphere );
    }

    // Textures
    texture = Texture2D::FromFile("Textures\\8-Ball.png");

    // Camera Manager
    GameObject* cameraManagerObject = AddGameObject("CameraManager");
    cameraManager = cameraManagerObject->AddComponent<CameraManager>();

    // Default Camera
    GameObject* cameraObject = AddGameObject("CameraObject");
    Camera* camera = cameraObject->AddComponent<Camera>();
    camera->SetPosition(vec3(5, 0, 0));
    camera->LookAtPosition(vec3(0, 0, 0));
    cameraManager->AddCamera(camera);

    // Smooth Follow Camera
    GameObject* cameraObjectSmoothFollower = AddGameObject("CameraObjectSmoothFollow");
    Camera* cameraSmoothFollower = cameraObjectSmoothFollower->AddComponent<Camera>();
    cameraSmoothFollower->SetPosition(vec3(-4, 4, -4));
    SmoothFollow* smoothFollow = cameraObjectSmoothFollower->AddComponent<SmoothFollow>();
    smoothFollow->SetTarget(cube->GetTransform());
    cameraManager->AddCamera(cameraSmoothFollower);

    // Tracker Camera
    GameObject* cameraObjectTracker = AddGameObject("CameraObjectTracker");
    Camera* cameraTracker = cameraObjectTracker->AddComponent<Camera>();
    cameraTracker->SetPosition(vec3(4, 2, -8));
    Tracker* tracker = cameraObjectTracker->AddComponent<Tracker>();
    tracker->SetTarget(sphere->GetTransform());
    cameraManager->AddCamera(cameraTracker);

    // FPS Camera
    GameObject* cameraObjectFPS = AddGameObject("CameraObjectFPS");
    Camera* cameraFPS = cameraObjectFPS->AddComponent<Camera>();
    cameraFPS->SetPosition(vec3(0));
    FPSController* fPSController = cameraObjectFPS->AddComponent<FPSController>();
    cameraManager->AddCamera(cameraFPS);
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
        if (object->GetActive())
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

    // counts Frames per second
    if ( tickCount >= 1.0f )
    {
        std::string title = "Billy's 3D Billiards";

        title += "    |    " + std::to_string( frameCount ) + " FPS";
        _window->SetTitle( title );

        frameCount = 0;
        tickCount -= 1.0f;
    }

    // Sets materials on Objects
    for (auto& object : _gameObjects)
    {
        if (object->GetActive())
        {
            Material* material = object->GetComponentOfType<Material>();
            if (material)
            {
                float aspectRatio = static_cast<float>(_window->GetWidth()) / _window->GetHeight();

                material->SetTexture("MyTexture", texture);

                material->ApplyCamera(cameraManager->GetActiveCamera());
            }
            object->Update();
        }
    }
    
    // Sets positons
    vec3 cubePosition(glm::sin(Time::GetTotalTime() / 4) * 4, 0, glm::cos(Time::GetTotalTime() / 4) * 4);
    vec3 spherePosition = vec3(5.0f, abs(sin(Time::GetTotalTime())) , -5);
    vec3 otherSpherePosition = vec3(5.0f, abs(cos(Time::GetTotalTime())), -5);
    
    // get transformations then set positons
    cube->GetTransform()->SetPosition(cubePosition);
    sphere->GetComponent<RigidBody>()->SetAcceleration(vec3(1.0f, 1.0f, 1.0f));
    sphere->GetTransform()->SetPosition(vec3(1.0f,1.0f,1.0f));
    otherSphere->GetTransform()->SetPosition(vec3(2.0f, 2.0f, 2.0f));

    // Test positions
    testSphere->GetTransform()->SetPosition(vec3(5.0f, 0.0f, 0.0f));
    otherTestSphere->GetTransform()->SetPosition(vec3(-5.0f, 0.0f, 0.0f));
    testSphere->GetComponent<RigidBody>()->SetVelocity(vec3(-1.0f, 0.0f, 0.0f));
    otherTestSphere->GetComponent<RigidBody>()->SetVelocity(vec3(1.0f, 0.0f, 0.0f));
    testSphere->GetComponent<RigidBody>()->SetAcceleration(vec3(-.5f, .0f, .0f));
    otherTestSphere->GetComponent<RigidBody>()->SetAcceleration(vec3(.5f, .0f, .0f));

    // Update the physics
    Physics::Update();

    
    // If escape is being pressed, then we should close the window
    if ( glfwGetKey( glfwGetCurrentContext(), GLFW_KEY_ESCAPE ) )
    {
        _window->Close();
    }
}
