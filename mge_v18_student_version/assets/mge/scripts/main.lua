io.stdout:setvbuf('no') -- enable flushed printing, slower, but usefull 'no', 'line', 'full'
 math.randomseed( os.time() )
if Debug then
  print("Lua in Debug mode, is slower")
  require("mobdebug").start() 
end

local stageOneGateCount = { } -- number
local stageTwoGateCount = { } -- number
local amountOfGates = { } -- number
local gateWaveSpawnTimeInSec = {} -- number
local gateZDestructionOffset = { } -- number
local gateSpeed = { } -- number
local passedWaveCount = { }
local lifeCount = { } -- number
local gateMaterials = { } -- array of materials
local playerMaterials = { } -- array of materials
local gateMeshes = { } -- array of meshes
local currentGateColor = { } -- number
local currentPlayerColor = { } -- number
local currentGateCount = { } -- number
local createdGateCount = { } -- number
local gateColorArray = { } -- array of colors in order that the player will change into
local highestSuccessfullyPassedGate = { }

local passedSuccessfully = { }
local score = { } --number
local gateScore = { } --number
local gateScoreMultiplier = { } --number
local stageCount = { } --number
local difficultyCount = { } --number
local stageOneHigh = { } --number
local stageTwoHigh = { } --number
local stageThreeHigh = { } --number
local stageFourIncreaseStep = { } --number
local gameOver = { } -- bool

local pickupSound = { } -- sound
local laneSwapSound = { } -- sound
local winSound = { } -- sound
local loseSound = { } -- sound
local deathSound = { } -- sound
local playerMaterialNumber = { } --number
local gateMaterialNumber = { } --number
local truePlayerColor = { } --number
local currentShapeNumber = { } -- 
local time = { }

local actualMat = { }

local highScore = { } --number
local displayingFeedback = { } --bool
local achievementWait = { } --number

local difficultyCountSaved = { } --number
local passedSuccessfullySaved = { } --number

local timeSpentPlaying = { }

local passedHighscore = { } --bool

local scoreSound1 = { } --sound
local scoreSound2 = { } --sound
local scoreSound3 = { } --sound
local scoreSound4 = { } --sound
  
local comboSound1 = { } --sound
local comboSound2 = { } --sound
local comboSound3 = { } --sound
local comboSound4 = { } --sound

local embarassingSound1 = { } --sound
local embarassingSound2 = { } --sound
local embarassingSound3 = { } --sound
local embarassingSound4 = { } --sound
  
local gameOverSound1 = { } --sound
local highScoreSound = { } --sound
local tutorialFailSound1 = { } --sound
local tutorialFailSound2 = { } --sound
local tutorialFailSound3 = { } --sound

