The code is for setting separate slide-like wallpaper for multi monitors in Windows(8+). Item `Next Wallpaper` is added to desktop menu.

可替换Windows下的多显示屏墙纸，实现在不同的显示器上播放不同的幻灯片背景的效果。同时在桌面菜单可右击更换下一张背景。

## How to use

1. replace the YOUR_GALLERY_FOLDERS_HERE macro with your own gallery folder paths.
2. Build 2 files, respectively, to get 2 exe.(e.g. Visual Studio)
3. Create a shortcut for the `slidewallpaper` exe and paste it to the auto-startup folder.(`Win+R` to open `shell:startup`)
4. `Win+R` to open `regedit`. Add Key `Next Wallpaper` under path `HKEY_CLASSES_ROOT\Directory\Background\shell`. Add sub-key `command` and set value to the path of `menunextwallpaper`.

## FAQ

#### What's the sequence of my monitors?

You can check `Desplay Settings -> Display` to find it. Usually DP comes a higher priority than HDMI.

#### How to remove the console window?

Add the `subsystem` and `entry point` macro.