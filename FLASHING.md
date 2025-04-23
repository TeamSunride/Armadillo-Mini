# How to Flash to the Armadillo Mini (ESP32) Using Arduino IDE

## 🧰 Requirements

- **Arduino IDE** installed ([Download here](https://www.arduino.cc/en/software/))
- **USB-to-Serial Cable** (FTDI or CP2102 recommended – only **GND**, **TX**, and **RX** are required)
→ Example: [Amazon UART Cable](https://www.amazon.co.uk/uart-cable/s?k=uart+cable)
- **Armadillo Mini Board**
- **Jumper Wires** to connect pins manually during the flashing process

## 🛠️ Step 1: Set Up the Arduino IDE for ESP32

1. Open **Arduino IDE**.
2. Go to **File > Preferences**.
3. In the "**Additional Board Manager URLs**" field, add:

```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

*If you already have other boards added, simply separate them using a comma.*

4. Go to **Tools > Board > Boards Manager**.
5. Search for "**esp32**" and install the **Espressif ESP32** board package.

## 🔌 Step 2: Wiring the Armadillo Mini for Flashing

Connect your USB-to-Serial adapter to the Armadillo Mini, with the TX and RX connected to the appropriate pins, and GND on the adapter connected to GND on the Armadillo Mini.

> Don't worry about connecting TX and RX the wrong way around if you don't know which way round they go, it won't break anything and we'll verify they are correct in the next step.

To enter bootloader mode, you’ll also need to connect IO0 (GPIO 0, labelled BOOT) to GND using a jumper wire (only while flashing). This needs to be done before powering the board.

## 🔧 Step 3: Select the Board and Port

1. Go to **Tools > Board > esp32**, and select **ESP32-WROOM-DA Module**.
2. Go to **Tools > Port**, and select the correct COM port for your USB-to-Serial adapter. It might have "serial" in the name (but this depends on the specific cable and drivers you're using).
3. Go to **Tools** and select **Serial Monitor** and then set the baud to 115200. It's often set to 9600 by default and **will not work** if not changed to 115200.
4. Now, with the USB-to-Serial adapter connected, and BOOT connected to GND, power the Armadillo Mini on. The following (or something similar) should be output to the Serial Monitor:

```
edets Jul 29 2019 12:21:46

rst:0x1 (POWERON_RESET),boot:0x3 (DOWNLOAD_BOOT(UART0/UART1/SDIO_REI_REO_V2))
waiting for download
ets Jul 29 2019 12:21:46
```

> If nothing is output, this is probably a sign that the RX and TX pins are connected the wrong way around. In which case, remove power from the board, swap the pin connections, power the board back on and check the Serial Monitor again.

## ⚡ Step 4: Upload Your Sketch

1. Paste the Arduino code into Arduino IDE.
2. Click the **Upload** button.
3. Watch the Serial Monitor, as it shows the progress of the flashing. Wait until it reaches 100% and says `Hard resetting via RTS pin...`.

## ✅ Step 5: Done!

If the upload succeeds, remove the **BOOT/IO0 to GND** connection. Then cycle the power to boot into the newly flashed firmware!