function Start()
	print("Beginning of start function")
  
  -------------------------------------------------------------
                  --TRACK AND PLAYER LOAD--
  -------------------------------------------------------------
  
  player = GameObject.create("player", "Player", 0, 1, 31)
    GameObject.setScale(player, 1, 1, 1)
    GameObject.setMaterial(player, "texture-legacy", "/CubePlayer/Red_CubePlayer.png")
    GameObject.setBehaviour(player, "PlayerBehaviour", -5, "Cube_player.obj", "Tris_player.obj", "Sphere_player.obj", "laneswitch.wav", "cube.wav", "sphere.wav", "pyramid.wav")
  
  plane = GameObject.create("plane", "Floor", 0, -1, 0)
    GameObject.setScale(plane, 1, 1, 1)
    GameObject.setMesh(plane, "planeNew.obj")
    GameObject.setMaterial(
      plane,
      "texture",
      "Track/TrackMaterial_dots.png",
      "green.jpg",
      "blue.jpg",
      "red.jpg",
      "normalTestNormalMap.jpg",
      "glossTestMatNo.jpg", 
      1, 1, 1, -- diffuse color
      1, 1, 1, -- specular color
      0, -- shinyness
      0.02, -- scroll multiplier for texture 1
      0, -- scroll multiplier for texture 2
      0, -- scroll multiplier for texture 3
      1, -- transparency for first texture
      1, -- transparency for second texture
      1, -- transparency for third texture
      0) -- complete reflection?
  
  plane2 = GameObject.create("plane", "Floor", 0, -1.1, 0)
    GameObject.setScale(plane2, 1, 1, 1)
    GameObject.setMesh(plane2, "planeNew.obj")

    GameObject.setMaterial(
      plane2,
      "texture",
      "Track/TrackMaterial2.jpg",
      "green.jpg",
      "blue.jpg",
      "red.jpg",
      "normalTestNormalMap.jpg",
      "glossTestMatNo.jpg", 
      1, 1, 1, -- diffuse color
      1, 1, 1, -- specular color
      0, -- shinyness
      0.01, -- scroll multiplier for texture 1
      0, -- scroll multiplier for texture 2
      0, -- scroll multiplier for texture 3
      0.5, -- transparency for first texture
      1, -- transparency for second texture
      1, -- transparency for third texture
      0) -- complete reflection?

  sky = GameObject.create("sphere", "Sky", 0, -65, -45)
    GameObject.rotate(sky, -10, 1, 0, 0)
    GameObject.setScale(sky, 2, 2, 2)
    GameObject.setMesh(sky, "Sky.obj")
    GameObject.setMaterial(sky, "texture-legacy", "Sky.jpg")
    
  -------------------------------------------------------------
                  --CAMERA IMPLEMENTATION--
  -------------------------------------------------------------
  
  camera = Camera.create("camera", "MainCamera", 0, 8, 37) -- There should always only be one with tag: 'MainCamera', no more, no less.
    GameObject.rotate(camera, -35, 1, 0, 0)
  --
  
  -------------------------------------------------------------
                  --GATE MATERIALS--
  -------------------------------------------------------------
  
  gateMaterials[0] = Material.create("texture",
      "CubeGate/CubeGate_Color.jpg",
      "green.jpg",
      "red.jpg",
      "CubeGate/CubeGate_smap_new.jpg",
      "CubeGate/CubeGate_Normal.jpg",
      "CubeGate/CubeGate_Glossiness.jpg",
      1, 1, 1, -- diffuse color
      1, 1, 1, -- specular color
      5, -- shinyness
      0, -- scroll multiplier for texture 1
      0, -- scroll multiplier for texture 2
      0, -- scroll multiplier for texture 3
      1, -- transparency for first texture
      1, -- transparency for second texture
      1, -- transparency for third texture
      0) --red square
  gateMaterials[1] = Material.create("texture",
      "CubeGate/CubeGate_Color.jpg",
      "green.jpg",
      "green.jpg",
      "CubeGate/CubeGate_smap_new.jpg",
      "CubeGate/CubeGate_Normal.jpg",
      "CubeGate/CubeGate_Glossiness.jpg",
      1, 1, 1, -- diffuse color
      1, 1, 1, -- specular color
      5, -- shinyness
      0, -- scroll multiplier for texture 1
      0, -- scroll multiplier for texture 2
      0, -- scroll multiplier for texture 3
      1, -- transparency for first texture
      1, -- transparency for second texture
      1, -- transparency for third texture
      0) --green square
  gateMaterials[2] = Material.create("texture",
      "CubeGate/CubeGate_Color.jpg",
      "green.jpg",
      "blue.jpg",
      "CubeGate/CubeGate_smap_new.jpg",
      "CubeGate/CubeGate_Normal.jpg",
      "CubeGate/CubeGate_Glossiness.jpg",
      1, 1, 1, -- diffuse color
      1, 1, 1, -- specular color
      5, -- shinyness
      0, -- scroll multiplier for texture 1
      0, -- scroll multiplier for texture 2
      0, -- scroll multiplier for texture 3
      1, -- transparency for first texture
      1, -- transparency for second texture
      1, -- transparency for third texture
      0) --blue square
  gateMaterials[3] = Material.create("texture",
      "CubeGate/CubeGate_Color.jpg",
      "green.jpg",
      "white.jpg",
      "CubeGate/CubeGate_smap_new.jpg",
      "CubeGate/CubeGate_Normal.jpg",
      "CubeGate/CubeGate_Glossiness.jpg",
      1, 1, 1, -- diffuse color
      1, 1, 1, -- specular color
      5, -- shinyness
      0, -- scroll multiplier for texture 1
      0, -- scroll multiplier for texture 2
      0, -- scroll multiplier for texture 3
      1, -- transparency for first texture
      1, -- transparency for second texture
      1, -- transparency for third texture
      0) --white square

  gateMaterials[4] = Material.create("texture",
      "SphereGate/SphereGate_Color.jpg",
      "green.jpg",
      "red.jpg",
      "SphereGate/SphereGate_smap_new.png",
      "SphereGate/SphereGate_Normal.jpg",
      "SphereGate/SphereGate_Glossiness.jpg",
      1, 1, 1, -- diffuse color
      1, 1, 1, -- specular color
      5, -- shinyness
      0, -- scroll multiplier for texture 1
      0, -- scroll multiplier for texture 2
      0, -- scroll multiplier for texture 3
      1, -- transparency for first texture
      1, -- transparency for second texture
      1, -- transparency for third texture
      0) --red circle
  gateMaterials[5] = Material.create("texture",
      "SphereGate/SphereGate_Color.jpg",
      "green.jpg",
      "green.jpg",
      "SphereGate/SphereGate_smap_new.png",
      "SphereGate/SphereGate_Normal.jpg",
      "SphereGate/SphereGate_Glossiness.jpg",
      1, 1, 1, -- diffuse color
      1, 1, 1, -- specular color
      5, -- shinyness
      0, -- scroll multiplier for texture 1
      0, -- scroll multiplier for texture 2
      0, -- scroll multiplier for texture 3
      1, -- transparency for first texture
      1, -- transparency for second texture
      1, -- transparency for third texture
      0) --green circle
  gateMaterials[6] = Material.create("texture",
      "SphereGate/SphereGate_Color.jpg",
      "green.jpg",
      "blue.jpg",
      "SphereGate/SphereGate_smap_new.png",
      "SphereGate/SphereGate_Normal.jpg",
      "SphereGate/SphereGate_Glossiness.jpg",
      1, 1, 1, -- diffuse color
      1, 1, 1, -- specular color
      5, -- shinyness
      0, -- scroll multiplier for texture 1
      0, -- scroll multiplier for texture 2
      0, -- scroll multiplier for texture 3
      1, -- transparency for first texture
      1, -- transparency for second texture
      1, -- transparency for third texture
      0) --blue circle
  gateMaterials[7] = Material.create("texture",
      "SphereGate/SphereGate_Color.jpg",
      "green.jpg",
      "white.jpg",
      "SphereGate/SphereGate_smap_new.png",
      "SphereGate/SphereGate_Normal.jpg",
      "SphereGate/SphereGate_Glossiness.jpg",
      1, 1, 1, -- diffuse color
      1, 1, 1, -- specular color
      5, -- shinyness
      0, -- scroll multiplier for texture 1
      0, -- scroll multiplier for texture 2
      0, -- scroll multiplier for texture 3
      1, -- transparency for first texture
      1, -- transparency for second texture
      1, -- transparency for third texture
      0) --any color, circle
  
  gateMaterials[8] = Material.create("texture",
      "TrisGate/TrisGate_Color.jpg",
      "green.jpg",
      "red.jpg",
      "TrisGate/TrisGate_smap_new.jpg",
      "TrisGate/TrisGate_Normal.jpg",
      "TrisGate/TrisGate_Glossiness.jpg",
      1, 1, 1, -- diffuse color
      1, 1, 1, -- specular color
      5, -- shinyness
      0, -- scroll multiplier for texture 1
      0, -- scroll multiplier for texture 2
      0, -- scroll multiplier for texture 3
      1, -- transparency for first texture
      1, -- transparency for second texture
      1, -- transparency for third texture
      0) --red triangle
  gateMaterials[9] = Material.create("texture",
      "TrisGate/TrisGate_Color.jpg",
      "green.jpg",
      "green.jpg",
      "TrisGate/TrisGate_smap_new.jpg",
      "TrisGate/TrisGate_Normal.jpg",
      "TrisGate/TrisGate_Glossiness.jpg",
      1, 1, 1, -- diffuse color
      1, 1, 1, -- specular color
      5, -- shinyness
      0, -- scroll multiplier for texture 1
      0, -- scroll multiplier for texture 2
      0, -- scroll multiplier for texture 3
      1, -- transparency for first texture
      1, -- transparency for second texture
      1, -- transparency for third texture
      0) --green triangle
  gateMaterials[10] = Material.create("texture",
      "TrisGate/TrisGate_Color.jpg",
      "green.jpg",
      "blue.jpg",
      "TrisGate/TrisGate_smap_new.jpg",
      "TrisGate/TrisGate_Normal.jpg",
      "TrisGate/TrisGate_Glossiness.jpg",
      1, 1, 1, -- diffuse color
      1, 1, 1, -- specular color
      5, -- shinyness
      0, -- scroll multiplier for texture 1
      0, -- scroll multiplier for texture 2
      0, -- scroll multiplier for texture 3
      1, -- transparency for first texture
      1, -- transparency for second texture
      1, -- transparency for third texture
      0) --blue triangle
  gateMaterials[11] = Material.create("texture",
      "TrisGate/TrisGate_Color.jpg",
      "green.jpg",
      "white.jpg",
      "TrisGate/TrisGate_smap_new.jpg",
      "TrisGate/TrisGate_Normal.jpg",
      "TrisGate/TrisGate_Glossiness.jpg",
      1, 1, 1, -- diffuse color
      1, 1, 1, -- specular color
      5, -- shinyness
      0, -- scroll multiplier for texture 1
      0, -- scroll multiplier for texture 2
      0, -- scroll multiplier for texture 3
      1, -- transparency for first texture
      1, -- transparency for second texture
      1, -- transparency for third texture
      0) --any color, triangle

  gateMaterials[12] = Material.create("texture",
      "ShapeIrrelevantGate/ShapeIrrelevantGate_Color.jpg",
      "green.jpg",
      "red.jpg",
      "ShapeIrrelevantGate/ShapeIrrelevantGate_spmap_new.jpg",
      "ShapeIrrelevantGate/ShapeIrrelevantGate_Normal.jpg",
      "ShapeIrrelevantGate/ShapeIrrelevantGate_Glossiness.jpg",
      1, 1, 1, -- diffuse color
      1, 1, 1, -- specular color
      5, -- shinyness
      0, -- scroll multiplier for texture 1
      0, -- scroll multiplier for texture 2
      0, -- scroll multiplier for texture 3
      1, -- transparency for first texture
      1, -- transparency for second texture
      1, -- transparency for third texture
      0) --red anyshape
    
  gateMaterials[13] = Material.create("texture",
      "ShapeIrrelevantGate/ShapeIrrelevantGate_Color.jpg",
      "green.jpg",
      "green.jpg",
      "ShapeIrrelevantGate/ShapeIrrelevantGate_spmap_new.jpg",
      "ShapeIrrelevantGate/ShapeIrrelevantGate_Normal.jpg",
      "ShapeIrrelevantGate/ShapeIrrelevantGate_Glossiness.jpg",
      1, 1, 1, -- diffuse color
      1, 1, 1, -- specular color
      5, -- shinyness
      0, -- scroll multiplier for texture 1
      0, -- scroll multiplier for texture 2
      0, -- scroll multiplier for texture 3
      1, -- transparency for first texture
      1, -- transparency for second texture
      1, -- transparency for third texture
      0) --green anyshape
    
  gateMaterials[14] = Material.create("texture",
      "ShapeIrrelevantGate/ShapeIrrelevantGate_Color.jpg",
      "green.jpg",
      "blue.jpg",
      "ShapeIrrelevantGate/ShapeIrrelevantGate_spmap_new.jpg",
      "ShapeIrrelevantGate/ShapeIrrelevantGate_Normal.jpg",
      "ShapeIrrelevantGate/ShapeIrrelevantGate_Glossiness.jpg",
      1, 1, 1, -- diffuse color
      1, 1, 1, -- specular color
      5, -- shinyness
      0, -- scroll multiplier for texture 1
      0, -- scroll multiplier for texture 2
      0, -- scroll multiplier for texture 3
      1, -- transparency for first texture
      1, -- transparency for second texture
      1, -- transparency for third texture
      0) --blue anyshape
    
  gateMaterials[15] = Material.create("texture",
      "ShapeIrrelevantGate/ShapeIrrelevantGate_Color.jpg",
      "green.jpg",
      "white.jpg",
      "ShapeIrrelevantGate/ShapeIrrelevantGate_spmap_new.jpg",
      "ShapeIrrelevantGate/ShapeIrrelevantGate_Normal.jpg",
      "ShapeIrrelevantGate/ShapeIrrelevantGate_Glossiness.jpg",
      1, 1, 1, -- diffuse color
      1, 1, 1, -- specular color
      5, -- shinyness
      0, -- scroll multiplier for texture 1
      0, -- scroll multiplier for texture 2
      0, -- scroll multiplier for texture 3
      1, -- transparency for first texture
      1, -- transparency for second texture
      1, -- transparency for third texture
      0) --white anyshape

  gateMaterials[16] = Material.create("texture",
      "XGate/XGate_Color.jpg",
      "green.jpg",
      "red.jpg",
      "red.jpg",
      "XGate/XGate_Normal.jpg",
      "XGate/XGate_Glossiness.jpg",
      1, 1, 1, -- diffuse color
      1, 1, 1, -- specular color
      5, -- shinyness
      0, -- scroll multiplier for texture 1
      0, -- scroll multiplier for texture 2
      0, -- scroll multiplier for texture 3
      1, -- transparency for first texture
      1, -- transparency for second texture
      1, -- transparency for third texture
      0) --red wall
    
  gateMaterials[17] = Material.create("texture",
      "XGate/XGate_Color.jpg",
      "green.jpg",
      "green.jpg",
      "red.jpg",
      "XGate/XGate_Normal.jpg",
      "XGate/XGate_Glossiness.jpg",
      1, 1, 1, -- diffuse color
      1, 1, 1, -- specular color
      5, -- shinyness
      0, -- scroll multiplier for texture 1
      0, -- scroll multiplier for texture 2
      0, -- scroll multiplier for texture 3
      1, -- transparency for first texture
      1, -- transparency for second texture
      1, -- transparency for third texture
      0) --green wall
    
  gateMaterials[18] = Material.create("texture",
      "XGate/XGate_Color.jpg",
      "green.jpg",
      "blue.jpg",
      "red.jpg",
      "XGate/XGate_Normal.jpg",
      "XGate/XGate_Glossiness.jpg",
      1, 1, 1, -- diffuse color
      1, 1, 1, -- specular color
      5, -- shinyness
      0, -- scroll multiplier for texture 1
      0, -- scroll multiplier for texture 2
      0, -- scroll multiplier for texture 3
      1, -- transparency for first texture
      1, -- transparency for second texture
      1, -- transparency for third texture
      0) --blue wall
    
  gateMaterials[19] = Material.create("texture",
      "XGate/XGate_Color.jpg",
      "green.jpg",
      "white.jpg",
      "red.jpg",
      "XGate/XGate_Normal.jpg",
      "XGate/XGate_Glossiness.jpg",
      1, 1, 1, -- diffuse color
      1, 1, 1, -- specular color
      5, -- shinyness
      0, -- scroll multiplier for texture 1
      0, -- scroll multiplier for texture 2
      0, -- scroll multiplier for texture 3
      1, -- transparency for first texture
      1, -- transparency for second texture
      1, -- transparency for third texture
      0) --white wall
  -------------------------------------------------------------
                  --PLAYER MATERIALS--
  -------------------------------------------------------------
  
  playerMaterials[0] = Material.create("texture",
      "/CubePlayer/Red_CubePlayer.png",
      "green.jpg",
      "blue.jpg",
      "red.jpg",
      "normalTestNormalMap.jpg",
      "glossTestMatNo.jpg",
      1, 1, 1, -- diffuse color
      1, 1, 1, -- specular color
      5, -- shinyness
      0, -- scroll multiplier for texture 1
      0, -- scroll multiplier for texture 2
      0, -- scroll multiplier for texture 3
      1, -- transparency for first texture
      1, -- transparency for second texture
      1, -- transparency for third texture
      0) --red cube
  playerMaterials[1] = Material.create("texture",
      "/CubePlayer/Green_CubePlayer.png",
      "green.jpg",
      "blue.jpg",
      "red.jpg",
      "normalTestNormalMap.jpg",
      "glossTestMatNo.jpg",
      1, 1, 1, -- diffuse color
      1, 1, 1, -- specular color
      5, -- shinyness
      0, -- scroll multiplier for texture 1
      0, -- scroll multiplier for texture 2
      0, -- scroll multiplier for texture 3
      1, -- transparency for first texture
      1, -- transparency for second texture
      1, -- transparency for third texture
      0) --green cube
  playerMaterials[2] = Material.create("texture",
      "/CubePlayer/Blue_CubePlayer.png",
      "green.jpg",
      "blue.jpg",
      "red.jpg",
      "normalTestNormalMap.jpg",
      "glossTestMatNo.jpg",
      1, 1, 1, -- diffuse color
      1, 1, 1, -- specular color
      5, -- shinyness
      0, -- scroll multiplier for texture 1
      0, -- scroll multiplier for texture 2
      0, -- scroll multiplier for texture 3
      1, -- transparency for first texture
      1, -- transparency for second texture
      1, -- transparency for third texture
      0) --blue cube
  
  playerMaterials[3] = Material.create("texture",
      "/SpherePlayer/Red_SpherePlayer.png",
      "green.jpg",
      "blue.jpg",
      "red.jpg",
      "normalTestNormalMap.jpg",
      "glossTestMatNo.jpg",
      1, 1, 1, -- diffuse color
      1, 1, 1, -- specular color
      5, -- shinyness
      0, -- scroll multiplier for texture 1
      0, -- scroll multiplier for texture 2
      0, -- scroll multiplier for texture 3
      1, -- transparency for first texture
      1, -- transparency for second texture
      1, -- transparency for third texture
      0) --red sphere
  playerMaterials[4] = Material.create("texture",
      "/SpherePlayer/Green_SpherePlayer.png",
      "green.jpg",
      "blue.jpg",
      "red.jpg",
      "normalTestNormalMap.jpg",
      "glossTestMatNo.jpg",
      1, 1, 1, -- diffuse color
      1, 1, 1, -- specular color
      5, -- shinyness
      0, -- scroll multiplier for texture 1
      0, -- scroll multiplier for texture 2
      0, -- scroll multiplier for texture 3
      1, -- transparency for first texture
      1, -- transparency for second texture
      1, -- transparency for third texture
      0) --green sphere
  playerMaterials[5] = Material.create("texture",
      "/SpherePlayer/Blue_SpherePlayer.png",
      "green.jpg",
      "blue.jpg",
      "red.jpg",
      "normalTestNormalMap.jpg",
      "glossTestMatNo.jpg",
      1, 1, 1, -- diffuse color
      1, 1, 1, -- specular color
      5, -- shinyness
      0, -- scroll multiplier for texture 1
      0, -- scroll multiplier for texture 2
      0, -- scroll multiplier for texture 3
      1, -- transparency for first texture
      1, -- transparency for second texture
      1, -- transparency for third texture
      0) --blue sphere
  
  playerMaterials[6] = Material.create("texture",
      "/TrisPlayer/Red_TrisPlayer.png",
      "green.jpg",
      "blue.jpg",
      "red.jpg",
      "normalTestNormalMap.jpg",
      "glossTestMatNo.jpg",
      1, 1, 1, -- diffuse color
      1, 1, 1, -- specular color
      5, -- shinyness
      0, -- scroll multiplier for texture 1
      0, -- scroll multiplier for texture 2
      0, -- scroll multiplier for texture 3
      1, -- transparency for first texture
      1, -- transparency for second texture
      1, -- transparency for third texture
      0) --red pyramid
  playerMaterials[7] = Material.create("texture",
      "/TrisPlayer/Green_TrisPlayer.png",
      "green.jpg",
      "blue.jpg",
      "red.jpg",
      "normalTestNormalMap.jpg",
      "glossTestMatNo.jpg",
      1, 1, 1, -- diffuse color
      1, 1, 1, -- specular color
      5, -- shinyness
      0, -- scroll multiplier for texture 1
      0, -- scroll multiplier for texture 2
      0, -- scroll multiplier for texture 3
      1, -- transparency for first texture
      1, -- transparency for second texture
      1, -- transparency for third texture
      0) --green pyramid
  playerMaterials[8] = Material.create("texture",
      "/TrisPlayer/Blue_TrisPlayer.png",
      "green.jpg",
      "blue.jpg",
      "red.jpg",
      "normalTestNormalMap.jpg",
      "glossTestMatNo.jpg",
      1, 1, 1, -- diffuse color
      1, 1, 1, -- specular color
      5, -- shinyness
      0, -- scroll multiplier for texture 1
      0, -- scroll multiplier for texture 2
      0, -- scroll multiplier for texture 3
      1, -- transparency for first texture
      1, -- transparency for second texture
      1, -- transparency for third texture
      0) --blue pyramid
  
  -------------------------------------------------------------
                  --GATE MESHES--
  -------------------------------------------------------------
 
  gateMeshes[0] = Mesh.create("Cube_gate.obj")
  gateMeshes[1] = Mesh.create("Sphere_gate.obj")
  gateMeshes[2] = Mesh.create("Tris_gate.obj")
  gateMeshes[3] = Mesh.create("ShapeIrrelevant_gate.obj")
  gateMeshes[4] = Mesh.create("X_gate.obj")
  
  stageOneGateCount = 4
  stageTwoGateCount = 6
  amountOfGates = 3
  gateWaveSpawnTimeInSec = 4 
  gateZDestructionOffset = 5
  gateSpeed = 20
  passedWaveCount = 0
  lifeCount = 2
  currentGateColor = 0 
  currentPlayerColor = 0 
  currentGateCount = 0 
  createdGateCount = 0
  passedSuccessfully = 0
  score = 0
  gateScore = 1
  gateScoreMultiplier = 1
  scoreCounter = 1
  difficultyCount = 0
  stageOneHigh = 5
  stageTwoHigh = 10
  stageThreeHigh = 15
  stageFourIncreaseStep = 5
  stageCount = 1
  gameOver = false
  gateMaterialNumber = 0
  playerMaterialNumber = 0
  highestSuccessfullyPassedGate = 0
  truePlayerColor = 0
  currentShapeNumber = GameObject.getShape(player)
  timeSpentPlaying = 0
 
  Game.initializeGameManager(
    player, stageOneGateCount, stageTwoGateCount, 
    amountOfGates, gateSpeed, gateWaveSpawnTimeInSec, 
    gateZDestructionOffset)
  
  highScore = Stats.getValue("highScore")
  passedSuccessfullySaved = Stats.getValue("highestCombo") 
  difficultyCountSaved = Stats.getValue("difficulty")
  
  displayingFeedback = false
  achievementWait = 1
  passedHighscore = false

  -------------------------------------------------------------
                  --SOUND LOAD--
  -------------------------------------------------------------

  music = Music.create("bg_music.wav", 100, true)
  Music.play(music)
  
  pickupSound = Sound.create("pickup.wav", 100, false)
  laneSwapSound = Sound.create("laneswitch.wav", 100, false)
  winSound = Sound.create("gatewin.wav", 100, false)
  loseSound = Sound.create("gatelose.wav", 100, false)
  deathSound = Sound.create("death.wav", 100, false)
  
  scoreSound1 = Sound.create("Cool.wav", 100, false)
  scoreSound2 = Sound.create("Great.wav", 100, false)
  scoreSound3 = Sound.create("Amazing.wav", 100, false)
  scoreSound4 = Sound.create("Spectacular.wav", 100, false)
  
  comboSound1 = Sound.create("Wuew.wav", 100, false)
  comboSound2 = Sound.create("Skippippip.wav", 100, false)
  comboSound3 = Sound.create("GOGOGO.wav", 100, false)
  comboSound4 = Sound.create("MovesPlays.wav", 100, false)
  
  embarassingSound1 = Sound.create("SoClose.wav", 100, false)
  embarassingSound2 = Sound.create("TryAgain.wav", 100, false)
  embarassingSound3 = Sound.create("YouHadIt.wav", 100, false)
  embarassingSound4 = Sound.create("PrettyGoodHuh.wav", 100, false)
  
  --gameOverSound1 = Sound.create("PlayingIsEasy.wav", 100, false)
  highScoreSound = Sound.create("HighscoreCheer.wav", 100, false)
  tutorialFailSound1 = Sound.create("TutorialMoveKeys.wav", 100, false)
  tutorialFailSound2 = Sound.create("WhiteSpace.wav", 100, false)
  tutorialFailSound3 = Sound.create("ColorAndShape.wav", 100, false)

  UI.initializeText("lifeCount", 40, 1, 1, 1, 1)
  UI.initializeText("feedback", 24, 1, 1, 0, 1)
  UI.initializeText("achievementGood", 16, 0, 1, 0, 1)
  UI.initializeText("achievementBad", 16, 1, 1, 0, 1)
  UI.initializeText("score", 40, 1, 1, 1, 1)
  UI.initializeText("highScore", 32, 1, 1, 1, 1)
  UI.initializeText("combo", 36, 1, 1, 1, 1)
  UI.initializeText("wip", 14, 1, 1, 1, 1)
  
  UI.setText("wip", "version 0.9 build 4 - (c)2018 AIFC - All rights reserved", 0, 930, 0.35, 0, false)
  UI.setText("combo", "Combo: "..passedSuccessfully, 0, 200, 0.05, 0, false)
  UI.setText("score", "Score: 0", 0, 100, 0.05, 0, false)
  UI.setText("highScore", "Highscore: "..highScore, 0, 130, 0.05, 0, false)
  UI.setText("lifeCount", "Lives: "..lifeCount, 0, 100, 0.84, 0, false)
  
  print("End of start function")
