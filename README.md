The code is for setting separate slide-like wallpaper for multi monitors in Windows(8+).

可替换Windows下的多显示屏墙纸，实现在不同的显示器上播放不同的幻灯片背景的效果。

## How to use

1. replace the YOUR_GALLERY_FOLDERS_HERE with your own gallery folder paths.
2. Build it.(e.g. Visual Studio)
3. Create a shortcut for the exe and paste it to the auto-startup folder.(`Win+R` to open `shell:startup`)

## FAQ

#### What's the sequence of my monitors?

You can check `Desplay Settings -> Display` to find it. Usually DP comes a higher priority than HDMI.

#### How to remove the console window?

Add the `subsystem` and `entry point` macro.