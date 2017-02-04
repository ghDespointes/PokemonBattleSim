Pokemon Battle Simulator

Author: Guilain Huyghues-Despointes

Summary:
	A text based Pokemon battle sim. Create a team using any
first generation fully evoled pokemon.

Currently Implemented:
	1. Full battle simulation
		1.1. Use attacks your pokemon know
		1.2. Switch out to any pokemon
		1.3. Battle an AI
		1.4. Working status conditions (Sleep, Burn, Freeze, Poison, ...)
		1.5. Damage calculation
		1.6. Stat boosts
		1.7. Critical hits
	2. 69 fully evolved Generation 1 Pokemon
	3. Roughly 70 Generation 1 moves (See list of moves below)
	4. Player generated teams to fight with and against
		4.1. Customizable Evs and IVs

Things to Implement in the Future:
	1. Hold items
	2. Pokemon Abilities
	3. More moves and more Pokemon
	4. Double battles
	5. Mega evolutions and Z moves

NOTE: Due to the fact that many moves are not implemented, pokemon 
	currently have access to any move.

------------------------------------------------------------------------

Quick Pokemon Team Maker Tips

To edit the teams look into the PlayerTeam.txt and EnemyTeam.txt files

EVs increase stats by 1 for every 4
IVs increase stats by 1 for every 1

Physical attacks use the attacking Pokemon's Attack and the defeneding Pokemon's Defense
Special attacks use the attacking Pokemon's Special Attack and defending Pokemon's Special Defense
Status moves don't deal damage but have a variety of other effects

Each nature increases a stat by 10% and decreases another by the same ammount

The general format for teams is shown in the files themselves

------------------------------------------------------------------------

List of Currently Implemented Moves in no particular order (Use ctr-f to find what's here)

Pound
Karate Chop
Double Slap
Comet Punch
Mega Punch
Pay Day
Fire Punch
Ice Punch
Thunder Punch
Scratch
Vice Grip
Guillotine
Swords Dance
Cut
Gust
Wing Attack
Whirlwind
Slam
Vine Whip
Stomp
Double Kick
Mega Kick
Jump Kick
Rolling Kick
Sand Attack
Headbutt
Horn Attack
Fury Attack
Horn Drill
Tackle
Body Slam
Take Down
Double-Edge
Tail Whip
Poison Sting
Twin Needle
Pin Missle
Leer
Bite
Growl
Roar
Sing
Supersonic
Sonic Boom
Disable
Acid
Ember
Flamethrower
Water Gun
Hydro Pump
Surf
Ice Beam
Blizzard
Psybeam
Bubble Beam
Aurora Beam
Peck
Drill Peck
Submission
Low Kick
Counter
Seismic Toss
Strength
Absorb
Mega Drain
Growth
Razor Leaf
Poison Powder
Stun Powder
Sleep Powder
String Shot
Bubble

------------------------------------------------------------------------

Behind the Scenes: Class Explanation

Util:	Utility
	Holds any and all Magic Numbers used by the rest of the code
	Holds all the pokemon and move data
	Contains all enum values
	Contains any necesarry Utility functions (Converting enums to strings)

FileReader: Utility File Parser
	Reads any files and passes it on in a usable format
	Also has uses for string manipulation (Formatting)

Pokemon: Holds Pokemon data
	Read Only
	Holds all the pokemon base data
		Name
		Base Stats
		Typing

Move: Holds Move data
	Read Only
	Holds all the Move base data
		Name
		Type
		Damage Type (Physical, Special, or Status)
		Effect Function (int determining what this move does)
		BP (Base Power)
		Accurcy
		PP (# uses)
		Chance of effect occuring if not always
		Priority of move
		Description (For players)

ActivePokemon:	Actual Pokemon
	Used in teams and is what players actually use
	Refers to its corresponding class if data is needed
	Holds any dynamic pokemon data
		Name
		Level
		Item 	(Not implemented)
		Ability (Not implemented)
		Natures
		EVs (Effort values, each 4 adds 1 to stats at level 100)
		IVs (Individual values, each adds 1 to stats at level 100)
		Stats [HP, Attack, Defense, Sp. Attack, Sp. Defense, Speed]
		Stat Boosts [Attack,Defense,Sp. Attack, Sp. Defense, Speed, Accuracy, Evasion, Crit Chance]
		Health
		Active Moves (up to 4)
		Status Condition (Sleep, Burn, Poison, Freeze, Paralysis)
		Volatile Statuses (Flinched, Bound, Disabled, ...)
		and more...
	It is used to calculate any attacks and other parts of the battle

ActiveMove:	Actual Moves
	Used in teams and is what players actually use
	Refers to its corresponding class if data is needed
	Holds any dynamic move data
		Name
		MaxPP (Is always 1.6 * max PP in Move info class)
		CurrentPP

Team:	List of Active Pokemon
	Holds the user's pokemon
	Can set pokmon to the front

TurnAction:	Simple data holder
	Simple class to hold action values
	Used to determine who is doing what
	Contains information for
		User (To know which player is performing the action)
		Action (Attack, Switch, Other to know what they are doing)
		Param (Any parameter the action may require to be performed)

ActionManager:	Performs the brunt of the work
	Main class to handles all interactions
	Takes player input and AI input
	Orders the actions by type and Pokemon speed
	Performs action in the correct order
	Takes care of any specifics of the actions
	Performs clean up duties at end of turn

AIManager:	Controls the AI
	Decides on what attacks to perform 
		Prefers damaging moves when at low health
		Uses the most damaging move available
	Decides on what pokemon to switch to
		Switches to the safest choice (Deals the most damage, while taking the least)
		Only looks at active pokemon and its type

DamageManager:	Calculates any damage
	Computes type effectivness
	Determines critical hits effectivness
	Calculates damage of moves depending on user and defender

MoveEffectManager: Controls the effects of moves
	Does everything moves need it to

Future potential Classes:

Scene:
	Move any stat boosts and volatile condition into a scene.
		It wastes space having each pokemon store its own value
		when it gets reset whenever they switch.
	Contains any data about the scene (Sunny, Rainy, ...)