end

  -------------------------------------------------------------
                  --STAGE CHANGER--
  -------------------------------------------------------------

function StageChanger(stage)
  if (Game.isPaused() == false) then
    if (stage==1) then
      stageCount = 1
      gateScore = 1
      gateScoreMultiplier = 1
      gateWaveSpawnTimeInSec = 4
      gateSpeed = 16
      Game.setGateSpeed(gateSpeed)
    elseif (stage==2) then
      stageCount = 2
      gateScore = 2
      gateScoreMultiplier = 1
      gateWaveSpawnTimeInSec = 3.5
      gateSpeed = 18
      Game.setGateSpeed(gateSpeed)   
    elseif (stage==3) then
      stageCount = 3
      gateScore = 3
      gateScoreMultiplier = 1
      gateWaveSpawnTimeInSec = 2.5
      gateSpeed = 19
      Game.setGateSpeed(gateSpeed) 
    elseif (stage==4) then
      stageCount = 4
      gateScore = 4
      gateScoreMultiplier = 1
      gateWaveSpawnTimeInSec = 2.5
      gateSpeed = 22
      Game.setGateSpeed(gateSpeed) 
    end
  end
end

  -------------------------------------------------------------
                  --INCREASE SPEED--
  -------------------------------------------------------------
  
function IncreaseSpeed()
  gateScore = 4
  gateScoreMultiplier = gateScoreMultiplier + 0.1
  if (difficultyCount<=6) then
    gateWaveSpawnTimeInSec = gateWaveSpawnTimeInSec - 1/difficultyCount
    gateSpeed = gateSpeed + 3/difficultyCount
  elseif (difficultyCount > 6 and difficultyCount < 20) then
    gateWaveSpawnTimeInSec = gateWaveSpawnTimeInSec - 0.4/difficultyCount
    gateSpeed = gateSpeed + 1.5/difficultyCount
  elseif (difficultyCount >= 20) then
    gateWaveSpawnTimeInSec = gateWaveSpawnTimeInSec - 0.5/difficultyCount
    gateSpeed = gateSpeed + 1/difficultyCount
  end
  Game.setGateSpeed(gateSpeed)
  modifiedSpeed = true
