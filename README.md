# About

**TrickyUtilities** is a plugin for rapid prototyping in Unreal Engine 5.

It provides a collection of useful actors and functions, which facilitate prototyping.

## Features

- **Utility Functions Library** a library which provides a collection of helper functions.
- **Annotation Actor** an actor which can be used to create annotations in levels.
- **Spline Actor** an actor with SplineComponent as root.

## Installation

1. Copy the plugin folder into your project's `Plugins` directory.
2. Open your Unreal Engine project.
3. Enable the **TrickyUtilities** in the plugins menu.
4. Restart the editor.

## Content Overview

### UTrickyUtilityLibrary

A powerful Blueprint Function Library with helper functions.

#### Time and Timeline Utilities

- **CalculateTimelinePlayRate**: Automatically adjusts a Timeline component's play rate to achieve a specific duration
- **FormatTimeSeconds**: Converts time values to formatted strings with multiple display options (MM:SS.MS, MM:SS, etc.)

#### Transform Pattern Generation

- **CalculateGridTransforms**: Creates a 2D grid of transforms with customizable cell size
- **CalculateRingTransform**: Generates points in a perfect circle with adjustable radius and point count
- **CalculateArcTransforms**: Creates partial ring segments with customizable angle span
- **CalculateSunFlowerTransforms**: Produces the classic sunflower pattern for optimal 2D point distribution
- **CalculateCubeTransforms**: Builds a 3D grid (cube) of transforms with customizable dimensions
- **CalculateCylinderTransforms**: Creates cylindrical arrangements with control over height and ring count
- **CalculateSphereTransforms**: Distributes points on a sphere surface with latitude/longitude constraints
- **CalculateConcentricRingsTransforms**: Creates multiple concentric rings with consistent point density
- **CalculateDynamicConcentricRingsTransforms**: Creates rings where outer rings have proportionally more points
- **CalculateConcentricArcsTransforms**: Generates multiple partial arcs with consistent point distribution
- **CalculateDynamicConcentricArcsTransforms**: Creates partial arcs with increasing point count for outer arcs
- **CalculateArcCylinderTransforms**: Builds a 3D cylindrical pattern using partial arcs

#### Spline Utilities

- **GetTransformsAlongSplineByPoints**: Evenly distributes a specific number of points along a spline
- **GetLocationsAlongSplineByPoints**: Retrieves evenly spaced locations along a spline
- **GetTransformsAlongSplineByDistance**: Places transforms at regular distance intervals along a spline
- **GetLocationsAlongSplineByDistance**: Gets locations at consistent distance spacing along a spline

### ATrickyAnnotationActor

A customizable actor for adding visible text annotations to your levels:

- Double-sided text rendering for visibility from any angle
- Adjustable size, color, and alignment
- Custom text materials support
- Perfect for level design notes and documentation

### ATrickySplineActor

A simple but effective spline-based actor for creating paths and object placement guides.
