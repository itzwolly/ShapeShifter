	local Game = { -- following the order of the docs
		type = "lib",
		description = "thirdPersonEngine 1.00 API",
		childs = {
			interval = { 
				type = "function",
				description = "Returns true when an internal timer has waited the given amount of time in seconds, else will return false",
				args = "(number)",
				returns = "boolean",
			},
			hasPassedWave = { 
				type = "function",
				description = "Returns true when the given gameobject has passed a wave of gates, else will return false.",
				args = "(gameobject)",
				returns = "boolean",
			},
			passedGateSuccessfully = { 
				type = "function",
				description = "Returns true when the player went through the gate using the right color and shape, else returns false",
				args = "(gameobject)",
				returns = "boolean",
			},
			initializeGameManager = {
				type = "function",
				description = "Initializes the gamemanager with the needed values to operate in C++.",
				args = "(player, stageOneGateCount, stageTwoGateCount, amountOfGates, gateSpeed, gateWaveSpawnTimeInSec, gateZDestructionOffset, gatePosX, gatePosY, gatePosZ, gateDirX, gateDirY, gateDirZ)",
				returns = "",
			},
			setGameSpeed = {
				type = "function",
				description = "Sets the movement speed of the gates.",
				args = "(number)",
				returns = "",
			},
			createProps = {
				type = "function",
				description = "Creates props based on the amount of gates and the offset between gates.",
				args = "(amountOfGates, gateAmountOffset, spacing, maxPropSpacing, minMultiplier)",
				returns = "",
			},
			isPaused = {
				type = "function",
				description = "A check to see if the game is paused (by an outside source).",
				args = "",
				returns = "True if the game is paused, else false.",
			},
			setPaused = {
				type = "function",
				description = "Allows you to pause/unpause the game.",
				args = "(bool)",
				returns = "",
			},
			displayResolution = {
				type = "function",
				description = "Handles the death sequence and shows the resoltuion screen.",
				args = "",
				returns = "",
			},
		},
	}
	local GameObject = { -- following the order of the docs
		type = "lib",
		description = "thirdPersonEngine 1.00 API",
		childs = {
			-- GameObject
			create = { 
				type = "function",
				description = "Creates a new gameobject.",
				args = "(name, tag, x, y, z)",
				returns = "gameobject",
			},
			setScale = {
				type = "function",
				description = "Sets the scale of the gameobject.",
				args = "(gameobject, x, y, z)",
				returns = "",
			},
			setMesh = {
				type = "function",
				description = "Sets the mesh of the gameobject. Don't forget filetype e.g: .obj",
				args = "(gameobject, meshName)",
				returns = "",
			},
			setMaterial = {
				type = "function",
				description = "Sets the material of the gameobject. \nA list of parameters can be found here: \n\nColor: (gameobject, materialType, colorX, colorY, colorZ)\n\nTexture: (gameobject, materialType, textureName)\n\nLit: (gameobject, materialType, colorX, colorY, colorZ, shinynessValue, specularColorX, specularColorY, specularColorZ)",
				args = "( gameobject, materialType, ... )",
				returns = "",
			},
			pos = {
				type = "function",
				description = "Gets the objects position.",
				args = "(gameobject)",
				returns = "x, y, z",
			},
			rotate = {
				type = "function",
				description = "Rotates the given gameobject.",
				args = "(gameobject, angleInDegrees, x, y, z)",
				returns = "x, y, z",
			},
			setBehaviour = {
				type = "function",
				description = "Creates and sets a new behaviour to a given gameobject. \nA list of behaviours can be found here: \n\nPlayerBehaviour: (gameobject, behaviourType(string), padding (number), meshOne, meshTwo, meshThree)\n\nGateBehaviour: (gameobject, behaviourType (string), x, y, z, colorType (number), shapeType (number))",
				args = "(gameobject, behaviourType (string), ...)",
				returns = "x, y, z",
			},
			setExistingMaterial = {
				type = "function",
				description = "Applys an existing material to a gameobject.",
				args = "(gameobject, material)",
				returns = "",
			},
			setExistingMesh = {
				type = "function",
				description = "Applys an existing mesh to a gameobject.",
				args = "(gameobject, mesh)",
				returns = "",
			},
			getShape = {
				type = "function",
				description = "Gets the players shape data.",
				args = "(gameobject, mesh)",
				returns = "An integer of which shape you currently are; 0 = Cube, 1 = Shere, 2 = Pyramid",
			},
		},
	}
	local Camera = { -- following the order of the docs
		type = "lib",
		description = "thirdPersonEngine 1.00 API",
		childs = { 
			-- GameObject
			create = { 
				type = "function",
				description = "Creates a new camera.",
				args = "(name, tag, x, y, z)",
				returns = "camera",
			},
		},
	}
	local Material = { -- following the order of the docs
		type = "lib",
		description = "thirdPersonEngine 1.00 API",
		childs = { 
			-- GameObject
			create = { 
				type = "function",
				description = "Creates a new material. \nA list of parameters can be found here: \n\nColor: (materialType, colorX, colorY, colorZ)\n\nTexture: (materialType, textureName)\n\nLit: (materialType, colorX, colorY, colorZ, shinynessValue, specularColorX, specularColorY, specularColorZ)",
				args = "(materialType, ... )",
				returns = "material",
			},
		},
	}
	local Mesh = { -- following the order of the docs
		type = "lib",
		description = "thirdPersonEngine 1.00 API",
		childs = { 
			-- GameObject
			create = { 
				type = "function",
				description = "Creates a new mesh.",
				args = "(meshName (string))",
				returns = "mesh",
			},
		},
	}
	local Gate = { -- following the order of the docs
		type = "lib",
		description = "thirdPersonEngine 1.00 API",
		childs = { 
			-- GameObject
			create = { 
				type = "function",
				description = "Creates a new gate.",
				args = "(name, tag, x, y, z)",
				returns = "",
			},
		},
	}
	local Sound = { -- following the order of the docs
		type = "lib",
		description = "thirdPersonEngine 1.00 API",
		childs = { 
			-- UI
			create = {
				type = "function",
				description = "Creates a sound instance.",
				args = "(fileName, volume, isLooping)",
				returns = "sound",
			},
			play = {
				type = "function",
				description = "Plays the sound specified in the arguments.",
				args = "(sound)",
				returns = "",
			},
			stop = {
				type = "function",
				description = "Stops the sound specified in the arguments.",
				args = "(sound)",
				returns = "",
			},
		},
	}
	local Music = { -- following the order of the docs
		type = "lib",
		description = "thirdPersonEngine 1.00 API",
		childs = { 
			-- UI
			create = {
				type = "function",
				description = "Creates a music instance.",
				args = "(fileName, volume, isLooping)",
				returns = "music",
			},
			play = {
				type = "function",
				description = "Plays the music specified in the arguments.",
				args = "(music)",
				returns = "",
			},
			stop = {
				type = "function",
				description = "Stops the music specified in the arguments.",
				args = "(music)",
				returns = "",
			},
		},
	}
	local UI = { -- following the order of the docs
		type = "lib",
		description = "thirdPersonEngine 1.00 API",
		childs = { 
			-- UI
			initializeText = {
				type = "function",
				description = "Initializes a text object which can be accessed using the given tag.",
				args = "(tag, fontSize, r, g, b, a)",
				returns = "",
			},
			setText = {
				type = "function",
				description = "Sets the text of an object with the same name as the given tag.",
				args = "(tag, text, x, y, anchorX, anchorY)",
				returns = "",
			},
		},
	}
	local Stats = { -- following the order of the docs
		type = "lib",
		description = "thirdPersonEngine 1.00 API",
		childs = { 
			-- UI
			updateMap = {
				type = "function",
				description = "Updates the map, which contains the stats of the player, using its key.",
				args = "(key, value)",
				returns = "",
			},
			save = {
				type = "function",
				description = "Saves the map, which contains the stats of the player, to a file.",
				args = "",
				returns = "",
			},
			getValue = {
				type = "function",
				description = "Returns the value of a given key from the file that contains the user stats.",
				args = "(key, value)",
				returns = "(value)",
			},
		},
	}
	
return {
	Game = Game,
	GameObject = GameObject,
	Gate = Gate,
	Mesh = Mesh,
	Material = Material,
	Camera = Camera,
	Sound = Sound,
	Music = Music,
	UI = UI,
	Stats = Stats,
}
