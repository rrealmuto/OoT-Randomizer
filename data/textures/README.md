# Custom textures
To add custom textures, create RGBA PNG files of the appropriate size and place them inside the folder
data/Custom/{texture-pack-name}/{type}/{texture_name}

where
{texture-pack-name} is the name that will be used to select the set of custom textures in the GUI
{type} is chest, pot, crate (small crate textures go in the crate folder)
{texture_name} as described below

Textures must be named as follows:
Pixel dimensions are Width x Height

## Pots: 32x64 texture
- major item - texture_pot_gold.png
- small key/silver rupee - texture_pot_key.png
- boss key - texture_pot_bosskey.png
- skulltula token - texture_pot_skull.png
- heart - texture_pot_side_heart.png

## Pot Top: 16x16 texture
- Pot Top (heart) - texture_pot_top_heart.png

## Crates: 32x128 texture. First 64 lines are for one side of the crate and next 64 lines are the other side of the crate
Note: Crates are limited to 256 color palettes 
- default - texture_crate_default.png
- major item - texture_crate_gold.png
- small key/silver rupee - texture_crate_key.png
- skulltula token - texture_crate_skull.png
- Boss Key - texture_crate_bosskey.png
- heart - texture_crate_heart.png

## Small crates: 32x64 texture
- major item - texture_smallcrate_gold.png
- small key/silver rupee - texture_smallcrate_key.png
- skulltula token - texture_smallcrate_skull.png
- Boss Key - texture_smallcrate_bosskey.png
- heart - texture_smallcrate_heart.png

## Chests: 32x64 texture for front and 32x64 for base
- Front, major item - texture_chest_front_gilded.png
- Base, major item - texture_chest_base_gilded.png
- Front, small key/silver rupee - texture_chest_front_silver.png
- Base, small key/silver rupee - texture_chest_base_silver.png
- Front, skulltula token - texture_chest_front_skull.png
- Base, skulltula token - texture_chest_base_skull.png
- Front, heart - texture_chest_front_heart.png
- Base, heart - texture_chest_base_heart.png
