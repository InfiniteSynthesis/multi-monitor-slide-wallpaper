This project supports multi-monitor wallpaper display on Windows. It can replace the wallpaper of each monitor from different folders for a specific interval. In addition, the python script can help automatically scale up the wallpaper images to fit the monitor resolution(e.g. 4K/2K/1440p) based on waifu2x-caffe.

## How to use

### SlideWallpaper

1. replace the YOUR_GALLERY_FOLDERS_HERE macro with your own gallery folder paths.
2. Set `Configurature Properties -> General -> TargetName` to `MultiMonitorSlideWallpaper`
3. Build it, and get the exe file `MultiMonitorSlideWallpaper.exe`.
4. Create a shortcut for `MultiMonitorSlideWallpaper.exe` and paste it to the system auto-startup folder.(`Win+R` to open `shell:startup`)

### MenuNextWallpaper
1. Build it, and get the exe file.
2. `Win+R` to open `regedit`. Add Key `Next Wallpaper` under path `HKEY_CLASSES_ROOT\Directory\Background\shell`. Add sub-key `command` and set value to the path of `menunextwallpaper.exe`.

### BackgroundScaleupWaifu.py

- Use `--help` to view the instructions.

## FAQ

#### What's the sequence of my monitors?

You can check `Desplay Settings -> Display` to find it. Usually DP comes a higher priority than HDMI.

#### How to remove the console window?

Add the `subsystem` and `entry point` macro.