end

  -------------------------------------------------------------
                  --DECREASE SPEED--
  -------------------------------------------------------------

function DecreaseSpeed()
  gateScore = 4
  gateScoreMultiplier = gateScoreMultiplier - 0.3
  if (gateScoreMultiplier<1) then
    gateScoreMultiplier = 1
  end 
  if (difficultyCount<=6) then
    gateWaveSpawnTimeInSec = gateWaveSpawnTimeInSec + 1/difficultyCount
    gateSpeed = gateSpeed - 3/difficultyCount
  elseif (difficultyCount > 6 and difficultyCount < 20) then
    gateWaveSpawnTimeInSec = gateWaveSpawnTimeInSec + 0.5/difficultyCount
    gateSpeed = gateSpeed - 1.6/difficultyCount
  elseif (difficultyCount >= 20) then
    gateWaveSpawnTimeInSec = gateWaveSpawnTimeInSec + 0.53/difficultyCount
    gateSpeed = gateSpeed - 1.1/difficultyCount
  end
  Game.setGateSpeed(gateSpeed)
  modifiedSpeed = true
end

  -------------------------------------------------------------
                  --DIFFICULTY ADJUSTER--
  -------------------------------------------------------------

function DifficultyAdjuster()
  if (difficultyCountSaved == 1) then
    stageOneHigh = 1
    stageTwoHigh = 5
    stageThreeHigh = 10
  elseif (difficultyCountSaved == 2) then
    stageOneHigh = 1
    stageTwoHigh = 2
    stageThreeHigh =10
  elseif (difficultyCountSaved >= 3) then
    stageOneHigh = 1
    stageTwoHigh = 2
    stageThreeHigh = 3
  elseif (difficultyCountSaved == 0) then
    stageOneHigh = 3
    stageTwoHigh = 5
    stageThreeHigh = 10
  end
