The code is for setting separate slide-like wallpaper for multi monitors in Windows(8+).

## How to use

1. replace the FILE_FOLDER_PATH with your own gallery folder.
2. Build it.(e.g. Visual Studio)
3. Create a shortcut for the exe and paste it to the auto-startup folder.(`Win+R` to open `shell:startup`)

## FAQ

#### What's the sequence of my monitors?

You can check `Desplay Settings -> Display` to find it. Usually DP comes a higher priority than HDMI.

#### How to remove the console window?

Add the `subsystem` and `entry point` macro.