function loadShaders(vs_path, fs_path)
{
	var vs_source, fs_source;
	var jvs = jQuery.get(vs_path, function(data){ vs_source = data; }),
		jfs = jQuery.get(fs_path, function(data){ fs_source = data; });
	return $.when(jvs, jfs).done(function() {
			if(this.program = compileProgram(vs_source, fs_source)) {
				gl.useProgram(this.program);
				console.log("Shaders loaded successfully");
			}
		});
}

function compileProgram(vs_source, fs_source)
{
	var vs = compileShader(vs_source, gl.VERTEX_SHADER),
		fs = compileShader(fs_source, gl.FRAGMENT_SHADER);

	var program = gl.createProgram();
	gl.attachShader(program, vs);
	gl.attachShader(program, fs);
	gl.linkProgram(program);

	if (!gl.getProgramParameter(program, gl.LINK_STATUS)) {
		document.write("</br>Failed to compile shader program:</br>" + gl.getProgramInfoLog(program) + "</br>");
		return null;
	}

	console.log("Shader program compiled");
	return program;
}

function compileShader(source, type)
{
    var	shader = gl.createShader(type); // VERTEX_SHADER or FRAGMENT_SHADER
  	gl.shaderSource(shader, source);
  	gl.compileShader(shader);

  	if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS))
    	document.write("</br>Failed to compile shader:</br>" + gl.getShaderInfoLog(shader) + "</br>");

	console.log("Shader compiled: " + type);
  	return shader;
}
