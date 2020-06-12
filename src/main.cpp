#include "application.h"
#include "systems.h"

int main() {
  Systems systems;
  systems.InitSystems();

  std::unique_ptr<Application> app = std::make_unique<Application>(systems.GetWindow());

  app->RunLoop();

  app.reset();

  systems.DestroySystems();
  
  return 0;
}