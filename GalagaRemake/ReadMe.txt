Need to transfer kinetic energy vs momentum as velocity has a bigger impact than mass
	objects with large mass shooting other objects off of it feels weird
Conflating detecting and object changes may not be the best idea
Need to fix the id system IDK why its so buggy






Design Themes(*will be updated as/if themes change)
---------------------------------------------------------------
- Every manager should know how to manage their assets for the production of the game
- The orchestrators and the creators need to tell the managers what to do with their part of the production,
	and the managers implement those tasks as needed
- extern managers wherever you need has been nice, but should prob put them all in level orchestrator and
  they should comminicate to each other on that level field  



TODOs
---------------------------------------------------------------


- Think every object that has points to be had, should pocess their own version of what they want that temp text to look like
- remove control selection widow, tie control changes to function keys instead for now, menus further down the line


- Scrub world bounds from all parameters, use GameState::WorldBounds in its stead
- Need to move some GameState stuff into new LevelState object or just within the levels themselves


Notes
---------------------------------------------------------------
- all sprites need to be placed in C:\Users\Sean McCrobie\source\repos\GalageRemake\GalagaRemake in order to be loaded correctly


Bugs/Visual issues
---------------------------------------------------------------
- Sometimes point values can appear to close to the edge of the screen, should investigate


Code Comments
---------------------------------------------------------------
"this is why you need to standardize ship creation, prob set position before any creation period"

