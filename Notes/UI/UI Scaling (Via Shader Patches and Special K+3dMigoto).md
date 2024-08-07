##### How to scale the UI properly:

Divide 1920 by your current resolution, get 0.75 and then multiply whatever first row's first and fourth values by 0.75 to get the desired results. This is only for 21:9, 32:9, 48:9 and other aspect ratios of the sorts. This doesn't cover stuff like 16:10, 4:3, and others of that sorts.
##### Character Bustups, status screen portraits, in-game UI, battle UI:
```
gProjectionMatrix2D {0.00104, 0.00, 0.00, -1.00052}
{0.00, -0.00185, 0.00, 1.00093}
{0.00, 0.00, -1.00, 1.00}
{0.00, 0.00, 0.00, 1.00} float4x4 (row_major)
```
##### Patched Version:
```
gProjectionMatrix2D {0.000751, 0.00, 0.00, -0.75039}
{0.00, -0.00185, 0.00, 1.00093}
{0.00, 0.00, -1.00, 1.00}
{0.00, 0.00, 0.00, 1.00} float4x4 (row_major)
```
##### Battle menu prompt:
**NOTE:** Need to get a capture in RenderDoc before I can modify these values.
##### Title Screen stuff, Main Menu (Pause Screen Scaling), VN Sequence box scaling:

```
Mk_ViewProjection {0.00104, 0.00, 0.00, -1.00}
{0.00, -0.00185, 0.00, 1.00}
{0.00, 0.00, 1.00, 0.00}
{0.00, 0.00, 0.00, 1.00} float4x4 (row_major)
```
##### Patched:
```
Mk_ViewProjection {0.000751, 0.00, 0.00, -0.750} (On the pause menu bustups, it may be needed to use 0.0007825, 0.0, 0.0, -0.750)
{0.00, -0.00185, 0.00, 1.00}
{0.00, 0.00, 1.00, 0.00}
{0.00, 0.00, 0.00, 1.00} float4x4 (row_major)
```