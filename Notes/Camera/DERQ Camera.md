#### Camera Zoom:
``
```
Application.exe+69055B - E8 20FCE2FF           - call Application.exe+4C0180 (Something Speed related)
Application.exe+69056C - F3 0F10 83 A8010000   - movss xmm0,[rbx+000001A8]
```
#### FOV:
###### Cutscene FOV Calls:

**NOTE:** Every function that calls the battle/cutscene FOV calls, stuff that can seemingly be NOP'd (at least right now, without issues)

```
7ff60cfef671 (Application.exe+5BF671)
7ff60d0177bf (Application.exe+5E77BF)
7ff60d0357e4 (Application.exe+6057E4)
7ff60d0621ef (Application.exe+6321EF)
7ff60d0d4e4d (Application.exe+6A4E4D)
```

###### Stuff that can't be NOP'd, because it's tied to cutscene FOV and other things:

As evident by the fact that the top one of the two that shouldn't be NOP'd is seemingly part of an if statement in a function that has a bunch of functions calling reading and writing to it, I might need to trace back which functions are writing to it, which may take time.

``7ff60d11fcfc (Application.exe+6EFCFC)`` - This is an address that is called during the glitch transformation cutscenes alongside the victory camera at least. Although there's seemingly an interesting side effect where after selecting attacks and during said camera transitions during attacks, the FOV changes, which is undesired. It might be necessary to trace this instruction to another call which calls upon this to find the culprit. Don't NOP this one.

``7ff60d1c8d71 (Application.exe+798D71)`` - This is seemingly the address that is called for the opening cutscene at least. Don't NOP this one.

``Application.exe+6E1A6B - movss [rcx+000004C4],xmm0`` - Seemingly something that is written when you go into the command menu during battles

###### Stuff that is planned to be modified:

``"Application.exe"+6E1F6F`` - Seemingly writes the FOV to the player every frame. This doesn't seem to apply to the battle mode FOV however.

**For this function to read from another value, just replace:**

``"movss [rcx+000004C4],xmm0"``

with:

```
variableAddress:
  dd (float)90.0

movss xmm1,[variableAddress]
movss [rcx+000004C4],xmm1
```