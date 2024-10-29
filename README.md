Project features

The project features a ready-to-use gameplay framework that makes it easy for designers and other developers to add features based on the building blocks of the framework I made. 

- Third Person Character movement with Enhanced Input.

- Multiplayer/Replication support for all available gameplay features.

- An ActionSystem similar in implementation to Epic's GAS.
	-> Low performance overhead.
	-> Replication support with prediction and server-side authentication.
	-> Utilises GameplayTags throughout.
	-> Support for Actions (one-time) and Effects (over-time).
	-> Blueprint Action & Effect support for easy iteration for designers.
	-> Some actions include:
		-> Damaging projectile ability
		-> Black hole projectile ability (gravity affects simulating objects)
		-> Dash ability (projectile teleportation)
		-> Reflection ability (reflects projectiles back attackers)
	-> Some effects include:
		-> Fire damage over time effect
		-> Stunned effect
		-> Thorns effect (reflects a percentage of damage back to the attacker)

- An AttributeSystem holding the Health and Rage of the player and relevant information.
	-> Rage is given to the player upon receiving damage.
	-> Replication support with server-side authentication.
	-> Values are customizable in Blueprints on a per-usage basis.

- AI behavior and AI enemies
	-> Custom behavior tree enemy AI behavior: roaming, sensing, chasing, positioning, attacking, fleeing, hiding, healing
	-> EQS queries to support the AI's various positioning-related tasks.
	-> Custom BT tasks and services to support the AI's varios abilities: healing, attacking, etc.

- A Delegate-based logic to update UI and various other systems. 14 different delegates are used instead of ticks throughout. In shipped code, no tick functions are used.

- GameMode and other framework implementations
	-> GameMode responsibilites are separated into Components.
	-> GameMode EQS queries upon gameplay start to spawn a set of pickups in random places.
	-> GameMode reads enemy data, spawn chance weights and such from DataAssets and DataTables.
	-> GameMode spawn enemies over time, with weighted randomized chances, based on a difficulty curve.
	-> GameMode rewards players with credits upon destroying enemies.

- Various world interactions
	-> Treasure Chests can be opened, giving credits to the player.
	-> Pickups can be collected for credits, health, or even to enable new abilities.
	-> Levers can be toggled to give the player key-cards that can enable the opening of closed items (such as chests, doors, etc.)
	-> World interactors also respond with relevant localized state descriptions to queriers.

- A SaveGame system encapsulated in its own GameplaySubsystem.
	-> The SaveGame system saves various world-state elements.
	-> Not currently replicated as that would've required EOS/Steam integration for UUIDs which was outside of the project scope.

- AssetManager C++ implementation
	-> Loads the DataAssets and their DataTables asynchrounously.
	-> Also includes async loading of other assets elsewhere in the project.

- UMG implementations
	-> Main menu, pause menu, gameplay HUD, all with animations, states, and standardized behavior.

- Various GameplayTags, console commands and developer settings defined in C++.
	-> All GameplayTags are defined in C++
	-> Console commands include elements such as: Health/Damage multipliers, flags to prevent/enable spawning of enemies and drawing of debug elements in world.
	-> Developer settings include a project-level setting to specificy save-game file names without needing to launch the game from a shipped build.
