# fast64_to_ootr toolchain
This toolchain was developed to assist in compiling Fast64 exported assets for use in OOTR. It is designed to be used in conjunction with the oot decompilation project [https://github.com/zeldaret/oot]. This was created primarily because I didn't feel like learning the modloader PlayAs/zzconvert method for building player models. Because I had some basic experience with Fast64 I decided to do this instead.

## How to use
### Prerequisites
* Blender [https://www.blender.org/]
* Blender Fast64 plugin [https://github.com/Fast-64/fast64]
* OOT decompilation project [https://github.com/zeldaret/oot] (make sure you are able to build the ntsc-1.0 version before proceeding)
* Python
* MIPS toolchain, recommend either glankk's N64 build tools [https://github.com/glankk/n64] or mips-linux-gnu

### Setup
To set up, just copy fast64_to_ootr.py and ModelHelpers.py to the top level directory of your decomp installation

### Compiling Player Models
1) Import Child or Adult player Skeleton + Mesh into Blender using Fast64's Import Skeleton feature. For "Mode" select Adult Link/Child Link
2) Delete or edit the link mesh as desired. When rigging the mesh, make sure that all bone's have vertices assigned. If your model does not actually use a certain limb, rig a small plane to the bone and if necessary to hide it, make it transparent
3) A complete model should have a skeleton named gLinkChildSkel with the same bone names that were imported. The skeleton should include a single mesh for the entire model.
4) Once your new model is complete use Fast64's Export Skeleton function, again setting "Mode" to Adult Link/Child Link. Leave all other settings default. When you press "Export Skeleton" if you get the message "Success" you're probably in good shape to continue.
5) Your model should be exported a gLinkAdultSkel.c/h or gLinkChildSkel.c/h. If you left the default export settings they should be in the folder assets/objects/object_link_(boy/child).
7) Copy the LUT.c/.h files appropriate for the model into the same folder as the exported skeleton.
9) Compile the file with the following command:
python3 fast64_to_ootr.py --is_link --in_file assets/objects/object_link_boy/gLinkAdultSkel.c
python3 fast64_to_ootr.py --is_link --in_file assets/objects/object_link_child/gLinkChildSkel.c

Note: the toolchain is configured by default to use the mips64-ultra-elf compilation tools from the glankk n64 toolchain. If you have a different n64 compilation toolset, use the --build_prefix option to change the build tools. Ex. --build_prefix mips-linux-gnu

This will create a .zobj file that should be ready to import to OOTR
10) Enjoy your new model :)

## Advanced Topics
### Eyes and Mouth Textures
Eyes and mouth textures are dynamically set by the game during run-time.

Eyes are a 64x32 CI8 texture. To add the textures in Blender/Fast64, use a material with the "Set Texture Reference" property set to 0x08000000 which will unlock the "Flipbook Properties" section of the material. Select "Export Flipbook Textures" and add each texture. 

Mouth is a 32x32 CI8. Otherwise, do the same thing as eyes in Blender/Fast64

These textures must be placed at the start of the LUT. See the beginning of the LUT.c file for instructions on how to add the textures to the LUT.

### Additional pieces (Slingshot, Hookshot, Bow, Hammer, etc.)
You may want to create additional pieces for your model. Things like slingshot, boomerang, hookshot, hammer, bow. These should be modelled and exported as their own meshes and exported using Fast64's DL Exporter. These will result in additional .c/.h files which you need to manually include at the end of the main skeleton .c file. It is recommended to model these pieces without the hand, and modelling a single closed hand for them (and FPS arm for hookshot/slingshot). The default LUT is set up to automatically draw both the hand and the equipment and will use the model's default hands if none are specified.

To add additional pieces you will need to manually edit the lookup table in the LUT.c. See the notes in the "Base Parts" section of that file