#include "CesiumCameraManager.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Misc/AutomationTest.h"

BEGIN_DEFINE_SPEC(
    CesiumCamerManagerSpec,
    "Cesium.CameraManager",
    EAutomationTestFlags::ApplicationContextMask |
        EAutomationTestFlags::ProductFilter)
END_DEFINE_SPEC(CesiumCamerManagerSpec)

UWorld* getGlobalWorldContext () {
	const TIndirectArray<FWorldContext>& worldContexts = GEngine->GetWorldContexts();
	FWorldContext firstWorldContext = worldContexts[0];
	return firstWorldContext.World();
}

void CesiumCamerManagerSpec::Define() {

  Describe("GetDefaultCameraManager", [this]() {
		It("should get the default camera manager", [this]() {
			UWorld* world = getGlobalWorldContext();
			ACesiumCameraManager* cameraManager = ACesiumCameraManager::GetDefaultCameraManager(world);
			TestNotNull("Returned pointer is valid", cameraManager);
		});

		It("should fail to get the default camera manager, when world context is null", [this]() {
			ACesiumCameraManager* cameraManager = ACesiumCameraManager::GetDefaultCameraManager(nullptr);
			TestNull("Returned pointer should be null", cameraManager);
		});
  });

  Describe("AddCamera", [this]() {
		It("should add and remove a single camera", [this]() {
			UWorld* world = getGlobalWorldContext();
			ACesiumCameraManager* cameraManager = ACesiumCameraManager::GetDefaultCameraManager(world);
			TestNotNull("Returned pointer is valid", cameraManager);

			const TMap<int32, FCesiumCamera>& camerasMapRef = cameraManager->GetCameras();
			TestEqual("Starting camera count is 0", camerasMapRef.Num(), 0);

			FCesiumCamera newCamera;
			int32 newCameraId = cameraManager->AddCamera(newCamera);
			TestEqual("Camera count is 1 after camera is added", camerasMapRef.Num(), 1);

			bool removeSuccess = cameraManager->RemoveCamera(newCameraId);
			TestTrue("Remove function returns success", removeSuccess);
			TestEqual("Camera count returns to 0", camerasMapRef.Num(), 0);
    });

		It("should fail to remove a camera, when the id is invalid", [this]() {
			UWorld* world = getGlobalWorldContext();
			ACesiumCameraManager* cameraManager = ACesiumCameraManager::GetDefaultCameraManager(world);
			TestNotNull("Returned pointer is valid", cameraManager);

			const TMap<int32, FCesiumCamera>& camerasMapRef = cameraManager->GetCameras();
			TestEqual("Starting camera count is 0", camerasMapRef.Num(), 0);

			int32 bogusZeroCameraId = 0;
			bool removeSuccess = cameraManager->RemoveCamera(bogusZeroCameraId);
			TestFalse("Remove function fails with bogus zero camera id", removeSuccess);
			TestEqual("Camera count remains at 0", camerasMapRef.Num(), 0);

			int32 bogusPositiveCameraId = 5;
			removeSuccess = cameraManager->RemoveCamera(bogusPositiveCameraId);
			TestFalse("Remove function fails with bogus positive camera id", removeSuccess);
			TestEqual("Camera count remains at 0", camerasMapRef.Num(), 0);

			int32 bogusNegativeCameraId = -5;
			removeSuccess = cameraManager->RemoveCamera(bogusNegativeCameraId);
			TestFalse("Remove function fails with bogus negative camera id", removeSuccess);
			TestEqual("Camera count remains at 0", camerasMapRef.Num(), 0);
    });
  });
}
