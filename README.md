# Terra Height Field Simplification Software

This software is in the public domain and is provided **AS IS**. Use it at **YOUR OWN RISK**.

* * *

This is Terra, the successor to Scape. Not only is this software **UNSUPPORTED**, but it is in the early stages of development. This software is clearly incomplete and may still have bugs. However, the basic mechanisms do function properly; I have run several large terrains through Terra, and the results were correct.

For up-to-date information on Terra, Scape, and related topics, watch:

<pre>	[http://www.cs.cmu.edu/~garland/scape/](http://www.cs.cmu.edu/~garland/scape/)
</pre>

* * *

## Introduction

This is **Terra**, a program for generating polygonal approximations of terrains and other height fields. It is based on algorithms described in:

> <cite>[Fast Polygonal Approximation of Terrains and Height Fields](http://www.cs.cmu.edu/~garland/scape/scape.ps.gz),  
> by Michael Garland and Paul Heckbert (Technical Report CMU-CS-95-181).</cite>  
> A [color plate](http://www.cs.cmu.edu/~garland/scape/color.ps.gz) is included separately.

The **Scape** package is the companion software for this paper. It was written with speed and memory efficiency as the primary concerns. Although it was designed strictly for the testing of the algorithms described in our paper, we made it available so that people interested in our results could examine it. We also hoped that it might be of interest to people who were attempting to build terrain approximations. However, Scape is not particularly easy to use and the code is definitely less than aesthetically pleasing.

I wrote Terra because I was dissatisfied with Scape. I wanted code which was better structured and programs which were easier to use. Terra will also provide more features than Scape.

**Disclaimer:** Please remember that both Terra and Scape are unsupported programs that I tinker with in my spare time. As such, development is typically sporadic.

### Brief feature summary

<dl>

<dt>_PGM input files_</dt>

<dd>Terra uses the PGM file format for data input. At first, this might seem odd; however, it has several advantages. PGM is a standard format which provides for both textual (i.e., human editable) and binary data files. Since PGM is an image file format, height field data is directly viewable with most available image viewers. Plus, there are many programs available to perform various manipulations on PGM files.

In particular, many of the tools provided by the **NetPBM** package can be used to manipulate PGM terrain data. The NetPBM package can be found online at:

<pre>	[http://wuarchive.wustl.edu/graphics/graphics/packages/NetPBM/](http://wuarchive.wustl.edu/graphics/graphics/packages/NetPBM/)
</pre>

or by anonymous ftp to:

<pre>	[wuarchive.wustl.edu:/graphics/graphics/packages/NetPBM](ftp://wuarchive.wustl.edu/graphics/graphics/packages/NetPBM/)
</pre>

</dd>

<dt>_Flexible output_</dt>

<dd>Terra can output its approximations in several different formats. The supported formats are described below.</dd>

<dt>_Extended approximation features_</dt>

<dd>Terra supports preinsertion scripts and importance masks. See below for details on what these are and how they work.</dd>

<dt>_Portable graphics_</dt>

<dd>The interactive program, <tt>xterra</tt>, uses the GLUT library for windowing and OpenGL for rendering. In theory, this should make it portable to machines other than SGI's.</dd>

</dl>

### Currently absent features

All these features are currently missing. Ideally, I would like to include these and other features. In reality, what gets done and how fast it gets done might be highly variable.

<dl>

<dt>_Multiple simultaneous height fields_</dt>

<dd>Scape provided for approximating height fields with RGB textures applied to them. Ultimately, Terra will support the approximation of arbitrarily many simultaneous height fields. Currently, however, Terra will only accept the input of a _single_ height field.</dd>

<dt>_Data-dependent triangulation_</dt>

<dd>One of the significant features of Scape was the option to use data-dependent triangulation. This triangulation scheme has not yet been ported to Terra.</dd>

<dt>_Data import facilities_</dt>

<dd>I would like to be able to import USGS DEM data into Terra. Although correcting for the spherical mapping of USGS data is beyond the scope of this project, Terra is in need of some simple tools to facilitate conversion of USGS data. I definitely don't have the time to write these tools, so if someone wants to suggest some reasonable ones, please let me know.</dd>

<dt>_Robust PGM input routines_</dt>

<dd>The PGM input routines are rather fragile at present. You should make sure that there is no extraneous white space and _no comments_ in the file. For example, the <tt>xv</tt> program will insert a comment in the PGM file citing itself as the creator of the file. You will need to remove this comment from the file.</dd>

</dl>

## Installation

2.  In order to compile the interactive version of Terra (<tt>xterra</tt>), you must obtain the GLUT library. It can be found at:

    <pre>	[http://www.sgi.com/Technology/openGL/glut.html](http://www.sgi.com/Technology/openGL/glut.html)
    </pre>

    or by anonymous ftp at:

    <pre>	[sgigate.sgi.com:/pub/opengl/xjournal/GLUT](ftp://sgigate.sgi.com/pub/opengl/xjournal/GLUT/)
    </pre>

    NOTE: For proper viewing, <tt>xterra</tt> needs to set the aspect ratio of its windows. This is currently not supported via GLUT. Therefore, I've had to hack things a bit by accessing GLUT internals. The file gui.cc includes the glutint.h header. This is not installed by GLUT. You must install this manually. Again, this is only of importance if you want to build <tt>xterra</tt>.

3.  Edit the Makefile for local customization. Basically, you should set the compilation flags, the libraries you need, and the location of GLUT if you are compiling <tt>xterra</tt>.
4.  Typing '<tt>make</tt>' will build both <tt>terra</tt> and <tt>xterra</tt>. However, you can selectively compile either of them (e.g., '<tt>make terra</tt>').

## Using Terra

The Terra software provides two programs: <tt>terra</tt>, a simple batch program, and <tt>xterra</tt>, an interactive application which uses OpenGL to display the surface approximation being constructed. Both programs utilize the same command line interface; <tt>xterra</tt> simply layers an interactive interface on top of <tt>terra</tt>. This section contains an outline of the operation of <tt>terra</tt>. All this information applies to <tt>xterra</tt> as well.

The operation of Terra goes through a simple series of phases:

1.  _Data input_.  
    This will load the terrain data and the importance mask (if specified).
2.  _Pre-insertion_.  
    If you have specified a pre-insertion script, it will be executed at this point.
3.  _Greedy insertion_.  
    The iterative greedy insertion procedure will begin. Terra will continue selecting points until it meets the goals that you have specified.
4.  _Output_.  
    Terra will output the approximation it has constructed. You have a choice of a handful of different formats.

Currently, all the information Terra needs to run is communicated on the command line. The correct usage of Terra is:

<pre>	usage: terra <options> filename
	       where <options> is some combination of: 
	-e <thresh>      Sets the tolerable error threshold
	-p <count>       Sets the maximum number of allowable points
	-o <file> <type> When finished, output the approximation to <file>.
	                 Valid types:  tin, eps, dem, obj
	-m <file>        Load the importance mask from <file>
	-s <file>        Execute preinsertion script from <file>
</pre>

The error threshold and point limit set the termination criteria. Terra will continue adding points as long as it it below the point limit and above the error threshold. The default error threshold is 0; the default point limit is the total number of points in the input grid.

The type of output desired is also specified on the command line. The <tt>eps</tt> output format simply generates an Encapsulated PostScript rendering of the approximation mesh. The <tt>dem</tt> output format creates an approximate DEM from the approximate mesh. This can be useful for comparison with the original. Both the <tt>tin</tt> and <tt>obj</tt> output formats generate 3D surfaces. The <tt>obj</tt> format is just the Wavefront <tt>.OBJ</tt> file format. The <tt>tin</tt> format is a very simple model description; it is a series of lines where each line is of the form:

<pre>	t x1 y1 z1 x2 y2 z2 x3 y3 z3
</pre>

Each such line describes a triangle with the three corners (x1,y1,z1) (x2,y2,z2) (x3,y3,z3).

The remaining options, involving importance masks and preinsertion scripts, are described in detail below.

## Xterra User Interface

<tt>xterra</tt> accepts the same options as <tt>terra</tt> and operates in much the same way. It introduces one extra command line option:

<pre>	-h <factor>      Sets the height scaling factor.  For example,
	                 if grid points are 25m apart, use a factor of 0.04
</pre>

This is used to properly scale the display of the height field in 3D. The input to Terra is specified in pixel coordinates; data samples are taken at integral pixel coordinates. However, the height values are probably not given in pixel coordinates. So, for display purposes, the height values are scaled by a constant factor to account for this loss of units in Terra.

When <tt>xterra</tt> starts up, it performs any preinsertion actions that you request, and then it displays two windows: a mesh view and a surface view. It does not perform greedy insertion until told to do so. The mesh view provides a 2D view of the triangulation being generated for the approximation of the height field. The surface view displays the approximate surface in 3D. The interaction with these windows is currently quite simple and will probably change in the future, but here is an outline of how they work at the moment:

<pre>	Surface view:
	   Left mouse drag   :   spin the surface
	   Middle mouse drag :   translate the camera side to side
	   Right mouse drag  :   move the camera in and out

	Mesh view:
	   Left mouse click   :  insert a point at the mouse location
	   Middle mouse click :  run greedy insertion until goal is met
	   Right mouse click  :  popup menu -- allows output
</pre>

## Providing Input for Terra

As stated above, Terra uses PGM files to read and write height field data. Unfortunately, Terra does not as yet provide any direct means of acquiring PGM data. For now, you will have to use the conversion software provided with Scape. The <tt>STM-tools/stm2pgm</tt> utility distributed with Scape can convert Scape's STM file format into PGM's appropriate for use with Terra. Given an STM file,

<pre>	<tt>stm2pgm sample.stm exact > sample.pgm</tt>
</pre>

will generate a PGM file. Note that the keyword <tt>exact</tt> is very important. Don't forget it! The resulting file will be textual, so you can even edit by hand if you need to.

## Importance Masks

One of the new features in Terra not found in Scape is the use of importance masks. In order to determine the next point for insertion, Terra ranks the available points by an importance measure. Importance masks allow you to bias this ranking. For each data point, the mask assigns a weight in the range [0..1] which multiplies the computed importance value.

Importance masks are PGM files, just like the height field input. However, their interpretation is slightly different. The input values are all integers. They are scaled such that their maximum value will be mapped to 1\. One significant point is that this maximum value is taken from the PGM header, not determined from the data. Therefore, by controlling the stated "maximum", you have much greater flexibility over the mapping of PGM pixel values to importance mask weights.

Currently, no means for constructing importance masks is provided.

## Preinsertion Scripts

The other new feature of Terra is its support for preinsertion scripts. An important feature of the greedy insertion algorithm is that essentially provides for progressive refinement of the approximation. Thus, the initial approximation can be arbitrary. The preinsertion scripts allow you set up an approximation before greedy insertion begins.

A preinsertion script is a series of lines, each of the form:

<pre>	<op> X Y
</pre>

The values for <tt>X</tt> and <tt>Y</tt> are the coordinates of a particular data point. The currently supported opcodes are:

<pre>	**s** -- Select this point for use in the approximation and insert
	     it into the current mesh.

	**i** -- Mark this point as one to be ignored.  Terra will never
	     process this point for insertion or evaluation.

	**u** -- Mark the height value at this point as unknown.
</pre>

Currently, Terra makes no distinction between points to be ignored and points whose height value is unknown; it ignores them equally.

* * *

January 19, 1996

<address>[Michael Garland](http://www.cs.cmu.edu/~garland/home.html)  
[garland@cs.cmu.edu](mailto:garland@cs.cmu.edu)</address>