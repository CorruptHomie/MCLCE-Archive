# Minecraft Infdev (2014)

## Download This Folder

```bash
git clone --depth 1 --filter=blob:none --sparse https://github.com/M2noa/MCLCE-Archive.git
cd MCLCE-Archive
git sparse-checkout set infdev-2014
```

[Download via Browser](https://download-directory.github.io/?url=https://github.com/M2noa/MCLCE-Archive/tree/main/infdev-2014)

> [!NOTE]
> PC Minecraft Infdev version source code with custom game engine implementation.

---

## Overview

| Attribute | Value |
|-----------|-------|
| **Date** | June 30, 2014 |
| **Platform** | PC |
| **Size** | 4.6 MB |
| **Files** | 430 |
| **Language** | C++ |
| **Build System** | CMake |

---

## source/

| Attribute | Value |
|-----------|-------|
| **Source** | `Infdev_20140630_master.zip` (gofile.io/d/RONXEc) |

### Structure

```
source/
├── src/
│   ├── Animation/      # Animation system
│   ├── Audio/          # Audio system
│   ├── Client/         # Client-side code
│   ├── Culling/        # Rendering culling system
│   ├── General/        # Core utilities (Vector, Matrix, Color, etc.)
│   ├── Input/          # Input handling
│   ├── Math/           # Math utilities
│   ├── Memory/         # Memory management and spatial partitioning (Octree)
│   ├── Model/          # 3D model loading (Assimp)
│   ├── Network/        # Networking
│   ├── Physics/        # Physics engine
│   ├── Rendering/      # OpenGL/DirectX rendering
│   ├── Server/         # Server-side code
│   ├── Utils/          # Utilities
│   ├── Window/         # Window management
│   └── World/          # World/level management (BSP, entities)
├── vendor/             # Third-party libraries
└── CMakeLists.txt      # CMake build configuration
```

---

## Build System

> [!NOTE]
> Uses CMake for cross-platform building. Supports OpenGL and DirectX rendering backends.

---

## Sources

| Source | URL |
|--------|-----|
| Download | gofile.io/d/RONXEc |
| DMCA Info | https://bin.skylvi.net/dmca |
