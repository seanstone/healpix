var main = function ()
{
    var canvas = document.getElementById('renderCanvas');

    var engine = new BABYLON.Engine(canvas, true);

    // create a basic BJS Scene object
    var scene = new BABYLON.Scene(engine);

    // create a FreeCamera, and set its position to (x:0, y:5, z:-10)
    // var camera = new BABYLON.FreeCamera('camera1', new BABYLON.Vector3(0, 5,-10), scene);
    var camera = new BABYLON.ArcRotateCamera("ArcRotateCamera", 1, 0.8, 10, new BABYLON.Vector3(0, 0, 0), scene);

    // target the camera to scene origin
    camera.setTarget(BABYLON.Vector3.Zero());

    // attach the camera to the canvas
    camera.attachControl(canvas, false, true);
    camera.lowerRadiusLimit = 2;
    camera.wheelPrecision = 40;

    // create a basic light, aiming 0,1,0 - meaning, to the sky
    var light = new BABYLON.HemisphericLight('light1', new BABYLON.Vector3(0,1,0), scene);

    // create a built-in "sphere" shape; its constructor takes 5 params: name, width, depth, subdivisions, scene
    var sphere1 = BABYLON.Mesh.CreateSphere('sphere1', 16, 2, scene);

    var materialSphere1 = new BABYLON.StandardMaterial("texture1", scene);
    sphere1.material = materialSphere1;
    //materialSphere1.diffuseColor = new BABYLON.Color3(1.0, 0.2, 0.7);

    var fire = new BABYLON.StarfieldProceduralTexture("firePT", 512, scene);
    sphere1.material.diffuseTexture = fire;

    // move the sphere upward 1/2 of its height
    //sphere.position.y = 1;

    // var skybox = BABYLON.Mesh.CreateBox("skyBox", 100.0, scene);
    // var skyboxMaterial = new BABYLON.StandardMaterial("skyBox", scene);
    // skyboxMaterial.backFaceCulling = false;
    // skyboxMaterial.disableLighting = true;
    // skybox.material = skyboxMaterial;
    // skyboxMaterial.diffuseColor = new BABYLON.Color3(0, 0, 0);
    // skyboxMaterial.specularColor = new BABYLON.Color3(0, 0, 0);
    // skyboxMaterial.reflectionTexture = new BABYLON.CubeTexture("textures/skybox", scene);
    // skyboxMaterial.reflectionTexture.coordinatesMode = BABYLON.Texture.SKYBOX_MODE;
    // skybox.renderingGroupId = 0

    engine.runRenderLoop(function() { scene.render(); });
    window.addEventListener('resize', function() { engine.resize(); });
}
