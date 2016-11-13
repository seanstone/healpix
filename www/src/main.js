"use strict";

var gl;
var canvas;
var renderer;

function main()
{
	// Initialize the GL context
	canvas = document.getElementById("glCanvas");
	gl = initWebGL(canvas);
	if(!gl) return null;

	renderer = new Renderer();
	renderer.init().done(function(){renderer.start();})

	document.onmousemove = handleMouseMove;
}

function initWebGL(canvas)
{
	gl = null;

	try {
		// Try to grab the standard context. If it fails, fallback to experimental.
		gl = canvas.getContext("webgl") || canvas.getContext("experimental-webgl");
 	} catch(e) {}

	updateCanvasSize(canvas);

	if (!gl) document.write("</br>Unable to initialize WebGL.</br>");
  	return gl;
}

function updateCanvasSize(canvas)
{
	canvas.width = canvas.clientWidth;
	canvas.height = canvas.clientHeight;
	gl.viewport(0, 0, canvas.clientWidth, canvas.clientHeight);
}

function initTexture ()
{
	var texture = gl.createTexture();
	gl.bindTexture(gl.TEXTURE_2D, texture);

	// Fill the texture with a 1x1 blue pixel.
	var width, height;
	gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, width, height, 0, gl.RGBA, gl.UNSIGNED_BYTE, new Uint8Array([0, 0, 255, 255]));

	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.REPEAT);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.REPEAT);
}

function handleMouseMove(event)
{
	document.getElementById("mousePosDiv").innerHTML = event.clientX + " " + event.clientY;
}
