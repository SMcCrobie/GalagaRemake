TODOs
----------------------------------------------------------------
- Switch all CircleProjectiles to sf::circle, so that it can be used for more than just the projectile
- Come up with an easier way to write text in UI, in formatted bi colorized way

- Maybe have BaseScale scale all text additionally so that it can be used as a settings scale across all text;


- Think every object that has points to be had, should pocess their own version of what they want that temp text to look like
- remove control selection widow, tie control changes to function keys instead for now, menus further down the line


- Scrub world bounds from all parameters, use GameState::WorldBounds in its stead
- Need to move some GameState stuff into new LevelState object or just within the levels themselves



Design Themes(*will be updated as/if themes change)
---------------------------------------------------------------
- Every manager should know how to manage their assets for the production of the game
- The orchestrators and the creators need to tell the managers what to do with their part of the production,
	and the managers implement those tasks as needed
- extern managers wherever you need has been nice, but should prob put them all in level orchestrator and
  they should comminicate to each other on that level field  


Notes
---------------------------------------------------------------
- all sprites need to be placed in C:\...\source\repos\GalagaRemake\GalagaRemake in order to be loaded correctly
- The Direction of rotate, visually, improvement
	- Have it be based off the current momentum, on the x-axis, being positive or negative
	- If its neutral, rand()
git ss
- play around with more game music options
- eventually will need a pause for sounds, and maybe just music too


Bugs/Visual issues
---------------------------------------------------------------
- Sometimes point values can appear to close to the edge of the screen, should investigate
- double firing immediatly after a turn, bug or feature??


Game Notes
===============================
- you need to add health packs into game
	- extra points for life currently had
	- can have a base point value, so even if your almost at death you can still get a
	  point bonus for collecting the pack, more for more health you have
- each level is going to have its own score
	- after you beat the game you can run all the levels back to back
	  and try and place a score on that