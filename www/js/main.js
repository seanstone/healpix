var gl; // A global variable for the WebGL context
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
	canvas.width = canvas.clientWidth; canvas.height = canvas.clientHeight;
	gl.viewport(0, 0, canvas.clientWidth, canvas.clientHeight);
}
