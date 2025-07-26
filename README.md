# 🖱️ pvpgod\_mouse

`pvpgod_mouse` is a minimal Linux utility that gives you **zero debounce time** for mouse input, perfect for PVP games where every millisecond matters. It works by intercepting mouse button presses via `evtest` and injecting them instantly back into the system using `uinput`.

## ⚡ What It Does

* Captures physical mouse button events from `/dev/input/eventX`
* Sends virtual mouse click events with **virtually zero delay**
* Bypasses traditional debounce logic, giving you raw click responsiveness

## ✅ Features

* Zero debounce on `BTN_LEFT` and `BTN_RIGHT`
* Lightweight and minimal (\~100 lines of C++)
* Works entirely in userspace via `evtest` + `uinput`

## 🔧 Requirements

* Linux with `uinput` module enabled
* `evtest` installed
* Root privileges (`sudo`) to access input and uinput devices

## 📦 Build

```bash
git clone https://github.com/darkyboys/pvpgod_mouse
cd pvpgod_mouse
g++ -o pvpgod_mouse pvpgod_mouse.cpp
```

## 🚀 Usage

```bash
sudo ./pvpgod_mouse /dev/input/eventX
```

> Replace `/dev/input/eventX` with your actual mouse event device (check with `evtest` or `ls /dev/input/`).

### Example

```bash
sudo ./pvpgod_mouse /dev/input/event5
```

## 🧪 How It Works

1. Opens the real mouse event stream using `evtest`.
2. Parses each line for `BTN_LEFT` and `BTN_RIGHT` codes.
3. Forwards those inputs immediately using `uinput`, simulating near-zero latency clicks.

## 🛑 Warning

* This tool creates **virtual clicks**, which may interfere with existing input software.
* Ensure you **only run it when needed**, and test it thoroughly in a safe environment.
* May trigger **anti-cheat** mechanisms in competitive games. Use responsibly.

## 🧼 Clean Up

When the program exits (CTRL+C), it automatically destroys the virtual device.

## 🧠 Credits

Written by `darkyboys`
Licensed under the MIT License