end

  -------------------------------------------------------------
                  --SCORE UPDATER--
  -------------------------------------------------------------

function ScoreUpdater()
  if (stageCount==1 and passedSuccessfully >= stageOneHigh and passedSuccessfully < stageTwoHigh) then
    StageChanger(2)
    difficultyCount = 1
  elseif (stageCount==2 and passedSuccessfully >= stageTwoHigh and passedSuccessfully < stageThreeHigh) then
    StageChanger(3)
    difficultyCount = 2
  elseif (stageCount==3 and passedSuccessfully >= stageThreeHigh) then
    StageChanger(4)
    difficultyCount = 3
  elseif (stageCount==4 and passedSuccessfully % stageFourIncreaseStep == 0 and passedSuccessfully > 0) then
    IncreaseSpeed() 
    difficultyCount = difficultyCount + 1
  elseif (stageCount==4 and passedSuccessfully==0) then
    if (difficultyCount==3) then
      StageChanger(4)
    elseif (difficultyCount>=4) then
      DecreaseSpeed()
    elseif (difficultyCount==2) then
      StageChanger(3)
    end
  elseif (stageCount==3 and passedSuccessfully==0) then
    if (difficultyCount==2) then
      StageChanger(3)
    elseif (difficultyCount==1) then
      StageChanger(2)
    end
  elseif (stageCount==2 and passedSuccessfully==0) then
    if (difficultyCount==1) then
      StageChanger(2)
    elseif (difficultyCount==0) then
      StageChanger(1)
    end
  elseif (difficultyCount==0) then
    StageChanger(1)
  end
