"use strict";

function Renderer()
{
	var shaderProgram;
	var startTime;

	// Uniforms
	var iResolution,
		iGlobalTime,
		iMouse;
	// Attributes
	var aVertexPosition, vertexPositionBuffer;

	// Init
	this.init = function()
	{
		initAttribBuffers();
		initOptions();
		return loadShaders('glsl/shadertoy.vs', 'glsl/simple.fs').done(function() {
				shaderProgram = this.program;
				if(shaderProgram) initShaderVars();
			});
	}

	// Init render options
	function initOptions()
	{
		gl.enable(gl.DEPTH_TEST);
		gl.depthFunc(gl.LEQUAL);
		console.log("Render options initialized");
	}

	// Initialize shader variables
	function initShaderVars()
	{
		// Init uniforms
		iResolution = 	gl.getUniformLocation(shaderProgram, "iResolution");
		iGlobalTime = 	gl.getUniformLocation(shaderProgram, "iGlobalTime");
		iMouse =  		gl.getUniformLocation(shaderProgram, "iMouse");

		// Init attributes
		aVertexPosition = gl.getAttribLocation(shaderProgram, "aVertexPosition");
		gl.enableVertexAttribArray(aVertexPosition);

		console.log("Shader variables initialized");
	}

	// Initialize attribute buffers
	function initAttribBuffers()
	{
		vertexPositionBuffer = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER, vertexPositionBuffer);

		var vertices = new Float32Array ([
	    	1.0,  1.0,  0.0,
	    	-1.0, 1.0,  0.0,
	    	1.0,  -1.0, 0.0,
	    	-1.0, -1.0, 0.0
	  	]);
	  	gl.bufferData(gl.ARRAY_BUFFER, vertices, gl.STATIC_DRAW);
		console.log("Attribute buffers initialized");
	}

	// Start the loop
	this.start = function()
	{
		if(!shaderProgram)
		{
			document.write("</br>Failed to start render loop.</br>");
			return null;
		}
		startTime = new Date().getTime();
		setInterval(draw, 50);
		console.log("Render loop started");
	}

	// Draw on the canvas
	function draw()
	{
		var seconds = (new Date().getTime() - startTime) / 1000;

		// Clear buffers
		gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

		// Uniforms
		gl.uniform3f(iResolution, gl.drawingBufferWidth, gl.drawingBufferHeight, gl.drawingBufferWidth*1.0/gl.drawingBufferHeight);
		gl.uniform1f(iGlobalTime, seconds);
		gl.uniform4f(iMouse, 0, 0, 0, 0); // TODO: implement mouse position

		// Attribute: vertexPosition
		gl.bindBuffer(gl.ARRAY_BUFFER, vertexPositionBuffer);
		gl.vertexAttribPointer(aVertexPosition, 3, gl.FLOAT, false, 0, 0);

		// Draw
		gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);
	}

}
