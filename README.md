# CrowPanel-ESP32-2.9-inch-nametag-firmware

Custom firmware for the **CrowPanel ESP32-S3 2.9" e-paper display** with Wi-Fi upload support.

## 📦 Features

- Hosts its own Wi-Fi access point (`ESP32_Config`)
- Web-based interface to upload `.bin` image files
- Displays uploaded image fullscreen on the 296×128 e-paper display
- Removes the default top banner ("Ap_29demo.h" bar)
- Designed for clean, minimal, centered image output
- Auto-powers on the display via GPIO 7

## 🛠 Based On

This firmware is **based on Elecrow's original Arduino demo** for the CrowPanel (`2.9_WIFI_refresh.zip`) but modified for:

- Standalone operation without USB tethering
- Full-screen rendering (no top bar UI)
- Better support for custom bit-packed images (see `binner` tool)

## 📷 Uploading Images

1. Connect to the Wi-Fi AP named `ESP32_Config`.
2. Open `http://192.168.4.1/` in your browser.
3. Upload a `txt.bin` file (generated using the [binner](https://github.com/MariusQuabeck/CrowPanel-ESP32-2.9-inch-binner/tree/main) script).
4. The display will immediately update with your image.

## 🧱 Display Format

Images must be:

- **296×128 pixels**
- **1-bit monochrome**
- **Packed in column-major format** (8 vertical pixels per byte)
- **Bit-reversed in each byte** to correct for horizontal mirroring

Use the companion [binner](https://github.com/MariusQuabeck/CrowPanel-ESP32-2.9-inch-binner/tree/main) script to generate valid `.bin` files from PNGs.