end

  -------------------------------------------------------------
                  --GATE HANDLER--
  -------------------------------------------------------------

function GateHandler()
  if (Game.isPaused() == false ) then
    if (Game.interval(gateWaveSpawnTimeInSec) == true) then    
      timeSpentPlaying = timeSpentPlaying + gateWaveSpawnTimeInSec
      DifficultyAdjuster()
      ScoreUpdater()
      createdGateCount = createdGateCount + amountOfGates
      currentGateCount = currentGateCount + amountOfGates
      if (currentGateCount > 3) then
        local color = 0
        repeat
          color = math.random(0, 2)
        until color ~= currentGateColor
        
        currentGateColor = color
        table.insert(gateColorArray, currentGateColor)
      end
      CreateGateWave(currentGateColor) 
    end
  end

  if (Game.hasPassedWave(player)) then
    currentGateCount = currentGateCount - amountOfGates
    if (currentGateCount == 0) then
      -- gate color is set to player color
      local color = 0
      repeat
        color = math.random(0, 2)
      until color ~= currentPlayerColor
      
      currentPlayerColor = color
      local numba1 = GameObject.getShape(player)
      MaterialAssigner(numba1, color, 1)
      --MaterialAssigner(numba1, color, 0)
      GameObject.setExistingMaterial(player, playerMaterials[playerMaterialNumber], color)
      currentGateColor = color
      truePlayerColor = currentGateColor
    else
      -- player changes to gate color
      local newPlayerColor = gateColorArray[1]
      local numba2 = GameObject.getShape(player)
      MaterialAssigner(numba2, newPlayerColor, 1)
      
      GameObject.setExistingMaterial(player, playerMaterials[playerMaterialNumber], newPlayerColor) 
      table.remove(gateColorArray, 1)
      truePlayerColor = newPlayerColor
    end
    
    if (Game.passedGateSuccessfully()) then
      --print("Passed gate successfully!")
      
      UI.setText("feedback", "", 0, 100, 0.2, 0, false)
      displayingFeedback = false
      
      if (achievementWait==0) then
        UI.setText("achievementGood", "", 0, 200, 0.2, 0, false)
        UI.setText("achievementBad", "", 0, 200, 0.2, 0, false)
        if (stageCount<4) then
          achievementWait = 1
        else
          achievementWait = 2
        end
      else
        achievementWait = achievementWait - 1
      end
      
      passedSuccessfully = passedSuccessfully + 1
      UI.setText("combo", "Combo: "..passedSuccessfully, 0, 200, 0.05, 0, false)
      Sound.play(winSound)
      score = score + gateScore * gateScoreMultiplier
      score = math.floor(score + 0.5)
      local scoreDisplay = "Score: "..score
      
      if (highScore>=score) then
        UI.setText("highScore", "Highscore: "..highScore-score, 0, 130, 0.05, 0, false)
      else
        highScore = score
        if (passedHighscore==false) then
          UI.setText("highScore", "", 0, 130, 0.05, 0, false)
          Sound.play(highScoreSound)
          passedHighscore = true
        end
      end
      UI.setText("score", scoreDisplay, 0, 100, 0.05, 0, false)
      
      if (score>9 and score<14) then
        UI.setText("achievementGood", "KINDA COOL SCORE", 0, 200, 0.2, 0, false)
        Sound.play(scoreSound1)
      end
      if (score>99 and score<120) then
        UI.setText("achievementGood", "YOU ARE DOING IT!", 0, 200, 0.2, 0, false)
        Sound.play(scoreSound2)
      end
      if (score>500 and score<520) then
        UI.setText("achievementGood", "GREAT SCORE!", 0, 200, 0.2, 0, false)
        Sound.play(scoreSound3)
      end
      if (score>1000 and score<1050) then
        UI.setText("achievementGood", "SUPREME SCORE!", 0, 200, 0.2, 0, false)
        Sound.play(scoreSound4)
      end
      
      if (passedSuccessfully==20) then
        UI.setText("achievementGood", "YOU BADASS MOTHERFUCKER!", 0, 200, 0.2, 0, false)
        Sound.play(comboSound1)
      end
      if (passedSuccessfully==50) then
        UI.setText("achievementGood", "CAN YOU DO EVEN BETTER COMBO?", 0, 200, 0.2, 0, false)
        Sound.play(comboSound2)
      end
      if (passedSuccessfully==100) then
        UI.setText("achievementGood", "MOM WOULD BE PROUD OF THIS COMBO", 0, 200, 0.2, 0, false)
        Sound.play(comboSound3)
      end
      if (passedSuccessfully==200) then
        UI.setText("achievementGood", "COMBO HALL OF FAME, BABY", 0, 200, 0.2, 0, false)
        Sound.play(comboSound4)
      end
    else
      --print("Passed gate failed!")
      if (passedSuccessfully<20 and passedSuccessfully>10) then
        UI.setText("achievementBad", "WE WON'T TELL ANYONE", 0, 215, 0.2, 0, false)
        Sound.play(embarassingSound1)
      end
      if (passedSuccessfully>20 and passedSuccessfully<50) then
         UI.setText("achievementBad", "TRY HARDER...", 0, 215, 0.2, 0, false)
         Sound.play(embarassingSound2)
      end
      if (passedSuccessfully>50 and passedSuccessfully<100) then
        UI.setText("achievementBad", "YOU CAN'T STOP NOW", 0, 215, 0.2, 0, false)
        Sound.play(embarassingSound3)
      end
      if (passedSuccessfully>100) then
         UI.setText("achievementBad", "YOU ALMOST HAD IT!", 0, 215, 0.2, 0, false)
         Sound.play(embarassingSound4)
      end
      Sound.play(loseSound)     
      if (lifeCount > 0) then
        lifeCount = lifeCount - 1
        UI.setText("lifeCount", "Lives: "..lifeCount, 0, 100, 0.84, 0, false)
        if (passedSuccessfully > highestSuccessfullyPassedGate) then
          highestSuccessfullyPassedGate = passedSuccessfully
        end
        passedSuccessfully = 0
        UI.setText("combo", "Combo: "..passedSuccessfully, 0, 200, 0.05, 0, false)
          if (difficultyCount<=0) then
            difficultyCount = 0
          else
            difficultyCount = difficultyCount - 1
          end
      else
        GameOver()
      end
    end
    passedWaveCount = passedWaveCount + 1;
  end
