This file documents parts of the OoT save data that go unused in vanilla but are used by the randomizer. **It may be incomplete.**

# Scene flags

## Unused field

The unused field (offset 0x10) of the permanent scene flags (save context + 0xd4 + 0x1c * scene ID) is used for the following purposes:

* Total small key count: Scenes 0x00–0x0F (Each dungeon uses its own unused field. There's still room here for other dungeon-specific features.)
* Scrub Shuffle: Scenes 0x10–0x27
* Shopsanity: Scene 0x2C
* FW in both ages: Scenes 0x3E–0x47
* Triforce Hunt: Scene 0x48
* Pending ice traps: Scene 0x49

## Collectibles field

With `shuffle_cows`, the flags representing which cows have been talked to are stored in the collectibles field (offset 0x0c) of the permanent scene flags of the cow's respective scene:

* Jabu Jabus Belly MQ Cow: scene 0x02, bit 0100_0000
* KF Links House Cow: scene 0x34, bit 0100_0000
* LLR Stables Right Cow: scene 0x36, bit 0200_0000
* LLR Stables Left Cow: scene 0x36, bit 0100_0000
* Kak Impas House Cow: scene 0x37, bit 0100_0000
* HF Cow Grotto Cow: scene 0x37, bit 0200_0000
* DMT Cow Grotto Cow: scene 0x3e, bit 0100_0000
* LLR Tower Left Cow: scene 0x4c, bit 0200_0000
* LLR Tower Right Cow: scene 0x4c, bit 0100_0000
* GV Cow: scene 0x5a, bit 0100_0000

With `shuffle_beans` or `shuffle_medigoron_carpet_salesman`, flags for the bean salesman or Medigoron and the carpet salesman, respectively, are similarly stored in collectibles fields:

* ZR Magic Bean Salesman: scene 0x54, bit 0000_0002
* Wasteland Bombchu Salesman: scene 0x5e, bit 0000_0002
* GC Medigoron: scene 0x62, bit 0000_0002

# inf_table

Additional flags stored in `inf_table` (an array of 16-bit integers at save context + 0x0ef8):

* Entry 0x1b, bit 0002 is set when the Temple of Time altar is read as child. This allows the pause menu dungeon info to display stone locations depending on settings.
* Entry 0x1b, bit 0001 is set when the Temple of Time altar is read as adult. This allows the pause menu dungeon info to display medallion locations depending on settings.
