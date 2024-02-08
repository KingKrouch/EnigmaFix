B8G8R8A8_UNORM 3440x1440 buffer that has hardcoded 1920x1080 viewport width/height and right/bottom scissor rect.
I suspect that this possibly could be the render target that is giving us issues on the pause menu.
There are two of them that do similar things, after the draw-calls for the odd number rendering stuff is finished.

![[RT_01.png]]
![[RT_02.png]]
 
As one can see, despite the render target being 3440x1440, It's only using a 1920x1080 portion of the screen.
Looking at the Execution State of the DrawIndexed call shows us this:
![[RT_03.png]]

This actually echos what was mentioned earlier, but wait, there's more! If you look at the Vertex Buffer inside of the inputs section, it shows something truly shocking 😲

![[RT_04.png]] 
![[RT_05.png]] 
  
This seemingly goes on for a while for some reason (It's into the thousands count for the index). This might actually be the culprit behind the cropped camera transitions.

![[RT_06.png]]


