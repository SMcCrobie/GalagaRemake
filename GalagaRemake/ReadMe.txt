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
- all sprites need to be placed in C:\Users\Sean McCrobie\source\repos\GalageRemake\GalagaRemake in order to be loaded correctly


Bugs/Visual issues
---------------------------------------------------------------
- Sometimes point values can appear to close to the edge of the screen, should investigate

