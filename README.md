# xfce4-wintitle-plugin
A xfce4-panel plugin that shows activate window title and icon, which is quite useful in some tiling window managers  
I write this to keep my newly configurated i3wm (with xfce4-panel) clean and expressive, since the out-of-box Window Buttons plugin introduces a lot of duplicated window information (for a tiling window manager).  
I haven't tried to develop a GTK based application before, though the plugin runs smoothly on my laptop, there might be some small issues.

## Compile and Installation
```bash
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
```

## Screenshots
![](https://raw.githubusercontent.com/AdamYuan/xfce4-wintitle-plugin/main/screenshots/1.png)
![](https://raw.githubusercontent.com/AdamYuan/xfce4-wintitle-plugin/main/screenshots/2.png)
