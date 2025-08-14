# Supermoves8
Completely custom map-loading apparatus for Supermoves [WIP]

**The latest release will be found [here](https://github.com/Aeyth8/Supermoves8/releases)**

## How to install the Maploader:

* Download ``dxgi.dll`` and ``GYM_Maps.zip`` and place them in your game's directory, which will be located in your Steam folder.

* Your Steam folder should be similar to ``Steam\steamapps\common\Supermoves\GYMGAME\Binaries\Win64``

* Extract ``GYM_Maps.zip`` into the folder, you can delete the zip file afterwards.

* Launch the game, you should be good to go!

## How to use the Maploader:

* Open the UConsole by pressing Tilde ``~`` on your keyboard, it is the key above ``Tab``
* (If you are on a smaller form factor keyboard/laptop, press ``FN`` + ``Esc``)

* Choose the **MapIndex** from the identifier on ``MapIndex.txt``, and choose the **MapName** from the list below

```
Courtyard_Empty = Courtyard
Flatlands_Empty = Flatlands
Industrial_Empty = Industrial
Ocean_Empty = Ocean
Plywood_Empty = Warehouse
Rooftops_Emilia = Rooftops
Schoolyard_Empty= Schoolyard
Stadium_Small_Empty = Stadium
Streets_Empty = Streets
Tutorial_Empty = Gym
TournamentLobby = Tournament
```

* Type loadmap {**MapIndex**}?{**MapName**} and press enter, be aware that you have to manually select the map template, if the original map uses a different map template the placement will be broken.
* Example : ``loadmap 9143?Tutorial_Empty`` will load you into the map ``Don't drown in balls haha by ddanel``

