# xfce4-wintitle-plugin
**A xfce4-panel plugin that shows activate window title and icon, which is quite useful in some tiling window managers.**  
![](https://raw.githubusercontent.com/AdamYuan/xfce4-wintitle-plugin/main/screenshots/2.png)
I write this to keep my newly configurated i3wm (with xfce4-panel) clean and expressive, since the out-of-box Window Buttons plugin introduces a lot of duplicated window information (for a tiling window manager).  
I haven't tried to develop a GTK based application before. Though the plugin runs smoothly on my laptop (xfce4-panel 4.16), there might be some small issues.

## Compile and Installation
```bash
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
```

## Settings page
![](https://raw.githubusercontent.com/AdamYuan/xfce4-wintitle-plugin/main/screenshots/1.png)
