# ACC2
Kayleigh Boekhoudt(s3447707) and Heejun Lee (p301521)
Final project: ACC2
Date: 18-01-2021


## Table of contents
* [Description](#description)
* [Extra Functionality](#extra-functionality)
* [Code](#code)
* [Supplemental materials](#supplemental-materials)


## Description
This project implements a method to approximate Catmull-Clark surfaces using Gregory patches. The GUI consists of two main sections. The first section is dedicated to Catmull-Clark. After loading a mesh, the window displays the control mesh. The user can subdivide the surface using Catmull-Clark subdivision by changing the subdivision steps. In addition, it possible to show the limit positions. For visual distinction between the different shapes, the user can select "Quads and Triangles" (Mesh Coloring) and "Edges" (only works when not using wiremode, press z). Examples are provided in the screenshots folder.


## Extra Functionality
The other section is dedicated to ACC2. The option "Regular Surface Patches" displays the regular regions of the surface. The option "Gregory Patches" shows the approximated surface using Gregory Patches for irregular quads and triangles. The user can change the inner and outer tessellation levels using the slider.
In addition, there is an option to follow the current subdivision level of Catmull-Clark by selecting "Follow Subdiv level". Otherwise, the mesh at the level displayed under "Based Subdivision Level" is used for the tessellation. The tessellation is then not dependent on changes of the Catmull Clark subdivision step. This is particularly useful when the user wants to compare ACC2 based on the control mesh (level 0) to the Catmull-Clark surface after a certain amount of steps (e.g. level 4).
It is also possible to visually distinct between the shapes using "Quads and Triangles" and "Edges", just as for the Catmull-Clark surface. Examples are provided in the screenshots folder.

To compare the ACC2 approximation to the Catmull-Clark surface, the user can display both the Catmull-Clark surface and the ACC2 surfaces. An example setting is provide in the screenshots folder.

We have also provided a mesh called quad_tri.obj that only consists of irregular quads and triangles that we used to test our implementation.


## Code
Code is written in C++. To get the code running please run ACC2.pro and include the following:\
a. [Qt5](https://www.qt.io/)\
b. [OpenGL](https://www.opengl.org//)\


## Supplemental materials
Supplementary materials (video) added in the Supplementary_materials folder.
