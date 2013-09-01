// Ŭnicode please 
//-------------------------------------------------------------------------------------
// ***** OculusWorldDemo Description

// This app renders a simple flat-shaded room allowing the user to move along the
// floor and look around with an HMD, mouse and keyboard. The following keys work:
//
//  'W', 'S', 'A', 'D' and Arrow Keys - Move forward, back; strafe left/right.
//  F1 - No stereo, no distortion.
//  F2 - Stereo, no distortion.
//  F3 - Stereo and distortion.
//  F4 - Toggle MSAA.
//  F9 - Cycle through fullscreen and windowed modes. Necessary for previewing content with Rift.
//
// Important Oculus-specific logic can be found at following locations:
//
//  OculusWorldDemoApp::OnStartup - This function will initialize OVR::DeviceManager and HMD,
//									creating SensorDevice and attaching it to SensorFusion.
//									This needs to be done before obtaining sensor data.
//
//  OculusWorldDemoApp::OnIdle    - Here we poll SensorFusion for orientation, apply it
//									to the scene and handle movement.
//									Stereo rendering is also done here, by delegating to
//									to Render function for each eye.
//

//-------------------------------------------------------------------------------------
// ***** OculusWorldDemo Application class

#include "Platform/Platform_Default.h"

#include "Player.h"
#include "Platform/Gamepad.h"

// An instance of this class is created on application startup (main/WinMain).
// It then works as follows:
//  - Graphics and HMD setup is done OculusWorldDemoApp::OnStartup(). This function
//    also creates the room model from Slab declarations.
//  - Per-frame processing is done in OnIdle(). This function processes
//    sensor and movement input and then renders the frame.
//  - Additional input processing is done in OnMouse, OnKey.

using namespace OVR;
using namespace OVR::Render;

class OculusWorldDemoApp : public OVR::Platform::Application, public OVR::MessageHandler
{
public:
    OculusWorldDemoApp();
    ~OculusWorldDemoApp();

    virtual int  OnStartup(int argc, const char** argv);
    virtual void OnIdle();

    virtual void OnMouseMove(int x, int y, int modifiers);
    virtual void OnKey(OVR::KeyCode key, int chr, bool down, int modifiers);
    virtual void OnResize(int width, int height);

    virtual void OnMessage(const OVR::Message& msg);

    void         Render(const OVR::Render::StereoEyeParams& stereo);

    // Sets temporarily displayed message for adjustments
    void         SetAdjustMessage(const char* format, ...);
    // Overrides current timeout, in seconds (not the future default value);
    // intended to be called right after SetAdjustMessage.
    void         SetAdjustMessageTimeout(float timeout);

    // Stereo setting adjustment functions.
    // Called with deltaTime when relevant key is held.
    void         AdjustFov(float dt);
    void         AdjustAspect(float dt);
    void         AdjustIPD(float dt);
    void         AdjustEyeHeight(float dt);

    void         AdjustMotionPrediction(float dt);

    void         AdjustDistortion(float dt, int kIndex, const char* label);
    void         AdjustDistortionK0(float dt)  { AdjustDistortion(dt, 0, "K0"); }
    void         AdjustDistortionK1(float dt)  { AdjustDistortion(dt, 1, "K1"); }
    void         AdjustDistortionK2(float dt)  { AdjustDistortion(dt, 2, "K2"); }
    void         AdjustDistortionK3(float dt)  { AdjustDistortion(dt, 3, "K3"); }

    // Adds room model to scene.
    void         PopulateScene(const char* fileName);
    void         PopulatePreloadScene();
    void		 ClearScene();

    // Magnetometer calibration procedure
    void         UpdateManualMagCalibration();

protected:
    OVR::Render::RenderDevice*       pRender;
    OVR::Render::RendererParams      RenderParams;
    int                 Width, Height;
    int                 Screen;
    int                 FirstScreenInCycle;

    // Magnetometer calibration and yaw correction
    OVR::Util::MagCalibration      MagCal;
	bool			          MagAwaitingForwardLook;