end

  -------------------------------------------------------------
                           --TUTORIAL FEEDBACK--
  -------------------------------------------------------------
  
  function TutorialFeedback()
    if (passedSuccessfully==0 and displayingFeedback==false and difficultyCount<15) then
            if (stageCount==1) then
              UI.setText("feedback", "PRESS A, D TO MOVE;  PRESS J, K, L to SWITCH SHAPE", 0, 100, 0.5, 0, true)
              Sound.play(tutorialFailSound1)
            elseif (stageCount==2) then
              UI.setText("feedback", "YOU CAN ALWAYS PASS THROUGH WHITE SPACE", 0, 100, 0.5, 0, true)
              Sound.play(tutorialFailSound2)
            elseif (stageCount==3) then
              UI.setText("feedback", "ALIGN YOUR COLOR AND SHAPE WITH THE CORRECT GATE", 0, 100, 0.5, 0, true)
              Sound.play(tutorialFailSound3)
            elseif (stageCount==4 and difficultyCount<=10) then
              UI.setText("feedback", "WATCH OUT FOR WALLS", 0, 100, 0.5, 0, true)
            end
      displayingFeedback = true
    end
  end

  -------------------------------------------------------------
                  --GAME OVER--
  -------------------------------------------------------------

function GameOver()
  --Game.setPaused(true)
  gateSpeed = 0
  Game.setGateSpeed(gateSpeed)
  Sound.play(deathSound)
  --[[if (score>500) then
    Sound.play(gameOverSound1)
  end]]
  score = 0
  
  Stats.updateMap("timeSpent", timeSpentPlaying)
  Stats.updateMap("difficulty", difficultyCountSaved)
  Stats.updateMap("highestCombo", passedSuccessfullySaved)
  Stats.updateMap("highScore", highScore)
  Stats.updateMap("gatesPassed", highestSuccessfullyPassedGate)
  Stats.save()
  
  Game.displayResolution();
