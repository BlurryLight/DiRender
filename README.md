# DiRender

Not so tiny path tracer

- Linux gcc-7 /clang-7 / gcc-10 / clang-10

![Linux-CI](https://github.com/BlurryLight/DiRender/workflows/Linux-CI/badge.svg?branch=master)

- Windows MSVC 2019

![CI](https://github.com/BlurryLight/DiRender/workflows/CI/badge.svg)

**Note**:
This program is being modified to meet the requirements
of [the CSE168 course by Prof Ravi Ramamoorthi](http://cseweb.ucsd.edu/~viscomp/classes/cse168/sp20/schedule.html).

## Milestone

- Cornel Box (1200 x 1200 x 1024spp,about 35mins)
  
  Matte, dielectric, glass and Cook-Torrance material
  ![cornel_box](images/cornel_box_new.png)

- Debugger
  ![cornel_box debugger](images/debugger.gif)

- QuadLight(640 x 480, 128spp, cosine weight sampling)
  ![quadlight](images/quadlight.png)

- QuadLight(640 x 480, 128spp, Blinn-Phong importance sampling)
  This one looks much better.
  ![quadlight](images/quadlight_with_IS.png)
  