    // *** Oculus HMD Variables
    OVR::Ptr<OVR::DeviceManager>  pManager;
    OVR::Ptr<OVR::SensorDevice>   pSensor;
    OVR::Ptr<OVR::HMDDevice>      pHMD;
    OVR::Ptr<OVR::Profile>        pUserProfile;
    OVR::SensorFusion        SFusion;
    OVR::HMDInfo             TheHMDInfo;

    OVR::Ptr<OVR::LatencyTestDevice>  pLatencyTester;
    OVR::Util::LatencyTest   LatencyUtil;

    double              LastUpdate;
    int                 FPS;
    int                 FrameCounter;
    double              NextFPSUpdate;

    OVR::Array<OVR::Ptr<OVR::Render::CollisionModel> > CollisionModels;
    OVR::Array<OVR::Ptr<OVR::Render::CollisionModel> > GroundCollisionModels;

    // Loading process displays screenshot in first frame
    // and then proceeds to load until finished.
    enum LoadingStateType
    {
        LoadingState_Frame0,
        LoadingState_DoLoad,
        LoadingState_Finished
    };

	// Player
    Player				ThePlayer;
    OVR::Matrix4f            View;
    OVR::Render::Scene               MainScene;
    OVR::Render::Scene               LoadingScene;
    OVR::Render::Scene               GridScene;
    OVR::Render::Scene               YawMarkGreenScene;
    OVR::Render::Scene               YawMarkRedScene;
    OVR::Render::Scene               YawLinesScene;

    LoadingStateType    LoadingState;

    OVR::Ptr<ShaderFill>     LitSolid, LitTextures[4];

    // Stereo view parameters.
    OVR::Render::StereoConfig        SConfig;
    OVR::Render::PostProcessType     PostProcess;

    // LOD
    OVR::String	            MainFilePath;
    OVR::Array<OVR::String>       LODFilePaths;
    int					ConsecutiveLowFPSFrames;
    int					CurrentLODFileIndex;

    float               DistortionK0;
    float               DistortionK1;
    float               DistortionK2;
    float               DistortionK3;

    OVR::String              AdjustMessage;
    double              AdjustMessageTimeout;

    // Saved distortion state.
    float               SavedK0, SavedK1, SavedK2, SavedK3;
    float               SavedESD, SavedAspect, SavedEyeDistance;

    // Allows toggling color around distortion.
    OVR::Color               DistortionClearColor;

    // Stereo settings adjustment state.
    typedef void (OculusWorldDemoApp::*AdjustFuncType)(float);
    bool                ShiftDown;
    AdjustFuncType      pAdjustFunc;
    float               AdjustDirection;

    enum SceneRenderMode
    {
        Scene_World,
        Scene_Grid,
        Scene_Both,
        Scene_YawView
    };
    SceneRenderMode    SceneMode;


    enum TextScreen
    {
        Text_None,
        Text_Orientation,
        Text_Config,
        Text_Help,
        Text_Count
    };
    TextScreen          TextScreen;

    struct DeviceStatusNotificationDesc
    {
        DeviceHandle    Handle;
        MessageType     Action;

        DeviceStatusNotificationDesc():Action(Message_None) {}
        DeviceStatusNotificationDesc(MessageType mt, const DeviceHandle& dev) 
            : Handle(dev), Action(mt) {}
    };
    Array<DeviceStatusNotificationDesc> DeviceStatusNotificationsQueue; 


    OVR::Render::Model* CreateModel(OVR::Vector3f pos, struct SlabModel* sm);
    OVR::Render::Model* CreateBoundingModel(OVR::Render::CollisionModel &cm);
    void PopulateLODFileNames();
    void DropLOD();
    void RaiseLOD();
    void CycleDisplay();
    void GamepadStateChanged(const OVR::Platform::GamepadState& pad);

	// Variable used by UpdateManualCalibration
    Anglef FirstMagYaw;
	int ManualMagCalStage;
	int ManualMagFailures;
};