end

  -------------------------------------------------------------
                  --CREATE GATE WAVE--
  -------------------------------------------------------------

function CreateGateWave( playerColor )
  local gateColorType = -1
  local gateShapeType = -1
  local spacing = 5
  local gateAmountOffset = 1
  local randomIndex = math.random(1, amountOfGates)
   
  for index = 1, amountOfGates do
    if (randomIndex == index) then
      if (stageCount==1) then
        gateShapeType = math.random(0, 2) --YOUR GATE
        gateColorType = 3
      elseif (stageCount==2) then
        gateShapeType = 3
        gateColorType = 3
      elseif (stageCount==3) then
        gateShapeType = math.random(0, 3)
        gateColorType = playerColor
      elseif (stageCount==4) then
        gateShapeType = math.random(0, 3)
        gateColorType = playerColor
      end
    else 
      if (stageCount==1) then
        gateShapeType = 4 --OTHER GATES
      elseif (stageCount==2) then
        gateShapeType = math.random(0, 3)
      elseif (stageCount==3) then
        gateShapeType = math.random(0, 3)
      elseif (stageCount==4) then
        gateShapeType = math.random(0, 4)
      end
      repeat
        if (stageCount==1) then
        gateColorType = 3
        elseif (stageCount==2) then
        gateColorType = math.random(0, 2)
        elseif (stageCount==3) then
        gateColorType = math.random(0, 2)
        elseif (stageCount==4) then
        gateColorType = math.random(0, 3)
        end
      until gateColorType ~= playerColor
    end
    
    local gateName = "Gate: "..index.." in wave: "..passedWaveCount
    local gatePosX = ((index - 1) - gateAmountOffset) * spacing
    MaterialAssigner(gateShapeType, gateColorType, 0)
    CreateGate(gateName, gatePosX, 1, -80, 0, 0, 1, gateColorType, gateShapeType) 
  end
  
  Game.createProps(amountOfGates, gateAmountOffset, 7, 10, 1)
end

  -------------------------------------------------------------
                  --MATERIAL ASSIGNER--
  -------------------------------------------------------------

function MaterialAssigner(shape, color, target)
  if (shape==0) then
    if (color==0) then
      --red cube
      if (target==1) then
        playerMaterialNumber = 0 --target 1 is player
      else
        gateMaterialNumber = 0
      end
    elseif (color==1) then
      --green cube
      if (target==1) then
        playerMaterialNumber = 1
      else
        gateMaterialNumber = 1
      end
    elseif (color==2) then
      --blue cube
      if (target==1) then
        playerMaterialNumber = 2
      else
        gateMaterialNumber = 2
      end
    elseif (color==3) then
      --white cube 
      gateMaterialNumber = 3
    end
  elseif (shape==1) then
    if (color==0) then
      --red sphere
      if (target==1) then
        playerMaterialNumber = 3
      else
        gateMaterialNumber = 4
      end
    elseif (color==1) then
      --green sphere
      if (target==1) then
        playerMaterialNumber = 4
      else
        gateMaterialNumber = 5
      end
    elseif (color==2) then
      --blue sphere
      if (target==1) then
        playerMaterialNumber = 5
      else
        gateMaterialNumber = 6
      end
    elseif (color==3) then
      --white sphere 
      gateMaterialNumber = 7
    end
  elseif (shape==2) then
    if (color==0) then
      --red pyramid
      if (target==1) then
        playerMaterialNumber = 6
      else
        gateMaterialNumber = 8
      end
    elseif (color==1) then
      --green pyramid
      if (target==1) then
        playerMaterialNumber = 7
      else
        gateMaterialNumber = 9
      end
    elseif (color==2) then
      --blue pyramid
      if (target==1) then
        playerMaterialNumber = 8
      else
        gateMaterialNumber = 10
      end
    elseif (color==3) then
      --white pyramid 
      gateMaterialNumber = 11
    end
  elseif (shape==3) then
    if (color==0) then
      --red anyshape 
      gateMaterialNumber = 12
    elseif (color==1) then
      --green anyshape 
      gateMaterialNumber = 13
    elseif (color==2) then
      --blue anyshape
      gateMaterialNumber = 14
    elseif (color==3) then
      --white anyshape 
      gateMaterialNumber = 15
    end
  elseif (shape==4) then
    if (color==0) then
      --red wall
      gateMaterialNumber = 16
    elseif (color==1) then
      --green wall
      gateMaterialNumber = 17
    elseif (color==2) then
      --blue wall 
      gateMaterialNumber = 18
    elseif (color==3) then
      --white wall
      gateMaterialNumber = 19
    end
  end
end

-------------------------------------------------------------
                      --CREATE GATE--
-------------------------------------------------------------

function CreateGate(name, posX, posY, posZ, moveX, moveY, moveZ, colorType, shapeType)
  gate = Gate.create(name, "Gate", posX, posY, posZ)
  GameObject.setBehaviour(gate, "GateBehaviour", moveX, moveY, moveZ, colorType, shapeType)
  GameObject.setExistingMesh(gate, gateMeshes[shapeType]) --shapeType
  GameObject.setExistingMaterial(gate, gateMaterials[gateMaterialNumber]) --gateMaterialNumber
end

  -------------------------------------------------------------
                           --UPDATE--
  -------------------------------------------------------------

function Update(pStep)
	GateHandler()
  TutorialFeedback()
  if (difficultyCountSaved<difficultyCount) then
    difficultyCountSaved = difficultyCount 
  end
  if (passedSuccessfullySaved<passedSuccessfully) then
    passedSuccessfullySaved = passedSuccessfully
  end
  if (GameObject.getShape(player) ~= currentShapeNumber) then
    MaterialAssigner(GameObject.getShape(player), truePlayerColor, 1)
    GameObject.setExistingMaterial(player, playerMaterials[playerMaterialNumber], truePlayerColor)
    currentShapeNumber = GameObject.getShape(player)
  end
  
end