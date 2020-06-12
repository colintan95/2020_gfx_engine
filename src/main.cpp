#include "application.h"
#include "platform/platform.h"

int main() {
  platform::Platform platform;
  platform.Initialize();

  std::unique_ptr<Application> app = std::make_unique<Application>(platform.GetWindow());
  app->RunLoop();
  app.reset();

  platform.Cleanup();
  
  return 0;
}