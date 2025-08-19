# Escape Paragon

A 2D raycasting game engine inspired by classic DOOM, built with OpenGL and GLUT. This project serves as the foundation for a future 3D first-person shooter experience.

## Features

- **2D Top-Down View**: Navigate through a maze-like map with walls and open spaces
- **Smooth Player Movement**: 
  - WASD controls for movement and rotation
  - Micro-stepping collision detection for precise movement
  - Radius-based collision system prevents wall clipping
- **Real-time Rendering**: 60 FPS gameplay with immediate visual feedback
- **Direction Indicator**: Visual line showing player's facing direction
- **Grid-Based Map System**: Easy to modify 8x8 tile-based level design

## Controls

- **W** - Move forward
- **S** - Move backward  
- **A** - Rotate left
- **D** - Rotate right

## Technical Details

- **Engine**: OpenGL with GLUT
- **Language**: C
- **Map Size**: 8x8 grid (64x64 pixels per tile)
- **Player**: Cyan dot with direction indicator
- **Collision**: 1-pixel radius collision detection
- **Movement**: 12-pixel movement with 3-pixel micro-stepping

## Building & Running

```bash
gcc peepee.c -framework OpenGL -framework GLUT -DGL_SILENCE_DEPRECATION -o escape-paragon
./escape-paragon
```

## Roadmap

🚧 **Coming Soon**: 3D raycasting engine transformation
- First-person 3D perspective
- Textured walls
- DOOM-style rendering
- Enhanced lighting system

## Map Layout

The current map features a simple maze with walls (white) and walkable areas (black). The player spawns in the upper-left corridor and can explore the connected rooms.

---

*Inspired by the legendary DOOM engine - bringing classic FPS mechanics to modern development.*
