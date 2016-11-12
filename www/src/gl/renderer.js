function Renderer()
{
	var shaderProgram;
	var startTime;

	// Uniforms
	var iResolution,
		iGlobalTime,
		iMouse;
	// Attributes
	var vertexPositionAttribute, vertexPositionBuffer;

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
		iResolutionUniform = 	gl.getUniformLocation(shaderProgram, "iResolution");
		iGlobalTimeUniform = 	gl.getUniformLocation(shaderProgram, "iGlobalTime");
		iMouseUniform =  		gl.getUniformLocation(shaderProgram, "iMouse");

		// Init attributes
		vertexPositionAttribute = gl.getAttribLocation(shaderProgram, "aVertexPosition");
		gl.enableVertexAttribArray(vertexPositionAttribute);
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
		if(!shaderProgram) {
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
		seconds = (new Date().getTime() - startTime) / 1000;

		// Clear buffers
		gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

		// Uniforms
		gl.uniform3f(iResolutionUniform, gl.drawingBufferWidth, gl.drawingBufferHeight, gl.drawingBufferWidth*1.0/gl.drawingBufferHeight);
		gl.uniform1f(iGlobalTimeUniform, seconds);
		gl.uniform4f(iMouseUniform, 0, 0, 0, 0); // TODO

		// Attribute: vertexPosition
		gl.bindBuffer(gl.ARRAY_BUFFER, vertexPositionBuffer);
		gl.vertexAttribPointer(vertexPositionAttribute, 3, gl.FLOAT, false, 0, 0);

		// Draw
		gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);
	}

}
