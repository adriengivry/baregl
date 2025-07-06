workspace "baregl-examples"
	configurations { "Debug", "Release" }
	platforms { "x64" }
	startproject "1-triangle"

outputdir = "%{wks.location}/../bin/"
objoutdir = "%{wks.location}/../obj/"
depsdir = "%{wks.location}/deps/"

group "examples"
	include "1-triangle"
	include "2-cube"
	include "3-framebuffer"
	include "4-geometry"
	include "5-compute"
	include "6-tessellation"
group ""

group "examples/deps"
	include "deps/_glm"
	include "deps/_glfw"
group ""

include "../"
