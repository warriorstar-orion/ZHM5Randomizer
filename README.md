## ZHM5Randomizer WSO

![](randomizer_logo.png)

**Now with HITMAN3 support.**

This is Warriorstar's branch of pawREP's excellent ZHM5Randomizer. It includes
a handful of new randomization types, as well as the `CUSTOM` mode which allows
for modifying random generation via the configuration file.

[Download][] the latest preview release, and install as described in the
original README below. To configure `CUSTOM` mode, follow the instructions in
`hitman_randomizer.toml`, which should have been unzipped in `HITMAN3/Retail`.

[Download]: https://github.com/warriorstar-orion/ZHM5Randomizer/releases/latest

## How It Works

There are four "inventories" that the Randomizer juggles:

1. 47's, or the "Hero" starting inventory
2. The "NPC" inventory of anyone carrying weapons, keys, or quest items
3. The "world" inventory which changes the statically placed objects in the level
4. The "stash" inventory which alters the contents of 47's stashed item

## Limitations

**This mod only supports missions started from the main menu.** This means that:

- The mod will stop working properly if you save and load a mission.
- The mod will stop working properly if you restart the mission from the pause
  menu, or the mission failed menu.
- The mod will stop working properly if you replan the mission.

## The Game Modes

The Randomizer comes pre-configured with a handful of game modes to make your
missions more fun or challenging.

### `DEFAULT` and `NONE`

Every inventory supports a `DEFAULT` config. `DEFAULT` configs all work the
same way: all items are randomly placed. In the case of weapons, they are
randomly replaced with a weapon of the same type.

Similarly, every inventory supports a `NONE` config, which disables
randomization for that inventory.

### World Configs

Besides `DEFAULT`, the "world" inventory has the following configs:

1. `OOPS_ALL_EXPLOSIVES` replaces all items with explosives.
2. `TREASURE_HUNT` randomizes items, but replaces ten of them with golden
   idols. Find them all!
3. `NO_ITEMS` replaces all non-essential items with coins, and all weapons with
   the Hackl 9S.
4. `ACTION` replaces all non-essential items with explosives, coins, or
   weapons.
5. `CUSTOM`: see the section below.

### NPC Configs

Besides `DEFAULT`, the "NPC" inventory has the following configs:

1. `UNLIMITED` replaces all NPC weapons with any weapon that the randomizer
   knows about.
2. In `HARD` mode, all NPCs will spawn with one of a handful of pre-chosen,
   non-silenced weapons.  Nearly all NPCs will be carrying SMGs instead of
   pistols.
3. `SLEEPY` mode replaces all NPC weapons with cure coins. Cure coins cause a
   person to become unconscious after being picked up, and always attracts
   NPCs.
4. `CHAIN_REACTION` mode replaces some NPC weapons with cure coins, and some
   with octane boosters.
5. `CUSTOM`: see the section below.

### `CUSTOM`

World and NPC inventories support `CUSTOM` mode. In `CUSTOM` mode, you can
configure the randomization rules yourself!

Items will only be added to the randomization if they match one of the words in
the allow list. Likewise, if an item matches a word in the ignore list, it
won't spawn.

If ignored_words are used, if an item has that string in its name or matches a
category, they will never be added to the randomization. ignored_words is kind
of buggy but the logic should still make sense.

A word in an item name like the "syringe" in "Lethal Syringe" is allowed, or a
category as listed below. These are the categories it understands:

- "melee"
- "explosives"
- "tool"
- "sniperrifle"
- "assaultrifle"
- "shotgun"
- "suitcase"
- "pistol"
- "distraction"
- "poison"
- "smg"

Here's an example configuration for `CUSTOM` mode. Try it yourself!

```toml

[Custom.World]
allowed_words = ["coin cure", "kalmer", "lethal proximity", "pristine", "Lethal Poison Syringe Modern", "Emetic Poison Syringe S2"]
ignored_words = ["remote", "anti-virus", "masamune", "sedative", "antique", "taser", "cx", "duck", "serum", "agent", "rusty", "lockpick"]

[Custom.NPC]
allowed_words = ["striker"]
ignored_words = []

```

## Known Issues

- Certain items are not subject to randomization. These include 47's katana in
  the Mountain Pass start of Situs Inversus, as well as the gifts in Holiday
  Hoarders.
- Many items unused in the game release are included in randomization. These
  items may or may not work as expected.
