# Drum Live Station Keyboard Hook

Tool that allows you to use your keyboard on Drum Live Station.

Video:

[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/sdj5QoLypgk/0.jpg)](https://www.youtube.com/watch?v=sdj5QoLypgk)

# Usage
Download the pre-compiled binary or compile it, then add it to LD_PRELOAD before executing DLS.

# Default keys

D for hi hat

F for snare

G for bass

H for tom 1

J for tom 2

K for cymbal

F1 for service, F2 for test / insert coin, F3 for clear coins

# Compiling

Requires a C++11 compiler.

To compile it yourself, run these commands:

```
sudo apt-get install git build-essential libx11-dev
git clone https://github.com/therathatter/dls-kb-hook
cd dls-kb-hook
make
```
