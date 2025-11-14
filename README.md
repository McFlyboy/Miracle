# Miracle

[![Build & Test](https://github.com/McFlyboy/Miracle/actions/workflows/build-and-test.yml/badge.svg)](https://github.com/McFlyboy/Miracle/actions/workflows/build-and-test.yml)

Cross-platform C++20 framework-style game engine for developing minimalistic 2D and 3D games on Windows, macOS and Linux

## Setup
Miracle requires very little code to get started. Here is a minimal setup for an empty application:
```cpp
#include <Miracle/Miracle.hpp>

int main() {
	return Miracle::App("My app").run();
}
```

Let's break that down:
```cpp
#include <Miracle/Miracle.hpp>

using namespace Miracle;

int main() {
  // Step 1: Create the app
	auto app = App("My app");

  // Step 2: Run the app
	int exitCode = app.run();

  // Step 3 (optional): Return the app's exitcode
	return exitCode;
}
```
### Configure the app
In order to add content to your app and configure its behavior, we use an `AppConfig` during construction of the app, as shown below.
```cpp
auto app = App("My app", AppConfig{});
```

`AppConfig` is a struct containing several smaller structs that configures different parts of your app. Here's an example of an app with a resizable window and v-sync enabled:
```cpp
auto app = App(
  "My app",
  AppConfig{
	  .windowConfig = WindowConfig{
		  .size = WindowSize{
			  .width = 1280,
			  .height = 720
		  },
		  .resizable = true
	  },
	  .rendererConfig = RendererConfig{
		  .swapchainConfig = SwapchainConfig{
			  .useVsync = true
		  }
	  }
  }
);
```

### Adding content
#### Scenes
Miracle uses a scene system where content is split up into different scenes, and only a single scene can be active and displayed to the user at a time. Right now, Miracle does not support having multiple scenes, but this will change in the future. The content of a scene is made up of entities. Everything in a scene, except the solid color background of a scene, is an entity. A scene can be created by adding a `SceneConfig` to the app's `AppConfig`, and then entities that should load into this scene immediately can be created by adding `EntityConfig`s to the `SceneConfig`.
```cpp
AppConfig{
  // A scene containing three entities
  .sceneConfig = SceneConfig{
    .entityConfigs = std::vector{
      EntityConfig{},
      EntityConfig{},
      EntityConfig{},
    }
  }
}
```
